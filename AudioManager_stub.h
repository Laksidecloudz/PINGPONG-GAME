#pragma once
#include <string>

// Stub audio manager - does nothing but allows compilation
enum class UISound {
    CONFIRM = 0,
    CANCEL,
    SELECT,
    PAUSE,
    RESUME,
    OPEN_MENU,
    CLOSE_MENU,
    EQUIP,
    SAVED,
    SHOP,
    UNEQUIP,
    EXIT,
    COUNT
};

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }
    
    bool init() { return true; }
    void cleanup() {}
    void setUIVolume(int volume) {}
    int getUIVolume() const { return 96; }
    void setMuted(bool mute) {}
    bool isMuted() const { return false; }
    void playUI(UISound sound) {}
    void stopAll() {}
    bool isInitialized() const { return true; }

private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};
