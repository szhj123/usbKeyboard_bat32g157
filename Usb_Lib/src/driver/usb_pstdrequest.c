/******************************************************************************
 * File Name    : usb_pstdrequest.c
 * Description  : USB Peripheral standard request code
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "usb_basic_mini_if.h"
#include "usb_typedef.h"
#include "usb_extern.h"
#include "usb_bitdefine.h"
#include "usb_reg_access.h"

#if defined(USB_CFG_PMSC_USE)
    #include "usb_pmsc_mini_if.h"
#endif  /* defined(USB_CFG_PMSC_USE) */


#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

/******************************************************************************
 Macro definitions
******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void usb_pstd_get_status1 (void);
static void usb_pstd_get_descriptor1 (void);
static void usb_pstd_get_configuration1 (void);
static void usb_pstd_get_interface1 (void);
static void usb_pstd_clr_feature0 (void);
static void usb_pstd_clr_feature3 (void);
static void usb_pstd_set_feature0 (void);
static void usb_pstd_set_feature3 (void);
static void usb_pstd_set_address0 (void);
static void usb_pstd_set_address3 (void);
static void usb_pstd_set_descriptor2 (void);
static void usb_pstd_set_configuration0 (void);
static void usb_pstd_set_configuration3 (void);
static void usb_pstd_set_interface0 (void);
static void usb_pstd_set_interface3 (void);
static void usb_pstd_synch_rame1 (void);
#if USB_CFG_REQUEST == USB_CFG_ENABLE
    static void usb_pstd_request_event_set (void);
#endif /* USB_CFG_REQUEST == USB_CFG_ENABLE */


/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_pstd_stand_req0
 Description     : The idle and setup stages of a standard request from host.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req0 (void)
{
    g_usb_pstd_std_request = USB_NO;

    switch ((g_usb_pstd_req_type & USB_BREQUEST))
    {
        case USB_CLEAR_FEATURE :
            usb_pstd_clr_feature0();            /* Clear Feature0 */
            break;

        case USB_SET_FEATURE :
            usb_pstd_set_feature0();            /* Set Feature0 */
            break;

        case USB_SET_ADDRESS :
            usb_pstd_set_address0();            /* Set Address0 */
            break;

        case USB_SET_CONFIGURATION :
            usb_pstd_set_configuration0();      /* Set Configuration0 */
            break;

        case USB_SET_INTERFACE :
            usb_pstd_set_interface0();          /* Set Interface0 */
            break;

        default :
            break;
    }
} /* End of function usb_pstd_stand_req0 */

/******************************************************************************
 Function Name   : usb_pstd_stand_req1
 Description     : The control read data stage of a standard request from host.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req1 (void)
{
    switch ((g_usb_pstd_req_type & USB_BREQUEST))
    {
        case USB_GET_STATUS :
            usb_pstd_get_status1();           /* Get Status1 */
            break;

        case USB_GET_DESCRIPTOR :
            usb_pstd_get_descriptor1();       /* Get Descriptor1 */
            break;

        case USB_GET_CONFIGURATION :
            usb_pstd_get_configuration1();    /* Get Configuration1 */
            break;

        case USB_GET_INTERFACE :
            usb_pstd_get_interface1();        /* Get Interface1 */
            break;

        case USB_SYNCH_FRAME :
            usb_pstd_synch_rame1();           /* Synch Frame */
            break;

        default :
            usb_pstd_set_stall_pipe0();       /* Set pipe USB_PID_STALL */
            break;
    }
} /* End of function usb_pstd_stand_req1 */


/******************************************************************************
 Function Name   : usb_pstd_stand_req2
 Description     : The control write data stage of a standard request from host.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req2 (void)
{
    if ((g_usb_pstd_req_type & USB_BREQUEST) == USB_SET_DESCRIPTOR)
    {
        /* Set Descriptor2 */
        usb_pstd_set_descriptor2();
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_stand_req2 */


/******************************************************************************
 Function Name   : usb_pstd_stand_req3
 Description     : Standard request process. This is for the status stage of a
                 : control write where there is no data stage.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req3 (void)
{
    usb_ctrl_t ctrl;

    switch ((g_usb_pstd_req_type & USB_BREQUEST))
    {
        case USB_CLEAR_FEATURE :
            usb_pstd_clr_feature3();            /* ClearFeature3 */
            break;

        case USB_SET_FEATURE :
            usb_pstd_set_feature3();            /* SetFeature3 */
            break;

        case USB_SET_ADDRESS :
            usb_pstd_set_address3();            /* SetAddress3 */
            break;

        case USB_SET_CONFIGURATION :
            usb_pstd_set_configuration3();      /* SetConfiguration3 */
            break;

        case USB_SET_INTERFACE :
            usb_pstd_set_interface3();          /* SetInterface3 */
            break;

        default :
            usb_pstd_set_stall_pipe0();         /* Set pipe USB_PID_STALL */
            break;
    }

    if (USB_YES == g_usb_pstd_std_request)
    {
        ctrl.setup.type   = g_usb_pstd_req_type;    /* Request type */
        ctrl.setup.value  = g_usb_pstd_req_value;   /* Value */
        ctrl.setup.index  = g_usb_pstd_req_index;   /* Index */
        ctrl.setup.length = g_usb_pstd_req_length;  /* Length */
        ctrl.size   = 0;
        ctrl.status = USB_ACK;
        ctrl.type   = USB_REQUEST;
        usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);

        g_usb_pstd_std_request = USB_NO;
    }

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END); /* Control transfer stop(end) */

} /* End of function usb_pstd_stand_req3 */


