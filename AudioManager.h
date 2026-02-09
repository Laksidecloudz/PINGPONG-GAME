#pragma once
#include "SDL3_mixer/SDL_mixer.h"
#include <string>

// UI Sound effects from SoupTonic UI1 SFX Pack
enum class UISound {
    CONFIRM = 0,      // SFX_UI_Confirm.wav - Menu selection confirmed
    CANCEL,           // SFX_UI_Cancel.wav - Back/cancel action
    SELECT,           // SFX_UI_MenuSelections.wav - Navigate menu (hover/up/down)
    PAUSE,            // SFX_UI_Pause.wav - Game paused
    RESUME,           // SFX_UI_Resume.wav - Game resumed
    OPEN_MENU,        // SFX_UI_OpenMenu.wav - Open main menu
    CLOSE_MENU,       // SFX_UI_CloseMenu.wav - Close menu/quit
    EQUIP,            // SFX_UI_Equip.wav - Shield pickup (collectible)
    SAVED,            // SFX_UI_Saved.wav - Settings changed
    SHOP,             // SFX_UI_Shop.wav - Color selection menu
    UNEQUIP,          // SFX_UI_Unequip.wav - Shield consumed/used
    EXIT,             // SFX_UI_Exit.wav - Alternative quit sound
    COUNT             // Total number of UI sounds
};

class AudioManager {
public:
    // Singleton access
    static AudioManager& getInstance();
    
    // Initialize SDL_mixer and load all sounds
    // Returns false if audio could not be initialized (game continues silently)
    bool init();
    
    // Cleanup all loaded sounds and quit SDL_mixer
    void cleanup();
    
    // Volume control (0-128, where 128 is max)
    void setUIVolume(int volume);
    int getUIVolume() const { return uiVolume; }
    
    // Master mute toggle
    void setMuted(bool mute);
    bool isMuted() const { return muted; }
    
    // Play a UI sound effect (non-blocking, immediate)
    void playUI(UISound sound);
    
    // Stop all currently playing sounds
    void stopAll();
    
    // Check if audio is properly initialized
    bool isInitialized() const { return initialized; }

private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    
    // Load a single WAV file
    bool loadSound(UISound sound, const char* filepath);
    
    // Sound chunks storage
    Mix_Chunk* sounds[static_cast<int>(UISound::COUNT)] = { nullptr };
    
    // State
    bool initialized = false;
    bool muted = false;
    int uiVolume = 96;  // Default 75% volume (96/128)
    
    // File paths for each sound (relative to executable)
    static const char* soundFiles[static_cast<int>(UISound::COUNT)];
};
