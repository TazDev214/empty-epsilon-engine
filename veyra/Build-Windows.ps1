param(
    [string]$BuildRoot = (Join-Path $env:TEMP 'veyra-fork-build'),
    [string]$PythonExecutable = 'python'
)
$ErrorActionPreference = 'Stop'
$sourceRoot = Split-Path -Parent $PSScriptRoot
& (Join-Path $PSScriptRoot 'bootstrap.ps1') -BuildRoot $BuildRoot -PythonExecutable $PythonExecutable
& (Join-Path $PSScriptRoot 'build.ps1') -BuildRoot $BuildRoot -SourceDir $sourceRoot -Veyra
