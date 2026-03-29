#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ---- MCU ----
#define CFG_TUSB_MCU OPT_MCU_RP2040
#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_NONE
#endif

// ---- RHPort ----
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE)

// ---- Memory ----
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))

// ---- Device ----
#define CFG_TUD_ENDPOINT0_SIZE 64

// ---- HID (we need this) ----
#define CFG_TUD_HID 1

// buffer sizes
#define CFG_TUD_HID_EP_BUFSIZE 64

#ifdef __cplusplus
}
#endif
