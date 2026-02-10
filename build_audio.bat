@echo off
echo Building PingPong with Audio (SDL2_mixer + SDL2 Headers)...

REM SDL2 paths
set SDL2_INCLUDE=C:/libs/SDL2-2.30.8/x86_64-w64-mingw32/include
set SDL2_LIB=C:/libs/SDL2-2.30.8/x86_64-w64-mingw32/lib

REM SDL2_mixer paths
set SDL2_MIXER_INCLUDE=C:/libs/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include
set SDL2_MIXER_LIB=C:/libs/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib

C:/mingw64/bin/g++.exe -O2 ^
  -I"C:/libs/SDL3-3.2.26/include" ^
  -I"C:/libs/SDL3-3.2.26/include/SDL3" ^
  -I"%SDL2_INCLUDE%" ^
  -I"%SDL2_INCLUDE%/SDL2" ^
  -I"%SDL2_MIXER_INCLUDE%" ^
  -I"%SDL2_MIXER_INCLUDE%/SDL2" ^
  main.cpp Game.cpp Paddle.cpp Ball.cpp AudioManager.cpp ^
  -L"C:/libs/SDL3-3.2.26/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib" ^
  -L"%SDL2_LIB%" ^
  -L"%SDL2_MIXER_LIB%" ^
  -lSDL3 -lSDL2 -lSDL2_mixer -lopengl32 ^
  -o PingPong_audio.exe

if %ERRORLEVEL% == 0 (
  echo Build successful!
  echo.
  echo Audio-enabled executable: PingPong_audio.exe
  echo Required DLLs: SDL3.dll, SDL2.dll, SDL2_mixer.dll
  echo.
  echo Copy these DLLs to run with audio:
  echo - SDL3.dll (already present)
  echo - SDL2.dll (from C:/libs/SDL2-2.30.8/x86_64-w64-mingw32/bin/)
  echo - SDL2_mixer.dll (already present)
) else (
  echo Build failed!
)

pause