/******************************************************************************
 Function Name   : usb_pstd_stand_req4
 Description     : The control read status stage of a standard request from host.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req4 (void)
{
    usb_ctrl_t ctrl;

    switch ((g_usb_pstd_req_type & USB_BREQUEST))
    {
        case USB_GET_STATUS :

            usb_cstd_set_buf((uint16_t) USB_PIPE0);            /* GetStatus4 */
            break;

        case USB_GET_DESCRIPTOR :

            usb_cstd_set_buf((uint16_t) USB_PIPE0);            /* GetDescriptor4 */
            break;

        case USB_GET_CONFIGURATION :

            usb_cstd_set_buf((uint16_t) USB_PIPE0);            /* GetConfiguration4 */
            break;

        case USB_GET_INTERFACE :

            usb_cstd_set_buf((uint16_t) USB_PIPE0);            /* GetInterface4 */
            break;

        case USB_SYNCH_FRAME :

            usb_cstd_set_buf((uint16_t) USB_PIPE0);            /* SynchFrame4 */
            break;

        default :

            usb_pstd_set_stall_pipe0();            /* Set pipe USB_PID_STALL */
            break;
    }

    if (USB_YES == g_usb_pstd_std_request)
    {
        ctrl.setup.type   = g_usb_pstd_req_type;    /* Request type */
        ctrl.setup.value  = g_usb_pstd_req_value;   /* Value */
        ctrl.setup.index  = g_usb_pstd_req_index;   /* Index */
        ctrl.setup.length = g_usb_pstd_req_length;  /* Length */
        ctrl.size   = 0;
        ctrl.status = USB_ACK;
        ctrl.type   = USB_REQUEST;
        usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);

        g_usb_pstd_std_request = USB_NO;
    }

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END);    /* Control transfer stop(end) */

} /* End of function usb_pstd_stand_req4 */


/******************************************************************************
 Function Name   : usb_pstd_stand_req5
 Description     : The control write status stage of a standard request from host.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_stand_req5 (void)
{
    usb_ctrl_t ctrl;

    if ((g_usb_pstd_req_type & USB_BREQUEST) == USB_SET_DESCRIPTOR)
    {
        /* Set pipe PID_BUF */
        usb_cstd_set_buf((uint16_t) USB_PIPE0);
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_set_stall_pipe0();
    }

    if (USB_YES == g_usb_pstd_std_request)
    {
        ctrl.setup.type   = g_usb_pstd_req_type;    /* Request type */
        ctrl.setup.value  = g_usb_pstd_req_value;   /* Value */
        ctrl.setup.index  = g_usb_pstd_req_index;   /* Index */
        ctrl.setup.length = g_usb_pstd_req_length;  /* Length */

        ctrl.size   = 0;
        ctrl.status = USB_ACK;
        ctrl.type   = USB_REQUEST;
        usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);

        g_usb_pstd_std_request = USB_NO;
    }

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END);    /* Control transfer stop(end) */

} /* End of function usb_pstd_stand_req5 */

