# build.ps1
$blastemDir = "C:\Users\Mike\Documents\Blast EM\blastem-win64-1.0.0\blastem-win64-1.0.0"
$blastem    = Join-Path $blastemDir "blastem.exe"
$rom        = Join-Path $PWD "out\rom.bin"

& "$env:GDK\bin\make.exe" -f "$env:GDK\makefile.gen"

if ($LASTEXITCODE -eq 0) {
    # close any previous BlastEm instance
    Get-Process blastem -ErrorAction SilentlyContinue | Stop-Process
    Start-Process -FilePath $blastem -ArgumentList "`"$rom`"" -WorkingDirectory $blastemDir
} else {
    Write-Host "Build failed, not launching." -ForegroundColor Red
}