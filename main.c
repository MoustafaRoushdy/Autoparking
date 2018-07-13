#include <stdint.h>
#include "PLL_Confg.h"
#include "SysTick.h"
#include "CarMotor.h"
#include "ParkingCount.h"
#include "Bluetooth_UART.h"
#include "TM4C123GH6PM.h"

volatile unsigned char count = 0;
uint8_t start_flag=0;
unsigned char data = 0;

void UART1_Handler(void){
    if(UART1_RIS_R&UART_RIS_RXRIS){
        UART1_ICR_R = UART_ICR_RXIC;       // Acknowledge RX FIFO.
    data = UART_InCharNonBlocking();
    start_flag=1;
    }

    if(UART1_RIS_R&UART_RIS_RTRIS){
        UART1_ICR_R = UART_ICR_RTIC;      // Acknowledge Receiver timeout.
        data = UART_InCharNonBlocking();
        start_flag=1;
    }
}

int main(void)
{
    PLL_Init();
    SysTick_Init();
    BLUETOOTH_Init();
    Parking_count_init();
    Motor_Init();
    while(1)
    {
        if (start_flag == 1)
        {
            count = Parking_count(count);
            Straight(4000,0.25);
            if (count == (data-48))
                Stand();
        }
    }
}
