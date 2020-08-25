//Blinky BLUE LED T=2s, Timer 1B
//Include libraries
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123ge6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/timer.c"
//Define variables
#define LED_PORT         GPIO_PORTF_BASE
#define LED              GPIO_PIN_2
void Timer0_ISR(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(LED_PORT, LED)==0)
    {
        GPIOPinWrite(LED_PORT, LED,4);
    }
    else
    {
        GPIOPinWrite(LED_PORT, LED,0);
    }
}

int main(void)
    {
    uint32_t ui32Period;
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //GPIO Configurations
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(LED_PORT, LED);

    //Timer Configurations (Full-width periodic timer)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // Enable Timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    //Delay calculation      (Cycles to get the Frequency design = SysCtrlClockGet()/Frequency design)
    ui32Period = (SysCtlClockGet()/4); //T=2, => F = 0.5
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1);


    //Interrupt Configurations
    IntMasterEnable(); // Allow all interrupts
    IntEnable(INT_TIMER0A);   // Allow interrupts for timer0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0_ISR); //Define the ISQ
    while(1)
    {
    }
}


