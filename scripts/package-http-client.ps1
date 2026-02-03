param(
    [string]$BuildDir = "build/http-client",
    [string]$OutDir = "dist/http-client",
    [string]$Config = "Release"
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
$SourceDir = Join-Path $Root "libs/http-client"
$BuildDirFull = Join-Path $Root $BuildDir
$OutDirFull = Join-Path $Root $OutDir

New-Item -ItemType Directory -Force -Path $BuildDirFull | Out-Null
New-Item -ItemType Directory -Force -Path $OutDirFull | Out-Null

cmake -S "$SourceDir" -B "$BuildDirFull" -DCMAKE_INSTALL_PREFIX="$OutDirFull"
cmake --build "$BuildDirFull" --config "$Config"
cmake --install "$BuildDirFull" --config "$Config"

Write-Host "HTTP 客户端库已打包到: $OutDirFull"