/******************************************************************************
 Function Name   : usb_pstd_get_status1
 Description     : Analyze a Get Status command and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_get_status1 (void)
{
    static uint8_t s_tbl[2];
    uint16_t ep;
    uint16_t buffer;
    uint16_t pipe;

    if ((0 == g_usb_pstd_req_value) && (2 == g_usb_pstd_req_length))
    {
        s_tbl[0] = 0;
        s_tbl[1] = 0;

        /* Check request type */
        switch ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP))
        {
            case USB_DEVICE :
                if (0 == g_usb_pstd_req_index)
                {
                    /* Self powered / Bus powered */
                    s_tbl[0] = usb_pstd_get_current_power();

                    /* Support remote wakeup ? */
                    if (USB_TRUE == g_usb_pstd_remote_wakeup)
                    {
                        s_tbl[0] |= USB_GS_REMOTEWAKEUP;
                    }

                    /* Control read start */
                    usb_pstd_ctrl_read((uint32_t) 2, s_tbl);
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            case USB_INTERFACE :
                if (usb_pstd_chk_configured() == USB_TRUE)
                {
                    if (g_usb_pstd_req_index < usb_pstd_get_interface_no())
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) 2, s_tbl);
                    }
                    else
                    {
                        /* Request error (not exist interface) */
                        usb_pstd_set_stall_pipe0();
                    }
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            case USB_ENDPOINT :

                /* Endpoint number */
                ep = (uint16_t) (g_usb_pstd_req_index & USB_EPNUMFIELD);

                /* Endpoint 0 */
                if (0 == ep)
                {
                    buffer = hw_usb_read_dcpctr();

                    if ((buffer & USB_PID_STALL) != (uint16_t) 0)
                    {
                        /* Halt set */
                        s_tbl[0] = USB_GS_HALT;
                    }

                    /* Control read start */
                    usb_pstd_ctrl_read((uint32_t) 2, s_tbl);
                }

                /* EP1 to max */
                else if (ep <= USB_MAX_EP_NO)
                {
                    if (usb_pstd_chk_configured() == USB_TRUE)
                    {
                        pipe = usb_pstd_epadr_to_pipe(g_usb_pstd_req_index);

                        if (USB_ERROR == pipe)
                        {
                            /* Set pipe USB_PID_STALL */
                            usb_pstd_set_stall_pipe0();
                        }
                        else
                        {
                            buffer = usb_cstd_get_pid(pipe);

                            if ((buffer & USB_PID_STALL) != (uint16_t) 0)
                            {
                                /* Halt set */
                                s_tbl[0] = USB_GS_HALT;
                            }

                            /* Control read start */
                            usb_pstd_ctrl_read((uint32_t) 2, s_tbl);
                        }
                    }
                    else
                    {
                        /* Set pipe USB_PID_STALL */
                        usb_pstd_set_stall_pipe0();
                    }
                }
                else
                {
                    /* Set pipe USB_PID_STALL */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            default :

                /* Set pipe USB_PID_STALL */
                usb_pstd_set_stall_pipe0();
                break;
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_get_status1 */

/******************************************************************************
 Function Name   : usb_pstd_get_descriptor1
 Description     : Analyze a Get Descriptor command from host and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_get_descriptor1 (void)
{
    uint16_t len;
    uint16_t idx;
    uint8_t  *p_table;

    if ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_DEVICE)
    {
        idx = (uint16_t) (g_usb_pstd_req_value & USB_DT_INDEX);

        switch ((uint16_t) USB_GET_DT_TYPE(g_usb_pstd_req_value))
        {
            /*---- Device descriptor ----*/
            case USB_DT_DEVICE :
                if (((uint16_t) 0 == g_usb_pstd_req_index) && ((uint16_t) 0 == idx))
                {
                    p_table = g_usb_pstd_driver.p_devicetbl;

                    if (g_usb_pstd_req_length < p_table[0])
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) g_usb_pstd_req_length, p_table);
                    }
                    else
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) p_table[0], p_table);
                    }
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            /*---- Configuration descriptor ----*/
            case USB_DT_CONFIGURATION :
                if ((0 == g_usb_pstd_req_index) && ((uint16_t) 0 == idx))
                {
                    p_table = g_usb_pstd_driver.p_configtbl;
                    len = (uint16_t) (*(uint8_t*) ((uint32_t) p_table + (uint32_t) 3));
                    len = (uint16_t) (len << 8);
                    len += (uint16_t) (*(uint8_t*) ((uint32_t) p_table + (uint32_t) 2));

                    /* Descriptor > wLength */
                    if (g_usb_pstd_req_length < len)
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) g_usb_pstd_req_length, p_table);
                    }
                    else
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) len, p_table);
                    }
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            /*---- String descriptor ----*/
            case USB_DT_STRING :
                if (idx < g_usb_pstd_driver.num_string)
                {
                    p_table = g_usb_pstd_driver.pp_stringtbl[idx];
                    len = (uint16_t) (*(uint8_t*) ((uint32_t) p_table + (uint32_t) 0));

                    if (g_usb_pstd_req_length < len)
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) g_usb_pstd_req_length, p_table);
                    }
                    else
                    {
                        /* Control read start */
                        usb_pstd_ctrl_read((uint32_t) len, p_table);
                    }
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            /*---- Interface descriptor ----*/
            case USB_DT_INTERFACE :

                /* Set pipe USB_PID_STALL */
                usb_pstd_set_stall_pipe0();
                break;

            /*---- Endpoint descriptor ----*/
            case USB_DT_ENDPOINT :

                /* Set pipe USB_PID_STALL */
                usb_pstd_set_stall_pipe0();
                break;

            case USB_DT_DEVICE_QUALIFIER :
                /* Request error */
                usb_pstd_set_stall_pipe0();
                break;

            case USB_DT_OTHER_SPEED_CONF :
                /* Request error */
                usb_pstd_set_stall_pipe0();
                break;

            case USB_DT_INTERFACE_POWER :

                /* Not support */
                usb_pstd_set_stall_pipe0();
                break;

            default :

                /* Set pipe USB_PID_STALL */
                usb_pstd_set_stall_pipe0();
                break;
        }
    }
    else if ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_INTERFACE)
    {
        g_usb_pstd_req_reg.type   = g_usb_pstd_req_type;
        g_usb_pstd_req_reg.value  = g_usb_pstd_req_value;
        g_usb_pstd_req_reg.index  = g_usb_pstd_req_index;
        g_usb_pstd_req_reg.length = g_usb_pstd_req_length;

        if (USB_NULL != g_usb_pstd_driver.ctrltrans)
        {
            (*g_usb_pstd_driver.ctrltrans)((usb_setup_t *) &g_usb_pstd_req_reg, (uint16_t) USB_NO_ARG);
        }
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_get_descriptor1 */

/******************************************************************************
 Function Name   : usb_pstd_get_configuration1
 Description     : Analyze a Get Configuration command and process it.
                 : (for control read data stage)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_get_configuration1 (void)
{
    static uint8_t tbl[2];

    /* check request */
    if (((((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_DEVICE) && (0 == g_usb_pstd_req_value))
            && (0 == g_usb_pstd_req_index)) && (1 == g_usb_pstd_req_length))
    {
        tbl[0] = (uint8_t) g_usb_pstd_config_no;

        /* Control read start */
        usb_pstd_ctrl_read((uint32_t) 1, tbl);
    }
    else
    {
        /* Set pipe USB_PID_STALL */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_get_configuration1 */

/******************************************************************************
 Function Name   : usb_pstd_get_interface1
 Description     : Analyze a Get Interface command and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_get_interface1 (void)
{
    static uint8_t tbl[2];

    /* check request */
    if ((((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_INTERFACE) && (0 == g_usb_pstd_req_value))
            && (1 == g_usb_pstd_req_length))
    {
        if (g_usb_pstd_req_index < USB_ALT_NO)
        {
            tbl[0] = (uint8_t) g_usb_pstd_alt_no[g_usb_pstd_req_index];

            /* Start control read */
            usb_pstd_ctrl_read((uint32_t) 1, tbl);
        }
        else
        {
            /* Request error */
            usb_pstd_set_stall_pipe0();
        }
    }
    else
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_get_interface1 */

/******************************************************************************
 Function Name   : usb_pstd_clr_feature0
 Description     : Clear Feature0
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_clr_feature0 (void)
{
    #if USB_CFG_REQUEST == USB_CFG_ENABLE

    if (0 == g_usb_pstd_req_length)
    {
        /* check request type */
        if (USB_DEVICE == (g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP))
        {
            if ((USB_DEV_REMOTE_WAKEUP == g_usb_pstd_req_value) && (0 == g_usb_pstd_req_index))
            {
                if (usb_pstd_chk_remote() == USB_TRUE)
                {
                    usb_pstd_request_event_set ();
                }
            }
        }
    }

    #endif /* USB_CFG_REQUEST == USB_CFG_ENABLE */

} /* End of function usb_pstd_clr_feature0 */

/******************************************************************************
 Function Name   : usb_pstd_clr_feature3
 Description     : Analyze a Clear Feature command and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_clr_feature3 (void)
{
    uint16_t pipe;
    uint16_t ep;

    if (0 == g_usb_pstd_req_length)
    {
        /* check request type */
        switch ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP))
        {
            case USB_DEVICE :
                if ((USB_DEV_REMOTE_WAKEUP == g_usb_pstd_req_value) && (0 == g_usb_pstd_req_index))
                {
                    if (usb_pstd_chk_remote() == USB_TRUE)
                    {
                        g_usb_pstd_remote_wakeup = USB_FALSE;

                        /* Set pipe PID_BUF */
                        usb_cstd_set_buf((uint16_t) USB_PIPE0);
                    }
                    else
                    {
                        /* Not support remote wakeup */
                        /* Request error */
                        usb_pstd_set_stall_pipe0();
                    }
                }
                else
                {
                    /* Not specification */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            case USB_INTERFACE :

                /* Request error */
                usb_pstd_set_stall_pipe0();
                break;

            case USB_ENDPOINT :

                /* Endpoint number */
                ep = (uint16_t) (g_usb_pstd_req_index & USB_EPNUMFIELD);

                if (USB_ENDPOINT_HALT == g_usb_pstd_req_value)
                {
                    /* EP0 */
                    if (0 == ep)
                    {
                        /* Stall clear */
                        usb_cstd_clr_stall((uint16_t) USB_PIPE0);

                        /* Set pipe PID_BUF */
                        usb_cstd_set_buf((uint16_t) USB_PIPE0);
                    }

                    /* EP1 to max */
                    else if (ep <= USB_MAX_EP_NO)
                    {
                        pipe = usb_pstd_epadr_to_pipe(g_usb_pstd_req_index);

                        if (USB_ERROR == pipe)
                        {
                            /* Request error */
                            usb_pstd_set_stall_pipe0();
                        }
                        else
                        {
                            if (USB_PID_BUF == usb_cstd_get_pid(pipe))
                            {
                                usb_cstd_set_nak(pipe);

                                /* SQCLR=1 */
                                hw_usb_set_sqclr(pipe);

                                /* Set pipe PID_BUF */
                                usb_cstd_set_buf(pipe);
                            }
                            else
                            {
                                usb_cstd_clr_stall(pipe);

                                /* SQCLR=1 */
                                hw_usb_set_sqclr(pipe);
                            }

                            /* Set pipe PID_BUF */
                            usb_cstd_set_buf((uint16_t) USB_PIPE0);

                            if (USB_TRUE == g_usb_pstd_is_stall_pipe[pipe])
                            {
                                g_usb_pstd_is_stall_pipe[pipe] = USB_FALSE;
                                (*g_usb_pstd_stall_cb)(USB_NULL, USB_NULL, USB_NULL);
                            }
                        }
                    }
                    else
                    {
                        /* Request error */
                        usb_pstd_set_stall_pipe0();
                    }
                }
                else
                {
                    /* Request error */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            default :
                usb_pstd_set_stall_pipe0();
                break;
        }
    }
    else
    {
        /* Not specification */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_clr_feature3 */

/******************************************************************************
 Function Name   : usb_pstd_set_feature0
 Description     : Set Feature0 (for idle/setup stage)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_feature0 (void)
{
    #if USB_CFG_REQUEST == USB_CFG_ENABLE

    if (0 == g_usb_pstd_req_length)
    {
        /* check request type */
        if (USB_DEVICE == (g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP))
        {
            if ((USB_DEV_REMOTE_WAKEUP == g_usb_pstd_req_value) && (0 == g_usb_pstd_req_index))
            {
                if (usb_pstd_chk_remote() == USB_TRUE)
                {
                    usb_pstd_request_event_set ();

                }
            }
        }
    }

    #endif /* USB_CFG_REQUEST == USB_CFG_ENABLE */
} /* End of function usb_pstd_set_feature0 */

