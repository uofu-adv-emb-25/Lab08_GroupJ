#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>

int main( void )
{
    stdio_init_all();
    while(1)
    {
        printf("Hello from can.c\n");
        sleep_ms(1000);
    }
    return 0;
}