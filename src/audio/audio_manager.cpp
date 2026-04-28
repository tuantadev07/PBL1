#include "audio/audio_manager.h"

#include "raylib.h"

void audio_manager_init() {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
}

void audio_manager_update(float dt) {
    (void)dt;
}

void audio_manager_shutdown() {
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}
