#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Board identifier.
 */
#define BOARD_TRS_GMSK_MODEM
#define BOARD_NAME              "GMSK Modem"

/*
 * Board frequencies.
 */
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F10X_MD

/*
 * IO pins assignments.
 */
#define GPIOA_TX_DATA           0
#define GPIOA_TX_CLK            1
#define GPIOA_RX_CLK            2
#define GPIOA_RX_SN             3
#define GPIOA_CLK_DIV_A         4
#define GPIOA_CLK_DIV_B         5
#define GPIOA_BT                6
#define GPIOA_JMP_X4            7
#define GPIOA_UNUSED_1          8
#define GPIOA_USART1_TX         9
#define GPIOA_USART1_RX         10
#define GPIOA_USART1_CTS        11
#define GPIOA_USART1_RTS        12

#define GPIOB_PTT               0
#define GPIOB_RX_DATA           1
#define GPIOB_PLL_ACQ           2
#define GPIOB_JTDO              3
#define GPIOB_JNTRST            4
#define GPIOB_LED_R             5
#define GPIOB_I2C1_SDA          6
#define GPIOB_I2C1_SDC          7
#define GPIOB_LED_G             8
#define GPIOB_UNUSED            9
#define GPIOB_USART3_RX         10
#define GPIOB_USART3_TX         11
#define GPIOB_TX_PS             12
#define GPIOB_RX_PS             13
#define GPIOB_TX_ENABLE         14
#define GPIOB_UNUSED2           15

#define GPIOC_RX_DC_ACQ         13
#define GPIOC_RX_HOLD           14

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0  - TX_DATA (2MHz output)
 *
 * PA9  - UART_TX (2MHz output)
 * PA10 - UART_RX
 * PA11 - UART_CTS
 * PA12 - UART_RTS (2MHz output)
 */
#define VAL_GPIOACRL            0x88888882      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x888A44A8      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0 - PTT (2MHz output)
 * PB2 - PLL_ACQ (2MHz output)
 * PB5 - LED (Open Drain output 2MHz.)
 * PB6 - I2C (Alternate Open Drain output 2MHz.)
 * PB7 - I2C (Alternate Open Drain output 2MHz.)
 *
 * PB8 - LED (Open Drain output 2MHz.)
 * PB10 - UART_TX (2MHz output)
 * PB11 - UART_RX
 * PB12 - TX_PS
 * PB13 - RX_PS
 * PB14 - TX_ENABLE
 */
#define VAL_GPIOBCRL            0xEE688282      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x82228A86      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC13 - RX_DC_ACQ
 * PC14 - RX_HOLD
 */
#define VAL_GPIOCCRL            0x88888888      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x82288888      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888888      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp) palClearPad(GPIOC, GPIOC_USB_DISC)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOC, GPIOC_USB_DISC)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
