/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/** \ingroup group_demo
 * \defgroup group_board Boards Abstraction Layer
 *  @{ */

#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "sct_neopixel.h"
#include "ansi_escape.h"
#include "tusb.h"

#define CFG_BOARD_UART_BAUDRATE    115200

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+
#define LED_PORT              0
#define LED_PIN               1
#define LED_STATE_ON          1

// WAKE button
#define BUTTON_PORT           0
#define BUTTON_PIN            5
#define BUTTON_STATE_ACTIVE   0

// Number of neopixels
#define NEOPIXEL_NUMBER       2
#define NEOPIXEL_PORT         0
#define NEOPIXEL_PIN          27
#define NEOPIXEL_CH           6
#define NEOPIXEL_TYPE         0   /* TBD */

// UART
#define UART_DEV              USART0

// IOCON pin mux
#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_FUNC4 0x04u         /*!<@brief Selects pin function 4 */
#define IOCON_PIO_FUNC7 0x07u         /*!<@brief Selects pin function 7 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

#define IOCON_PIO_DIG_FUNC0_EN   (IOCON_PIO_DIGITAL_EN | IOCON_PIO_FUNC0) /*!<@brief Digital pin function 0 enabled */
#define IOCON_PIO_DIG_FUNC1_EN   (IOCON_PIO_DIGITAL_EN | IOCON_PIO_FUNC1) /*!<@brief Digital pin function 1 enabled */
#define IOCON_PIO_DIG_FUNC4_EN   (IOCON_PIO_DIGITAL_EN | IOCON_PIO_FUNC4) /*!<@brief Digital pin function 2 enabled */
#define IOCON_PIO_DIG_FUNC7_EN   (IOCON_PIO_DIGITAL_EN | IOCON_PIO_FUNC7) /*!<@brief Digital pin function 2 enabled */

//--------------------------------------------------------------------+
// Board Porting API
// For simplicity, only one LED and one Button are used
//--------------------------------------------------------------------+

// Initialize on-board peripherals : led, button, uart and USB
void board_init(void);

// Turn LED on or off
void board_led_write(bool state);
/*
void neopixel_set(uint32_t pixel, uint32_t color);
void neopixel_update(void);
*/
// Get the current state of button
// a '1' means active (pressed), a '0' means inactive.
uint32_t board_button_read(void);

// Get characters from UART
int board_uart_read(uint8_t* buf, int len);

// Send characters to UART
int board_uart_write(void const * buf, int len);

#if CFG_TUSB_OS == OPT_OS_NONE
  // Get current milliseconds, must be implemented when no RTOS is used
  uint32_t board_millis(void);

#elif CFG_TUSB_OS == OPT_OS_FREERTOS
  static inline uint32_t board_millis(void)
  {
    return ( ( ((uint64_t) xTaskGetTickCount()) * 1000) / configTICK_RATE_HZ );
  }

#elif CFG_TUSB_OS == OPT_OS_MYNEWT
  static inline uint32_t board_millis(void)
  {
    return os_time_ticks_to_ms32( os_time_get() );
  }

#else
  #error "board_millis() is not implemented for this OS"
#endif

//--------------------------------------------------------------------+
// Helper functions
//--------------------------------------------------------------------+
static inline void board_led_on(void)
{
  board_led_write(true);
}

static inline void board_led_off(void)
{
  board_led_write(false);
}

// TODO remove
static inline void board_delay(uint32_t ms)
{
  uint32_t start_ms = board_millis();
  while (board_millis() - start_ms < ms)
  {
    #if TUSB_OPT_DEVICE_ENABLED
    // take chance to run usb background
    tud_task();
    #endif
  }
}

static inline int board_uart_getchar(void)
{
  uint8_t c;
  return board_uart_read(&c, 1) ? (int) c : (-1);
}

static inline int board_uart_putchar(uint8_t c)
{
  return board_uart_write(&c, 1);
}

#ifdef __cplusplus
 }
#endif

#endif /* _BSP_BOARD_H_ */

/** @} */
