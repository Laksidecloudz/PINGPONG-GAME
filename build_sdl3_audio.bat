@echo off
echo Building PingPong with SDL3-Only Audio System...

REM SDL3 paths only (no SDL2 dependencies)
set SDL3_INCLUDE=C:/libs/SDL3-3.2.26/include
set SDL3_LIB=C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib

C:/mingw64/bin/g++.exe -O2 ^
  -I"%SDL3_INCLUDE%" ^
  -I"%SDL3_INCLUDE%/SDL3" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp AudioManager_SDL3.cpp ^
  -L"%SDL3_LIB%" ^
  -lSDL3 -lopengl32 ^
  -o PingPong_sdl3_audio.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
  echo.
  echo SDL3-Only Audio executable: PingPong_sdl3_audio.exe
  echo Required DLLs: SDL3.dll (already present)
  echo.
  echo Features:
  echo - Custom SDL3 audio mixing (no SDL2_mixer dependency)
  echo - No header conflicts
  echo - Full UI sound support
  echo - Volume and mute controls
) else (
  echo Build failed!
)

pause
