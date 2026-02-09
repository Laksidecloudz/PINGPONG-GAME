#include "AudioManager.h"
#include <cstdio>

// Map sound enum to actual file paths
const char* AudioManager::soundFiles[static_cast<int>(UISound::COUNT)] = {
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Confirm.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Cancel.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_MenuSelections.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Pause.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Resume.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_OpenMenu.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_CloseMenu.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Equip.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Saved.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Shop.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Unequip.wav",
    "assets/Music/SoupTonic UI1 SFX Pack 1 - wav (menu music)/SFX_UI_Exit.wav"
};

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    if (initialized) {
        return true;
    }
    
    // Initialize SDL_mixer with standard settings
    // 44100 Hz, default format, 2 channels (stereo), 2048 byte chunks
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::fprintf(stderr, "SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
        return false;
    }
    
    // Allocate 16 mixing channels (sounds that can play simultaneously)
    Mix_AllocateChannels(16);
    
    // Set default volume for all channels
    Mix_Volume(-1, uiVolume);
    
    // Load all UI sounds
    bool allLoaded = true;
    for (int i = 0; i < static_cast<int>(UISound::COUNT); ++i) {
        if (!loadSound(static_cast<UISound>(i), soundFiles[i])) {
            // Log error but continue - missing one sound shouldn't break audio
            std::fprintf(stderr, "Warning: Failed to load sound %d: %s\n", i, soundFiles[i]);
            allLoaded = false;
        }
    }
    
    initialized = true;
    
    // Even if some sounds failed, we can still use the ones that loaded
    if (!allLoaded) {
        std::fprintf(stderr, "AudioManager: Some sounds failed to load, but continuing...\n");
    }
    
    return true;
}

void AudioManager::cleanup() {
    if (!initialized) {
        return;
    }
    
    // Free all loaded sounds
    for (int i = 0; i < static_cast<int>(UISound::COUNT); ++i) {
        if (sounds[i] != nullptr) {
            Mix_FreeChunk(sounds[i]);
            sounds[i] = nullptr;
        }
    }
    
    // Close SDL_mixer
    Mix_CloseAudio();
    
    initialized = false;
}

bool AudioManager::loadSound(UISound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file
    sounds[index] = Mix_LoadWAV(filepath);
    
    if (sounds[index] == nullptr) {
        std::fprintf(stderr, "Failed to load sound '%s': %s\n", filepath, Mix_GetError());
        return false;
    }
    
    return true;
}

void AudioManager::playUI(UISound sound) {
    if (!initialized || muted) {
        return;
    }
    
    int index = static_cast<int>(sound);
    
    if (sounds[index] == nullptr) {
        // Sound not loaded, silently skip
        return;
    }
    
    // Play on first available channel (-1), don't loop (0)
    int channel = Mix_PlayChannel(-1, sounds[index], 0);
    
    if (channel < 0) {
        // All channels busy - this is non-critical
        // Could implement priority system here if needed
    }
}

void AudioManager::setUIVolume(int volume) {
    // Clamp to valid range
    if (volume < 0) volume = 0;
    if (volume > 128) volume = 128;
    
    uiVolume = volume;
    
    if (!initialized) {
        return;
    }
    
    // Set volume for all channels
    Mix_Volume(-1, muted ? 0 : uiVolume);
}

void AudioManager::setMuted(bool mute) {
    muted = mute;
    
    if (!initialized) {
        return;
    }
    
    Mix_Volume(-1, muted ? 0 : uiVolume);
}

void AudioManager::stopAll() {
    if (!initialized) {
        return;
    }
    
    // Halt all channels
    Mix_HaltChannel(-1);
}
