<# build.ps1
   Self-bootstrapping MASM (ml/ml64) build for Windows using PowerShell.
   - Finds Visual Studio / Build Tools via vswhere (with fallbacks)
   - Calls vcvarsall.bat to load the toolchain env
   - Compiles and links a 64-bit console app from src\main.asm
#>

[CmdletBinding()]
param(
  [string]$Src    = "src\main.asm",
  [string]$OutDir = "build",
  [ValidateSet("x64","x86")][string]$Arch = "x64",
  [switch]$ShowToolsPath
)

$ErrorActionPreference = "Stop"

function Find-VSInstallation {
  # Prefer vswhere (installed with VS / Build Tools)
  $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
  if (Test-Path $vswhere) {
    $inst = & $vswhere -latest -products * `
      -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
      -property installationPath 2>$null
    if ($LASTEXITCODE -eq 0 -and $inst) { return $inst.Trim() }
  }

  # Fallback: common VS2022 locations (Community/Professional/Enterprise/BuildTools)
  $roots = @(
    Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Community",
    Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Professional",
    Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Enterprise",
    Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\BuildTools"
  )
  foreach ($r in $roots) {
    $vcvars = Join-Path $r "VC\Auxiliary\Build\vcvarsall.bat"
    if (Test-Path $vcvars) { return $r }
  }

  throw "Could not locate Visual Studio Build Tools. Install 'Desktop development with C++' or VS Build Tools."
}

# Resolve VS install and vcvarsall
$VSINST = Find-VSInstallation
$VcVarsAll = Join-Path $VSINST "VC\Auxiliary\Build\vcvarsall.bat"
if (-not (Test-Path $VcVarsAll)) {
  throw "vcvarsall.bat not found at: $VcVarsAll"
}

if ($ShowToolsPath) {
  Write-Host "[info] Using Visual Studio at: $VSINST"
}

# Ensure output dir exists
if (-not (Test-Path $OutDir)) {
  New-Item -ItemType Directory -Path $OutDir | Out-Null
}

# Choose ml/ml64 by arch
$ML = if ($Arch -eq "x86") { "ml.exe" } else { "ml64.exe" }

# Resolve output names
$srcFile = Resolve-Path $Src
$objFile = Join-Path $OutDir ("{0}.obj" -f [IO.Path]::GetFileNameWithoutExtension($srcFile))
$exeFile = Join-Path $OutDir ("{0}.exe" -f [IO.Path]::GetFileNameWithoutExtension($srcFile))

# Build steps to run under a single cmd.exe session (so vcvars env persists)
$cmdScript = @"
@echo off
call "$VcVarsAll" $Arch || exit /b 1
"$ML" /nologo /c /Fo "$objFile" "$srcFile" || exit /b 1
link /nologo /entry:main /subsystem:console /OUT:"$exeFile" "$objFile"  kernel32.lib || exit /b 1
"@

# Write to a temp .cmd and execute
$tmp = Join-Path $env:TEMP ("build_masm_{0}.cmd" -f ([Guid]::NewGuid().ToString("N")))
Set-Content -Path $tmp -Value $cmdScript -Encoding Ascii

try {
  & cmd.exe /c `"$tmp`"
  if ($LASTEXITCODE -ne 0) {
    throw "Build failed with exit code $LASTEXITCODE."
  }
  Write-Host ""
  Write-Host "Build succeeded: $exeFile"
}
finally {
  Remove-Item $tmp -ErrorAction SilentlyContinue
}
