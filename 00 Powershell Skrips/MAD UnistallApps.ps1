# Wartezeit von 1 Sekunde
Start-Sleep -Seconds 1
# Deinstalliere WhatsApp App
Get-AppxPackage *whatsapp* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Spotify App
Get-AppxPackage *spotify* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere LinkedIn App
Get-AppxPackage *linkedin* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Cortana App
Get-AppxPackage *cortana* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Filme & TV App
Get-AppxPackage *filme&tv* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Karten App
Get-AppxPackage *karten* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Kontakte App
Get-AppxPackage *kontakte* | Remove-AppxPackage
Start-Sleep -Seconds 1
# Deinstalliere Kurznotizen App
Get-AppxPackage *kurznotizen* | Remove-AppxPackage