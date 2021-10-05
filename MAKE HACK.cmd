@echo off
set "base_dir=%~dp0"
set "source_rom=%base_dir%FE8_clean.gba"
set "target_rom=%base_dir%FEtest_MSS.gba"
set "main_event=%base_dir%ROM Buildfile.event"


echo Copying ROM
cd %base_dir%
copy "%source_rom%" "%target_rom%"

echo:
echo Assembling
cd "%base_dir%Event Assembler"
REM Core A FE8 "-output:%target_rom%" "-input:%main_event%"
ColorzCore A FE8 "-output:%target_rom%" "-input:%main_event%"

echo:
echo Done!
pause