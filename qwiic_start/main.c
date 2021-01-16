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


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define WAIT_TIME                  10U

#define I2C_DATA_LENGTH            32U

#define SSD1306_ADDR               0x3C
#define SSD1306_CMD                0x00
#define SSD1306_CMD_RPT            0x80
#define SSD1306_DATA               0x40
#define SSD1306_DATA_RPT           0xC0


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
  board_init();
  int i;
  
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

  i2cBuff[0] = SSD1306_CMD; 
  i2cBuff[1] = 0xAE;  // Display off 
  siic_write(SSD1306_ADDR, i2cBuff, 2);

/*
  i2cBuff[1] = 0xA1;  // Segment Re-map 
  siic_write(SSD1306_ADDR, i2cBuff, 2);
*/

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x81;  // Set Contrast
  i2cBuff[2] = SSD1306_CMD;
  i2cBuff[3] = 0x8F;  // value from Adafruit driver 
  siic_write(SSD1306_ADDR, i2cBuff, 4);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0xDA;  // Set COM Pins
  i2cBuff[2] = SSD1306_CMD;
  i2cBuff[3] = 0x02;  // sequential 
  siic_write(SSD1306_ADDR, i2cBuff, 4);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x20;  // Address mode command
  i2cBuff[2] = SSD1306_CMD;
  i2cBuff[3] = 0x00;  // Horizontal address mode
  siic_write(SSD1306_ADDR, i2cBuff, 4);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x21;  // Column Address
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0U;
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 127U;
  siic_write(SSD1306_ADDR, i2cBuff, 6);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x22;  // Page Address
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0x00;
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 0x03;
  siic_write(SSD1306_ADDR, i2cBuff, 6);

  i2cBuff[0] = SSD1306_DATA;
  i2cBuff[1] = 0x00;
  for (i = 0; i < 512; i++) {
    siic_write(SSD1306_ADDR, i2cBuff, 2);
  }

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x8D;  // Charge Pump Command 
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0x14;  // Charge Pump Enabled
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 0xAF;  // Display on
  siic_write(SSD1306_ADDR, i2cBuff, 6);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x21;  // Column Address
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0U;
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 127U;
  siic_write(SSD1306_ADDR, i2cBuff, 6);

  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x22;  // Page Address
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0U;
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 3U;
  siic_write(SSD1306_ADDR, i2cBuff, 6);

  i2cBuff[0] = SSD1306_DATA_RPT;
  i2cBuff[1] = 0xFF;
  i2cBuff[2] = SSD1306_DATA_RPT;
  i2cBuff[3] = 0x02;
  i2cBuff[4] = SSD1306_DATA_RPT;
  i2cBuff[5] = 0x04;
  i2cBuff[6] = SSD1306_DATA_RPT;
  i2cBuff[7] = 0x08;
  i2cBuff[8] = SSD1306_DATA_RPT;
  i2cBuff[9] = 0x10;
  i2cBuff[10] = SSD1306_DATA_RPT;
  i2cBuff[11] = 0xF0;
  i2cBuff[12] = SSD1306_DATA_RPT;
  i2cBuff[13] = 0x10;
  i2cBuff[14] = SSD1306_DATA_RPT;
  i2cBuff[15] = 0x08;
  i2cBuff[16] = SSD1306_DATA_RPT;
  i2cBuff[17] = 0x04;
  i2cBuff[18] = SSD1306_DATA;
  i2cBuff[19] = 0x02;
  for (i = 0; i < 15; i++) {
    siic_write(SSD1306_ADDR, i2cBuff, 20);
  }

  i2cBuff[0] = SSD1306_DATA_RPT;
  i2cBuff[1] = 0xFF;
  i2cBuff[2] = SSD1306_DATA_RPT;
  i2cBuff[3] = 0x81;
  i2cBuff[4] = SSD1306_DATA_RPT;
  i2cBuff[5] = 0x81;
  i2cBuff[6] = SSD1306_DATA_RPT;
  i2cBuff[7] = 0x81;
  i2cBuff[8] = SSD1306_DATA_RPT;
  i2cBuff[9] = 0x81;
  i2cBuff[10] = SSD1306_DATA_RPT;
  i2cBuff[11] = 0x3C;
  i2cBuff[12] = SSD1306_DATA_RPT;
  i2cBuff[13] = 0x81;
  i2cBuff[14] = SSD1306_DATA_RPT;
  i2cBuff[15] = 0x81;
  i2cBuff[16] = SSD1306_DATA_RPT;
  i2cBuff[17] = 0x81;
  i2cBuff[18] = SSD1306_DATA;
  i2cBuff[19] = 0x81;
  for (i = 0; i < 15; i++) {
    siic_write(SSD1306_ADDR, i2cBuff, 20);
  }

  sctpix_setPixel(NEOPIXEL_CH, 0, NEO_BLUE);
  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_GREEN);
  sctpix_show();


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
