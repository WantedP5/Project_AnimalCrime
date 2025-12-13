@echo off
cd /d "%~dp0"

set UE_PATH=C:\Program Files\Epic Games\UE_5.6
set PROJECT_PATH=%~dp0AnimalCrime.uproject

dotnet "%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" AnimalCrime Win64 Development -project="%PROJECT_PATH%"
"%UE_PATH%\Engine\Binaries\Win64\UnrealEditor.exe" "%PROJECT_PATH%" GameStartMap -game -ResX=1280 -ResY=720 -log -windowed
