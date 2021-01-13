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

#ifndef _SIMPLE_I2C_H_
#define _SIMPLE_I2C_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

//--------------------------------------------------------------------+
// Simple I2C Driver
//--------------------------------------------------------------------+

/*!
  @brief  Initializes I2C controller for master operation
  @param  base  Base address of the I2C instance to be initialized
  @param  rate  Desired bits per second rate to be used on the bus
*/
void siic_init(uint32_t base, uint32_t rate);

/*!
  @brief  Performs I2C write , restart, and read. 
  @param  addr I2C slave address
  @param  wData buffer containing write data
  @param  wSize number of bytes to write (write is skipped if wSize = 0)
  @param  rData buffer to fill with read data
  @param  rSize number of bytes to read (read is skipped if rSize =0)
*/
void siic_xfer(uint32_t addr, uint8_t *wData, uint32_t wSize, uint8_t *rData, uint32_t rSize);

/*!
  @brief  Performs I2C write , restart, and read. 
  @param  addr I2C slave address
  @param  wData buffer containing write data
  @param  wSize number of bytes to write (write is skipped if wSize = 0)
*/
void siic_write(uint32_t addr, uint8_t *wData, uint32_t wSize);

/*!
  @brief  Performs I2C write , restart, and read. 
  @param  addr I2C slave address
  @param  rData buffer to fill with read data
  @param  rSize number of bytes to read (read is skipped if rSize =0)
*/
void siic_read(uint32_t addr, uint8_t *rData, uint32_t rSize);

#ifdef __cplusplus
 }
#endif

#endif