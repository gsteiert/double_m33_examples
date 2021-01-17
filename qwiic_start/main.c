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
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "ssd1306.h"


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define WAIT_TIME                  10U

#define I2C_DATA_LENGTH            32U
#define QGPIO_ADDR                 0x38

#define DISP_X          128
#define DISP_Y          32


/* Blink pattern
 * - 250 ms  : button is not pressed
 * - 1000 ms : button is pressed (and hold)
 */
enum  {
  BLINK_PRESSED = 250,
  BLINK_UNPRESSED = 1000
};

#define HELLO_STR   "Hello from TinyUSB\r\n"

int main(void)
{
  uint8_t i2cBuff[I2C_DATA_LENGTH];
  uint8_t dispBuff[(DISP_X * DISP_Y)/8];
  uint8_t countStr[20];
  board_init();
  
  uint32_t start_ms = 0;
  uint32_t pallet[8] = {
    0x000000, 0x000010, 0x001000, 0x100000,
    0x101010, 0x101000, 0x100010, 0x001010
  };
  uint32_t count = 0;

  sctpix_setPixel(NEOPIXEL_CH, 0, NEO_YELLOW);
  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_YELLOW);
  sctpix_show();

  siic_init(I2C1_BASE, 400000);

  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_CYAN);
  sctpix_show();

  ssd1306_init(DISP_X, DISP_Y, dispBuff);

  sctpix_setPixel(NEOPIXEL_CH, 0, NEO_BLUE);
  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_GREEN);
  sctpix_show();

  uint8_t testStr[96];
  for (int i = 0; i<96; i++){
    testStr[i] = i + 0x20;
  }

  ssd1306_print(0, 32, &testStr[0], 16);
  ssd1306_print(1, 32, &testStr[16], 16);
  ssd1306_print(2, 32, &testStr[32], 16);
  ssd1306_print(3, 32, &testStr[48], 16);

//  memcpy(dispBuff, font_bmp, 125);
  ssd1306_update();


  while (1)
  {
    uint32_t interval_ms = board_button_read() ? BLINK_PRESSED : BLINK_UNPRESSED;

    // uart echo
//    uint8_t ch;
//    if ( board_uart_read(&ch, 1) ) board_uart_write(&ch, 1);

    // Blink every interval ms
    if ( !(board_millis() - start_ms < interval_ms) )
    {
      board_uart_write(HELLO_STR, strlen(HELLO_STR));

      start_ms = board_millis();

      board_led_write(count & 0x1);
      
      // Write the low byte of the count to the PCF8574
      i2cBuff[0] = 0xFF & count;
      siic_write(QGPIO_ADDR, i2cBuff, 1);

      sprintf((char *)countStr, "   0x%08lX   ", count);
      ssd1306_print(0, 32, countStr, 16);
      ssd1306_update();

//      sctpix_setPixel(NEOPIXEL_CH, 0, pallet[(count & 0x7U)]);
      sctpix_setPixel(NEOPIXEL_CH, 1, pallet[((count +4) & 0x7U)]);
      sctpix_show();
      count += 0x1;
    }
  }

  return 0;
}

#if CFG_TUSB_MCU == OPT_MCU_ESP32S2
void app_main(void)
{
  main();
}
#endif
