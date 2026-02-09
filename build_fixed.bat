@echo off
echo Building PingPong with Audio...

REM SDL3_mixer location (updated for your folder structure)
set SDL3_MIXER_INCLUDE=C:/Libs/SDL3_mixer-3.1.2/x86_64-w64-mingw32/include
set SDL3_MIXER_LIB=C:/Libs/SDL3_mixer-3.1.2/x86_64-w64-mingw32/lib

C:/mingw64/bin/g++.exe -O2 ^
  -I"C:/libs/SDL3-3.2.26/include" ^
  -I"C:/libs/SDL3-3.2.26/include/SDL3" ^
  -I"%SDL3_MIXER_INCLUDE%" ^
  -I"%SDL3_MIXER_INCLUDE%/SDL3_mixer" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp AudioManager.cpp ^
  -L"C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib" ^
  -L"%SDL3_MIXER_LIB%" ^
  -lSDL3 -lSDL3_mixer -lopengl32 ^
  -o PingPong.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
) else (
  echo Build failed!
)

pause
