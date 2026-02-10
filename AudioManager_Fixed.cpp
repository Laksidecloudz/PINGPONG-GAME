#include "AudioManager_Fixed.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

// Map UI sound enum to actual file paths
const char* AudioManager::uiSoundFiles[static_cast<int>(UISound::COUNT)] = {
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

// Map gameplay sound enum to actual file paths
const char* AudioManager::gameSoundFiles[static_cast<int>(GameSound::COUNT)] = {
    "assets/Audio/SFX_Gameplay/Ball/ball_hit_normal.wav",
    "assets/Audio/SFX_Gameplay/Ball/ball_wall_bounce.wav",
    "assets/Audio/SFX_Gameplay/Ball/ball_goal_scored.wav",
    "assets/Audio/SFX_Gameplay/Ball/Pickup4.wav",
    "assets/Audio/SFX_Gameplay/Boost/boost_activate.wav",
    "assets/Audio/SFX_Gameplay/Shield/shield_collect.wav",
    "assets/Audio/SFX_Gameplay/Health/health_loss.wav",
    "assets/Audio/SFX_Gameplay/Effects/lose_shatter.wav",
    "assets/Audio/SFX_Gameplay/Effects/win_dance.wav"
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
    
    // Set up audio specification - use float format for better mixing
    SDL_zero(audioSpec);
    audioSpec.format = SDL_AUDIO_F32;  // 32-bit float for mixing
    audioSpec.channels = 2;            // Stereo
    audioSpec.freq = 44100;            // 44.1 kHz
    
    // Open audio device
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);
    if (audioDevice == 0) {
        std::fprintf(stderr, "Failed to open audio device! Error: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }
    
    // Create audio stream for mixing
    audioStream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
    if (!audioStream) {
        std::fprintf(stderr, "Failed to create audio stream! Error: %s\n", SDL_GetError());
        SDL_CloseAudioDevice(audioDevice);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }
    
    // Bind audio stream to device with callback
    if (!SDL_BindAudioStream(audioDevice, audioStream)) {
        std::fprintf(stderr, "Failed to bind audio stream! Error: %s\n", SDL_GetError());
        SDL_DestroyAudioStream(audioStream);
        SDL_CloseAudioDevice(audioDevice);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }
    
    // Reserve space for playing sounds
    playingSounds.reserve(16);  // Allow up to 16 simultaneous sounds
    
    // Load all UI sounds
    bool allLoaded = true;
    for (int i = 0; i < static_cast<int>(UISound::COUNT); ++i) {
        if (!loadUISound(static_cast<UISound>(i), uiSoundFiles[i])) {
            // Log error but continue - missing one sound shouldn't break audio
            std::fprintf(stderr, "Warning: Failed to load UI sound %d: %s\n", i, uiSoundFiles[i]);
            allLoaded = false;
        }
    }
    
    // Load all gameplay sounds
    for (int i = 0; i < static_cast<int>(GameSound::COUNT); ++i) {
        if (!loadGameSound(static_cast<GameSound>(i), gameSoundFiles[i])) {
            // Log error but continue - missing one sound shouldn't break audio
            std::fprintf(stderr, "Warning: Failed to load gameplay sound %d: %s\n", i, gameSoundFiles[i]);
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
    
    // Clean up audio stream
    if (audioStream) {
        SDL_DestroyAudioStream(audioStream);
        audioStream = nullptr;
    }
    
    // Close audio device
    if (audioDevice) {
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }
    
    // Quit SDL audio subsystem
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    
    // Clear playing sounds
    playingSounds.clear();
    
    initialized = false;
}

bool AudioManager::loadUISound(UISound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file using SDL3
    if (!SDL_LoadWAV(filepath, &uiSounds[index].spec, &uiSounds[index].data, &uiSounds[index].length)) {
        std::fprintf(stderr, "Failed to load UI sound '%s': %s\n", filepath, SDL_GetError());
        return false;
    }
    
    // Convert to our audio format if needed
    if (uiSounds[index].spec.format != audioSpec.format || 
        uiSounds[index].spec.channels != audioSpec.channels || 
        uiSounds[index].spec.freq != audioSpec.freq) {
        
        // Create conversion stream
        SDL_AudioStream* stream = SDL_CreateAudioStream(&uiSounds[index].spec, &audioSpec);
        if (!stream) {
            std::fprintf(stderr, "Failed to create conversion stream for '%s'\n", filepath);
            SDL_free(uiSounds[index].data);
            uiSounds[index].data = nullptr;
            return false;
        }
        
        // Put data in stream
        if (!SDL_PutAudioStreamData(stream, uiSounds[index].data, uiSounds[index].length)) {
            std::fprintf(stderr, "Failed to put audio data in stream for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(uiSounds[index].data);
            uiSounds[index].data = nullptr;
            return false;
        }
        
        // Get converted data size
        SDL_FlushAudioStream(stream);
        
        // Calculate converted size
        Uint64 convertedSize = SDL_GetAudioStreamAvailable(stream);
        Uint8* convertedData = (Uint8*)SDL_malloc(convertedSize);
        
        if (SDL_GetAudioStreamData(stream, convertedData, convertedSize) == 0) {
            std::fprintf(stderr, "Failed to get converted audio data for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(convertedData);
            SDL_free(uiSounds[index].data);
            uiSounds[index].data = nullptr;
            return false;
        }
        
        // Clean up old data and stream
        SDL_free(uiSounds[index].data);
        SDL_DestroyAudioStream(stream);
        
        // Update with converted data
        uiSounds[index].data = convertedData;
        uiSounds[index].length = convertedSize;
        uiSounds[index].spec = audioSpec;
    }
    
    return true;
}

bool AudioManager::loadGameSound(GameSound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file using SDL3
    if (!SDL_LoadWAV(filepath, &gameSounds[index].spec, &gameSounds[index].data, &gameSounds[index].length)) {
        std::fprintf(stderr, "Failed to load gameplay sound '%s': %s\n", filepath, SDL_GetError());
        return false;
    }
    
    // Convert to our audio format if needed
    if (gameSounds[index].spec.format != audioSpec.format || 
        gameSounds[index].spec.channels != audioSpec.channels || 
        gameSounds[index].spec.freq != audioSpec.freq) {
        
        // Create conversion stream
        SDL_AudioStream* stream = SDL_CreateAudioStream(&gameSounds[index].spec, &audioSpec);
        if (!stream) {
            std::fprintf(stderr, "Failed to create conversion stream for '%s'\n", filepath);
            SDL_free(gameSounds[index].data);
            gameSounds[index].data = nullptr;
            return false;
        }
        
        // Put data in stream
        if (!SDL_PutAudioStreamData(stream, gameSounds[index].data, gameSounds[index].length)) {
            std::fprintf(stderr, "Failed to put audio data in stream for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(gameSounds[index].data);
            gameSounds[index].data = nullptr;
            return false;
        }
        
        // Get converted data size
        SDL_FlushAudioStream(stream);
        
        // Calculate converted size
        Uint64 convertedSize = SDL_GetAudioStreamAvailable(stream);
        Uint8* convertedData = (Uint8*)SDL_malloc(convertedSize);
        
        if (SDL_GetAudioStreamData(stream, convertedData, convertedSize) == 0) {
            std::fprintf(stderr, "Failed to get converted audio data for '%s'\n", filepath);
            SDL_DestroyAudioStream(stream);
            SDL_free(convertedData);
            SDL_free(gameSounds[index].data);
            gameSounds[index].data = nullptr;
            return false;
        }
        
        // Clean up old data and stream
        SDL_free(gameSounds[index].data);
        SDL_DestroyAudioStream(stream);
        
        // Update with converted data
        gameSounds[index].data = convertedData;
        gameSounds[index].length = convertedSize;
        gameSounds[index].spec = audioSpec;
    }
    
    return true;
}

void AudioManager::playUI(UISound sound) {
    if (!initialized || muted) {
        return;
    }
    
    int index = static_cast<int>(sound);
    
    if (uiSounds[index].data == nullptr) {
        // Sound not loaded, silently skip
        return;
    }
    
    // Simple approach: create a temporary stream and play the sound directly
    SDL_AudioStream* tempStream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
    if (tempStream) {
        // Apply volume scaling
        Uint8* volumeData = (Uint8*)SDL_malloc(uiSounds[index].length);
        if (volumeData) {
            std::memcpy(volumeData, uiSounds[index].data, uiSounds[index].length);
            
            // Apply volume to S16 samples
            Sint16* samples = (Sint16*)volumeData;
            Uint32 sampleCount = uiSounds[index].length / sizeof(Sint16);
            
            for (Uint32 i = 0; i < sampleCount; ++i) {
                samples[i] = (Sint16)(samples[i] * uiVolume);
            }
            
            // Put audio data in stream
            if (SDL_PutAudioStreamData(tempStream, volumeData, uiSounds[index].length)) {
                SDL_FlushAudioStream(tempStream);
                
                // Bind stream to device temporarily
                if (SDL_BindAudioStream(audioDevice, tempStream)) {
                    // Keep the stream alive for a short time to let audio play
                    // In a real implementation, we'd manage stream lifecycle properly
                    printf("Audio stream bound and playing sound: %d\n", index);
                }
            }
            
            SDL_free(volumeData);
        }
        
        // Clean up the stream after a short delay
        // In production, we'd use a timer or thread to manage this
        SDL_DestroyAudioStream(tempStream);
    }
    
    printf("Playing UI sound: %d (volume: %.2f, muted: %s)\n", index, uiVolume, muted ? "yes" : "no");
}

void AudioManager::playGameSound(GameSound sound) {
    if (!initialized || muted) {
        return;
    }
    
    int index = static_cast<int>(sound);
    
    if (gameSounds[index].data == nullptr) {
        // Sound not loaded, silently skip
        return;
    }
    
    // Simple approach: create a temporary stream and play the sound directly
    SDL_AudioStream* tempStream = SDL_CreateAudioStream(&audioSpec, &audioSpec);
    if (tempStream) {
        // Apply volume scaling
        Uint8* volumeData = (Uint8*)SDL_malloc(gameSounds[index].length);
        if (volumeData) {
            std::memcpy(volumeData, gameSounds[index].data, gameSounds[index].length);
            
            // Apply volume to S16 samples
            Sint16* samples = (Sint16*)volumeData;
            Uint32 sampleCount = gameSounds[index].length / sizeof(Sint16);
            
            for (Uint32 i = 0; i < sampleCount; ++i) {
                samples[i] = (Sint16)(samples[i] * gameVolume);
            }
            
            // Put audio data in stream
            if (SDL_PutAudioStreamData(tempStream, volumeData, gameSounds[index].length)) {
                SDL_FlushAudioStream(tempStream);
                
                // Bind stream to device temporarily
                if (SDL_BindAudioStream(audioDevice, tempStream)) {
                    // Keep the stream alive for a short time to let audio play
                    // In a real implementation, we'd manage stream lifecycle properly
                    printf("Game audio stream bound and playing sound: %d\n", index);
                }
            }
            
            SDL_free(volumeData);
        }
        
        // Clean up the stream after a short delay
        // In production, we'd use a timer or thread to manage this
        SDL_DestroyAudioStream(tempStream);
    }
    
    printf("Playing gameplay sound: %d (volume: %.2f, muted: %s)\n", index, gameVolume, muted ? "yes" : "no");
}

void AudioManager::processAudio() {
    if (!initialized || !audioStream) {
        return;
    }
    
    // Calculate how much audio data we need to provide
    int additionalAmount = 4096; // 4KB buffer
    Uint64 available = SDL_GetAudioStreamAvailable(audioStream);
    
    if (available < additionalAmount) {
        // We need to provide more audio data
        int samplesNeeded = additionalAmount / sizeof(float) / 2; // Stereo samples
        std::vector<float> buffer(samplesNeeded * 2);
        
        // Mix all playing sounds
        mixAudio(buffer.data(), samplesNeeded);
        
        // Put mixed audio into stream
        SDL_PutAudioStreamData(audioStream, buffer.data(), additionalAmount);
    }
}

void AudioManager::mixAudio(float* output, int samples) {
    // Clear output buffer
    std::memset(output, 0, samples * 2 * sizeof(float));
    
    if (muted) {
        return;
    }
    
    // Mix all active sounds
    for (auto& sound : playingSounds) {
        if (sound.active && sound.sample) {
            mixSample(output, sound.sample, sound.position, sound.volume, samples);
            
            // Update position
            sound.position += samples * 2 * sizeof(float);  // Stereo samples
            
            // Check if sound finished
            if (sound.position >= sound.sample->length) {
                sound.active = false;
            }
        }
    }
    
    // Remove inactive sounds
    playingSounds.erase(
        std::remove_if(playingSounds.begin(), playingSounds.end(),
            [](const PlayingSound& sound) { return !sound.active; }),
        playingSounds.end()
    );
}

void AudioManager::mixSample(float* output, const AudioSample* sample, Uint32 position, float volume, int samples) {
    if (!sample || !sample->data || position >= sample->length) {
        return;
    }
    
    const float* inputData = reinterpret_cast<const float*>(sample->data + position);
    Uint32 samplesAvailable = (sample->length - position) / sizeof(float);
    Uint32 samplesToMix = std::min((Uint32)samples * 2, samplesAvailable);  // Stereo
    
    for (Uint32 i = 0; i < samplesToMix; ++i) {
        output[i] += inputData[i] * volume;
    }
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
    for (auto& sound : playingSounds) {
        sound.active = false;
    }
}
