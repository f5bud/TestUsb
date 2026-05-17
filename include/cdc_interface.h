#ifndef CDC_INTERFACE_H
#define CDC_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize CDC interface
 * @return 0 on success, -1 on error
 */
int cdc_init(void);

/**
 * @brief Called when USB host connects
 */
void cdc_connect(void);

/**
 * @brief Called when USB host disconnects
 */
void cdc_disconnect(void);

/**
 * @brief Check if CDC is connected and ready for communication
 * @return true if connected and DTR set, false otherwise
 */
bool cdc_is_connected(void);

/**
 * @brief Receive data from USB CDC host
 * @param buffer Destination buffer
 * @param length Maximum bytes to read
 * @return Number of bytes read
 */
uint32_t cdc_recv(uint8_t *buffer, uint32_t length);

/**
 * @brief Send data to USB CDC host
 * @param buffer Source buffer
 * @param length Number of bytes to send
 * @return Number of bytes queued
 */
uint32_t cdc_send(const uint8_t *buffer, uint32_t length);

/**
 * @brief Get number of bytes available to read
 * @return Number of bytes in RX buffer
 */
uint32_t cdc_available(void);

/**
 * @brief Get available space in TX buffer
 * @return Number of bytes available to write
 */
uint32_t cdc_write_space(void);

/**
 * @brief Send single character
 * @param c Character to send
 * @return Character on success, -1 on error
 */
int cdc_putchar(int c);

/**
 * @brief Receive single character
 * @return Character on success, -1 if none available
 */
int cdc_getchar(void);

/* ============== USB Control Request Handlers ============== */

/**
 * @brief Handle SET_LINE_CODING control request
 * @param data Pointer to line coding data
 * @param length Length of data
 */
void cdc_handle_line_coding_request(const uint8_t *data, uint16_t length);

/**
 * @brief Provide current line coding data
 * @param buffer Destination buffer
 * @param length Pointer to receive data length
 */
void cdc_get_line_coding(uint8_t *buffer, uint16_t *length);

/**
 * @brief Handle CONTROL_LINE_STATE change
 * @param state Control line state (DTR, RTS flags)
 */
void cdc_handle_control_line_state(uint16_t state);

/* ============== USB Data Handlers ============== */

/**
 * @brief Called by USB stack when data is received from host
 * @param data Pointer to received data
 * @param length Length of received data
 */
void cdc_receive_data(const uint8_t *data, uint16_t length);

/**
 * @brief Called by USB stack to get data to transmit to host
 * @param buffer Destination buffer for TX data
 * @param length Maximum bytes to copy
 * @return Number of bytes copied to buffer
 */
uint32_t cdc_get_transmit_data(uint8_t *buffer, uint32_t length);

#endif // CDC_INTERFACE_H
