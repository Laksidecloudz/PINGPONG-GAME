@echo off
echo Building PingPong with Simple SDL3 Audio...

REM SDL3 paths only
set SDL3_INCLUDE=C:/libs/SDL3-3.2.26/include
set SDL3_LIB=C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib

C:/mingw64/bin/g++.exe -O2 ^
  -I"%SDL3_INCLUDE%" ^
  -I"%SDL3_INCLUDE%/SDL3" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp AudioManager_Simple.cpp ^
  -L"%SDL3_LIB%" ^
  -lSDL3 -lopengl32 ^
  -o PingPong_simple_audio.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
  echo.
  echo Simple Audio executable: PingPong_simple_audio.exe
  echo Required DLLs: SDL3.dll (already present)
  echo.
  echo Features:
  echo - Simple SDL3 audio queuing (no callbacks)
  echo - No freezing issues
  echo - Basic volume control
  echo - All UI sounds loaded
) else (
  echo Build failed!
)

pause
