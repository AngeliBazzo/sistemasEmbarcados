//*****************************************************************************
//
// project0.c - Example to demonstrate minimal TivaWare setup
//
// Copyright (c) 2012-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"

#include <iostream>
using std::cout;
//*****************************************************************************
//
// Define pin to LED mapping.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Project Zero (project0)</h1>
//!
//! This example demonstrates the use of TivaWare to setup the clocks and
//! toggle GPIO pins to make the LED blink. This is a good place to start
//! understanding your launchpad and the tools that can be used to program it.
//
//*****************************************************************************

#define USER_LED1  GPIO_PIN_0
#define USER_LED2  GPIO_PIN_1
#define USER_SW0  GPIO_PIN_0
#define USER_SW1  GPIO_PIN_1

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

volatile uint32_t g_ui32counter=0;
volatile uint8_t g_ui8button=1;

//Tratador Interrupção do Botão 
void PortJHandler(){
  GPIOIntClear(GPIO_PORTJ_AHB_BASE,GPIO_PIN_0);
  g_ui8button=0;
}

//Tratador do Timer , acrescenta na contagem
void TIMERHandler(){
  TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  g_ui32counter++;
}


//Interrupt service routine
void GPIOIntHandler(void){
	GPIOIntClear(GPIO_PORTJ_AHB_BASE, GPIO_INT_PIN_0);
	int state = GPIOIntStatus(GPIO_PORTJ_AHB_BASE, true);
	
        // usuario apertou o botão
}
//*****************************************************************************
//
// Main 'C' Language entry point.  Toggle an LED using TivaWare.
//
//*****************************************************************************
int
main(void)
{
    //*****************************************************************************
    //
    // CONFIGURAÇÕES.
    //
    //*****************************************************************************
  
    uint32_t ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_240), 120000000);

    //
    // Enable and wait for the port to be ready for access
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))
    {
    }
    
    
    //
    // Configure the GPIO port for the LED AND SWITCH  operation.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, (USER_LED1|USER_LED2));
    GPIOPinTypeGPIOInput(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0|GPIO_PIN_1);
    
    GPIOPadConfigSet(GPIO_PORTJ_AHB_BASE ,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    
    GPIOIntTypeSet(GPIO_PORTJ_AHB_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTJ_AHB_BASE,PortJHandler);
    GPIOIntEnable(GPIO_PORTJ_AHB_BASE,GPIO_PIN_0);
    
    GPIOIntTypeSet(GPIO_PORTJ_AHB_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);
    IntPrioritySet(INT_GPIOJ, 0);
    IntRegister(INT_GPIOJ, GPIOIntHandler);
    IntEnable(INT_GPIOJ);
    IntMasterEnable();

    uint32_t ui32Value;
    
    //
    // Turn on the LED
    //
    GPIOPinWrite(GPIO_PORTN_BASE, (USER_LED1|USER_LED2), USER_LED1);
    
    //
    //Configure and enable the SysTick counter.
    //
    SysTickPeriodSet(1000);
    SysTickEnable();
    
   //ALTERAR PARA NAO PASSAR DA QUANTIDADE DE TEMPO PROPOSTA || OU ATÉ ALGUEM PRESSIONAR O BOTÃO
    while(1)
    {
        
      ui32Value = SysTickValueGet();
      int32_t teste;
      teste = GPIOPinRead(GPIO_PORTJ_BASE, (USER_SW0));
      cout << ui32Value << "\n";
    }
}
