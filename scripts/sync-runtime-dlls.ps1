param(
    [string]$SdkPrefix = "C:/Qt/MPF",
    [string]$HostBuildDir = "host/build",
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
$SdkBin = Join-Path $SdkPrefix "bin"
$HostBuildFull = Join-Path $Root $HostBuildDir
$HostBin = Join-Path $HostBuildFull "bin"
$HostBinConfig = Join-Path $HostBin $Config

if (Test-Path $HostBinConfig) {
    $TargetBin = $HostBinConfig
} else {
    $TargetBin = $HostBin
}

if (-not (Test-Path $SdkBin)) {
    throw "SDK bin 目录不存在: $SdkBin"
}

New-Item -ItemType Directory -Force -Path $TargetBin | Out-Null

$Dlls = Get-ChildItem -Path $SdkBin -Filter *.dll -File
if ($Dlls.Count -eq 0) {
    Write-Host "未找到需要同步的 DLL: $SdkBin"
    exit 0
}

Copy-Item -Path $Dlls.FullName -Destination $TargetBin -Force
Write-Host "已同步 DLL 到: $TargetBin"
