@echo off
echo Building PingPong with Audio (SDL2_mixer - Stable)...

REM SDL2_mixer location
set SDL2_MIXER_INCLUDE=C:/libs/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include
set SDL2_MIXER_LIB=C:/libs/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib

C:/mingw64/bin/g++.exe -O2 ^
  -I"C:/libs/SDL3-3.2.26/include" ^
  -I"C:/libs/SDL3-3.2.26/include/SDL3" ^
  -I"%SDL2_MIXER_INCLUDE%" ^
  -I"%SDL2_MIXER_INCLUDE%/SDL2" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp AudioManager.cpp ^
  -L"C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib" ^
  -L"%SDL2_MIXER_LIB%" ^
  -lSDL3 -lSDL2_mixer -lopengl32 ^
  -o PingPong.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
  echo.
  echo SDL2_mixer DLLs copied: SDL2_mixer.dll
  echo Ready to run with full audio support!
) else (
  echo Build failed!
)

pause
