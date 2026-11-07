param([string]$BuildRoot = (Join-Path $env:TEMP 'veyra-engine-build'), [switch]$ConfigureOnly, [switch]$Veyra, [string]$SourceDir)
$ErrorActionPreference = 'Stop'
$root = [IO.Path]::GetFullPath($BuildRoot)
$env:PATH = (Join-Path $root 'w64devkit\bin') + ';' + $env:PATH
$cmake = Join-Path $root 'python-tools\cmake\data\bin\cmake.exe'
$ninja = Join-Path $root 'python-tools\bin\ninja.exe'
$source = Join-Path $root 'EmptyEpsilon'
if ($SourceDir) { $source = [IO.Path]::GetFullPath($SourceDir) }
$build = Join-Path $root 'build'
$sdlPrefix = (Join-Path $root 'SDL2-2.0.16\x86_64-w64-mingw32').Replace('\','/')
$sdlConfig = Join-Path $root 'SDL2-2.0.16\x86_64-w64-mingw32\lib\cmake\SDL2\sdl2-config.cmake'
$sdlText = [IO.File]::ReadAllText($sdlConfig).Replace('/opt/local/x86_64-w64-mingw32', $sdlPrefix)
[IO.File]::WriteAllText($sdlConfig, $sdlText, [Text.UTF8Encoding]::new($false))
$versionPatch = if ($Veyra) { '108' } else { '8' }
& $cmake -S $source -B $build -G Ninja "-DCMAKE_MAKE_PROGRAM=$($ninja.Replace('\','/'))" `
    "-DCMAKE_C_COMPILER=$((Join-Path $root 'w64devkit\bin\gcc.exe').Replace('\','/'))" `
    "-DCMAKE_CXX_COMPILER=$((Join-Path $root 'w64devkit\bin\g++.exe').Replace('\','/'))" `
    "-DCMAKE_RC_COMPILER=$((Join-Path $root 'w64devkit\bin\windres.exe').Replace('\','/'))" `
    "-DSERIOUS_PROTON_DIR=$(Join-Path $root 'SeriousProton-af1daf430783483879f185b3f7efc3fc8ee1ffea')" `
    "-DSDL2_DIR=$(Join-Path $root 'SDL2-2.0.16\x86_64-w64-mingw32\lib\cmake\SDL2')" `
    '-DWITH_DISCORD=OFF' '-DENABLE_CRASH_LOGGER=OFF' '-DCMAKE_BUILD_TYPE=Debug' `
    '-DCPACK_PACKAGE_VERSION_MAJOR=2024' '-DCPACK_PACKAGE_VERSION_MINOR=12' "-DCPACK_PACKAGE_VERSION_PATCH=$versionPatch"
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed' }
if (-not $ConfigureOnly) {
    & $cmake --build $build --parallel 4
    if ($LASTEXITCODE -ne 0) { throw 'Engine build failed' }
}
