# STM32H743 USB Composite Device (CDC + Audio)

Projet embedded pour **STM32H743** implémentant un périphérique USB composite avec :
- **CDC (Communications Device Class)** : Interface série virtuelle
- **Audio** : Entrée/Sortie audio stéréo 16-bit @ 48kHz

## Architecture

```
┌─────────────────────────────┐
│    USB Host (PC/MAC/Linux)  │
└────────────┬────────────────┘
             │
        ┌────▼────┐
        │  USB 2.0│
        └────┬────┘
             │
    ┌────────┴──────────┐
    │   STM32H743       │
    │                  │
    ├─ CDC Interface   │ ◄─── Virtual Serial Port
    │                  │
    ├─ Audio I2S       │ ◄─── I2S Master
    │                  │
    └────────┬──────────┘
             │
        ┌────▼────┐
        │  Codec  │ (SSM2603 / WM8731)
        │  (I2C)  │
        └────┬────┘
             │
    ┌────────┴──────────┐
    │   Audio Connectors│
    │                  │
    ├─ Line-In  (3.5mm)│ ◄─── Audio Input
    │                  │
    └─ Line-Out (3.5mm)│ ◄─── Audio Output
```

## Arborescence

```
.
├── include/
│   ├── config.h              # Configuration centralisée
│   ├── cdc_interface.h       # Interface CDC
│   ├── audio_interface.h     # Interface Audio
│   ├── hal_audio.h           # HAL Codec/I2S
│   └── usb_descriptors.h     # Descripteurs USB
│
├── src/
│   ├── main.c               # Boucle principale
│   ├── cdc_interface.c      # Implémentation CDC
│   ├── audio_interface.c    # Implémentation Audio
│   ├── usb_descriptors.c    # Descripteurs USB
│   └── hal_audio.c          # HAL Codec/I2S
│
├── hal/
│   └── stm32h7xx/          # HAL spécifique STM32H7xx
│
├── lib/
│   ├── tinyusb/            # TinyUSB stack
│   ├── STM32H7xx_HAL_Driver/
│   └── CMSIS/
│
├── CMakeLists.txt          # Build configuration
├── STM32H743ZITx_FLASH.ld  # Linker script
└── README.md
```

## Spécifications

### Audio
- **Fréquence** : 48 kHz
- **Résolution** : 16-bit
- **Canaux** : Stéréo (Gauche/Droit)
- **Codec** : SSM2603 (par défaut, adaptable)
- **Interface** : I2S Master (STM32) → Slave (Codec)

### USB
- **Vitesse** : High-Speed (480 Mbps)
- **Classes** : CDC + Audio
- **VID:PID** : 0x1234:0x5678 (à personnaliser)
- **Power** : 100 mA bus-powered

### STM32H743
- **CPU** : Cortex-M7 dual core @ 480 MHz
- **RAM** : 864 KB
- **Flash** : 2 MB
- **USB HS OTG**

## Brochage STM32H743 (à adapter)

```
I2S2 (Audio):
  PB10  ──► I2S2_CK (Clock)
  PB9   ──► I2S2_WS (Word Select)
  PB15  ──► I2S2_SD (TX - Playback)
  PB14  ──► I2S2ext_SD (RX - Record)
  PC6   ──► I2S2_MCK (Master Clock)

CODEC I2C1:
  PB7   ──► I2C1_SDA
  PB6   ──► I2C1_SCL
  PD4   ──► Reset Codec (GPIO)

USB:
  PA11  ──► USB_DM
  PA12  ──► USB_DP
```

## Compilation et Programmation

### Prérequis
```bash
arm-none-eabi-gcc
arm-none-eabi-objcopy
cmake >= 3.17
st-flash (ou autre tool de programmation)
```

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Programmation
```bash
make flash
```

## Utilisation

### CDC (Série Virtuelle)
```c
// Envoyer des données
cdc_send((uint8_t *)"Hello\r\n", 7);

// Recevoir des données
uint8_t buffer[64];
uint32_t count = cdc_recv(buffer, 64);
```

### Audio
```c
// Démarrer streaming
audio_enable();

// Récupérer samples d'entrée (micphone/ligne)
audio_sample_t samples[192];
audio_get_input_samples(samples, 192);

// Envoyer samples de sortie (speaker/ligne)
audio_process_output(samples, 192);

// Arrêter streaming
audio_disable();
```

## À Compléter

- [ ] Implémentation HAL I2S/DMA (STM32)
- [ ] Implémentation HAL I2C Codec
- [ ] Configuration horloge système (480 MHz)
- [ ] Linker script complet
- [ ] Startup assembly
- [ ] Tests USB avec utilitaire (lsusb, etc.)
- [ ] Tuning audio (latence, jitter)

## Ressources

- [TinyUSB Documentation](https://github.com/hathach/tinyusb)
- [STM32H743 Datasheet](https://www.st.com/en/microcontrollers/stm32h743zi.html)
- [USB Audio Class 1.0 Spec](https://www.usb.org/)
- [SSM2603 Codec Datasheet](https://www.analog.com/)

## License

MIT
