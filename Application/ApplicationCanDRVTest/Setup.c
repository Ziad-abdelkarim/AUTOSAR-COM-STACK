/*
 * Setup.c
 *
 *  Created on: ??�/??�/????
 *      Author: PC
 */

#include "Setup.h"

void UART_Setup(void)
{
#if (UART0_ENABLED ==1)
    // For Debug
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {; }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTStdioConfig(0, BAUD_UART0, SysCtlClockGet());

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
#endif


#if UART1_ENABLED ==1
    //For Commands
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
       while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1))
       {; }
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
       UARTStdioConfig(1, BAUD_UART1, SysCtlClockGet());

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1))
    {; }
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), BAUD_UART1,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE));
    UARTEnable(UART1_BASE);
    IntMasterEnable(); //enable processor interrupts
       IntEnable(INT_UART1); //enable the UART interrupt
       UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
#endif

    return;
}






void Port_Init(void){
    /*Initializing PORTS*/
    #if (PORTA==1)
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
            {
                ;
            }
    #endif

    #if (PORTB==1)
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
            {
                ;
            }
    #endif

    #if PORTC==1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
            {
                ;
            }
    #endif

    #if PORTD==1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
            {
                ;
            }
    #endif

    #if PORTE==1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
            {
                ;
            }
    #endif


    #if PORTF==1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
            {
                ;
            }
    #endif

    return;
}



void CAN_Setup(void){
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    return;
}


void Timer_Setup(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    /**********************Setup TIMER0*****************************/
    /*while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    Period = (SysCtlClockGet() / 2);
    TimerLoadSet(TIMER0_BASE, TIMER_A, Period - 1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
    */
    /***************************************************************/
    return;
}
