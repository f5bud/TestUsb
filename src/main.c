#include "config.h"
#include "audio_interface.h"
#include "cdc_interface.h"
#include "tusb.h"

void usb_device_task(void *argument) {
    (void)argument;
    
    while (1) {
        tud_task();
    }
}

void usb_mount_cb(void) {
    // USB mounted
    cdc_init();
    audio_init();
}

void usb_unmount_cb(void) {
    // USB unmounted
    audio_disable();
}

void usb_suspend_cb(bool remote_wakeup_en) {
    (void)remote_wakeup_en;
    // Suspend USB
}

void usb_resume_cb(void) {
    // Resume USB
}

int main(void) {
    // Initialize system clock to 480 MHz
    // Initialize UART for debugging
    // Initialize I/O pins
    
    // Initialize TinyUSB
    tusb_init();

    // Initialize audio interface
    if (audio_init() != 0) {
        // Error handling
        while (1) {
            // Hang or retry
        }
    }

    // Enable audio streaming
    audio_enable();

    // Main loop
    while (1) {
        // Process USB tasks
        tud_task();

        // Check CDC connection
        if (cdc_is_connected()) {
            // Handle CDC data
            uint8_t buffer[64];
            uint32_t count = cdc_recv(buffer, sizeof(buffer));
            
            if (count > 0) {
                // Echo back received data
                cdc_send(buffer, count);
            }
        }

        // Check audio streaming
        if (audio_is_streaming()) {
            // Get input samples from codec
            audio_sample_t input_samples[AUDIO_BUFFER_SIZE];
            uint32_t count = audio_get_input_samples(input_samples, AUDIO_BUFFER_SIZE);

            if (count > 0) {
                // Send to USB (audio device)
                // tud_audio_write(input_samples, count * sizeof(audio_sample_t));
            }

            // Receive output samples from USB
            // audio_sample_t output_samples[AUDIO_BUFFER_SIZE];
            // uint32_t rx_count = tud_audio_read(output_samples, AUDIO_BUFFER_SIZE);
            // audio_process_output(output_samples, rx_count);
        }
    }

    return 0;
}

/* TinyUSB Callbacks */
void tud_mount_cb(void) {
    usb_mount_cb();
}

void tud_umount_cb(void) {
    usb_unmount_cb();
}

void tud_suspend_cb(bool remote_wakeup_en) {
    usb_suspend_cb(remote_wakeup_en);
}

void tud_resume_cb(void) {
    usb_resume_cb();
}
