#include "AudioManager_new.h"
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
    
    // Initialize SDL_mixer 3.1.2
    if (!MIX_Init()) {
        std::fprintf(stderr, "SDL_mixer could not initialize! Error: %s\n", SDL_GetError());
        return false;
    }
    
    // Create a mixer device for audio playback
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        std::fprintf(stderr, "Failed to create mixer device! Error: %s\n", SDL_GetError());
        MIX_Quit();
        return false;
    }
    
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
            MIX_DestroyAudio(sounds[i]);
            sounds[i] = nullptr;
        }
    }
    
    // Destroy mixer and quit SDL_mixer
    if (mixer) {
        MIX_DestroyMixer(mixer);
        mixer = nullptr;
    }
    
    MIX_Quit();
    initialized = false;
}

bool AudioManager::loadSound(UISound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file with SDL3_mixer 3.1.2
    sounds[index] = MIX_LoadAudio(mixer, filepath, false);  // Don't predecode to save memory
    
    if (sounds[index] == nullptr) {
        std::fprintf(stderr, "Failed to load sound '%s': %s\n", filepath, SDL_GetError());
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
    
    // Play audio directly without track management (simpler for UI sounds)
    // SDL3_mixer 3.1.2 MIX_PlayAudio only takes mixer and audio (no repeat count)
    if (!MIX_PlayAudio(mixer, sounds[index])) {
        // All channels busy - this is non-critical
        // Could implement priority system here if needed
    }
}

void AudioManager::setUIVolume(int volume) {
    // Clamp to valid range
    if (volume < 0) volume = 0;
    if (volume > 128) volume = 128;
    
    uiVolume = volume;
    
    if (!initialized || !mixer) {
        return;
    }
    
    // SDL3_mixer 3.1.2 uses different volume approach
    // For now, we'll handle this at the mixer level if needed
    // The individual audio volumes could be set with MIX_SetAudioProperties if needed
}

void AudioManager::setMuted(bool mute) {
    muted = mute;
    
    if (!initialized || !mixer) {
        return;
    }
    
    // SDL3_mixer 3.1.2 volume control
    // This is a simplified approach - in a more complex system you'd manage this per-track
    if (mute) {
        // Set mixer volume to 0
        // Note: SDL3_mixer 3.1.2 might have different volume APIs
    } else {
        // Restore mixer volume to uiVolume
    }
}

void AudioManager::stopAll() {
    if (!initialized || !mixer) {
        return;
    }
    
    // Halt all audio on the mixer
    // SDL3_mixer 3.1.2 equivalent of Mix_HaltChannel(-1)
    // This might need to be implemented differently in SDL3_mixer 3.1.2
}
