#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stdbool.h>
#include <stdio.h>
#include "BAT32G157.h"
#include "userdefine.h"
#include "clk.h"
#include "usb.h"
#include "tim4.h"

#include "usb_phid_apl.h"


void Clk_Init(void );
void Tim4_Channel0_Init(void );

#endif 
