/******************************************************************************
 * File Name    : usb_pcdc_mini_if.h
 * Description  : Interface file for USB PCDC API
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "usb_pcdc_mini_config.h"
#include <stdint.h>

#ifndef USB_PCDC_MINI_IF_H
#define USB_PCDC_MINI_IF_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/* CDC Class Requests IDs*/
#define USB_PCDC_SET_LINE_CODING            (0x2000u)
#define USB_PCDC_GET_LINE_CODING            (0x2100u)
#define USB_PCDC_SET_CONTROL_LINE_STATE     (0x2200u)

/* setup packet table size (uint16_t * 5) */
#define     USB_CDC_SETUP_TBL_BSIZE         (10u)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/


typedef struct
{
    union {
        uint16_t    WORD;
        /* CPU bit order rigth */
        struct {
            uint16_t b_rx_carrier :1;   /* DCD signal */
            uint16_t b_tx_carrier :1;   /* DSR signal */
            uint16_t b_break      :1;   /* State of break detection mechanism of teh device */
            uint16_t b_ring_signal:1;   /* State of ring signal detection of the device */
            uint16_t b_framing    :1;   /* Framing error */
            uint16_t b_parity     :1;   /* Parity error */
            uint16_t b_over_run   :1;   /* Over Run error */
            uint16_t rsv          :9;    /* reserve */
        } BIT;
    };
} usb_serial_state_bitmap_t;


/* Class Notification Serial State */
typedef union
{
    uint32_t                    word;
    usb_serial_state_bitmap_t   bit;
} usb_sci_serialstate_t;        /*USB_SCI_SerialState_t; */

typedef struct
{
    uint32_t dw_dte_rate;       /* Data terminal rate, in bits per second */
    uint8_t  b_char_format;     /* Stop bits */
    uint8_t  b_parity_type;     /* Parity */
    uint8_t  b_data_bits;       /* Data bits */
    uint8_t  rsv;               /* reserve */
} usb_pcdc_linecoding_t;        /* USB_PCDC_LineCoding_t; */


typedef struct
{
    union {
        uint16_t    WORD;
        /* CPU bit order rigth */
        struct {
            uint16_t b_dtr :1;          /* DTR */
            uint16_t b_rts :1;          /* RTS */
            uint16_t rsv   :14;         /* reserve */
        }BIT;
    };
} usb_pcdc_ctrllinestate_t;     /* USB_PCDC_ControlLineState_t; */


/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif /* USB_PCDC_MINI_IF_H */
/******************************************************************************
End Of File
 ******************************************************************************/
