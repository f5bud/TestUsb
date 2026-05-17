#include "cdc_interface.h"
#include "config.h"
#include <string.h>

/* ============== Ring Buffer Structure ============== */
typedef struct {
    uint8_t buffer[CDC_BUFFER_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
} ring_buffer_t;

/* ============== CDC State ============== */
typedef struct {
    ring_buffer_t tx_buffer;
    ring_buffer_t rx_buffer;
    cdc_line_coding_t line_coding;
    cdc_serial_state_t serial_state;
    bool connected;
} cdc_state_t;

static cdc_state_t cdc_state = {
    .connected = false,
    .line_coding = {
        .bitrate = CDC_BAUD_RATE,
        .stop_bits = 0,
        .parity = 0,
        .data_bits = 8
    },
    .serial_state = {0}
};

/* ============== Ring Buffer Helpers ============== */
static void ring_buffer_init(ring_buffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

static uint32_t ring_buffer_available(ring_buffer_t *rb)
{
    return rb->count;
}

static uint32_t ring_buffer_free(ring_buffer_t *rb)
{
    return CDC_BUFFER_SIZE - rb->count;
}

static bool ring_buffer_put(ring_buffer_t *rb, uint8_t data)
{
    if (rb->count >= CDC_BUFFER_SIZE) {
        return false;  // Buffer full
    }
    
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % CDC_BUFFER_SIZE;
    rb->count++;
    
    return true;
}

static bool ring_buffer_get(ring_buffer_t *rb, uint8_t *data)
{
    if (rb->count == 0) {
        return false;  // Buffer empty
    }
    
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % CDC_BUFFER_SIZE;
    rb->count--;
    
    return true;
}

static uint32_t ring_buffer_get_block(ring_buffer_t *rb, uint8_t *data, uint32_t length)
{
    uint32_t count = 0;
    
    while (count < length && rb->count > 0) {
        data[count] = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % CDC_BUFFER_SIZE;
        rb->count--;
        count++;
    }
    
    return count;
}

static uint32_t ring_buffer_put_block(ring_buffer_t *rb, const uint8_t *data, uint32_t length)
{
    uint32_t count = 0;
    
    while (count < length && ring_buffer_free(rb) > 0) {
        rb->buffer[rb->head] = data[count];
        rb->head = (rb->head + 1) % CDC_BUFFER_SIZE;
        rb->count++;
        count++;
    }
    
    return count;
}

/* ============== CDC Implementation ============== */

int cdc_init(void)
{
    ring_buffer_init(&cdc_state.tx_buffer);
    ring_buffer_init(&cdc_state.rx_buffer);
    cdc_state.connected = false;
    
    return 0;
}

bool cdc_is_connected(void)
{
    return cdc_state.connected && cdc_state.serial_state.bDTR;
}

uint32_t cdc_send(const uint8_t *data, uint32_t length)
{
    if (!data || length == 0) {
        return 0;
    }
    
    uint32_t sent = ring_buffer_put_block(&cdc_state.tx_buffer, data, length);
    
    // TODO: Trigger USB transfer if not already in progress
    // cdc_usb_transmit();
    
    return sent;
}

uint32_t cdc_recv(uint8_t *data, uint32_t length)
{
    if (!data || length == 0) {
        return 0;
    }
    
    return ring_buffer_get_block(&cdc_state.rx_buffer, data, length);
}

const cdc_line_coding_t* cdc_get_line_coding(void)
{
    return &cdc_state.line_coding;
}

int cdc_set_line_coding(const cdc_line_coding_t *coding)
{
    if (!coding) {
        return -1;
    }
    
    memcpy(&cdc_state.line_coding, coding, sizeof(cdc_line_coding_t));
    
    // TODO: Reconfigure UART/serial if needed
    
    return 0;
}

cdc_serial_state_t cdc_get_serial_state(void)
{
    return cdc_state.serial_state;
}

void cdc_set_control_line_state(uint16_t state)
{
    cdc_state.serial_state.bDTR = (state >> 0) & 1;
    cdc_state.serial_state.bRTS = (state >> 1) & 1;
    
    // Signal host that we're ready to receive data
    cdc_state.connected = true;
}

/* ============== CDC Callbacks (from USB stack) ============== */

void cdc_on_data_received(const uint8_t *data, uint32_t length)
{
    if (!data || length == 0) {
        return;
    }
    
    // Add received data to RX buffer
    ring_buffer_put_block(&cdc_state.rx_buffer, data, length);
}

void cdc_on_line_coding_changed(const cdc_line_coding_t *coding)
{
    if (!coding) {
        return;
    }
    
    cdc_set_line_coding(coding);
}

void cdc_on_line_state_changed(uint16_t state)
{
    cdc_set_control_line_state(state);
}

/* ============== USB Transfer Helpers ============== */

uint32_t cdc_get_tx_data(uint8_t *buffer, uint32_t max_length)
{
    if (!buffer || max_length == 0) {
        return 0;
    }
    
    return ring_buffer_get_block(&cdc_state.tx_buffer, buffer, max_length);
}

void cdc_on_tx_complete(void)
{
    // Triggered when USB transfer completes
    // Can start next transfer if data available
}
