#pragma once

#include <TFT_eSPI.h>
#include "JPEGDEC.h"

/**
 * @brief Enum class for different states of video playback.
 */
enum class VideoPlaybackState{
    STOPPED, /**< Video playback is stopped. */
    PLAYING, /**< Video playback is playing. */
    PAUSED /**< Video playback is paused. */
};