/******************************************************************************
 Function Name   : usb_pstd_set_feature3
 Description     : Analyze a Set Feature command and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_feature3 (void)
{
    uint16_t pipe;
    uint16_t ep;

    if (0 == g_usb_pstd_req_length)
    {
        /* check request type */
        switch ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP))
        {
            case USB_DEVICE :
                switch (g_usb_pstd_req_value)
                {
                    case USB_DEV_REMOTE_WAKEUP :
                        if (0 == g_usb_pstd_req_index)
                        {
                            if (USB_TRUE == usb_pstd_chk_remote())
                            {
                                g_usb_pstd_remote_wakeup = USB_TRUE;

                                /* Set pipe PID_BUF */
                                usb_cstd_set_buf((uint16_t) USB_PIPE0);
                            }
                            else
                            {
                                /* Not support remote wakeup */
                                /* Request error */
                                usb_pstd_set_stall_pipe0();
                            }
                        }
                        else
                        {
                            /* Not specification */
                            usb_pstd_set_stall_pipe0();
                        }

                        break;

                    case USB_TEST_MODE :
                        /* Not specification */
                        usb_pstd_set_stall_pipe0();
                        break;

                    default :
                        usb_pstd_set_feature_function();
                        break;
                }

                break;

            case USB_INTERFACE :

                /* Set pipe USB_PID_STALL */
                usb_pstd_set_stall_pipe0();
                break;

            case USB_ENDPOINT :

                /* Endpoint number */
                ep = (uint16_t) (g_usb_pstd_req_index & USB_EPNUMFIELD);

                if (USB_ENDPOINT_HALT == g_usb_pstd_req_value)
                {
                    /* EP0 */
                    if (0 == ep)
                    {
                        /* Set pipe PID_BUF */
                        usb_cstd_set_buf((uint16_t) USB_PIPE0);
                    }

                    /* EP1 to max */
                    else if (ep <= USB_MAX_EP_NO)
                    {
                        pipe = usb_pstd_epadr_to_pipe(g_usb_pstd_req_index);

                        if (USB_ERROR == pipe)
                        {
                            /* Request error */
                            usb_pstd_set_stall_pipe0();
                        }
                        else
                        {
                            /* Set pipe USB_PID_STALL */
                            usb_pstd_set_stall(pipe);

                            /* Set pipe PID_BUF */
                            usb_cstd_set_buf((uint16_t) USB_PIPE0);
                        }
                    }
                    else
                    {
                        /* Request error */
                        usb_pstd_set_stall_pipe0();
                    }
                }
                else
                {
                    /* Not specification */
                    usb_pstd_set_stall_pipe0();
                }

                break;

            default :

                /* Request error */
                usb_pstd_set_stall_pipe0();
                break;
        }
    }
    else
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_set_feature3 */


