#ifndef AUDIO_INTERFACE_H
#define AUDIO_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

/**
 * @brief Initialize audio interface
 * @return 0 on success, -1 on error
 */
int audio_init(void);

/**
 * @brief Enable audio streaming (starts I2S/DMA)
 */
void audio_enable(void);

/**
 * @brief Disable audio streaming (stops I2S/DMA)
 */
void audio_disable(void);

/**
 * @brief Check if audio is currently streaming
 * @return true if streaming active, false otherwise
 */
bool audio_is_streaming(void);

/**
 * @brief Get pointer to input buffer (recording/line-in)
 * @return Pointer to audio_sample_t array
 */
audio_sample_t* audio_get_input_buffer(void);

/**
 * @brief Get pointer to output buffer (playback/line-out)
 * @return Pointer to audio_sample_t array
 */
audio_sample_t* audio_get_output_buffer(void);

/**
 * @brief Process incoming audio samples (from USB to codec output)
 * @param samples Pointer to audio samples
 * @param count Number of samples
 */
void audio_process_output(const audio_sample_t *samples, uint32_t count);

/**
 * @brief Get recorded audio samples (from codec input)
 * @param samples Pointer to destination buffer
 * @param count Number of samples to retrieve
 * @return Number of samples actually retrieved
 */
uint32_t audio_get_input_samples(audio_sample_t *samples, uint32_t count);

/**
 * @brief DMA callback when buffer is half-complete
 */
void audio_dma_half_complete_callback(void);

/**
 * @brief DMA callback when buffer is complete
 */
void audio_dma_complete_callback(void);

#endif // AUDIO_INTERFACE_H
