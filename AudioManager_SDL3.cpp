#include "AudioManager_SDL3.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

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
    
    // Set up audio specification
    SDL_zero(audioSpec);
    audioSpec.format = SDL_AUDIO_F32;  // 32-bit float
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
    
    // Bind audio stream to device
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

bool AudioManager::loadSound(UISound sound, const char* filepath) {
    int index = static_cast<int>(sound);
    
    // Load WAV file using SDL3
    if (!SDL_LoadWAV(filepath, &sounds[index].spec, &sounds[index].data, &sounds[index].length)) {
        std::fprintf(stderr, "Failed to load sound '%s': %s\n", filepath, SDL_GetError());
        return false;
    }
    
    // Convert to our audio format if needed
    if (sounds[index].spec.format != SDL_AUDIO_F32 || 
        sounds[index].spec.channels != 2 || 
        sounds[index].spec.freq != 44100) {
        
        SDL_AudioStream* convertStream = SDL_CreateAudioStream(&sounds[index].spec, &audioSpec);
        if (!convertStream) {
            std::fprintf(stderr, "Failed to create conversion stream for '%s'\n", filepath);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Put data in stream
        if (!SDL_PutAudioStreamData(convertStream, sounds[index].data, sounds[index].length)) {
            std::fprintf(stderr, "Failed to put audio data in stream for '%s'\n", filepath);
            SDL_DestroyAudioStream(convertStream);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Get converted data
        SDL_FlushAudioStream(convertStream);
        
        // Calculate converted size
        Uint64 convertedSize = SDL_GetAudioStreamAvailable(convertStream);
        Uint8* convertedData = (Uint8*)SDL_malloc(convertedSize);
        
        if (SDL_GetAudioStreamData(convertStream, convertedData, convertedSize) == 0) {
            std::fprintf(stderr, "Failed to get converted audio data for '%s'\n", filepath);
            SDL_DestroyAudioStream(convertStream);
            SDL_free(convertedData);
            SDL_free(sounds[index].data);
            sounds[index].data = nullptr;
            return false;
        }
        
        // Clean up old data and stream
        SDL_free(sounds[index].data);
        SDL_DestroyAudioStream(convertStream);
        
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
    
    // Find an inactive slot or create a new one
    PlayingSound* slot = nullptr;
    for (auto& playingSound : playingSounds) {
        if (!playingSound.active) {
            slot = &playingSound;
            break;
        }
    }
    
    // If no inactive slot, add a new one (but limit total sounds)
    if (!slot && playingSounds.size() < 16) {
        playingSounds.emplace_back();
        slot = &playingSounds.back();
    }
    
    if (slot) {
        slot->sample = &sounds[index];
        slot->position = 0;
        slot->volume = uiVolume;
        slot->active = true;
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

void AudioManager::audioCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
    AudioManager* manager = static_cast<AudioManager*>(userdata);
    
    if (!manager || !manager->initialized) {
        return;
    }
    
    // Calculate how many samples we need
    int samplesNeeded = additional_amount / sizeof(float) / 2;  // Stereo samples
    
    // Create temporary buffer
    std::vector<float> buffer(samplesNeeded * 2);
    
    // Mix all playing sounds
    manager->mixAudio(buffer.data(), samplesNeeded);
    
    // Put mixed audio into stream
    SDL_PutAudioStreamData(stream, buffer.data(), additional_amount);
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
