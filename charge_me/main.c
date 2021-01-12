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
#include "fsl_i2c.h"


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define I2C_BASE                   (I2C1_BASE)
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define WAIT_TIME                  10U
#define I2C_MASTER                 ((I2C_Type *)I2C_BASE)

#define I2C_BQ25619_ADDR_7BIT      0x6AU
#define I2C_BAUDRATE               100000U
#define I2C_DATA_LENGTH            33U

#define NEO_RED     0x001000
#define NEO_GREEN   0x100000
#define NEO_BLUE    0x000010
#define NEO_YELLOW  0x101000
#define NEO_CYAN    0x100010
#define NEO_MAGENTA 0x001010
#define NEO_WHITE   0x101010
#define NEO_OFF     0x0

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
  i2c_master_config_t masterConfig;
  status_t reVal        = kStatus_Fail;
  uint32_t pixelData[NEOPIXEL_NUMBER];
  uint8_t i2cBuff[I2C_DATA_LENGTH];
//  neopixel_config_t neoConfig;
  board_init();
  sctpix_init(NEOPIXEL_TYPE);
  sctpix_addCh(NEOPIXEL_CH, pixelData, NEOPIXEL_NUMBER);
  
  /* attach 12 MHz clock to FLEXCOMM1 (I2C master) */
  CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);

  /* reset FLEXCOMM for I2C */
  RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);

  uint32_t start_ms = 0;
  uint32_t pallet[8] = {
    0x000000, 0x000010, 0x001000, 0x100000,
    0x101010, 0x101000, 0x100010, 0x001010
  };
  uint32_t count = 0;

  sctpix_setPixel(NEOPIXEL_CH, 0, NEO_YELLOW);
  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_YELLOW);
  sctpix_show();

  I2C_MasterGetDefaultConfig(&masterConfig);
  /* Initialize the I2C master peripheral */
  I2C_MasterInit(I2C_MASTER, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

  sctpix_setPixel(NEOPIXEL_CH, 1, NEO_CYAN);
  sctpix_show();


  /* Send master blocking data to slave */
  if (kStatus_Success == I2C_MasterStart(I2C_MASTER,
                                         I2C_BQ25619_ADDR_7BIT,
                                         kI2C_Write)) {
    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer
      + stop*/
    sctpix_setPixel(NEOPIXEL_CH, 1, NEO_GREEN);
    sctpix_show();
    i2cBuff[0] = 0x0;
    i2cBuff[1] = 0x4A;
    reVal = I2C_MasterWriteBlocking(I2C_MASTER, i2cBuff, 2,
                                    kI2C_TransferDefaultFlag);
    if (reVal != kStatus_Success) {
      sctpix_setPixel(NEOPIXEL_CH, 0, NEO_RED);
      sctpix_setPixel(NEOPIXEL_CH, 1, NEO_CYAN);
      sctpix_show();
      return -1;
    }

    reVal = I2C_MasterStop(I2C_MASTER);
    if (reVal != kStatus_Success) {
      sctpix_setPixel(NEOPIXEL_CH, 0, NEO_RED);
      sctpix_setPixel(NEOPIXEL_CH, 1, NEO_MAGENTA);
      sctpix_show();
      return -1;
    }
      sctpix_setPixel(NEOPIXEL_CH, 0, NEO_GREEN);
      sctpix_setPixel(NEOPIXEL_CH, 1, NEO_GREEN);
      sctpix_show();

  } else {
      sctpix_setPixel(NEOPIXEL_CH, 0, NEO_RED);
      sctpix_setPixel(NEOPIXEL_CH, 1, NEO_RED);
      sctpix_show();
  }

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
