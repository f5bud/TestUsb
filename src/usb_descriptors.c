#include "usb_descriptors.h"
#include "config.h"
#include <string.h>

/* ============== String Descriptors ============== */
#define USB_LANG_ID_EN_US           0x0409

// Language IDs
static const uint8_t string_lang_id[] = {
    0x04,                           // bLength
    USB_DESC_STRING,                // bDescriptorType
    0x09, 0x04                      // bString (USB_LANG_ID_EN_US)
};

// Manufacturer string
static const uint8_t string_manufacturer[] = {
    2 + 2 * 11,                     // bLength (2 + len*2)
    USB_DESC_STRING,                // bDescriptorType
    'M', 0, 'y', 0, 'C', 0, 'o', 0, 'm', 0, 'p', 0, 'a', 0, 'n', 0, 'y', 0, '!', 0
};

// Product string
static const uint8_t string_product[] = {
    2 + 2 * 23,                     // bLength
    USB_DESC_STRING,                // bDescriptorType
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, 'H', 0, '7', 0, '4', 0, '3', 0, 
    ' ', 0, 'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0, ' ', 0, 'D', 0, 'e', 0, 
    'v', 0, 'i', 0, 'c', 0, 'e', 0
};

// Serial number string
static const uint8_t string_serial[] = {
    2 + 2 * 9,                      // bLength
    USB_DESC_STRING,                // bDescriptorType
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0, '9', 0
};

// CDC string
static const uint8_t string_cdc[] = {
    2 + 2 * 3,                      // bLength
    USB_DESC_STRING,                // bDescriptorType
    'C', 0, 'D', 0, 'C', 0
};

// Audio string
static const uint8_t string_audio[] = {
    2 + 2 * 5,                      // bLength
    USB_DESC_STRING,                // bDescriptorType
    'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0
};

static const uint8_t* const string_descriptors[] = {
    string_lang_id,
    string_manufacturer,
    string_product,
    string_serial,
    string_cdc,
    string_audio
};

/* ============== Device Descriptor ============== */
const usb_device_descriptor_t device_descriptor = {
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DESC_DEVICE,
    .bcdUSB = 0x0200,                           // USB 2.0
    .bDeviceClass = 0xEF,                       // Miscellaneous (composite)
    .bDeviceSubClass = 0x02,                    // Common Class
    .bDeviceProtocol = 0x01,                    // Interface Association Descriptor
    .bMaxPacketSize0 = 64,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = 0x0100,                        // v1.00
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1
};

/* ============== Configuration Descriptor ============== */
typedef struct {
    usb_config_descriptor_t config;
    
    // CDC Interface Association
    usb_iad_descriptor_t cdc_iad;
    
    // CDC Control Interface
    usb_interface_descriptor_t cdc_ctrl_if;
    cdc_header_descriptor_t cdc_header;
    cdc_call_mgmt_descriptor_t cdc_call_mgmt;
    cdc_acm_descriptor_t cdc_acm;
    cdc_union_descriptor_t cdc_union;
    usb_endpoint_descriptor_t cdc_ctrl_ep;
    
    // CDC Data Interface (Alternate 0)
    usb_interface_descriptor_t cdc_data_if_alt0;
    
    // CDC Data Interface (Alternate 1)
    usb_interface_descriptor_t cdc_data_if_alt1;
    usb_endpoint_descriptor_t cdc_data_ep_out;
    usb_endpoint_descriptor_t cdc_data_ep_in;
    
    // Audio Interface Association
    usb_iad_descriptor_t audio_iad;
    
    // Audio Control Interface
    usb_interface_descriptor_t audio_ctrl_if;
    audio_control_header_descriptor_t audio_ctrl_header;
    audio_input_terminal_descriptor_t audio_input_term;
    audio_feature_unit_descriptor_t audio_feature_unit;
    audio_output_terminal_descriptor_t audio_output_term;
    
    // Audio Playback Interface (Alternate 0 - no streaming)
    usb_interface_descriptor_t audio_playback_if_alt0;
    
    // Audio Playback Interface (Alternate 1 - active)
    usb_interface_descriptor_t audio_playback_if_alt1;
    audio_streaming_header_descriptor_t audio_playback_header;
    audio_format_type1_descriptor_t audio_playback_format;
    usb_endpoint_descriptor_t audio_playback_ep;
    
    // Audio Recording Interface (Alternate 0 - no streaming)
    usb_interface_descriptor_t audio_recording_if_alt0;
    
    // Audio Recording Interface (Alternate 1 - active)
    usb_interface_descriptor_t audio_recording_if_alt1;
    audio_streaming_header_descriptor_t audio_recording_header;
    audio_format_type1_descriptor_t audio_recording_format;
    usb_endpoint_descriptor_t audio_recording_ep;
} __attribute__((packed)) usb_config_composite_t;

