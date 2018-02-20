/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/09/11 7:08p $
 * @brief    A project template for Nano100BN series MCUs
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "Nano100Series.h"

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk);

    /* Switch HCLK clock source to HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_HCLK_CLK_DIVIDER(1));

    /* Enable IP clock */
    CLK_EnableModuleClock(UART1_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_UART_CLK_DIVIDER(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    // UART 1 Port Settings
    SYS->PC_H_MFP &= ~(SYS_PC_H_MFP_PC10_MFP_Msk | SYS_PC_H_MFP_PC11_MFP_Msk);
    SYS->PC_H_MFP |= (SYS_PC_H_MFP_PC10_MFP_UART1_RX | SYS_PC_H_MFP_PC11_MFP_UART1_TX);

    /* Lock protected registers */
    SYS_LockReg();
}

void ADC_Init(void)
{
    /* Enable ADC clock */
    CLK_EnableModuleClock(ADC_MODULE);
    /* Set PA multi-function pins for ADC */
    SYS->PA_L_MFP &= ~(SYS_PA_L_MFP_PA0_MFP_Msk | SYS_PA_L_MFP_PA1_MFP_Msk |
                       SYS_PA_L_MFP_PA2_MFP_Msk | SYS_PA_L_MFP_PA3_MFP_Msk);
    SYS->PA_L_MFP |= SYS_PA_L_MFP_PA0_MFP_ADC_CH0 | SYS_PA_L_MFP_PA1_MFP_ADC_CH1 |
                     SYS_PA_L_MFP_PA2_MFP_ADC_CH2 | SYS_PA_L_MFP_PA3_MFP_ADC_CH3;

    /* Disable PA.0 PA.1 PA.2 digital input path */
    PA->OFFD |= (((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) << GP_OFFD_OFFD_Pos);
}
void initPWM(void)
{
    /* Enable PWM clock */
    CLK_EnableModuleClock(PWM0_CH01_MODULE);
    CLK_EnableModuleClock(PWM0_CH23_MODULE);

    /* Set HCLK as PWM clock source */
    CLK_SetModuleClock(PWM0_CH01_MODULE, CLK_CLKSEL1_PWM0_CH01_S_HCLK, 0);
    CLK_SetModuleClock(PWM0_CH01_MODULE, CLK_CLKSEL1_PWM0_CH23_S_HCLK, 0);

    /* Set PA.12  for PWM channel 0 */
    SYS->PA_H_MFP &= ~(SYS_PA_H_MFP_PA12_MFP_Msk | SYS_PA_H_MFP_PA13_MFP_Msk | SYS_PA_H_MFP_PA14_MFP_Msk);
    SYS->PA_H_MFP |= SYS_PA_H_MFP_PA12_MFP_PWM0_CH0 | SYS_PA_H_MFP_PA13_MFP_PWM0_CH1 | SYS_PA_H_MFP_PA14_MFP_PWM0_CH2;
}

void delay()
{
    int a, b;
    for (a = 0; a < 5000; a++)
    {
        for (b = 0; b < 500; b++)
            ;
    }
}
int main()
{
    uint32_t u32ResultLOW, u32ResultMIDDLE, u32ResultHIGH;
    uint8_t i;

    int id;
    uint8_t u8Ch;
    SYS_Init();
    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART1, 115200);
    id = *((volatile unsigned int *)(0x50000000 + 0x00));
    printf("Hello World 0x%x 0x%x\n ", id, SYS->PDID);
    initPWM();
    ADC_Init();

    // PWM0 frequency is 2000Hz, duty 10%,
    PWM_ConfigOutputChannel(PWM0, 0, 2000, 10);
    PWM_ConfigOutputChannel(PWM0, 1, 2000, 10);
    PWM_ConfigOutputChannel(PWM0, 2, 2000, 10);

    PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
    PWM_EnableOutput(PWM0, PWM_CH_1_MASK);
    PWM_EnableOutput(PWM0, PWM_CH_2_MASK);
    PWM_Start(PWM0, (PWM_CH_0_MASK | PWM_CH_1_MASK | PWM_CH_2_MASK));

    // Enable channel 0
    ADC_Open(ADC, ADC_INPUT_MODE_SINGLE_END, ADC_OPERATION_MODE_SINGLE_CYCLE, (ADC_CH_0_MASK | ADC_CH_1_MASK | ADC_CH_2_MASK));

    // Set reference voltage to AVDD
    ADC_SET_REF_VOLTAGE(ADC, ADC_REFSEL_POWER);

    // Power on ADC
    ADC_POWER_ON(ADC);
    ADC_START_CONV(ADC);

    while (1)
    {
        u32ResultLOW = ADC_GET_CONVERSION_DATA(ADC, 0);
        printf("Channel 0 conversion result is %d\n", u32ResultLOW);
        PWM_ConfigOutputChannel(PWM0, 0, 50, u32ResultLOW);

        u32ResultMIDDLE = ADC_GET_CONVERSION_DATA(ADC, 1);
        printf("Channel 1 conversion result is %d\n", u32ResultMIDDLE);
        PWM_ConfigOutputChannel(PWM0, 1, 50, u32ResultMIDDLE);

        u32ResultHIGH = ADC_GET_CONVERSION_DATA(ADC, 2);
        printf("Channel 2 conversion result is %d\n", u32ResultHIGH);
        PWM_ConfigOutputChannel(PWM0, 2, 50, u32ResultHIGH);

        delay();
        ADC_START_CONV(ADC);
    }
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/