/******************************************************************************
 Function Name   : usb_pstd_set_address0
 Description     : Set Address0 (for idle/setup stage).
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_address0 (void)
{
    /* Non processing. */
} /* End of function usb_pstd_set_address0 */


/******************************************************************************
 Function Name   : usb_pstd_set_address3
 Description     : Analyze a Set Address command and process it.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_address3 (void)
{
    if ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_DEVICE)
    {
        if ((0 == g_usb_pstd_req_index) && (0 == g_usb_pstd_req_length))
        {
            if (g_usb_pstd_req_value <= 127)
            {
                /* Set pipe PID_BUF */
                usb_cstd_set_buf((uint16_t) USB_PIPE0);
            }
            else
            {
                /* Not specification */
                usb_pstd_set_stall_pipe0();
            }
        }
        else
        {
            /* Not specification */
            usb_pstd_set_stall_pipe0();
        }
    }
    else
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_set_address3 */


/******************************************************************************
 Function Name   : usb_pstd_set_descriptor2
 Description     : Return STALL in response to a Set Descriptor command.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_descriptor2 (void)
{
    /* Not specification */
    usb_pstd_set_stall_pipe0();
} /* End of function usb_pstd_set_descriptor2 */


/******************************************************************************
 Function Name   : usb_pstd_set_configuration0
 Description     : Call callback function to notify the reception of SetConfiguration command
                 : (for idle /setup stage)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_configuration0 (void)
{
    uint16_t config_num;

    config_num = g_usb_pstd_config_no;

    /* Configuration number set */
    usb_pstd_set_config_no(g_usb_pstd_req_value);

    if (g_usb_pstd_req_value != config_num)
    {
        if (USB_NULL != g_usb_pstd_driver.devconfig)
        {
            /* Registration open function call */
            (*g_usb_pstd_driver.devconfig)(USB_NULL, g_usb_pstd_config_no, USB_NULL);
        }
    }
} /* End of function usb_pstd_set_configuration0 */

