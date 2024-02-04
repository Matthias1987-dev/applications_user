write-host "Installation aktuelle Firmware:/ Installation current official Firmware" -ForegroundColor Green
$folder = Read-Host "Gib den Ordnernamen ein./ Name of the main Folder."
Write-Host "Der Ordner $folder wird erstellt./ Create new folder $folder." -ForegroundColor Green
git clone https://github.com/flipperdevices/flipperzero-firmware.git $folder
Write-Host "Download der Firmware fertig. Ordner $folder wurde erstellt./ Finish Download of the Firmware. New Folder $folder greatet" -ForegroundColor Green
Set-Variable -Name folder1 -Value (Get-Location)
cd .\$folder
Write-Host "Download application_user von Matthias1987 Repository." -ForegroundColor Green
Remove-Item .\applications_user -Recurse
git clone --recursive https://github.com/Matthias1987-dev/applications_user.git
Write-Host ".\fbt wird gestartet./ .\fbt ist starting." -ForegroundColor Green
.\fbt
Write-Host ".\fbt Fertig./ .\fbt finish." -ForegroundColor Green
Write-Host ".\fbt vscode_dist wird gestartet./ .\fbt vscode_dist starting." -ForegroundColor Green
.\fbt vscode_dist
Write-Host ".\fbt vscode_dist Fertig./ .\fbt vscode_dist finish" -ForegroundColor Green
$read = Read-Host "In VSC öffnen ? Mit j bestätigen!/ Open in VSC? press j."
if ($read -eq "j") {
    $fullPath = Resolve-Path (Get-Location)
    code $fullPath
}