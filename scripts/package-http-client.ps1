param(
    [string]$BuildRoot = "build/libs",
    [string]$OutRoot = "dist/libs",
    [string]$Config = "Release",
    [string]$Generator = "",
    [string]$QtPrefix = ""
)

$ErrorActionPreference = "Stop"

& (Join-Path $PSScriptRoot "package-libs.ps1") `
    -Libs "http-client" `
    -BuildRoot $BuildRoot `
    -OutRoot $OutRoot `
    -Config $Config `
    -Generator $Generator `
    -QtPrefix $QtPrefix
