#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* ============== USB Configuration ============== */
#define USB_VID                0x1234
#define USB_PID                0x5678
#define USB_MANUFACTURER       "MyCompany"
#define USB_PRODUCT_NAME       "STM32H743 Audio Device"
#define USB_SERIAL_NUMBER      "123456789"

/* ============== Audio Configuration ============== */
#define AUDIO_SAMPLE_RATE      48000       // 48 kHz
#define AUDIO_BIT_DEPTH        16          // 16-bit
#define AUDIO_CHANNELS         2           // Stéréo
#define AUDIO_FRAME_SIZE       (AUDIO_CHANNELS * AUDIO_SAMPLE_RATE / 1000)  // Samples per ms
#define AUDIO_BUFFER_SIZE      (AUDIO_FRAME_SIZE * 10)  // 10ms buffer

typedef int16_t audio_sample_t;

/* ============== I2S Configuration ============== */
#define I2S_INSTANCE           SPI2        // I2S2
#define I2S_CLOCK_SOURCE       plli2s_48m  // From PLLI2S

// I2S2 Pins (STM32H743)
#define I2S2_CK_PORT           GPIOB
#define I2S2_CK_PIN            10          // PB10

#define I2S2_WS_PORT           GPIOB
#define I2S2_WS_PIN            9           // PB9

#define I2S2_SD_TX_PORT        GPIOB
#define I2S2_SD_TX_PIN         15          // PB15 (Master Output)

#define I2S2_SD_RX_PORT        GPIOB
#define I2S2_SD_RX_PIN         14          // PB14 (Slave Input via I2Sext)

#define I2S2_MCK_PORT          GPIOC
#define I2S2_MCK_PIN           6           // PC6 (Master Clock)

/* ============== Codec Configuration (SSM2603) ============== */
#define CODEC_I2C_INSTANCE     I2C1
#define CODEC_I2C_ADDR         0x10        // SSM2603 slave address (8-bit)

// I2C1 Pins
#define I2C1_SDA_PORT          GPIOB
#define I2C1_SDA_PIN           7           // PB7

#define I2C1_SCL_PORT          GPIOB
#define I2C1_SCL_PIN           6           // PB6

#define CODEC_RESET_PORT       GPIOD
#define CODEC_RESET_PIN        4           // PD4

/* ============== DMA Configuration ============== */
#define DMA_TX_STREAM          DMA1_Stream4
#define DMA_RX_STREAM          DMA1_Stream3
#define DMA_CHANNEL            DMA_CHANNEL_0

/* ============== CDC Configuration ============== */
#define CDC_BAUD_RATE          115200
#define CDC_BUFFER_SIZE        256

/* ============== System Clock Configuration ============== */
#define SYSTEM_CLOCK_HZ        480000000   // 480 MHz
#define PLLM_DIVIDER           3
#define PLLN_MULTIPLIER        120
#define PLLP_DIVIDER           2           // 480 MHz

// PLLI2S for I2S clock (48 MHz from PLLSAI)
#define PLLI2S_M               3
#define PLLI2S_N               96
#define PLLI2S_R               2           // 48 MHz

/* ============== Codec Control Registers ============== */
// SSM2603 Register Addresses (Right-aligned, 9-bit)
#define CODEC_REG_LLINEIN      0x00
#define CODEC_REG_RLINEIN      0x01
#define CODEC_REG_LHEADOUT     0x02
#define CODEC_REG_RHEADOUT     0x03
#define CODEC_REG_ANALOG       0x04
#define CODEC_REG_DIGITAL      0x05
#define CODEC_REG_POWERDOWN    0x06
#define CODEC_REG_INTERFACE    0x07
#define CODEC_REG_SAMPLING     0x08
#define CODEC_REG_ACTIVE       0x09
#define CODEC_REG_RESET        0x0F

// Default codec values
#define CODEC_DEFAULT_VOLUME   0x18        // Line-in volume
#define CODEC_PLAYBACK_VOLUME  0x79        // Headphone volume (max)
#define CODEC_DEFAULT_FORMAT   0x42        // I2S format, 16-bit

/* ============== Debug Configuration ============== */
#define DEBUG_ENABLED          1
#define ENABLE_USB_DEBUG       1
#define ENABLE_AUDIO_DEBUG     1

#endif // CONFIG_H
