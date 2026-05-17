#include "audio_interface.h"
#include "hal_audio.h"
#include "config.h"
#include <string.h>

typedef struct {
    audio_sample_t input_buffer[AUDIO_BUFFER_SIZE];
    audio_sample_t output_buffer[AUDIO_BUFFER_SIZE];
    uint32_t buffer_pos;
    bool streaming;
} audio_state_t;

static audio_state_t audio_state = {0};

int audio_init(void) {
    memset(&audio_state, 0, sizeof(audio_state));
    audio_state.streaming = false;

    // Initialize HAL layers
    if (hal_i2s_init() != 0) {
        return -1;
    }

    if (hal_codec_init() != 0) {
        return -1;
    }

    // Enable codec inputs and outputs
    hal_codec_enable_input();
    hal_codec_enable_output();
    hal_codec_set_volume(80);

    return 0;
}

void audio_enable(void) {
    audio_state.streaming = true;
    hal_i2s_start_dma();
}

void audio_disable(void) {
    audio_state.streaming = false;
    hal_i2s_stop_dma();
}

audio_sample_t* audio_get_input_buffer(void) {
    return audio_state.input_buffer;
}

audio_sample_t* audio_get_output_buffer(void) {
    return audio_state.output_buffer;
}

void audio_process_output(const audio_sample_t *samples, uint32_t count) {
    if (!samples || !audio_state.streaming) {
        return;
    }

    uint32_t copy_count = (count < AUDIO_BUFFER_SIZE) ? count : AUDIO_BUFFER_SIZE;
    memcpy(audio_state.output_buffer, samples, copy_count * sizeof(audio_sample_t));
}

uint32_t audio_get_input_samples(audio_sample_t *samples, uint32_t count) {
    if (!samples) {
        return 0;
    }

    uint32_t copy_count = (count < AUDIO_BUFFER_SIZE) ? count : AUDIO_BUFFER_SIZE;
    memcpy(samples, audio_state.input_buffer, copy_count * sizeof(audio_sample_t));

    return copy_count;
}

void audio_dma_callback(void) {
    if (audio_state.streaming) {
        // Process audio data here
        // Update input/output buffers based on DMA position
    }
}

bool audio_is_streaming(void) {
    return audio_state.streaming;
}