/******************************************************************************
 Function Name   : usb_pstd_set_configuration3
 Description     : Analyze a Set Configuration command and process it. This is
                 : for the status stage of a control write where there is no data
                 : stage.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_configuration3 (void)
{
    uint16_t i;
    uint16_t ifc;
    uint16_t cfgok;
    uint8_t * p_table2;

    if ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_DEVICE)
    {

        cfgok = USB_ERROR;

        p_table2 = g_usb_pstd_driver.p_configtbl;

        if ((((g_usb_pstd_req_value == p_table2[5]) || (0 == g_usb_pstd_req_value)) && (0 == g_usb_pstd_req_index))
                && (0 == g_usb_pstd_req_length))
        {
            usb_cstd_set_buf((uint16_t) USB_PIPE0);
            cfgok = USB_OK;

            if ((g_usb_pstd_req_value > 0) && (g_usb_pstd_req_value != g_usb_pstd_config_no))
            {
                ifc = usb_pstd_get_interface_no();

                /* WAIT_LOOP */
                for (i = 0; i < ifc; ++i)
                {
                    /* Pipe Information Table ("endpoint table") initialize */
                    usb_pstd_set_eptbl_index(i, (uint16_t) 0);
                }

                usb_cstd_pipe_reg_clear();
                usb_pstd_set_pipe_reg();

            }
        }

        if (USB_ERROR == cfgok)
        {
            /* Request error */
            usb_pstd_set_stall_pipe0();
        }
    }
    else
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_set_configuration3 */


/******************************************************************************
 Function Name   : usb_pstd_set_interface0
 Description     : Call callback function to notify reception of SetInterface com-
                 : mand. For idle/setup stage.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_interface0 (void)
{
    if (USB_NULL != g_usb_pstd_driver.interface)
    {

        #if USB_CFG_REQUEST == USB_CFG_ENABLE
        usb_pstd_request_event_set ();
        #endif /* USB_CFG_REQUEST == USB_CFG_ENABLE */

        /* Interfaced change function call */
        (*g_usb_pstd_driver.interface)(USB_NULL, g_usb_pstd_alt_no[g_usb_pstd_req_index], USB_NULL);
    }
} /* End of function usb_pstd_set_interface0 */


/******************************************************************************
 Function Name   : usb_pstd_set_interface3
 Description     : Analyze a Set Interface command and request the process for
                 : the command. This is for a status stage of a control write
                 : where there is no data stage.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_set_interface3 (void)
{
    /* Configured ? */
    if ((usb_pstd_chk_configured() == USB_TRUE) && ((g_usb_pstd_req_type & USB_BMREQUESTTYPERECIP) == USB_INTERFACE))
    {
        if ((g_usb_pstd_req_index <= usb_pstd_get_interface_no()) && (0 == g_usb_pstd_req_length))
        {
            if (g_usb_pstd_req_value <= usb_pstd_get_alternate_no(g_usb_pstd_req_index))
            {
                g_usb_pstd_alt_no[g_usb_pstd_req_index] = (uint16_t) (g_usb_pstd_req_value & USB_ALT_SET);
                usb_cstd_set_buf((uint16_t) USB_PIPE0);

                /* Search endpoint setting */
                usb_pstd_set_eptbl_index(g_usb_pstd_req_index, g_usb_pstd_alt_no[g_usb_pstd_req_index]);
                usb_pstd_set_pipe_reg();
            }
            else
            {
                /* Request error */
                usb_pstd_set_stall_pipe0();
            }
        }
        else
        {
            /* Request error */
            usb_pstd_set_stall_pipe0();
        }
    }
    else
    {
        /* Request error */
        usb_pstd_set_stall_pipe0();
    }
} /* End of function usb_pstd_set_interface3 */


/******************************************************************************
 Function Name   : usb_pstd_synch_rame1
 Description     : Return STALL response to SynchFrame command.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pstd_synch_rame1 (void)
{
    /* Set pipe USB_PID_STALL */
    usb_pstd_set_stall_pipe0();
} /* End of function usb_pstd_synch_rame1 */


