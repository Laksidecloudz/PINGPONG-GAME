#pragma once
#include "SDL3/SDL.h"
#include <string>
#include <vector>

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

// Audio sample data structure
struct AudioSample {
    Uint8* data;
    Uint32 length;
    SDL_AudioSpec spec;
    
    AudioSample() : data(nullptr), length(0) {}
    ~AudioSample() {
        if (data) SDL_free(data);
    }
};

// Playing sound instance
struct PlayingSound {
    AudioSample* sample;
    Uint32 position;
    float volume;
    bool active;
    
    PlayingSound() : sample(nullptr), position(0), volume(1.0f), active(false) {}
};

class AudioManager {
public:
    // Singleton access
    static AudioManager& getInstance();
    
    // Initialize SDL3 audio and load all sounds
    // Returns false if audio could not be initialized (game continues silently)
    bool init();
    
    // Cleanup all loaded sounds and quit SDL audio
    void cleanup();
    
    // Volume control (0.0-1.0)
    void setUIVolume(float volume);
    float getUIVolume() const { return uiVolume; }
    
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
    
    // Audio callback function
    static void audioCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount);
    
    // SDL3 audio objects
    SDL_AudioDeviceID audioDevice;
    SDL_AudioSpec audioSpec;
    SDL_AudioStream* audioStream;
    
    // Audio data
    AudioSample sounds[static_cast<int>(UISound::COUNT)];
    std::vector<PlayingSound> playingSounds;
    
    // State
    bool initialized = false;
    bool muted = false;
    float uiVolume = 0.75f;  // Default 75% volume
    
    // Audio processing
    void mixAudio(float* output, int samples);
    void mixSample(float* output, const AudioSample* sample, Uint32 position, float volume, int samples);
    
    // File paths for each sound (relative to executable)
    static const char* soundFiles[static_cast<int>(UISound::COUNT)];
};
