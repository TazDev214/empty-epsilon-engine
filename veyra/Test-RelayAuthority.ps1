param([Parameter(Mandatory=$true)][string]$CompilerRoot)
$ErrorActionPreference = 'Stop'
$env:PATH = $CompilerRoot + ';' + $env:PATH
$testRoot = Join-Path ([IO.Path]::GetTempPath()) ('veyra-authority-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $testRoot | Out-Null
# Compile the real helper in isolation against the deliberately minimal PlayerInfo fixture.
Copy-Item (Join-Path $PSScriptRoot '../src/veyraCrew.h') $testRoot
Copy-Item (Join-Path $PSScriptRoot 'tests/playerInfo.h') $testRoot
$source = [IO.File]::ReadAllText((Join-Path $PSScriptRoot 'tests/relay_authority.cpp')).Replace('../../src/veyraCrew.h','veyraCrew.h')
[IO.File]::WriteAllText((Join-Path $testRoot 'relay_authority.cpp'), $source)
& (Join-Path $CompilerRoot 'g++.exe') '-std=c++17' (Join-Path $testRoot 'relay_authority.cpp') '-o' (Join-Path $testRoot 'relay_authority.exe')
if ($LASTEXITCODE -ne 0) { throw 'Authority test compilation failed' }
& (Join-Path $testRoot 'relay_authority.exe')
if ($LASTEXITCODE -ne 0) { throw 'Authority test failed' }
