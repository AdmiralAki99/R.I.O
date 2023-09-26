#pragma once
#include "parsecs.h"

// #include "JPEGDEC.h"

/**
 * @brief Enum class for different states of video playback.
 */
enum class VideoPlaybackState{
    STOPPED, /**< Video playback is stopped. */
    PLAYING, /**< Video playback is playing. */
    PAUSED /**< Video playback is paused. */
};

class VideoPlayer{
    private:
        // JPEGDEC decoder;
        TFT_eSPI* tft;
        VideoPlaybackState state;

    public:

};

