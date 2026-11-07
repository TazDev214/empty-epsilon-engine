param([string]$BuildRoot = (Join-Path $env:TEMP 'veyra-engine-build'), [string]$PythonExecutable = 'python')
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
$root = [IO.Path]::GetFullPath($BuildRoot)
New-Item -ItemType Directory -Path $root -Force | Out-Null
$downloads = Join-Path $root 'downloads'
New-Item -ItemType Directory -Path $downloads -Force | Out-Null
function Download([string]$Url,[string]$Name) {
    $target = Join-Path $downloads $Name
    if (-not (Test-Path -LiteralPath $target)) { Invoke-WebRequest -Uri $Url -OutFile $target }
    return $target
}
$compiler = Download 'https://github.com/skeeto/w64devkit/releases/download/v2.0.0/w64devkit-x64-2.0.0.exe' 'w64devkit-x64-2.0.0.exe'
if (-not (Test-Path -LiteralPath (Join-Path $root 'w64devkit\bin\gcc.exe'))) {
    & $compiler '-y' ('-o' + $root) | Out-Null
    if ($LASTEXITCODE -ne 0) { throw 'Compiler archive extraction failed' }
}
$eeZip = Download 'https://github.com/daid/EmptyEpsilon/archive/45c3fd5a39f286572804c6724ad3d91be84a2a9f.zip' 'EmptyEpsilon-45c3fd5.zip'
if (-not (Test-Path -LiteralPath (Join-Path $root 'EmptyEpsilon'))) {
    Expand-Archive -LiteralPath $eeZip -DestinationPath $root
    Rename-Item -LiteralPath (Join-Path $root 'EmptyEpsilon-45c3fd5a39f286572804c6724ad3d91be84a2a9f') -NewName 'EmptyEpsilon'
}
$spZip = Download 'https://github.com/daid/SeriousProton/archive/af1daf430783483879f185b3f7efc3fc8ee1ffea.zip' 'SeriousProton-af1daf4.zip'
if (-not (Test-Path -LiteralPath (Join-Path $root 'SeriousProton-af1daf430783483879f185b3f7efc3fc8ee1ffea'))) {
    Expand-Archive -LiteralPath $spZip -DestinationPath $root
}
$sdlZip = Download 'https://www.libsdl.org/release/SDL2-devel-2.0.16-mingw.tar.gz' 'SDL2-devel-2.0.16-mingw.tar.gz'
if ((Get-FileHash -LiteralPath $sdlZip).Hash -ne '2BFE48628AA9635C12EAC7D421907E291525DE1D0B04B3BCA4A5BD6E6C881A6F') {
    Invoke-WebRequest -Uri 'https://www.libsdl.org/release/SDL2-devel-2.0.16-mingw.tar.gz' -OutFile ($sdlZip + '.partial')
    if ((Get-FileHash -LiteralPath ($sdlZip + '.partial')).Hash -ne '2BFE48628AA9635C12EAC7D421907E291525DE1D0B04B3BCA4A5BD6E6C881A6F') { throw 'SDL checksum mismatch' }
    Move-Item -LiteralPath ($sdlZip + '.partial') -Destination $sdlZip -Force
}
if (-not (Test-Path -LiteralPath (Join-Path $root 'SDL2-2.0.16\x86_64-w64-mingw32\lib\cmake\SDL2\sdl2-config.cmake'))) {
    & tar -xzf $sdlZip -C $root
    if ($LASTEXITCODE -ne 0) { throw 'SDL extraction failed' }
}
$python = $PythonExecutable
if (-not (Test-Path -LiteralPath (Join-Path $root 'python-tools\cmake\data\bin\cmake.exe'))) {
    & $python -m pip install --target (Join-Path $root 'python-tools') 'cmake==3.31.6' 'ninja==1.11.1.3'
    if ($LASTEXITCODE -ne 0) { throw 'CMake/Ninja preparation failed' }
}
Get-FileHash -LiteralPath $compiler,$eeZip,$spZip,$sdlZip | Select-Object Hash,Path
Write-Output ('BUILD_ROOT=' + $root)
