/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Greg Steiert
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

#include "ssd1306.h"
#include "board.h"
#include "simple_i2c.h"
#include "bitmaps.h"


uint8_t *_disp_buf;

/*!
  Initializes SSD1306 display
*/
void ssd1306_init(uint32_t x, uint32_t y, uint8_t *buffer) {
  uint8_t i2cBuff[16];
  _disp_buf = buffer;

  i2cBuff[0] = SSD1306_CMD; 
  i2cBuff[1] = 0xAE;  // Display off 
  siic_write(SSD1306_ADDR, i2cBuff, 2);

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

  memcpy(_disp_buf, splash_img, 512);

  ssd1306_update();
/*
  i2cBuff[0] = SSD1306_DATA;
  i2cBuff[1] = 0x00;
  for (i = 0; i < 512; i++) {
    siic_write(SSD1306_ADDR, i2cBuff, 2);
  }
*/
  i2cBuff[0] = SSD1306_CMD_RPT;
  i2cBuff[1] = 0x8D;  // Charge Pump Command 
  i2cBuff[2] = SSD1306_CMD_RPT;
  i2cBuff[3] = 0x14;  // Charge Pump Enabled
  i2cBuff[4] = SSD1306_CMD;
  i2cBuff[5] = 0xAF;  // Display on
  siic_write(SSD1306_ADDR, i2cBuff, 6);

}

/*!
  Updates SSD1306 display from buffer
*/
void ssd1306_update(void) {
  uint8_t i2cBuff[6];

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

  i2cBuff[0] = SSD1306_DATA;
  i2cBuff[1] = 0x00;
  for (int i = 0; i < 512; i++) {
    i2cBuff[1] = _disp_buf[i];
    siic_write(SSD1306_ADDR, i2cBuff, 2);
  }
}

