#ifndef HAL_AUDIO_H
#define HAL_AUDIO_H

#include <stdint.h>
#include <stdbool.h>

/* ============== Function Prototypes ============== */

/**
 * @brief Initialize I2S peripheral and DMA
 * @return 0 on success, -1 on error
 */
int hal_i2s_init(void);

/**
 * @brief Start I2S DMA transfers
 */
void hal_i2s_start_dma(void);

/**
 * @brief Stop I2S DMA transfers
 */
void hal_i2s_stop_dma(void);

/**
 * @brief Initialize codec I2C communication
 * @return 0 on success, -1 on error
 */
int hal_i2c_init(void);

/**
 * @brief Write codec register via I2C
 * @param reg Register address
 * @param value Register value
 * @return 0 on success, -1 on error
 */
int hal_codec_write_reg(uint8_t reg, uint16_t value);

/**
 * @brief Read codec register via I2C
 * @param reg Register address
 * @return Register value, -1 on error
 */
int hal_codec_read_reg(uint8_t reg);

/**
 * @brief Initialize codec (SSM2603)
 * @return 0 on success, -1 on error
 */
int hal_codec_init(void);

/**
 * @brief Enable codec audio input (line/mic)
 */
void hal_codec_enable_input(void);

/**
 * @brief Disable codec audio input
 */
void hal_codec_disable_input(void);

/**
 * @brief Enable codec audio output (speaker/line)
 */
void hal_codec_enable_output(void);

/**
 * @brief Disable codec audio output
 */
void hal_codec_disable_output(void);

/**
 * @brief Set codec output volume
 * @param volume Volume level (0-127)
 */
void hal_codec_set_volume(uint8_t volume);

/**
 * @brief Set codec input gain
 * @param gain Gain level (0-31)
 */
void hal_codec_set_input_gain(uint8_t gain);

/**
 * @brief Get DMA TX buffer pointer (current position)
 * @return Pointer to current TX buffer position
 */
void* hal_dma_get_tx_ptr(void);

/**
 * @brief Get DMA RX buffer pointer (current position)
 * @return Pointer to current RX buffer position
 */
void* hal_dma_get_rx_ptr(void);

/**
 * @brief Set DMA TX buffer address
 * @param addr Buffer address
 * @param size Buffer size in bytes
 */
void hal_dma_set_tx_buffer(void *addr, uint32_t size);

/**
 * @brief Set DMA RX buffer address
 * @param addr Buffer address
 * @param size Buffer size in bytes
 */
void hal_dma_set_rx_buffer(void *addr, uint32_t size);

/**
 * @brief Register DMA half-complete callback
 * @param callback Callback function
 */
void hal_dma_set_half_complete_callback(void (*callback)(void));

/**
 * @brief Register DMA complete callback
 * @param callback Callback function
 */
void hal_dma_set_complete_callback(void (*callback)(void));

/**
 * @brief Initialize system clock for 480 MHz
 * @return 0 on success, -1 on error
 */
int hal_clock_init(void);

/**
 * @brief Initialize GPIO pins for I2S/I2C/USB
 */
void hal_gpio_init(void);

#endif // HAL_AUDIO_H
