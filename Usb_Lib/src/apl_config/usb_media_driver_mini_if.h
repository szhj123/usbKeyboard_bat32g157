/******************************************************************************
 * File Name     : usb_media_driver_mini_if.h
 * Description   : This is the header file for r_media_driver_api.c 
 *                 Public functions prototypes, type definitions, and other 
 *                 public data structure for the block media driver API are 
 *                 declared here.
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#ifndef USB_MEDIA_DRIVER_MINI_IF_H    /* Multiple inclusion prevention */
#define USB_MEDIA_DRIVER_MINI_IF_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define STRG_SECTSIZE                   (0x0200ul)  /* 512 bytes per sector */

#define STA_NOINIT                      (0x01)      /* Drive not initialized */
#define STA_NODISK                      (0x02)      /* No medium in the drive */
#define STA_PROTECT                     (0x04)      /* Write protected */

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/* Return values for functions */
typedef enum
{
    USB_MEDIA_RET_OK = 0,           /* 0: Successful */
    USB_MEDIA_RET_RWERR,            /* 1: Read/Write Error */
    USB_MEDIA_RET_WRPRT,            /* 2: Write Protected */
    USB_MEDIA_RET_NOTRDY,           /* 3: Not Ready */
    USB_MEDIA_RET_PARERR,           /* 4: Invalid Parameter */
    USB_MEDIA_RET_OP_FAIL,          /* 5: Operation failed. */
    USB_MEDIA_RET_DEV_OPEN,         /* 6: The device is already open. */
    USB_MEDIA_RET_DEV_NO_INIT,      /* 7: Device not initialized. */

/* For expansion, add only after this line. */
} usb_media_ret_t;

/* 
 Definition of media driver function pointer types.
 The media driver API interfaces to the specific media device driver functions through an abstraction layer.
 Therefore each of the API functions has a corresponding type-defined type that consists of a pointer to a function
 with a matching parameter list.   
 */
typedef usb_media_ret_t     (*usb_media_open_t)(void);
typedef usb_media_ret_t     (*usb_media_close_t)(void);
typedef usb_media_ret_t     (*usb_media_read_t)(uint8_t*, uint32_t, uint8_t);
typedef usb_media_ret_t     (*usb_media_write_t)(uint8_t*, uint32_t, uint8_t);
typedef usb_media_ret_t     (*usb_media_ioctl_t)(uint8_t, uint32_t*);

/* Media driver Data Structure */
typedef struct
{
    usb_media_open_t    pf_media_open;      /* Pointer to driver open function. */
    usb_media_close_t   pf_media_close;     /* Pointer to driver close function. */
    usb_media_read_t    pf_media_read;      /* Pointer to driver read function. */
    usb_media_write_t   pf_media_write;     /* Pointer to driver write function. */
    usb_media_ioctl_t   pf_media_ctrl;      /* Pointer to driver control function. */
} usb_media_driver_t;

typedef enum
{
    /* Generic ioctl command (defined for compatibility with FatFs) */
    USB_MEDIA_IOCTL_SYNC                = 0,    /* Flush media write cache. */
    USB_MEDIA_IOCTL_GET_NUM_BLOCKS      = 1,    /* For use un calculating media size. */
    USB_MEDIA_IOCTL_GET_SECTOR_SIZE     = 2,    /* Get sector size (for multiple sector size. (_MAX_SS >= 1024)) */
    USB_MEDIA_IOCTL_GET_BLOCK_SIZE      = 3,    /* Get number of byes per block. */
    USB_MEDIA_IOCTL_CTRL_ERASE_SECTOR   = 4,    /* Force erased a sector group. (for only _USE_ERASE) */

/* Add more IOCTL commands starting below here. */
} usb_ioctl_cmd_t;

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* The USB_media_initialize() function initializes data structures and variables that are used by the target media device
   to support its operation for the first time. 
 */
bool                USB_media_initialize (usb_media_driver_t * p_media_driver);

/* The USB_media_open() function initializes the hardware registers for peripherals used by the media driver and leaves
   the media device ready for communications.
 */
usb_media_ret_t     USB_media_open(void);

/* The USB_media_close() function initializes the hardware registers for peripherals used by the media driver and leaves
   the media device ready for communications.
 */
usb_media_ret_t     USB_media_close(void);

/* The USB_media_read() function reads one or more blocks of data from the selected media device and places it into
   a buffer provided by the caller.
 */
usb_media_ret_t     USB_media_read(uint8_t    *p_rbuffer,     /* Pointer to read data buffer */
                                     uint32_t   start_block,    /* Start block number */
                                     uint8_t    block_count);   /* Number of blocks to read */

/* The USB_media_write() function writes one or more blocks of data to the selected media device from a source buffer
   provided by the caller.
 */
usb_media_ret_t     USB_media_write(uint8_t    *p_wbuffer,    /* Pointer to write data buffer */
                                      uint32_t   start_block,   /* Start block number */
                                      uint8_t    block_count);  /* Number of blocks to write */

/* The USB_media_ioctl() function provides a generalized means to pass special command 
 and control instructions to the media driver, and for the driver to return information.
 */
usb_media_ret_t     USB_media_ioctl(usb_ioctl_cmd_t   ioctl_cmd,      /* Control command code */
                                      void          *ioctl_data);   /* Data transfer buffer */

#endif /* USB_MEDIA_DRIVER_MINI_IF_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/

