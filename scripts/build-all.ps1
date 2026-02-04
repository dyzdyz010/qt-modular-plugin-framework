<#
.SYNOPSIS
    MPF 项目一键构建脚本

.DESCRIPTION
    按正确顺序构建 MPF 所有组件：
    1. Foundation SDK
    2. HTTP Client 库
    3. UI Components 库
    4. Host 应用
    5. Orders 插件

.PARAMETER QtPath
    Qt 安装路径，例如 "C:/Qt/6.8.3/mingw_64"

.PARAMETER SdkPath
    SDK 安装目录，例如 "C:/Qt/MPF"

.PARAMETER BuildType
    构建类型：Release 或 Debug

.PARAMETER SkipSdk
    跳过 SDK 构建（如果已安装）

.PARAMETER SkipLibs
    跳过库构建（如果已安装）

.EXAMPLE
    .\build-all.ps1 -QtPath "C:/Qt/6.8.3/mingw_64" -SdkPath "C:/Qt/MPF"

.EXAMPLE
    .\build-all.ps1 -SkipSdk -SkipLibs  # 只构建 Host 和插件
#>

param(
    [string]$QtPath = "C:/Qt/6.8.3/mingw_64",
    [string]$SdkPath = "C:/Qt/MPF",
    [string]$BuildType = "Release",
    [switch]$SkipSdk,
    [switch]$SkipLibs
)

# =============================================================================
# 配置
# =============================================================================

$ErrorActionPreference = "Stop"

# 获取项目根目录
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent $ScriptDir

# 设置工具链路径
$QtToolsPath = Split-Path -Parent (Split-Path -Parent $QtPath)
$env:PATH = "$QtToolsPath\Tools\mingw1310_64\bin;$QtToolsPath\Tools\Ninja;$QtToolsPath\Tools\CMake_64\bin;" + $env:PATH

# =============================================================================
# 辅助函数
# =============================================================================

function Write-Step {
    param([string]$Message)
    Write-Host ""
    Write-Host "==============================================================================" -ForegroundColor Cyan
    Write-Host " $Message" -ForegroundColor Cyan
    Write-Host "==============================================================================" -ForegroundColor Cyan
    Write-Host ""
}

function Test-Command {
    param([string]$Command)
    $null = Get-Command $Command -ErrorAction SilentlyContinue
    return $?
}

function Invoke-Build {
    param(
        [string]$Name,
        [string]$SourceDir,
        [string]$BuildDir,
        [hashtable]$CMakeArgs,
        [switch]$Install
    )
    
    Write-Step "构建 $Name"
    
    Push-Location $SourceDir
    
    try {
        # 配置
        $args = @("-B", $BuildDir, "-G", "Ninja")
        foreach ($key in $CMakeArgs.Keys) {
            $args += "-D$key=$($CMakeArgs[$key])"
        }
        
        Write-Host "cmake $($args -join ' ')" -ForegroundColor DarkGray
        & cmake @args
        if ($LASTEXITCODE -ne 0) { throw "CMake 配置失败" }
        
        # 构建
        Write-Host "cmake --build $BuildDir" -ForegroundColor DarkGray
        & cmake --build $BuildDir
        if ($LASTEXITCODE -ne 0) { throw "构建失败" }
        
        # 安装
        if ($Install) {
            Write-Host "cmake --install $BuildDir" -ForegroundColor DarkGray
            & cmake --install $BuildDir
            if ($LASTEXITCODE -ne 0) { throw "安装失败" }
        }
        
        Write-Host "$Name 构建成功!" -ForegroundColor Green
    }
    finally {
        Pop-Location
    }
}

# =============================================================================
# 环境检查
# =============================================================================

Write-Step "环境检查"

# 检查必要工具
$tools = @("cmake", "ninja", "g++")
foreach ($tool in $tools) {
    if (Test-Command $tool) {
        $version = & $tool --version | Select-Object -First 1
        Write-Host "  [OK] $tool - $version" -ForegroundColor Green
    } else {
        Write-Host "  [ERROR] $tool 未找到" -ForegroundColor Red
        Write-Host "  请确保 Qt 工具链在 PATH 中" -ForegroundColor Yellow
        exit 1
    }
}

# 检查 Qt 路径
if (-not (Test-Path "$QtPath/lib/cmake/Qt6")) {
    Write-Host "  [ERROR] Qt 路径无效: $QtPath" -ForegroundColor Red
    exit 1
}
Write-Host "  [OK] Qt 路径: $QtPath" -ForegroundColor Green

# 创建 SDK 目录
if (-not (Test-Path $SdkPath)) {
    Write-Host "  创建 SDK 目录: $SdkPath" -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $SdkPath -Force | Out-Null
}
Write-Host "  [OK] SDK 路径: $SdkPath" -ForegroundColor Green

# =============================================================================
# 构建 Foundation SDK
# =============================================================================

