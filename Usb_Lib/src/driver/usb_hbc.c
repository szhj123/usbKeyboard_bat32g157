/******************************************************************************
* File Name    : usb_hbc.c
* Version      : 1.01
* Description  : This is the USB host battery charging code.
*******************************************************************************/
/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "usb_basic_mini_if.h"
#include "usb_bitdefine.h"
#include "usb_typedef.h"
#include "usb_reg_access.h"            /* Definition of the USB register access macro */
#include "usb_extern.h"

#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST

#if USB_CFG_BC == USB_CFG_ENABLE
/*******************************************************************************
 Macro definitions
 ******************************************************************************/
/* PD Detect Define */
#define USB_BC_NODET        (0x00u)
#define USB_BC_PDDET        (0x01u)

/* DCP Mode Setting Define */
#if USB_CFG_DCP == USB_CFG_ENABLE
    #define USB_BC_DCPMODE      (0x01u)
#else   /* USB_CFG_DCP == USB_CFG_ENABLE */
    #define USB_BC_DCPMODE      (0x00u)
#endif  /* USB_CFG_DCP == USB_CFG_ENABLE */
/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
USB_STATIC    void      usb_hstd_bc_err(void);
USB_STATIC    void      usb_hstd_bc_init_vb(void);
USB_STATIC    void      usb_hstd_bc_det_at(void);
USB_STATIC    void      usb_hstd_bc_cdp_dt(void);
USB_STATIC    void      usb_hstd_bc_sdp_dt(void);
USB_STATIC    void      usb_hstd_bc_det_entry(void);
USB_STATIC    void      usb_hstd_bc_det_exit(void);
USB_STATIC    void      usb_hstd_bc_cdp_entry(void);
USB_STATIC    void      usb_hstd_bc_cdp_exit(void);
USB_STATIC    void      usb_hstd_bc_sdp_entry(void);
USB_STATIC    void      usb_hstd_bc_sdp_exit(void);
USB_STATIC    void      usb_hstd_bc_dcp_entry(void);

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* variables */
usb_bc_status_t g_usb_hstd_bc;

/* BC State change function table */
void (*gp_usb_hstd_bc_func[USB_BC_STATE_MAX][USB_BC_EVENT_MAX])(void) =
{
    /* VBUS_ON              ATTACH               DETACH */
    { usb_hstd_bc_init_vb, usb_hstd_bc_err, usb_hstd_bc_err    },
    { usb_hstd_bc_err, usb_hstd_bc_det_at, usb_hstd_bc_err    },
    { usb_hstd_bc_err, usb_hstd_bc_err, usb_hstd_bc_cdp_dt },
    { usb_hstd_bc_err, usb_hstd_bc_err, usb_hstd_bc_sdp_dt },
    { usb_hstd_bc_err, usb_hstd_bc_err, usb_hstd_bc_err    }
};



/******************************************************************************
Function Name   : usb_hstd_pddetint_process
Description     : PDDETINT process
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_pddetint_process(void)
{
    uint16_t    buf[3];

    /* PDDETSTS chattering cut */
    /* WAIT_LOOP */
    do
    {
        buf[0] = hw_usb_read_bcctrl();
        usb_cpu_delay_1us(10);
        buf[1] = hw_usb_read_bcctrl();
        usb_cpu_delay_1us(10);
        buf[2] = hw_usb_read_bcctrl();
    }
    while (((buf[0] & USB_PDDETSTS) != (buf[1] & USB_PDDETSTS)) ||
            ((buf[1] & USB_PDDETSTS) != (buf[2] & USB_PDDETSTS)) );

    if ((buf[0] & USB_PDDETSTS) == USB_PDDETSTS )       /* VDPSRC Detect */
    {
        if ((buf[0] & USB_VDMSRCE) != USB_VDMSRCE )
        {
            hw_usb_set_vdmsrce();
        }
    }
    else                                                /* VDPSRC Not detect */
    {
        if ((buf[0] & USB_VDMSRCE) == USB_VDMSRCE )
        {
            hw_usb_clear_vdmsrce();
            g_usb_hstd_bc.pd_detect = USB_BC_PDDET;
        }
    }
}   /* End of function usb_hstd_pddetint_process() */


/******************************************************************************
Function Name   : usb_hstd_bc_err
Description     : BC State change function [ERROR]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_err(void)
{
    /* Do Nothing */
}   /*  End of function  usb_hstd_bc_err() */


