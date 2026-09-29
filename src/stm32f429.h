/* Minimal STM32F429 register definitions - only what this demo uses. */
#ifndef STM32F429_H
#define STM32F429_H

#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))

/* Core */
#define SCB_VTOR        REG32(0xE000ED08)
#define SCB_CPACR       REG32(0xE000ED88)

/* Flash interface */
#define FLASH_ACR       REG32(0x40023C00)
#define FLASH_ACR_LATENCY_5WS  (5u << 0)
#define FLASH_ACR_PRFTEN       (1u << 8)
#define FLASH_ACR_ICEN         (1u << 9)
#define FLASH_ACR_DCEN         (1u << 10)

/* Power control */
#define PWR_CR          REG32(0x40007000)
#define PWR_CR_VOS_SCALE1      (3u << 14)

/* Reset and clock control */
#define RCC_BASE        0x40023800u
#define RCC_CR          REG32(RCC_BASE + 0x00)
#define RCC_PLLCFGR     REG32(RCC_BASE + 0x04)
#define RCC_CFGR        REG32(RCC_BASE + 0x08)
#define RCC_AHB1ENR     REG32(RCC_BASE + 0x30)
#define RCC_APB1ENR     REG32(RCC_BASE + 0x40)

#define RCC_CR_HSION           (1u << 0)
#define RCC_CR_HSIRDY          (1u << 1)
#define RCC_CR_PLLON           (1u << 24)
#define RCC_CR_PLLRDY          (1u << 25)
#define RCC_CFGR_SW_PLL        (2u << 0)
#define RCC_CFGR_SWS_MASK      (3u << 2)
#define RCC_CFGR_SWS_PLL       (2u << 2)
#define RCC_CFGR_PPRE1_DIV4    (5u << 10)
#define RCC_CFGR_PPRE2_DIV2    (4u << 13)
#define RCC_AHB1ENR_GPIOBEN    (1u << 1)
#define RCC_AHB1ENR_GPIOCEN    (1u << 2)
#define RCC_AHB1ENR_GPIODEN    (1u << 3)
#define RCC_APB1ENR_USART3EN   (1u << 18)
#define RCC_APB1ENR_PWREN      (1u << 28)

/* GPIO */
typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} GPIO_TypeDef;

#define GPIOB           ((GPIO_TypeDef *)0x40020400u)
#define GPIOC           ((GPIO_TypeDef *)0x40020800u)
#define GPIOD           ((GPIO_TypeDef *)0x40020C00u)

/* USART */
typedef struct {
    volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
} USART_TypeDef;

#define USART3          ((USART_TypeDef *)0x40004800u)
#define USART_SR_TXE           (1u << 7)
#define USART_CR1_TE           (1u << 3)
#define USART_CR1_UE           (1u << 13)

#endif
