# Audio System Integration Guide

## Overview
AudioManager provides centralized sound management using SDL3_mixer. It implements a singleton pattern for global access throughout the game.

## Files Added
- `AudioManager.h` - Header with sound enum and class interface
- `AudioManager.cpp` - Implementation with SDL3_mixer integration

## Sound Assets Required
Place your SoupTonic UI SFX Pack in:
```
assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/
```

Required files:
- `SFX_UI_Confirm.wav` - Menu selection confirmed
- `SFX_UI_Cancel.wav` - Back/cancel action  
- `SFX_UI_MenuSelections.wav` - Menu navigation
- `SFX_UI_Pause.wav` - Game paused
- `SFX_UI_Resume.wav` - Game resumed
- `SFX_UI_OpenMenu.wav` - Open menu
- `SFX_UI_CloseMenu.wav` - Close menu
- `SFX_UI_Equip.wav` - Shield pickup (collectible mode)
- `SFX_UI_Saved.wav` - Settings changed
- `SFX_UI_Shop.wav` - Color selection menu
- `SFX_UI_Unequip.wav` - Shield consumed/used
- `SFX_UI_Exit.wav` - Quit game

## SDL3_mixer Setup

### 1. Download SDL3_mixer
Get the SDL3_mixer development library from:
https://github.com/libsdl-org/SDL_mixer/releases

### 2. File Placement
Place these files in your project directory (same location as SDL3.dll):
- `SDL3_mixer.dll` - Runtime library (REQUIRED)
- `SDL3_mixer.lib` - Link library (for compilation)
- Header files in your include path

### 3. Build Configuration

#### For g++/MinGW:
Add to your compile command:
```bash
g++ -o PingPong main.cpp Game.cpp Ball.cpp Paddle.cpp AudioManager.cpp -lSDL3 -lSDL3_mixer -lopengl32
```

#### For VS Code tasks.json:
Add to your linker args:
```json
"args": [
    "-lSDL3",
    "-lSDL3_mixer",
    "-lopengl32"
]
```

## Usage

### Basic Play
```cpp
// Play a UI sound
AudioManager::getInstance().playUI(UISound::CONFIRM);
```

### Volume Control
```cpp
// Set UI volume (0-128)
AudioManager::getInstance().setUIVolume(96);

// Mute/Unmute
AudioManager::getInstance().setMuted(true);
```

### Adding to Game Loop
Already integrated in Game.cpp:
- Menu navigation: SELECT sound
- Menu confirm: CONFIRM sound
- Menu cancel: CANCEL sound
- Pause: PAUSE sound
- Resume: RESUME sound
- Settings toggle: SAVED sound

## Integration Points

The following locations in Game.cpp trigger sounds:

| Location | Sound | Trigger |
|----------|-------|---------|
| Start screen → Main menu | OPEN_MENU | Enter key / Mouse click |
| Menu navigation | SELECT | Up/Down arrows |
| Menu confirm | CONFIRM | Enter on menu item |
| Menu back | CANCEL | Escape key |
| Pause game | PAUSE | 'P' key / Esc in game |
| Resume game | RESUME | Select "Resume" |
| Settings change | SAVED | Toggle any setting |
| Color menu | SHOP | Enter color selection |
| Quit game | EXIT | Select "Quit" |

## Error Handling
- Audio initialization failure is non-critical
- Game continues silently if sounds fail to load
- Missing individual sounds are logged but don't crash

## Future Extensions

### Adding Gameplay Sounds
Extend the enum in AudioManager.h:
```cpp
enum class GameplaySound {
    PADDLE_HIT,
    WALL_HIT,
    SCORE_POINT,
    // ... etc
};
```

### Music Support
SDL3_mixer supports music via Mix_Music:
```cpp
Mix_Music* bgm = Mix_LoadMUS("assets/Music/background.ogg");
Mix_PlayMusic(bgm, -1);  // -1 = loop forever
```

## Troubleshooting

### No sound playing
1. Check SDL3_mixer.dll is in the same folder as the .exe
2. Verify WAV files are in correct path
3. Check volume isn't muted: `AudioManager::getInstance().setMuted(false)`
4. View console output for loading errors

### Build errors
- Ensure SDL3_mixer.h is in include path
- Link against SDL3_mixer.lib (not just SDL3)
- Use SDL3_mixer (not SDL2_mixer) for SDL3 projects
