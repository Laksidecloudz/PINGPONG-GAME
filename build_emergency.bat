@echo off
echo Building PingPong with Audio (SDL2_mixer + SDL3)...

REM Need to download SDL2-devel-2.x.x-mingw for headers
REM For now, let's create a simple no-audio version to test the game

echo Building PingPong (No Audio)...

C:/mingw64/bin/g++.exe -O2 ^
  -I"C:/libs/SDL3-3.2.26/include" ^
  -I"C:/libs/SDL3-3.2.26/include/SDL3" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp ^
  -L"C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib" ^
  -lSDL3 -lopengl32 ^
  -o PingPong_no_audio.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
  echo Run: PingPong_no_audio.exe
  echo.
  echo To add audio later:
  echo 1. Download SDL2-devel-2.30.8-mingw.tar.gz
  echo 2. Extract to C:\libs\SDL2-2.30.8\
  echo 3. Update build script with SDL2 paths
) else (
  echo Build failed!
)

pause
