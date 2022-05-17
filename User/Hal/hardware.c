/********************************************************
* @file       hardware.c
* @author     szhj13
* @version    V1.0
* @date       2022-05-18
* @brief      the start address of program
**********************************************************
* @Modify

**********************************************************/

/* Includes ---------------------------------------------*/
#include "hardware.h"
/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/


void Clk_Init(void )
{
    uint32_t waitCnt;
    
    //HOCO = 32MHz / fIH = 8MHz, UPLL = 48MHz, fclk = 64MHz
    
    CLK_Fclk_Select(MAINCLK_FIH);
    CLK_PLL_Setting(PLL_SR_fIH,PLL_DIV_2,PLL_MUL_16);
    CLK_UPLL_Setting(PLL_SR_fIH,PLL_DIV_2,PLL_MUL_12);
    CLK_PLL_Start();
    waitCnt = 10000;
    while(waitCnt--);
    CLK_UPLL_Start();
    waitCnt = 10000;
    while(waitCnt--);
    CLK_Fclk_Select(MAINCLK_FPLL);

    SystemCoreClock = 64000000;
}

void Tim4_Channel0_Init(void )
{    
    CGC->PER0 |= CGC_PER0_TM40EN_Msk;    /* enables input clock supply */
    TM40->TPS0 = _0000_TM4_CKM3_fCLK_8 | _0000_TM4_CKM2_fCLK_1 | _00F0_TM4_CKM1_fCLK_15 | _0000_TM4_CKM0_fCLK_0; 

    /* stop specify channel */
    TM40->TS0 |= TM4_CHANNEL_0;

    /* clear interrupt flag */
    INTC_ClearPendingIRQ(TM00_IRQn);
    NVIC_ClearPendingIRQ(TM00_IRQn);
    /* Channel 0 is used as interval timer */
    TM40->TMR00 = _0000_TM4_CLOCK_SELECT_CKM0 | _0000_TM4_CLOCK_MODE_CKS | _0000_TM4_TRIGGER_SOFTWARE |
                  _0000_TM4_MODE_INTERVAL_TIMER | _0000_TM4_START_INT_UNUSED;
    TM40->TDR00 = (SystemCoreClock/1000) - 1;
    TM40->TOE0 &= ~_0001_TM4_CH0_OUTPUT_ENABLE;

    TM40->TS0 |= TM4_CHANNEL_0;

    INTC_EnableIRQ(TM00_IRQn);

}

