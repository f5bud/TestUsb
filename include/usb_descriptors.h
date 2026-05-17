#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include <stdint.h>

/* USB Descriptor Types */
#define USB_DESC_DEVICE                     0x01
#define USB_DESC_CONFIG                     0x02
#define USB_DESC_STRING                     0x03
#define USB_DESC_INTERFACE                  0x04
#define USB_DESC_ENDPOINT                   0x05
#define USB_DESC_IAD                        0x0B    // Interface Association Descriptor

/* USB Device Classes */
#define USB_CLASS_CDC_CONTROL               0x02
#define USB_CLASS_AUDIO                     0x01

/* USB Subclasses */
#define USB_SUBCLASS_CDC_CONTROL            0x02
#define USB_SUBCLASS_AUDIO_CONTROL          0x01
#define USB_SUBCLASS_AUDIO_STREAMING        0x02

/* USB Protocols */
#define USB_PROTOCOL_NONE                   0x00
#define USB_PROTOCOL_AT_V25TER              0x01

/* Audio Control Interface Subtypes */
#define AUDIO_CTRL_HEADER                   0x01
#define AUDIO_CTRL_INPUT_TERMINAL           0x02
#define AUDIO_CTRL_OUTPUT_TERMINAL          0x03
#define AUDIO_CTRL_FEATURE_UNIT             0x06

/* Audio Streaming Interface Subtypes */
#define AUDIO_STREAM_GENERAL                0x01
#define AUDIO_STREAM_FORMAT_TYPE            0x02

/* Endpoint Types */
#define USB_ENDPOINT_CONTROL                0x00
#define USB_ENDPOINT_ISOCHRONOUS            0x01
#define USB_ENDPOINT_BULK                   0x02
#define USB_ENDPOINT_INTERRUPT              0x03

/* Endpoint Direction */
#define USB_ENDPOINT_IN                     0x80
#define USB_ENDPOINT_OUT                    0x00

/* ============== Device Descriptor ============== */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
} __attribute__((packed)) usb_device_descriptor_t;

/* ============== Configuration Descriptor ============== */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  bMaxPower;
} __attribute__((packed)) usb_config_descriptor_t;

/* ============== Interface Association Descriptor ============== */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bFirstInterface;
    uint8_t bInterfaceCount;
    uint8_t bFunctionClass;
    uint8_t bFunctionSubClass;
    uint8_t bFunctionProtocol;
    uint8_t iFunction;
} __attribute__((packed)) usb_iad_descriptor_t;

/* ============== Interface Descriptor ============== */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} __attribute__((packed)) usb_interface_descriptor_t;

/* ============== Endpoint Descriptor ============== */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
} __attribute__((packed)) usb_endpoint_descriptor_t;

/* ============== CDC Specific Descriptors ============== */

/* Header Functional Descriptor */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint16_t bcdCDC;
} __attribute__((packed)) cdc_header_descriptor_t;

/* Abstract Control Management Functional Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
} __attribute__((packed)) cdc_acm_descriptor_t;

/* Union Functional Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bMasterInterface;
    uint8_t bSlaveInterface;
} __attribute__((packed)) cdc_union_descriptor_t;

/* Call Management Functional Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bmCapabilities;
    uint8_t bDataInterface;
} __attribute__((packed)) cdc_call_mgmt_descriptor_t;

/* ============== Audio Specific Descriptors ============== */

/* Audio Control Header Descriptor */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint16_t bcdADC;
    uint16_t wTotalLength;
    uint8_t  bInCollection;
    uint8_t  baInterfaceNr;
} __attribute__((packed)) audio_control_header_descriptor_t;

/* Audio Input Terminal Descriptor */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  bTerminalID;
    uint16_t wTerminalType;
    uint8_t  bAssocTerminal;
    uint8_t  bNrChannels;
    uint16_t wChannelConfig;
    uint8_t  iChannelNames;
    uint8_t  iTerminal;
} __attribute__((packed)) audio_input_terminal_descriptor_t;

/* Audio Output Terminal Descriptor */
typedef struct {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bDescriptorSubtype;
    uint8_t  bTerminalID;
    uint16_t wTerminalType;
    uint8_t  bAssocTerminal;
    uint8_t  bSourceID;
    uint8_t  iTerminal;
} __attribute__((packed)) audio_output_terminal_descriptor_t;

/* Audio Feature Unit Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bSourceID;
    uint8_t bControlSize;
    uint8_t bmaControl0;
    uint8_t bmaControl1;
    uint8_t bmaControl2;
    uint8_t iFeature;
} __attribute__((packed)) audio_feature_unit_descriptor_t;

/* Audio Streaming Header Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalLink;
    uint8_t bDelay;
    uint16_t wFormatTag;
} __attribute__((packed)) audio_streaming_header_descriptor_t;

/* Audio Format Type I Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bFormatType;
    uint8_t bNrChannels;
    uint8_t bSubframeSize;
    uint8_t bBitResolution;
    uint8_t bSamFreqType;
    uint8_t tSamFreq[3];
} __attribute__((packed)) audio_format_type1_descriptor_t;

/* ============== Function Prototypes ============== */

extern const usb_device_descriptor_t device_descriptor;
extern const usb_config_descriptor_t config_descriptor;

/* Get string descriptor */
const uint8_t* usb_get_string_descriptor(uint8_t index, uint16_t *length);

#endif // USB_DESCRIPTORS_H
