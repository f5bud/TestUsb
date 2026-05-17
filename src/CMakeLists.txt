cmake_minimum_required(VERSION 3.17)

# STM32H743 Cortex-M7 Toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Project configuration
project(STM32H743_USB_Audio C CXX ASM)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyusb/src
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyusb/hw/cmsis/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/CMSIS/Core/Include
)

# Source files
set(SOURCES
    src/main.c
    src/cdc_interface.c
    src/audio_interface.c
    src/hal_audio.c
    src/usb_descriptors.c
    src/startup_stm32h743.c
)

# TinyUSB sources
set(TINYUSB_SOURCES
    lib/tinyusb/src/tusb.c
    lib/tinyusb/src/common/tusb_fifo.c
    lib/tinyusb/src/device/usbd.c
    lib/tinyusb/src/device/usbd_control.c
    lib/tinyusb/src/class/cdc/cdc_device.c
    lib/tinyusb/src/class/audio/audio_device.c
    lib/tinyusb/src/portable/st/synopsys/dcd_synopsys.c
)

# STM32 HAL sources
set(STM32_HAL_SOURCES
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
    lib/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
)

# Create executable
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${TINYUSB_SOURCES} ${STM32_HAL_SOURCES})

# Link flags
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    LINK_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -T${CMAKE_CURRENT_SOURCE_DIR}/STM32H743ZITx_FLASH.ld -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map -static"
)

# Link with standard C library
target_link_libraries(${PROJECT_NAME}.elf c m)

# Generate binary file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-size ${PROJECT_NAME}.elf
)

# Optional: Flash target
add_custom_target(flash
    COMMAND st-flash write ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.bin 0x08000000
    DEPENDS ${PROJECT_NAME}.elf
)
