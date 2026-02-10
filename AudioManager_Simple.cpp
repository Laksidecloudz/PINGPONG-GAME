#include "AudioManager_Simple.h"
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
    
    // Initialize SDL audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::fprintf(stderr, "SDL audio could not initialize! Error: %s\n", SDL_GetError());
        return false;
    }
    
    // Set up audio specification - use simple format
    SDL_zero(audioSpec);
    audioSpec.format = SDL_AUDIO_S16;  // 16-bit integer (more compatible)
    audioSpec.channels = 2;            // Stereo
    audioSpec.freq = 44100;            // 44.1 kHz
    
    // Open audio device
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
    if (audioDevice == 0) {
        std::fprintf(stderr, "Failed to open audio device! Error: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
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
    
    // Resume audio device
    SDL_ResumeAudioDevice(audioDevice);
    
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
    
    // Stop audio device
    SDL_PauseAudioDevice(audioDevice);
    
    // Close audio device
    if (audioDevice) {
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }
    
    // Quit SDL audio subsystem
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    
    initialized = false;
}

bool AudioManager::loadSound(UISound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file using SDL3
    if (!SDL_LoadWAV(filepath, &sounds[index].spec, &sounds[index].data, &sounds[index].length)) {
        std::fprintf(stderr, "Failed to load sound '%s': %s\n", filepath, SDL_GetError());
        return false;
    }
    
    // Convert to our audio format if needed
    if (sounds[index].spec.format != audioSpec.format || 
        sounds[index].spec.channels != audioSpec.channels || 
        sounds[index].spec.freq != audioSpec.freq) {
        
        // Create conversion stream
        SDL_AudioStream* stream = SDL_CreateAudioStream(&sounds[index].spec, &audioSpec);
        if (!stream) {
            std::fprintf(stderr, "Failed to create conversion stream for '%s'\n", filepath);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Put data in stream
        if (!SDL_PutAudioStreamData(stream, sounds[index].data, sounds[index].length)) {
            std::fprintf(stderr, "Failed to put audio data in stream for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Get converted data size
        SDL_FlushAudioStream(stream);
        Uint64 convertedSize = SDL_GetAudioStreamAvailable(stream);
        Uint8* convertedData = (Uint8*)SDL_malloc(convertedSize);
        
        if (SDL_GetAudioStreamData(stream, convertedData, convertedSize) == 0) {
            std::fprintf(stderr, "Failed to get converted audio data for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(convertedData);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Clean up old data and stream
        SDL_free(sounds[index].data);
        SDL_DestroyAudioStream(stream);
        
        // Update with converted data
        sounds[index].data = convertedData;
        sounds[index].length = convertedSize;
        sounds[index].spec = audioSpec;
    }
    
    return true;
}

void AudioManager::playUI(UISound sound) {
    if (!initialized || muted) {
        return;
    }
    
    int index = static_cast<int>(sound);
    
    if (sounds[index].data == nullptr) {
        // Sound not loaded, silently skip
        return;
    }
    
    // Simple queue audio - let SDL3 handle the mixing
    // Apply volume by scaling the audio data
    Uint32 sampleCount = sounds[index].length / sizeof(Sint16);  // S16 format
    Sint16* audioData = (Sint16*)sounds[index].data;
    
    // Apply volume
    for (Uint32 i = 0; i < sampleCount; ++i) {
        audioData[i] = (Sint16)(audioData[i] * uiVolume);
    }
    
    // Create a simple audio stream for playback
    SDL_AudioStream* stream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
    if (stream) {
        // Put the audio data in the stream
        if (SDL_PutAudioStreamData(stream, sounds[index].data, sounds[index].length)) {
            SDL_FlushAudioStream(stream);
            
            // Get the data and play it
            Uint64 available = SDL_GetAudioStreamAvailable(stream);
            if (available > 0) {
                Uint8* tempBuffer = (Uint8*)SDL_malloc(available);
                if (tempBuffer && SDL_GetAudioStreamData(stream, tempBuffer, available) > 0) {
                    // Apply volume scaling
                    Sint16* samples = (Sint16*)tempBuffer;
                    Uint32 sampleCount = available / sizeof(Sint16);
                    
                    for (Uint32 i = 0; i < sampleCount; ++i) {
                        samples[i] = (Sint16)(samples[i] * uiVolume);
                    }
                    
                    // Play through a temporary stream bound to the device
                    SDL_AudioStream* playStream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
                    if (playStream) {
                        if (SDL_BindAudioStream(audioDevice, playStream)) {
                            SDL_PutAudioStreamData(playStream, tempBuffer, available);
                            SDL_FlushAudioStream(playStream);
                        }
                        // Clean up after a short delay
                        SDL_DestroyAudioStream(playStream);
                    }
                }
                SDL_free(tempBuffer);
            }
        }
        SDL_DestroyAudioStream(stream);
    }
    
    printf("Playing sound: %d (volume: %.2f, muted: %s)\n", index, uiVolume, muted ? "yes" : "no");
}

void AudioManager::setUIVolume(float volume) {
    // Clamp to valid range
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    uiVolume = volume;
}

void AudioManager::setMuted(bool mute) {
    muted = mute;
}

void AudioManager::stopAll() {
    if (initialized && audioDevice) {
        // For SDL3, pause the device to stop audio
        SDL_PauseAudioDevice(audioDevice);
        SDL_ResumeAudioDevice(audioDevice);
    }
}