if (-not $SkipSdk) {
    Invoke-Build -Name "Foundation SDK" `
        -SourceDir "$ProjectRoot/foundation-sdk" `
        -BuildDir "build" `
        -CMakeArgs @{
            "CMAKE_PREFIX_PATH" = $QtPath
            "CMAKE_INSTALL_PREFIX" = $SdkPath
            "CMAKE_BUILD_TYPE" = $BuildType
        } `
        -Install
} else {
    Write-Host "跳过 Foundation SDK 构建" -ForegroundColor Yellow
}

# =============================================================================
# 构建 HTTP Client 库
# =============================================================================

if (-not $SkipLibs) {
    Invoke-Build -Name "HTTP Client 库" `
        -SourceDir "$ProjectRoot/libs/http-client" `
        -BuildDir "build" `
        -CMakeArgs @{
            "CMAKE_PREFIX_PATH" = "$QtPath;$SdkPath"
            "CMAKE_INSTALL_PREFIX" = $SdkPath
            "CMAKE_BUILD_TYPE" = $BuildType
        } `
        -Install
} else {
    Write-Host "跳过 HTTP Client 库构建" -ForegroundColor Yellow
}

# =============================================================================
# 构建 UI Components 库
# =============================================================================

if (-not $SkipLibs) {
    Invoke-Build -Name "UI Components 库" `
        -SourceDir "$ProjectRoot/libs/ui-components" `
        -BuildDir "build" `
        -CMakeArgs @{
            "CMAKE_PREFIX_PATH" = "$QtPath;$SdkPath"
            "CMAKE_INSTALL_PREFIX" = $SdkPath
            "CMAKE_BUILD_TYPE" = $BuildType
        } `
        -Install
} else {
    Write-Host "跳过 UI Components 库构建" -ForegroundColor Yellow
}

# =============================================================================
# 构建 Host 应用
# =============================================================================

Invoke-Build -Name "Host 应用" `
    -SourceDir "$ProjectRoot/host" `
    -BuildDir "build/Release" `
    -CMakeArgs @{
        "CMAKE_PREFIX_PATH" = "$QtPath;$SdkPath"
        "CMAKE_BUILD_TYPE" = "Debug"
    }

# =============================================================================
# 构建 Orders 插件
# =============================================================================

Write-Step "构建 Orders 插件"

Push-Location "$ProjectRoot/plugins/orders"

try {
    # 使用 CMake Preset
    Write-Host "cmake --preset default" -ForegroundColor DarkGray
    & cmake --preset default
    if ($LASTEXITCODE -ne 0) { throw "CMake 配置失败" }
    
    Write-Host "cmake --build --preset default" -ForegroundColor DarkGray
    & cmake --build --preset default
    if ($LASTEXITCODE -ne 0) { throw "构建失败" }
    
    Write-Host "Orders 插件构建成功!" -ForegroundColor Green
}
finally {
    Pop-Location
}

# =============================================================================
# 配置路径
# =============================================================================

Write-Step "配置运行环境"

$PluginBuildPath = "$ProjectRoot/plugins/orders/build/default"
$HostBuildPath = "$ProjectRoot/host/build/Release"

# 更新 paths.json
$PathsJson = @{
    "pluginPath" = "$PluginBuildPath/plugins".Replace("\", "/")
    "qmlPath" = ""
    "extraQmlPaths" = @(
        "$PluginBuildPath/qml".Replace("\", "/"),
        "$SdkPath/qml".Replace("\", "/")
    )
}

$PathsJsonPath = "$HostBuildPath/config/paths.json"
if (Test-Path (Split-Path -Parent $PathsJsonPath)) {
    $PathsJson | ConvertTo-Json -Depth 3 | Set-Content $PathsJsonPath -Encoding UTF8
    Write-Host "已更新: $PathsJsonPath" -ForegroundColor Green
}

# 同时更新源文件
$SourcePathsJson = "$ProjectRoot/host/config/paths.json"
$PathsJson | ConvertTo-Json -Depth 3 | Set-Content $SourcePathsJson -Encoding UTF8
Write-Host "已更新: $SourcePathsJson" -ForegroundColor Green

# =============================================================================
# 完成
# =============================================================================

Write-Step "构建完成!"

Write-Host "构建产物位置:" -ForegroundColor White
Write-Host "  SDK:    $SdkPath" -ForegroundColor Gray
Write-Host "  Host:   $HostBuildPath/bin/mpf-host.exe" -ForegroundColor Gray
Write-Host "  插件:   $PluginBuildPath/plugins/liborders-plugin.dll" -ForegroundColor Gray

Write-Host ""
Write-Host "运行方法:" -ForegroundColor White
Write-Host "  1. 设置环境变量:" -ForegroundColor Gray
Write-Host "     `$env:PATH = `"$SdkPath/bin;`" + `$env:PATH" -ForegroundColor DarkGray
Write-Host ""
Write-Host "  2. 运行应用:" -ForegroundColor Gray
Write-Host "     cd $HostBuildPath/bin" -ForegroundColor DarkGray
Write-Host "     ./mpf-host.exe" -ForegroundColor DarkGray
Write-Host ""
