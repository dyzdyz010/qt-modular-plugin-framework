param(
    [string]$SdkPrefix = "C:/Qt/MPF",
    [string]$Config = "Release",
    [string]$Generator = "",
    [string]$QtPrefix = ""
)

$ErrorActionPreference = "Stop"
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
$SdkPrefixFull = $SdkPrefix

$AppendPath = {
    param([string]$PathToAdd)
    if ([string]::IsNullOrWhiteSpace($PathToAdd)) { return $false }
    if (-not (Test-Path $PathToAdd)) { return $false }
    $PathSeparator = ";"
    $Current = $env:PATH
    $Segments = $Current -split [regex]::Escape($PathSeparator)
    if ($Segments -contains $PathToAdd) { return $false }
    $env:PATH = "$PathToAdd$PathSeparator$Current"
    return $true
}

$FindTool = {
    param(
        [string]$FileName,
        [string[]]$Roots
    )
    foreach ($root in $Roots) {
        if ([string]::IsNullOrWhiteSpace($root)) { continue }
        if (-not (Test-Path $root)) { continue }
        $found = Get-ChildItem -Path $root -Filter $FileName -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($found) { return $found.FullName }
    }
    return ""
}

$ToLower = {
    param([string]$Value)
    if ([string]::IsNullOrWhiteSpace($Value)) { return "" }
    return $Value.ToLowerInvariant()
}

$QtLower = & $ToLower $QtPrefix
$GenLower = & $ToLower $Generator

if ($QtPrefix) {
    $QtBin = Join-Path $QtPrefix "bin"
    $Added = & $AppendPath $QtBin
    if ($Added) {
        Write-Host "Added to PATH: $QtBin"
    }
}

$HasNinja = [bool](Get-Command "ninja" -ErrorAction SilentlyContinue)
$HasMingwMake = [bool](Get-Command "mingw32-make" -ErrorAction SilentlyContinue)
$HasVsWhere = [bool](Get-Command "vswhere" -ErrorAction SilentlyContinue)

$QtMingwMake = ""
$QtNinja = ""
$QtGcc = ""
$QtGxx = ""

if ($QtPrefix) {
    $QtBin = Join-Path $QtPrefix "bin"
    $QtMingwMake = Join-Path $QtBin "mingw32-make.exe"
    $QtNinja = Join-Path $QtBin "ninja.exe"
    $QtGcc = Join-Path $QtBin "gcc.exe"
    $QtGxx = Join-Path $QtBin "g++.exe"

    if (-not $HasMingwMake -and (Test-Path $QtMingwMake)) {
        $HasMingwMake = $true
    }

    if (-not $HasNinja -and (Test-Path $QtNinja)) {
        $HasNinja = $true
    }
}

if (-not $HasMingwMake) {
    $SearchRoots = @(
        $QtPrefix,
        (Split-Path $QtPrefix -Parent),
        "C:\Qt\Tools",
        "C:\Qt"
    )
    $QtMingwMake = & $FindTool "mingw32-make.exe" $SearchRoots
    if ($QtMingwMake) {
        $HasMingwMake = $true
        $Added = & $AppendPath (Split-Path $QtMingwMake -Parent)
        if ($Added) { Write-Host "Added to PATH: $(Split-Path $QtMingwMake -Parent)" }
    }
}

if (-not $HasNinja) {
    $SearchRoots = @(
        $QtPrefix,
        (Split-Path $QtPrefix -Parent),
        "C:\Qt\Tools",
        "C:\Qt"
    )
    $QtNinja = & $FindTool "ninja.exe" $SearchRoots
    if ($QtNinja) {
        $HasNinja = $true
        $Added = & $AppendPath (Split-Path $QtNinja -Parent)
        if ($Added) { Write-Host "Added to PATH: $(Split-Path $QtNinja -Parent)" }
    }
}