/******************************************************************************
 Function Name   : usb_pstd_class_request
 Description     : Class request processing for Device class
 Arguments       : usb_setup_t *p_req    ; Class request information
                 : uint16_t ctsq        ; Control Stage
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request(usb_setup_t *p_req, uint16_t ctsq)
{
    /* Check Request type TYPE */
    if (((p_req->type & USB_BMREQUESTTYPETYPE) == USB_REQUEST_TYPE_CLASS)
            || ((p_req->type & USB_BMREQUESTTYPETYPE) == USB_REQUEST_TYPE_VENDOR))
    {
        /* Branch by the Control Transfer Stage */
        switch (ctsq)
        {
            case USB_CS_IDST :
                g_usb_pstd_pipe0_request = USB_OFF;
                g_usb_pstd_std_request = USB_NO;
                usb_pstd_class_request_ioss(p_req);  /* class request (idle or setup stage) */
                break;

            case USB_CS_RDDS :
                usb_pstd_class_request_rwds(p_req);  /* class request (control read data stage) */
                break;

            case USB_CS_WRDS :
                #if defined(USB_CFG_PMSC_USE)
                usb_pstd_class_request_wds(p_req);   /* class request (control write data stage) */
                #else   /* defined(USB_CFG_PMSC_USE) */
                usb_pstd_class_request_rwds(p_req);  /* class request (control write data stage) */
                #endif  /* defined(USB_CFG_PMSC_USE) */
                break;

            case USB_CS_WRND :
                usb_pstd_class_request_wnss(p_req);  /* class request (control write nodata status stage) */
                break;

            case USB_CS_RDSS :
                usb_pstd_class_request_rss(p_req);   /* class request (control read status stage) */
                break;

            case USB_CS_WRSS :
                usb_pstd_class_request_wss(p_req);   /* class request (control write status stage) */
                break;

            case USB_CS_SQER :
                usb_pstd_ctrl_end((uint16_t) USB_DATA_ERR);     /* End control transfer. */
                break;

            default :
                usb_pstd_ctrl_end((uint16_t) USB_DATA_ERR);     /* End control transfer. */
                break;
        }
    }
    else
    {
        usb_pstd_other_request(p_req);
    }
} /* End of function usb_pstd_class_request */

/******************************************************************************
 Function Name   : usb_pstd_class_request_ioss
 Description     : Class Request (idle or setup stage)
 Arguments       : usb_setup_t *req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_ioss (usb_setup_t *p_req)
{
    /* Non */
} /* End of function usb_pstd_class_request_ioss */

/******************************************************************************
 Function Name   : usb_pstd_class_request_rwds
 Description     : Class request processing (control read/write data stage)
 Arguments       : usb_setup_t *req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_rwds (usb_setup_t * p_req)
{
    #if defined(USB_CFG_PMSC_USE)

    usb_ctrl_t ctrl;

    /* Is a request receive target Interface? */
    if ((0 == p_req->index) && ((p_req->type & USB_BMREQUESTTYPERECIP) == USB_INTERFACE))
    {
        if ((p_req->type & USB_BREQUEST) == USB_GET_MAX_LUN)
        {
            usb_pmsc_get_max_lun(p_req->value, p_req->index, p_req->length);
        }
        else
        {
            /* Get Line Coding Request */
            ctrl.setup  = *p_req; /* Save setup data. */
            ctrl.size   = 0;
            ctrl.status = USB_ACK;
            ctrl.type   = USB_REQUEST;
            usb_cstd_set_event(USB_STS_REQUEST, &ctrl);
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_set_stall_pipe0(); /* Req Error */
    }

    #else   /* defined(USB_CFG_PMSC_USE) */

    usb_ctrl_t ctrl;

    /* Get Line Coding Request */
    ctrl.setup  = *p_req; /* Save setup data. */
    ctrl.size   = 0;
    ctrl.status = USB_ACK;
    ctrl.type   = USB_REQUEST;
    usb_cstd_set_event(USB_STS_REQUEST, &ctrl);
    #endif  /* defined(USB_CFG_PMSC_USE) */
} /* End of function usb_pstd_class_request_rwds */

#if defined(USB_CFG_PMSC_USE)
/******************************************************************************
 Function Name   : usb_pstd_class_request_wds
 Description     : Class request processing (control write data stage)
 Arguments       : usb_setup_t *p_req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_wds (usb_setup_t * p_req)
{
    usb_pstd_set_stall_pipe0();
} /* End of function usb_pstd_class_request_wds */
#endif  /* defined(USB_CFG_PMSC_USE) */

/******************************************************************************
 Function Name   : usb_pstd_other_request
 Description     : Processing to notify the reception of the USB request
 Arguments       : usb_setup_t *p_req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_other_request (usb_setup_t *p_req)
{
    usb_ctrl_t ctrl;

    ctrl.type   = USB_REQUEST;
    ctrl.setup  = *p_req; /* Save setup data. */

    ctrl.size   = 0;
    ctrl.status = USB_ACK;
    usb_cstd_set_event(USB_STS_REQUEST, &ctrl);
} /* End of function usb_pstd_other_request */