static const usb_config_composite_t config_descriptor_data = {
    // Configuration Descriptor
    .config = {
        .bLength = sizeof(usb_config_descriptor_t),
        .bDescriptorType = USB_DESC_CONFIG,
        .wTotalLength = sizeof(usb_config_composite_t),
        .bNumInterfaces = 6,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = 0xC0,                   // Self-powered
        .bMaxPower = 50                         // 100 mA
    },
    
    // ========== CDC Interface Association ==========
    .cdc_iad = {
        .bLength = sizeof(usb_iad_descriptor_t),
        .bDescriptorType = USB_DESC_IAD,
        .bFirstInterface = 0,
        .bInterfaceCount = 2,
        .bFunctionClass = USB_CLASS_CDC_CONTROL,
        .bFunctionSubClass = USB_SUBCLASS_CDC_CONTROL,
        .bFunctionProtocol = 0x00,
        .iFunction = 4                          // "CDC"
    },
    
    // CDC Control Interface
    .cdc_ctrl_if = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_CLASS_CDC_CONTROL,
        .bInterfaceSubClass = 0x02,            // Abstract Control Model
        .bInterfaceProtocol = 0x01,            // AT commands
        .iInterface = 4
    },
    
    .cdc_header = {
        .bLength = sizeof(cdc_header_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = 0x00,
        .bcdCDC = 0x0110
    },
    
    .cdc_call_mgmt = {
        .bLength = sizeof(cdc_call_mgmt_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = 0x01,
        .bmCapabilities = 0x00,
        .bDataInterface = 1
    },
    
    .cdc_acm = {
        .bLength = sizeof(cdc_acm_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = 0x02,
        .bmCapabilities = 0x02                 // Set_Line_Coding, Get_Line_Coding
    },
    
    .cdc_union = {
        .bLength = sizeof(cdc_union_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = 0x06,
        .bMasterInterface = 0,
        .bSlaveInterface = 1
    },
    
    .cdc_ctrl_ep = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = 0x81,              // EP1 IN
        .bmAttributes = USB_ENDPOINT_INTERRUPT,
        .wMaxPacketSize = 16,
        .bInterval = 16
    },
    
    // CDC Data Interface (Alternate 0 - disabled)
    .cdc_data_if_alt0 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 1,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = 0x0A,               // CDC Data
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface = 4
    },
    
    // CDC Data Interface (Alternate 1 - active)
    .cdc_data_if_alt1 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 1,
        .bAlternateSetting = 1,
        .bNumEndpoints = 2,
        .bInterfaceClass = 0x0A,
        .bInterfaceSubClass = 0x00,
        .bInterfaceProtocol = 0x00,
        .iInterface = 4
    },
    
    .cdc_data_ep_out = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = 0x02,              // EP2 OUT
        .bmAttributes = USB_ENDPOINT_BULK,
        .wMaxPacketSize = 64,
        .bInterval = 0
    },
    
    .cdc_data_ep_in = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = 0x83,              // EP3 IN
        .bmAttributes = USB_ENDPOINT_BULK,
        .wMaxPacketSize = 64,
        .bInterval = 0
    },
    
    // ========== Audio Interface Association ==========
    .audio_iad = {
        .bLength = sizeof(usb_iad_descriptor_t),
        .bDescriptorType = USB_DESC_IAD,
        .bFirstInterface = 2,
        .bInterfaceCount = 4,
        .bFunctionClass = USB_CLASS_AUDIO,
        .bFunctionSubClass = USB_SUBCLASS_AUDIO_CONTROL,
        .bFunctionProtocol = 0x00,
        .iFunction = 5                         // "Audio"
    },
    
    // Audio Control Interface
    .audio_ctrl_if = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 2,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_SUBCLASS_AUDIO_CONTROL,
        .bInterfaceProtocol = 0x00,
        .iInterface = 5
    },
    
    .audio_ctrl_header = {
        .bLength = sizeof(audio_control_header_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_CTRL_HEADER,
        .bcdADC = 0x0100,                      // Audio Class v1.0
        .wTotalLength = 58,                    // Recalculate if needed
        .bInCollection = 2,                    // 2 streaming interfaces
        .baInterfaceNr = 3                     // First streaming interface
    },
    
    .audio_input_term = {
        .bLength = sizeof(audio_input_terminal_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_CTRL_INPUT_TERMINAL,
        .bTerminalID = 1,
        .wTerminalType = 0x0601,               // Line connector
        .bAssocTerminal = 0,
        .bNrChannels = 2,                      // Stereo
        .wChannelConfig = 0x0003,              // L, R
        .iChannelNames = 0,
        .iTerminal = 0
    },
    
    .audio_feature_unit = {
        .bLength = sizeof(audio_feature_unit_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_CTRL_FEATURE_UNIT,
        .bUnitID = 2,
        .bSourceID = 1,
        .bControlSize = 1,
        .bmaControl0 = 0x01,                   // Master volume
        .bmaControl1 = 0x00,                   // Ch1
        .bmaControl2 = 0x00,                   // Ch2
        .iFeature = 0
    },
    
    .audio_output_term = {
        .bLength = sizeof(audio_output_terminal_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_CTRL_OUTPUT_TERMINAL,
        .bTerminalID = 3,
        .wTerminalType = 0x0301,               // Speaker
        .bAssocTerminal = 0,
        .bSourceID = 2,                        // From Feature Unit
        .iTerminal = 0
    },
    
    // Audio Playback Interface (Alternate 0)
    .audio_playback_if_alt0 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 3,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_SUBCLASS_AUDIO_STREAMING,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0
    },
    
    // Audio Playback Interface (Alternate 1 - streaming)
    .audio_playback_if_alt1 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 3,
        .bAlternateSetting = 1,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_SUBCLASS_AUDIO_STREAMING,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0
    },
    
    .audio_playback_header = {
        .bLength = sizeof(audio_streaming_header_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_STREAM_GENERAL,
        .bTerminalLink = 3,                    // From Output Terminal
        .bDelay = 0,
        .wFormatTag = 0x0001                   // PCM
    },
    
    .audio_playback_format = {
        .bLength = sizeof(audio_format_type1_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_STREAM_FORMAT_TYPE,
        .bFormatType = 1,                      // Type I
        .bNrChannels = 2,                      // Stereo
        .bSubframeSize = 2,                    // 16-bit
        .bBitResolution = 16,
        .bSamFreqType = 1,                     // One sample rate
        .tSamFreq = {0x80, 0xBB, 0x00}        // 48000 Hz (24-bit LE)
    },
    
    .audio_playback_ep = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = 0x84,              // EP4 IN (isochronous)
        .bmAttributes = USB_ENDPOINT_ISOCHRONOUS | 0x04,  // Async
        .wMaxPacketSize = 192,                 // 48*2*2 bytes
        .bInterval = 1                         // 1 ms (for FS)
    },
    
    // Audio Recording Interface (Alternate 0)
    .audio_recording_if_alt0 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 4,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_SUBCLASS_AUDIO_STREAMING,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0
    },
    
    // Audio Recording Interface (Alternate 1 - streaming)
    .audio_recording_if_alt1 = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_DESC_INTERFACE,
        .bInterfaceNumber = 4,
        .bAlternateSetting = 1,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_CLASS_AUDIO,
        .bInterfaceSubClass = USB_SUBCLASS_AUDIO_STREAMING,
        .bInterfaceProtocol = 0x00,
        .iInterface = 0
    },
    
    .audio_recording_header = {
        .bLength = sizeof(audio_streaming_header_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_STREAM_GENERAL,
        .bTerminalLink = 1,                    // From Input Terminal
        .bDelay = 0,
        .wFormatTag = 0x0001                   // PCM
    },
    
    .audio_recording_format = {
        .bLength = sizeof(audio_format_type1_descriptor_t),
        .bDescriptorType = 0x24,
        .bDescriptorSubtype = AUDIO_STREAM_FORMAT_TYPE,
        .bFormatType = 1,                      // Type I
        .bNrChannels = 2,                      // Stereo
        .bSubframeSize = 2,                    // 16-bit
        .bBitResolution = 16,
        .bSamFreqType = 1,                     // One sample rate
        .tSamFreq = {0x80, 0xBB, 0x00}        // 48000 Hz (24-bit LE)
    },
    
    .audio_recording_ep = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_DESC_ENDPOINT,
        .bEndpointAddress = 0x05,              // EP5 OUT (isochronous)
        .bmAttributes = USB_ENDPOINT_ISOCHRONOUS | 0x04,  // Async
        .wMaxPacketSize = 192,
        .bInterval = 1                         // 1 ms
    }
};

const usb_config_descriptor_t config_descriptor = 
    *(usb_config_descriptor_t*)&config_descriptor_data;

/* ============== String Descriptor Handler ============== */
const uint8_t* usb_get_string_descriptor(uint8_t index, uint16_t *length)
{
    if (index >= sizeof(string_descriptors) / sizeof(string_descriptors[0])) {
        return NULL;
    }
    
    const uint8_t* descriptor = string_descriptors[index];
    if (length) {
        *length = descriptor[0];
    }
    
    return descriptor;
}

/* ============== Descriptor Access ============== */
const uint8_t* usb_get_descriptor(uint8_t type, uint8_t index, uint16_t language, uint16_t *length)
{
    switch (type) {
        case USB_DESC_DEVICE:
            *length = sizeof(usb_device_descriptor_t);
            return (const uint8_t*)&device_descriptor;
            
        case USB_DESC_CONFIG:
            *length = sizeof(usb_config_composite_t);
            return (const uint8_t*)&config_descriptor_data;
            
        case USB_DESC_STRING:
            return usb_get_string_descriptor(index, length);
            
        default:
            return NULL;
    }
}
