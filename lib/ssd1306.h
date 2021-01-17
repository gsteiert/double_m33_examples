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

#ifndef _SSD1306_H_
#define _SSD1306_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

//--------------------------------------------------------------------+
// SSD1306 Definitions
//--------------------------------------------------------------------+
#define SSD1306_ADDR               0x3C
#define SSD1306_CMD                0x00
#define SSD1306_CMD_RPT            0x80
#define SSD1306_DATA               0x40
#define SSD1306_DATA_RPT           0xC0

//--------------------------------------------------------------------+
// SSD1306 Prototypes
//--------------------------------------------------------------------+

/*!
  @brief  Initializes SSD1306 display
  @param  x  Horizontal resolution  
  @param  y  Vertical resolution  
  @param  buffer  pointer to display buffer
*/
void ssd1306_init(uint32_t x, uint32_t y, uint8_t *buffer);

/*!
  @brief  Updates SSD1306 display from buffer
*/
void ssd1306_update(void);

/*!
  @brief  Writes string to SSD1306 display
  @param  page    Horizontal resolution  
  @param  col     Vertical resolution  
  @param  buffer  pointer to string buffer
  @param  length  number of bytes to print
*/
void ssd1306_print(uint32_t page, uint32_t col, uint8_t *buffer, uint32_t length);

#ifdef __cplusplus
 }
#endif

#endif