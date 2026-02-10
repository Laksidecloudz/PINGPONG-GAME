@echo off
echo Testing Audio System...
echo.
echo Run: PingPong_sdl3_audio.exe
echo.
echo Test these actions to verify audio:
echo 1. Navigate menus (should hear selection sounds)
echo 2. Confirm menu choice (confirm sound)
echo 3. Cancel/go back (cancel sound)
echo 4. Pause game (pause sound)
echo 5. Resume game (resume sound)
echo 6. Pick up shield (equip sound)
echo 7. Use shield (unequip sound)
echo 8. Win/lose game (confirm/cancel sounds)
echo.
echo Settings menu:
echo - Toggle sound on/off
echo - Adjust volume (0-100%%)
echo.
echo If no sound plays:
echo - Check Windows volume
echo - Check that SDL3.dll is present
echo - Look for console error messages
echo.
pause
