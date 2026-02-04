# =============================================================================
# MPF 开发环境设置脚本 (Windows PowerShell)
# =============================================================================
#
# 用法: . .\scripts\setup-env.ps1 [-QtDir PATH] [-SdkPath PATH]
#
# 参数:
#   -QtDir PATH      Qt 安装路径 (默认: C:\Qt\6.8.3\mingw_64)
#   -SdkPath PATH    MPF SDK 安装路径 (默认: C:\Qt\MPF)
#
# 此脚本设置以下环境变量:
#   - QT_DIR: Qt 安装路径
#   - MPF_SDK: MPF SDK 安装路径
#   - CMAKE_PREFIX_PATH: CMake 搜索路径
#   - PATH: 添加 Qt 工具和 SDK bin 路径
#
# =============================================================================

param(
    [string]$QtDir = "",
    [string]$SdkPath = ""
)

# 默认值
if ([string]::IsNullOrEmpty($QtDir)) {
    $QtDir = if ($env:QT_DIR) { $env:QT_DIR } else { "C:\Qt\6.8.3\mingw_64" }
}

if ([string]::IsNullOrEmpty($SdkPath)) {
    $SdkPath = if ($env:MPF_SDK) { $env:MPF_SDK } else { "C:\Qt\MPF" }
}

# 验证路径
if (-not (Test-Path $QtDir)) {
    Write-Host "[WARN] Qt 路径不存在: $QtDir" -ForegroundColor Yellow
    Write-Host "       请设置正确的 QT_DIR 或使用 -QtDir 参数" -ForegroundColor Yellow
}

# 设置环境变量
$env:QT_DIR = $QtDir
$env:MPF_SDK = $SdkPath
$env:CMAKE_PREFIX_PATH = "$QtDir;$SdkPath"

# 添加到 PATH
$pathsToAdd = @(
    "$QtDir\bin",
    "$SdkPath\bin",
    "C:\Qt\Tools\mingw1310_64\bin",
    "C:\Qt\Tools\Ninja",
    "C:\Qt\Tools\CMake_64\bin"
)

foreach ($p in $pathsToAdd) {
    if ((Test-Path $p) -and ($env:PATH -notlike "*$p*")) {
        $env:PATH = "$p;$env:PATH"
    }
}

# 打印配置
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "MPF 开发环境已配置 (Windows)" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "QT_DIR:            $env:QT_DIR"
Write-Host "MPF_SDK:           $env:MPF_SDK"
Write-Host "CMAKE_PREFIX_PATH: $env:CMAKE_PREFIX_PATH"
Write-Host ""
Write-Host "现在可以使用以下命令:"
Write-Host "  cmake --preset windows-debug   # 配置插件"
Write-Host "  cmake --build build            # 构建"
Write-Host ""
