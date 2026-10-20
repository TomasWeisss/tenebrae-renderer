# Loads the MSVC x64 environment if it isn't already, then optionally runs a command.
#   . .\scripts\dev.ps1                      set up the current terminal session
#   .\scripts\dev.ps1 cmake --preset debug   set up, run the command, exit with its code
# PowerShell splits forwarded args at a colon after a leading dash (-DX=C:/y becomes
# -DX=C /y), so pass cache vars as two args: -D X=C:/y
if ($MyInvocation.InvocationName -eq '.' -and $args.Count -gt 0) {
    # The exit below would close the caller's session
    Write-Error "Don't pass a command when dot-sourcing; run '.\scripts\dev.ps1 <command>' instead"
    return
}

if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    # VsDevCmd calls vswhere.exe by bare name, so the installer dir must be on PATH
    $installerDir = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer"
    $env:PATH += ";$installerDir"
    $vsDir = & "$installerDir\vswhere.exe" -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsDir) {
        Write-Error "No Visual Studio installation with the MSVC x64 toolset found" -ErrorAction Stop
    }
    & "$vsDir\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64 -SkipAutomaticLocation
}

if ($args.Count -gt 0) {
    $rest = if ($args.Count -gt 1) { $args[1..($args.Count - 1)] } else { @() }
    & $args[0] @rest
    exit $LASTEXITCODE
}
