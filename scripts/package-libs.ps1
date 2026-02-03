param(
    [string[]]$Libs = @(),
    [switch]$All,
    [string]$BuildRoot = "build/libs",
    [string]$OutRoot = "dist/libs",
    [string]$Config = "Release",
    [string]$Generator = "",
    [string]$QtPrefix = ""
)

$ErrorActionPreference = "Stop"
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
$LibsRoot = Join-Path $Root "libs"

$AppendPath = {
    param([string]$PathToAdd)
    if ([string]::IsNullOrWhiteSpace($PathToAdd)) { return $false }
    if (-not (Test-Path $PathToAdd)) { return $false }
    $Sep = ";"
    $Current = $env:PATH
    $Segments = $Current -split [regex]::Escape($Sep)
    if ($Segments -contains $PathToAdd) { return $false }
    $env:PATH = "$PathToAdd$Sep$Current"
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

if ($QtPrefix) {
    $QtBin = Join-Path $QtPrefix "bin"
    $Added = & $AppendPath $QtBin
    if ($Added) { Write-Host "Added to PATH: $QtBin" }
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

    if (-not $HasMingwMake -and (Test-Path $QtMingwMake)) { $HasMingwMake = $true }
    if (-not $HasNinja -and (Test-Path $QtNinja)) { $HasNinja = $true }
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
    $QtLower = if ($QtPrefix) { $QtPrefix.ToLowerInvariant() } else { "" }
    if ($QtLower.Contains("mingw")) {
        if ($HasMingwMake) { $Generator = "MinGW Makefiles" }
        elseif ($HasNinja) { $Generator = "Ninja" }
        else { throw "MinGW Qt detected but no suitable generator found. Install mingw32-make or ninja." }
    } elseif ($QtLower.Contains("msvc")) {
        if ($HasVsWhere) { $Generator = "Visual Studio 17 2022" }
        elseif ($HasNinja) { $Generator = "Ninja" }
        else { throw "MSVC Qt detected but no suitable generator found. Install Visual Studio or ninja." }
    } else {
        if ($HasNinja) { $Generator = "Ninja" }
        elseif ($HasMingwMake) { $Generator = "MinGW Makefiles" }
        elseif ($HasVsWhere) { $Generator = "Visual Studio 17 2022" }
    }
}

if ($Generator) {
    Write-Host "Using generator: $Generator"
}

if ($All) {
    $Libs = Get-ChildItem -Path $LibsRoot -Directory |
        Where-Object { Test-Path (Join-Path $_.FullName "CMakeLists.txt") } |
        ForEach-Object { $_.Name }
}

if (-not $Libs -or $Libs.Count -eq 0) {
    throw "No libs specified. Use -Libs <name1,name2> or -All."
}

foreach ($lib in $Libs) {
    $SourceDir = Join-Path $LibsRoot $lib
    if (-not (Test-Path $SourceDir)) {
        throw "Lib not found: $lib ($SourceDir)"
    }

    $BuildDir = Join-Path $Root (Join-Path $BuildRoot $lib)
    if ($Generator) {
        $GenSafe = $Generator -replace '[^A-Za-z0-9_.-]', '_'
        $BuildDir = "$BuildDir-$GenSafe"
    }

    $OutDir = Join-Path $Root (Join-Path $OutRoot $lib)

    New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
    New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

    $CmakeArgs = @(
        "-S", "$SourceDir",
        "-B", "$BuildDir",
        "-DCMAKE_INSTALL_PREFIX=$OutDir"
    )

    if ($Generator) { $CmakeArgs += @("-G", "$Generator") }
    if ($Generator -notmatch "Visual Studio" -and $Generator -notmatch "Multi-Config") {
        $CmakeArgs += "-DCMAKE_BUILD_TYPE=$Config"
    }
    if ($QtPrefix) { $CmakeArgs += "-DCMAKE_PREFIX_PATH=$QtPrefix" }

    if ($Generator -match "MinGW") {
        if (-not $HasMingwMake) { throw "MinGW generator requires mingw32-make in PATH." }
        if ($QtMingwMake -and (Test-Path $QtMingwMake)) { $CmakeArgs += "-DCMAKE_MAKE_PROGRAM=$QtMingwMake" }
        if ($QtGcc -and $QtGxx -and (Test-Path $QtGcc) -and (Test-Path $QtGxx)) {
            $CmakeArgs += "-DCMAKE_C_COMPILER=$QtGcc"
            $CmakeArgs += "-DCMAKE_CXX_COMPILER=$QtGxx"
        }
    }

    if ( $Generator -match "Visual Studio" -and ($QtPrefix.ToLowerInvariant().Contains("mingw")) ) {
        throw "Detected MSVC generator with MinGW Qt. Use MSVC Qt or MinGW generator."
    }
    if ( $Generator -match "MinGW" -and ($QtPrefix.ToLowerInvariant().Contains("msvc")) ) {
        throw "Detected MinGW generator with MSVC Qt. Use MinGW Qt or MSVC generator."
    }

    cmake @CmakeArgs
    if ($LASTEXITCODE -ne 0) { throw "CMake configure failed: $lib" }

    cmake --build "$BuildDir" --config "$Config"
    if ($LASTEXITCODE -ne 0) { throw "Build failed: $lib" }

    cmake --install "$BuildDir" --config "$Config"
    if ($LASTEXITCODE -ne 0) { throw "Install failed: $lib" }

    Write-Host "Packaged lib: $lib -> $OutDir"
}