if (-not $Generator) {
    if ($QtLower.Contains("mingw")) {
        if ($HasMingwMake) {
            $Generator = "MinGW Makefiles"
        } elseif ($HasNinja) {
            $Generator = "Ninja"
        } else {
            throw "MinGW Qt detected but no suitable generator found. Install mingw32-make or ninja."
        }
    } elseif ($QtLower.Contains("msvc")) {
        if ($HasVsWhere) {
            $Generator = "Visual Studio 17 2022"
        } elseif ($HasNinja) {
            $Generator = "Ninja"
        } else {
            throw "MSVC Qt detected but no suitable generator found. Install Visual Studio or ninja."
        }
    } else {
        if ($HasNinja) {
            $Generator = "Ninja"
        } elseif ($HasMingwMake) {
            $Generator = "MinGW Makefiles"
        } elseif ($HasVsWhere) {
            $Generator = "Visual Studio 17 2022"
        }
    }
}

if ($Generator) {
    Write-Host "Using generator: $Generator"
}

$Projects = @(
    @{ Name = "foundation-sdk"; Source = "foundation-sdk"; Build = "build/sdk/foundation-sdk" },
    @{ Name = "http-client"; Source = "libs/http-client"; Build = "build/sdk/http-client" }
)

foreach ($proj in $Projects) {
    $SourceDir = Join-Path $Root $proj.Source
    $BuildDir = Join-Path $Root $proj.Build
    if ($Generator) {
        $GenSafe = $Generator -replace '[^A-Za-z0-9_.-]', '_'
        $BuildDir = "$BuildDir-$GenSafe"
    }

    New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

    $CmakeArgs = @(
        "-S", "$SourceDir",
        "-B", "$BuildDir",
        "-DCMAKE_INSTALL_PREFIX=$SdkPrefixFull"
    )

    if ($Generator) {
        $CmakeArgs += @("-G", "$Generator")
    }

    if ($Generator -notmatch "Visual Studio" -and $Generator -notmatch "Multi-Config") {
        $CmakeArgs += "-DCMAKE_BUILD_TYPE=$Config"
    }

    if ($QtPrefix) {
        $CmakeArgs += "-DCMAKE_PREFIX_PATH=$QtPrefix"
    }

    $IsMsvc = $false
    if ($Generator -match "Visual Studio") {
        $IsMsvc = $true
    } elseif ($env:VSCMD_VER) {
        $IsMsvc = $true
    }

    if ($IsMsvc) {
        $CmakeArgs += "-DCMAKE_CXX_FLAGS=/Zc:__cplusplus"
    }

    if ($Generator -match "MinGW") {
        if (-not $HasMingwMake) {
            throw "MinGW generator requires mingw32-make in PATH."
        }

        if ($QtMingwMake -and (Test-Path $QtMingwMake)) {
            $CmakeArgs += "-DCMAKE_MAKE_PROGRAM=$QtMingwMake"
        }

        if ($QtGcc -and $QtGxx -and (Test-Path $QtGcc) -and (Test-Path $QtGxx)) {
            $CmakeArgs += "-DCMAKE_C_COMPILER=$QtGcc"
            $CmakeArgs += "-DCMAKE_CXX_COMPILER=$QtGxx"
        }
    }

    if ( $Generator -match "Visual Studio" -and $QtLower.Contains("mingw") ) {
        throw "Detected MSVC generator with MinGW Qt. Use MSVC Qt or MinGW generator."
    }

    if ( $Generator -match "MinGW" -and $QtLower.Contains("msvc") ) {
        throw "Detected MinGW generator with MSVC Qt. Use MinGW Qt or MSVC generator."
    }

    cmake @CmakeArgs
    if ($LASTEXITCODE -ne 0) { throw "CMake configure failed: $($proj.Name)" }

    cmake --build "$BuildDir" --config "$Config"
    if ($LASTEXITCODE -ne 0) { throw "Build failed: $($proj.Name)" }

    cmake --install "$BuildDir" --config "$Config"
    if ($LASTEXITCODE -ne 0) { throw "Install failed: $($proj.Name)" }
}

Write-Host "SDK installed to: $SdkPrefixFull"