/******************************************************************************
 Function Name   : usb_pstd_class_request_wnss
 Description     : class request (control write nodata status stage)
 Arguments       : usb_setup_t *req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_wnss (usb_setup_t *p_req)
{
    #if defined(USB_CFG_PMSC_USE)

    /* Is a request receive target Interface? */
    if ((0 == p_req->index) && ((p_req->type & USB_BMREQUESTTYPERECIP) == USB_INTERFACE))
    {
        if (USB_MASS_STORAGE_RESET == (p_req->type & USB_BREQUEST))
        {
            usb_pmsc_mass_strage_reset (p_req->value, p_req->index, p_req->length);
        }
        else
        {
            usb_pstd_set_stall_pipe0(); /* Req Error */
        }
    }
    else
    {
        usb_pstd_set_stall_pipe0(); /* Req Error */
    }

    if (USB_MASS_STORAGE_RESET != (p_req->type & USB_BREQUEST))
    {
        usb_pstd_ctrl_end((uint16_t) USB_CTRL_END); /* End control transfer. */
    }

    #else   /* defined(USB_CFG_PMSC_USE) */

    usb_ctrl_t ctrl;

    /* Is a request receive target Interface? */
    ctrl.setup  = *p_req; /* Save setup data. */
    ctrl.size   = 0;
    ctrl.status = USB_ACK;
    ctrl.type   = USB_REQUEST;
    usb_cstd_set_event(USB_STS_REQUEST, &ctrl);

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END); /* End control transfer. */

    #endif  /* defined(USB_CFG_PMSC_USE) */

} /* End of function usb_pstd_class_request_wnss */

/******************************************************************************
 Function Name   : usb_pstd_class_request_rss
 Description     : class request (control read status stage)
 Arguments       : usb_setup_t *req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_rss (usb_setup_t *p_req)
{
    #if defined(USB_CFG_PMSC_USE)
    /* Is a request receive target Interface? */
    usb_ctrl_t ctrl;

    if (USB_GET_MAX_LUN == (p_req->type & USB_BREQUEST))
    {
        usb_cstd_set_buf((uint16_t) USB_PIPE0);
    }
    else
    {
        ctrl.setup  = *p_req; /* Save setup data. */
        usb_cstd_set_buf((uint16_t) USB_PIPE0); /* Set pipe PID_BUF */
        ctrl.size   = 0;
        ctrl.status = USB_ACK;
        ctrl.type   = USB_REQUEST;
        usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);
        g_usb_pstd_std_request = USB_NO;
    }

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END); /* End control transfer. */

    #else   /* defined(USB_CFG_PMSC_USE) */

    /* Is a request receive target Interface? */
    usb_ctrl_t ctrl;

    ctrl.setup  = *p_req; /* Save setup data. */
    usb_cstd_set_buf((uint16_t) USB_PIPE0); /* Set pipe PID_BUF */
    ctrl.size   = 0;
    ctrl.status = USB_ACK;
    ctrl.type   = USB_REQUEST;
    usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);
    g_usb_pstd_std_request = USB_NO;

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END); /* End control transfer. */

    #endif  /* defined(USB_CFG_PMSC_USE) */
} /* End of function usb_pstd_class_request_rss */

/******************************************************************************
 Function Name   : usb_pstd_class_request_wss
 Description     : class request (control write status stage)
 Arguments       : usb_setup_t *req : Pointer to usb_setup_t structure
 Return value    : none
 ******************************************************************************/
void usb_pstd_class_request_wss (usb_setup_t *p_req)
{
    usb_ctrl_t ctrl;

    ctrl.setup  = *p_req;
    usb_cstd_set_buf((uint16_t) USB_PIPE0); /* Set BUF */
    ctrl.size   = ctrl.setup.length - g_usb_pstd_data_cnt[USB_PIPE0];
    ctrl.status = USB_ACK;
    ctrl.type   = USB_REQUEST;
    usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);
    g_usb_pstd_std_request = USB_NO;

    usb_pstd_ctrl_end((uint16_t) USB_CTRL_END);
} /* End of function usb_pstd_class_request_wss */

#if USB_CFG_REQUEST == USB_CFG_ENABLE
/******************************************************************************
 Function Name   : usb_pstd_request_event_set
 Description     :
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_request_event_set (void)
{
    usb_ctrl_t ctrl;

    ctrl.setup.type   = g_usb_pstd_req_type;    /* Request type */
    ctrl.setup.value  = g_usb_pstd_req_value;   /* Value */
    ctrl.setup.index  = g_usb_pstd_req_index;   /* Index */
    ctrl.setup.length = g_usb_pstd_req_length;  /* Length */
    ctrl.size         = 0;
    ctrl.status       = USB_ACK;
    ctrl.type         = USB_REQUEST;
    usb_cstd_set_event(USB_STS_REQUEST, &ctrl);
} /* End of function usb_pstd_request_event_set */
#endif /* USB_CFG_REQUEST == USB_CFG_ENABLE */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 End  Of File
 ******************************************************************************/