/******************************************************************************
Function Name   : usb_hstd_bc_init_vb
Description     : BC State change function [INIT] [VbusOn]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_init_vb(void)
{
    g_usb_hstd_bc.dcpmode = USB_BC_DCPMODE;

    if (0 != g_usb_hstd_bc.dcpmode)
    {
        g_usb_hstd_bc.state = USB_BC_STATE_DCP;
        usb_hstd_bc_dcp_entry();
    }
    else
    {
        g_usb_hstd_bc.state = USB_BC_STATE_DET;
        usb_hstd_bc_det_entry();
    }
}   /*  End of function  usb_hstd_bc_init_vb() */


/******************************************************************************
Function Name   : usb_hstd_bc_det_at
Description     : BC State change function [DET] [Attach]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_det_at(void)
{
    usb_hstd_bc_det_exit();

    if (0 != g_usb_hstd_bc.pd_detect)
    {
        g_usb_hstd_bc.state = USB_BC_STATE_CDP;
        usb_hstd_bc_cdp_entry();
    }
    else
    {
        g_usb_hstd_bc.state = USB_BC_STATE_SDP;
        usb_hstd_bc_sdp_entry();
    }
}   /*  End of function  usb_hstd_bc_det_at() */


/******************************************************************************
Function Name   : usb_hstd_bc_cdp_dt
Description     : BC State change function [CDP] [Detach]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_cdp_dt(void)
{
    usb_hstd_bc_cdp_exit();
    g_usb_hstd_bc.state = USB_BC_STATE_DET;
    usb_hstd_bc_det_entry();
}   /*  End of function  usb_hstd_bc_cdp_dt() */


/******************************************************************************
Function Name   : usb_hstd_bc_sdp_dt
Description     : BC State change function [SDP] [Detach]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_sdp_dt(void)
{
    usb_hstd_bc_sdp_exit();
    g_usb_hstd_bc.state = USB_BC_STATE_DET;
    usb_hstd_bc_det_entry();
}   /*  End of function  usb_hstd_bc_sdp_dt() */


/******************************************************************************
Function Name   : usb_hstd_bc_det_entry
Description     : BC State entry function [DET]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_det_entry(void)
{
    hw_usb_set_batchge();
    hw_usb_set_idpsinke();
    hw_usb_hclear_sts_pddetint();
    hw_usb_hset_enb_pddetinte();
}   /*  End of function  usb_hstd_bc_det_entry() */


/******************************************************************************
Function Name   : usb_hstd_bc_det_exit
Description     : BC State exit function [DET]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_det_exit(void)
{
    hw_usb_hset_enb_pddetinte();
    hw_usb_hclear_sts_pddetint();
    hw_usb_clear_idpsinke();
    hw_usb_clear_batchge();
}   /*  End of function  usb_hstd_bc_det_exit() */


/******************************************************************************
Function Name   : usb_hstd_bc_cdp_entry
Description     : BC State entry function [CDP]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_cdp_entry(void)
{
    /* Do Nothing */
}   /*  End of function  usb_hstd_bc_cdp_entry() */


/******************************************************************************
Function Name   : usb_hstd_bc_cdp_exit
Description     : BC State exit function [CDP]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_cdp_exit(void)
{
    g_usb_hstd_bc.pd_detect = USB_BC_NODET;
}   /*  End of function  usb_hstd_bc_cdp_exit() */


/******************************************************************************
Function Name   : usb_hstd_bc_sdp_entry
Description     : BC State entry function [SDP]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_sdp_entry(void)
{
}   /*  End of function  usb_hstd_bc_sdp_entry() */


/******************************************************************************
Function Name   : usb_hstd_bc_sdp_exit
Description     : BC State exit function [SDP]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_sdp_exit(void)
{
    /* Do Nothing */
}   /*  End of function  usb_hstd_bc_sdp_exit() */


/******************************************************************************
Function Name   : usb_hstd_bc_dcp_entry
Description     : BC State entry function [DCP]
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_bc_dcp_entry(void)
{
    hw_usb_clear_drpd();
    hw_usb_set_batchge();
    hw_usb_hset_dcpmode();
}   /*  End of function  usb_hstd_bc_dcp_entry() */

#endif  /* USB_CFG_BC == USB_CFG_ENABLE */

#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
End of file
******************************************************************************/
