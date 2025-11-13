#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#define RX_MSG_TASK_PRIORITY     ( tskIDLE_PRIORITY + 5UL )
#define RX_MSG_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define TX_MSG_TASK_PRIORITY     ( tskIDLE_PRIORITY + 3UL )
#define TX_MSG_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

QueueHandle_t queue;

static struct can2040_msg tx_msg;

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

void transmit_message_task(void *args)
{
    vTaskDelay(pdTICKS_TO_MS(5000));
    while(1) {
        vTaskDelay(pdTICKS_TO_MS(500));
        if(can2040_check_transmit(&cbus))
        {
            can2040_transmit(&cbus, &tx_msg);
        }
    }
}

/*
 * Task that reads messages added to a queue from the CAN bus interrupt handler
 */
void read_message_task(void *args)
{
    struct can2040_msg rx_message;
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

    
    tx_msg.id = 0x112;
    tx_msg.dlc = 8;
    tx_msg.data[0] = 'g';
    tx_msg.data[1] = 'o';
    tx_msg.data[2] = 'o';
    tx_msg.data[3] = 'd';
    tx_msg.data[4] = ' ';
    tx_msg.data[5] = 'b';
    tx_msg.data[6] = 'y';
    tx_msg.data[7] = 'e';

    canbus_setup();

    // Create task
    TaskHandle_t rx_msg_task;
    TaskHandle_t tx_msg_task;
    xTaskCreate(read_message_task, "ReadCanMsg", RX_MSG_TASK_STACK_SIZE, (void *)&queue, RX_MSG_TASK_PRIORITY, &rx_msg_task);
    xTaskCreate(transmit_message_task, "ReadCanMsg", TX_MSG_TASK_STACK_SIZE, (void *)&queue, TX_MSG_TASK_PRIORITY, &tx_msg_task);

    vTaskStartScheduler();
}