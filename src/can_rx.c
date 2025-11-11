#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#define RX_MSG_TASK_PRIORITY     ( tskIDLE_PRIORITY + 5UL )
#define RX_MSG_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

QueueHandle_t queue;

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Callback function
    if(notify == CAN2040_NOTIFY_RX) {
        if(xQueueSendToBackFromISR(queue, msg, NULL) != pdPASS) {
            printf("ERROR: MSG QUEUE FULL\n");
        }
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

/*
 * Task that reads messages added to a queue from the CAN bus interrupt handler
 */
void read_message_task(void *args)
{
    struct can2040_msg rx_message;
    sleep_ms(10000);

    while (1)
    {
        printf("Waiting for message...\n\n");
        if(xQueueReceive(queue, &rx_message, portMAX_DELAY) != pdPASS) {
            printf("ERROR: QUEUE MESSAGE COULD NOT BE RECEIVED");
            continue;
        }
        char message[9];
        int i;
        for(i = 0; i < rx_message.dlc && i < 8; i++) {
            message[i] = rx_message.data[i];
        }
        message[i] = '\0';
        printf("Can Message Received:\n\t\"%s\"\n\n", message);
    }
}

int main (void) {
    stdio_init_all();
    // Setup Queue for reading messages
    queue = xQueueCreate(20, sizeof(struct can2040_msg));

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

    // Create task
    TaskHandle_t rx_msg_task;
    xTaskCreate(read_message_task, "ReadCanMsg", RX_MSG_TASK_STACK_SIZE, (void *)&queue, RX_MSG_TASK_PRIORITY, &rx_msg_task);

    vTaskStartScheduler();
}