#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Callback function - add message handling
    if(notify == CAN2040_NOTIFY_TX) {
        // Do something idk
        ;
    }
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    // might need to use SYS_CLK_HZ
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

int main (void) {
    stdio_init_all();

    canbus_setup();
    struct can2040_msg message;
    message.id = 0x102;
    message.dlc = 8;
    message.data[0] = 'h';
    message.data[1] = 'e';
    message.data[2] = 'l';
    message.data[3] = 'l';
    message.data[4] = 'o';
    message.data[5] = ' ';
    message.data[6] = ':';
    message.data[7] = ')';
//     struct can2040_msg {
//     uint32_t id;
//     uint32_t dlc;
//     union {
//         uint8_t data[8];
//         uint32_t data32[2];
//     };
// };
    sleep_ms(10000);
    while(1) {
        //printf("Transmitting 'hello :)'.");
        can2040_transmit(&cbus, &message);
        //printf("Transmitted message.");
        sleep_ms(5000);
    }
}