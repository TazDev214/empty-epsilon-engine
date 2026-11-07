# Star Wars – Der Veyra-Korridor

Engine-Fork für die Veyra-LAN-Kampagne, basierend auf [Empty Epsilon](https://github.com/daid/EmptyEpsilon), Release **EE-2024.12.08**, Commit `45c3fd5a39f286572804c6724ad3d91be84a2a9f`.

Entwicklung auf Branch **veyra**. Missionsentwürfe, Lua-Missionen und zusätzliche Veyra-Assets liegen getrennt im [Missionsprojekt](https://github.com/TazDev214/empty-epsilon).

## Anpassungen

- Rollenprofile für drei/vier Personen: Science + Relay für den Kapitän, Tactical und Engineering+ bei Dreiercrews, Helms/Weapons/Engineering bei Vierercrews.
- Database, Ship Log, Comms und lesende Strategic Map für alle Rollen.
- Gemeinsame Funkanzeige; nur Clients mit Relay-Rolle auf demselben Schiff dürfen Gespräche steuern und beantworten. Prüfung auch auf dem Server; Antworten auf veraltete Dialogstände werden verworfen.
- Strategic Map ohne Relay-Aktionsschaltflächen, Alarmsteuerung und Skriptaktionen.
- Eigener Startbildschirm, Credits und Filter auf sieben Veyra-Testszenarien.
- Windows-Protokolldatei und optionaler separater Testport.

Die Rollenprofile werden lokal gewählt; exklusive Rollenplätze sind noch nicht erzwungen. Zwei Kapitänsclients hätten beide Antwortrechte. Die Crew vergibt die Rolle einmal.

## Windows bauen

Voraussetzungen: Git, PowerShell 7, Python 3 mit pip, tar und Internetzugang. Den Branch vollständig klonen, nicht nur einzelne Dateien oder einen Sparse-Checkout.

```powershell
git clone --branch veyra https://github.com/TazDev214/empty-epsilon-engine.git
cd empty-epsilon-engine
./veyra/Build-Windows.ps1
```

Das Skript lädt w64devkit 2.0.0, SDL 2.0.16, CMake 3.31.6, Ninja 1.11.1.3 und den passenden SeriousProton-Commit `af1daf430783483879f185b3f7efc3fc8ee1ffea`. SDL wird per SHA-256 geprüft. Die Quellen dieses Checkouts werden gebaut; die ebenfalls geladene EE-Basis bleibt als Referenz im Buildverzeichnis. Standard ist ein Debug-Build ohne Steam, Discord und Crashlogger. Bauwerkzeuge werden nicht systemweit installiert.

Parameter: `-BuildRoot C:/Temp/veyra-fork-build` und bei Bedarf `-PythonExecutable C:/Pfad/python.exe`. Ausgabe: `<BuildRoot>/build/EmptyEpsilon.exe`.

Die Engine allein ist kein vollständiges Veyra-Spielpaket: Ressourcen, Moddaten und freigegebene Missionen müssen passend zusammengestellt werden. Der Szenarienfilter zeigt nur vorhandene, zugelassene Veyra-Dateien. Netzwerkkennung **2024.12.108** unterscheidet den Fork vom Original; alle Clients und der Server benötigen denselben Build.

## Validierung und Grenzen

Der übernommene Quellstand wurde am 07.11.2026 unter Windows gebaut. Sieben vorhandene Veyra-Szenarien bestanden Startup-Kurztests ohne erkannten Lua-Fehler. Sichtbar geprüft: Titel, Credits, Szenarienfilter, Rollenprofil und lesende Strategic Map. Ein isolierter Berechtigungstest ist mit `./veyra/Test-RelayAuthority.ps1 -CompilerRoot <w64devkit>/bin` ausführbar.

Mehrclientbetrieb, Internetbeitritt und vollständige Kampagnendurchläufe sind noch nicht abgenommen. Noch keine veröffentlichte spielbereite Release-Ausgabe.

## Credits und Veröffentlichung

Projekt: **Okko**. Ausarbeitung und Entwicklung mit **Codex** und **ChatGPT**. Belegte Modellangaben: [Arbeitslog](veyra/MODELLE-ARBEITSLOG.md); unbekannte historische Angaben bleiben offen. Original-Credits bleiben erhalten.

Die bestehende [GPL-Lizenz](LICENSE) der Engine bleibt erhalten. Dieses Repository enthält keine zusätzlich importierten Veyra-Schiffsmodelle und keine Veyra-Musik. Deren Weitergabe wird vor Veröffentlichung eines vollständigen Spielpakets separat geprüft. Fertige Binärpakete sollen später über GitHub Releases verteilt werden.
