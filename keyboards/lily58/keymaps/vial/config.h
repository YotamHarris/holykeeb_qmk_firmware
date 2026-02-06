#pragma once

/* USB plugs into right half */
#undef MASTER_LEFT
#define MASTER_RIGHT

#define QUICK_TAP_TERM 0
#define TAPPING_TERM 100

/* VIA layer count */
#define DYNAMIC_KEYMAP_LAYER_COUNT 10

/* Serial pin for split communication */
#define SERIAL_USART_TX_PIN GP1

/* Split keyboard settings */
#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT 3000

/* EEPROM for HK settings persistence */
#define EECONFIG_USER_DATA_SIZE 64

/* Extended mouse/wheel reports */
#define MOUSE_EXTENDED_REPORT
#define WHEEL_EXTENDED_REPORT

/* RP2040 bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

/* PS2 Trackpoint configuration */
#define PS2_MOUSE_USE_REMOTE_MODE
#define PS2_PIO_USE_PIO1
#define PS2_MOUSE_INVERT_X
#define PS2_MOUSE_INVERT_Y
#define PS2_DATA_PIN GP2
#define PS2_CLOCK_PIN GP3
#define PS2_MOUSE_INIT_DELAY 500

/* OLED split sync */
#if defined(OLED_ENABLE) && defined(SPLIT_KEYBOARD)
    #define HK_SPLIT_SYNC_STATE
#endif

#ifdef HK_SPLIT_SYNC_STATE
    #define SPLIT_LAYER_STATE_ENABLE
    #define SPLIT_TRANSACTION_IDS_USER HK_SYNC_STATE
    #define RPC_M2S_BUFFER_SIZE 64
#endif

/* Disable OLED auto-sleep (we handle idle animation ourselves) */
#define OLED_TIMEOUT 0

#undef PRINTF_SUPPORT_DECIMAL_SPECIFIERS
#define PRINTF_SUPPORT_DECIMAL_SPECIFIERS 1
