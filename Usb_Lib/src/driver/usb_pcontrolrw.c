/******************************************************************************
 * File Name    : usb_pcontrolrw.c
 * Description  : USB Peripheral control transfer API code
 ******************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "usb_basic_mini_if.h"
#include "usb_typedef.h"
#include "usb_extern.h"
#include "usb_bitdefine.h"
#include "usb_reg_access.h"

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_pstd_ctrl_read
 Description     : Called by USB_PstdCtrlRead, see it for description.
 Arguments       : uint32_t bsize    : Read size in bytes.
                 : uint8_t *table    : Start address of read data buffer.
 Return value    : uint16_t          : USB_WRITESHRT/USB_WRITE_END/USB_WRITING/USB_FIFOERROR.
 ******************************************************************************/
uint16_t usb_pstd_ctrl_read (uint32_t bsize, uint8_t *p_table)
{
    uint16_t end_flag;

    g_usb_pstd_pipe0_request = USB_ON;

    g_usb_pstd_data_cnt[USB_PIPE0] = bsize;
    gp_usb_pstd_data[USB_PIPE0] = p_table;

    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, (uint16_t) USB_ISEL);

    /* Buffer clear */
    hw_usb_set_bclr(USB_CUSE);

    hw_usb_clear_status_bemp(USB_PIPE0);

    /* Peripheral Control sequence */
    end_flag = usb_pstd_write_data(USB_PIPE0, USB_CUSE);

    /* Peripheral control sequence */
    switch (end_flag)
    {
        /* End of data write */
        case USB_WRITESHRT :

            /* Enable not ready interrupt */
            usb_cstd_nrdy_enable((uint16_t) USB_PIPE0);

            /* Set PID=BUF */
            usb_cstd_set_buf((uint16_t) USB_PIPE0);
            break;

        /* End of data write (not null) */
        case USB_WRITEEND :

        /* Continue */
        /* Continue of data write */
        case USB_WRITING :

            /* Enable empty interrupt */
            hw_usb_set_bempenb((uint16_t) USB_PIPE0);

            /* Enable not ready interrupt */
            usb_cstd_nrdy_enable((uint16_t) USB_PIPE0);

            /* Set PID=BUF */
            usb_cstd_set_buf((uint16_t) USB_PIPE0);

            break;

        /* FIFO access error */
        case USB_FIFOERROR :
            break;

        default :
            break;
    }

    return (end_flag);     /* End or error or continue */
} /* End of function usb_pstd_ctrl_read */

/******************************************************************************
 Function Name   : usb_pstd_ctrl_write
 Description     : Called by USB_PstdCtrlWrite, see it for description.
 Arguments       : uint32_t bsize      : Write size in bytes.
                 : uint8_t *p_table    : Start address of write data buffer.
 Return value    : none
 ******************************************************************************/
void usb_pstd_ctrl_write (uint32_t bsize, uint8_t *p_table)
{
    g_usb_pstd_pipe0_request = USB_ON;

    g_usb_pstd_data_cnt[USB_PIPE0] = bsize;
    gp_usb_pstd_data[USB_PIPE0] = p_table;

    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, USB_FALSE);

    /* Buffer clear */
    hw_usb_set_bclr(USB_CUSE);

    /* Interrupt enable */
    /* Enable ready interrupt */
    hw_usb_set_brdyenb((uint16_t) USB_PIPE0);

    /* Enable not ready interrupt */
    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0);

    /* Set PID=BUF */
    usb_cstd_set_buf((uint16_t) USB_PIPE0);
} /* End of function usb_pstd_ctrl_write */

/******************************************************************************
 Function Name   : usb_pstd_ctrl_end
 Description     : End control transfer
 Arguments       : uint16_t status  : Transfer end status
 Return value    : none
 ******************************************************************************/
void usb_pstd_ctrl_end (uint16_t status)
{
    g_usb_pstd_pipe0_request = USB_OFF;
    g_usb_pstd_std_request = USB_NO;

    /* Interrupt disable */
    /* BEMP0 disable */
    hw_usb_clear_bempenb((uint16_t) USB_PIPE0);

    /* BRDY0 disable */
    hw_usb_clear_brdyenb((uint16_t) USB_PIPE0);

    /* NRDY0 disable */
    hw_usb_clear_nrdyenb((uint16_t) USB_PIPE0);

    hw_usb_set_mbw(USB_CUSE, USB0_CFIFO_MBW);


    if ((USB_DATA_ERR == status) || (USB_DATA_OVR == status))
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
    else if (USB_DATA_STOP == status)
    {
        /* Pipe stop */
        usb_cstd_set_nak((uint16_t) USB_PIPE0);
    }
    else
    {
        /* Set CCPL bit */
        hw_usb_pset_ccpl();
    }
} /* End of function usb_pstd_ctrl_end */
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 End  Of File
 ******************************************************************************/
