/*
 * FreeRTOS demo for the NUCLEO-F429ZI.
 *
 *   blink task    - toggles the green LED (LD1, PB0) every 500 ms
 *   button task   - polls the blue user button (B1, PC13); each press toggles
 *                   the blue LED (LD2, PB7) and is sent to the report queue
 *   report task   - prints button events and a periodic status line over
 *                   USART3 (PD8), which the ST-LINK exposes as a USB serial port
 *
 * The red LED (LD3, PB14) lights up if FreeRTOS reports a stack overflow or
 * failed allocation.
 */
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f429.h"

#define LED_GREEN   0   /* PB0  */
#define LED_BLUE    7   /* PB7  */
#define LED_RED     14  /* PB14 */
#define BUTTON_PIN  13  /* PC13, active high */

#define APB1_HZ     (configCPU_CLOCK_HZ / 4)
#define UART_BAUD   115200

static QueueHandle_t button_queue;

/* 168 MHz SYSCLK from the 16 MHz HSI: PLLM=16, PLLN=336, PLLP=2, PLLQ=7. */
static void clock_init(void)
{
    RCC_APB1ENR |= RCC_APB1ENR_PWREN;
    PWR_CR |= PWR_CR_VOS_SCALE1;

    RCC_CR |= RCC_CR_HSION;
    while (!(RCC_CR & RCC_CR_HSIRDY)) { }

    RCC_PLLCFGR = (16u << 0) | (336u << 6) | (0u << 16) | (0u << 22) | (7u << 24);
    RCC_CR |= RCC_CR_PLLON;
    while (!(RCC_CR & RCC_CR_PLLRDY)) { }

    FLASH_ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    RCC_CFGR = RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_SW_PLL;
    while ((RCC_CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL) { }
}

static void gpio_init(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
    (void)RCC_AHB1ENR;  /* Let the clock enable settle */

    /* LEDs: general-purpose outputs */
    GPIOB->MODER &= ~((3u << (LED_GREEN * 2)) | (3u << (LED_BLUE * 2)) | (3u << (LED_RED * 2)));
    GPIOB->MODER |=  ((1u << (LED_GREEN * 2)) | (1u << (LED_BLUE * 2)) | (1u << (LED_RED * 2)));

    /* Button: input (board has an external pull-down) */
    GPIOC->MODER &= ~(3u << (BUTTON_PIN * 2));
}

static void led_on(int pin)     { GPIOB->BSRR = 1u << pin; }
static void led_toggle(int pin) { GPIOB->ODR ^= 1u << pin; }

static void uart_init(void)
{
    RCC_APB1ENR |= RCC_APB1ENR_USART3EN;
    (void)RCC_APB1ENR;

    /* PD8 = USART3_TX, alternate function 7 */
    GPIOD->MODER  = (GPIOD->MODER & ~(3u << 16)) | (2u << 16);
    GPIOD->AFR[1] = (GPIOD->AFR[1] & ~(0xFu << 0)) | (7u << 0);

    USART3->BRR = (APB1_HZ + UART_BAUD / 2) / UART_BAUD;
    USART3->CR1 = USART_CR1_UE | USART_CR1_TE;
}

static void uart_puts(const char *s)
{
    for (; *s; s++) {
        if (*s == '\n') {
            while (!(USART3->SR & USART_SR_TXE)) { }
            USART3->DR = '\r';
        }
        while (!(USART3->SR & USART_SR_TXE)) { }
        USART3->DR = (uint8_t)*s;
    }
}

static void uart_putu(uint32_t v)
{
    char buf[11];
    char *p = &buf[sizeof buf - 1];
    *p = '\0';
    do {
        *--p = (char)('0' + v % 10);
        v /= 10;
    } while (v);
    uart_puts(p);
}

static void blink_task(void *arg)
{
    (void)arg;
    TickType_t last = xTaskGetTickCount();
    for (;;) {
        led_toggle(LED_GREEN);
        vTaskDelayUntil(&last, pdMS_TO_TICKS(500));
    }
}

static void button_task(void *arg)
{
    (void)arg;
    uint32_t presses = 0;
    int was_down = 0;
    for (;;) {
        int down = (GPIOC->IDR >> BUTTON_PIN) & 1;
        if (down && !was_down) {
            presses++;
            led_toggle(LED_BLUE);
            xQueueSend(button_queue, &presses, 0);
        }
        was_down = down;
        vTaskDelay(pdMS_TO_TICKS(20));  /* Polling interval doubles as debounce */
    }
}

static void report_task(void *arg)
{
    (void)arg;
    uint32_t presses;
    for (;;) {
        if (xQueueReceive(button_queue, &presses, pdMS_TO_TICKS(2000)) == pdPASS) {
            uart_puts("Button pressed (");
            uart_putu(presses);
            uart_puts(" total)\n");
        } else {
            uart_puts("Uptime ");
            uart_putu(xTaskGetTickCount() / configTICK_RATE_HZ);
            uart_puts(" s, free heap ");
            uart_putu(xPortGetFreeHeapSize());
            uart_puts(" bytes\n");
        }
    }
}

int main(void)
{
    clock_init();
    gpio_init();
    uart_init();

    uart_puts("\nFreeRTOS " tskKERNEL_VERSION_NUMBER " demo on NUCLEO-F429ZI @ 168 MHz\n");

    button_queue = xQueueCreate(8, sizeof(uint32_t));
    configASSERT(button_queue);

    xTaskCreate(blink_task,  "blink",  configMINIMAL_STACK_SIZE,     NULL, 1, NULL);
    xTaskCreate(button_task, "button", configMINIMAL_STACK_SIZE,     NULL, 2, NULL);
    xTaskCreate(report_task, "report", configMINIMAL_STACK_SIZE * 2, NULL, 1, NULL);

    vTaskStartScheduler();
    for (;;) { }  /* Only reached if there is not enough heap for the idle task */
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
    (void)task;
    (void)name;
    taskDISABLE_INTERRUPTS();
    led_on(LED_RED);
    for (;;) { }
}

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    led_on(LED_RED);
    for (;;) { }
}
