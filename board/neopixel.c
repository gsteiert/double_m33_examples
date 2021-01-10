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

#include "neopixel.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_sctimer.h"


//--------------------------------------------------------------------+
// Neopixel Driver
//--------------------------------------------------------------------+
#define NEO_SCT           SCT0
#define NEO_MATCH_RISE    12
#define NEO_MATCH_PERIOD  13
#define NEO_MATCH_RESET   15
#define NEO_MATCH_0       0
#define NEO_MATCH_1       11
#define NEO_EVENT_RISE    12
#define NEO_EVENT_FALL_0  0
#define NEO_EVENT_FALL_1  11
#define NEO_EVENT_NEXT    13
#define NEO_EVENT_LAST    14
#define NEO_EVENT_RESET   15
#define NEO_SCT_OUTPUT    6
#define NEO_COUNT_RISE    1
#define NEO_COUNT_FALL0   31
#define NEO_COUNT_FALL1   61
#define NEO_COUNT_PERIOD  120
#define NEO_COUNT_RESET   8120
#define NEO_FIRST_BIT     23


volatile uint32_t _neopixel_array[NEOPIXEL_NUMBER] = {0x404040, 0x202020};
volatile uint32_t _neopixel_count = 0;
volatile bool     _neopixel_busy = false;

void neopixel_int_handler(void){
  uint32_t eventFlag = NEO_SCT->EVFLAG;
  if (eventFlag & (1 << NEO_EVENT_RESET)) {
    _neopixel_busy = false;
  } else if (eventFlag & (1 << NEO_EVENT_LAST)) {
    _neopixel_count += 1;
    if (_neopixel_count < (NEOPIXEL_NUMBER)) {
      NEO_SCT->STATE = NEO_FIRST_BIT; 
      NEO_SCT->EV[NEO_EVENT_FALL_0].STATE = (~_neopixel_array[_neopixel_count]);
      NEO_SCT->CTRL = SCT_CTRL_CLRCTR_L_MASK;
    } else {
      NEO_SCT->CTRL = 0x0;    
    }
  }
  NEO_SCT->EVFLAG = eventFlag;
}

void SCT0_DriverIRQHandler(void){
  neopixel_int_handler();
  SDK_ISR_EXIT_BARRIER;
}

void neopixel_setPixel(uint32_t pixel, uint32_t color){
  if (pixel < NEOPIXEL_NUMBER) { 
    _neopixel_array[pixel] = color;
  }
}

void neopixel_refresh(void){
//  while (NEO_SCT->CTRL & SCT_CTRL_HALT_L_MASK);
  _neopixel_busy = true;
  _neopixel_count = 0;
  NEO_SCT->STATE = NEO_FIRST_BIT; 
  NEO_SCT->EV[NEO_EVENT_FALL_0].STATE = (~_neopixel_array[0]);
  NEO_SCT->CTRL = SCT_CTRL_CLRCTR_L_MASK;
}


void neopixel_init(void) {
  CLOCK_EnableClock(kCLOCK_Sct0);
  RESET_PeripheralReset(kSCT0_RST_SHIFT_RSTn);

  NEO_SCT->CONFIG = (
    SCT_CONFIG_UNIFY(1) | 
    SCT_CONFIG_CLKMODE(kSCTIMER_System_ClockMode) | 
    SCT_CONFIG_NORELOAD_L(1) );
  NEO_SCT->CTRL = ( 
    SCT_CTRL_HALT_L(1) |
    SCT_CTRL_CLRCTR_L(1) );

  NEO_SCT->MATCH[NEO_MATCH_RISE] = NEO_COUNT_RISE;
  NEO_SCT->MATCH[NEO_MATCH_PERIOD] = NEO_COUNT_PERIOD;
  NEO_SCT->MATCH[NEO_MATCH_0] = NEO_COUNT_FALL0;
  NEO_SCT->MATCH[NEO_MATCH_1] = NEO_COUNT_FALL1;
  NEO_SCT->MATCH[NEO_MATCH_RESET] = NEO_COUNT_RESET;
  NEO_SCT->EV[NEO_EVENT_RISE].STATE = 0xFFFFFFFF;
  NEO_SCT->EV[NEO_EVENT_RISE].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_RISE) );
  NEO_SCT->EV[NEO_EVENT_FALL_0].STATE = 0x0;
  NEO_SCT->EV[NEO_EVENT_FALL_0].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_0) );
  NEO_SCT->EV[NEO_EVENT_FALL_1].STATE = 0xFFFFFFFF;
  NEO_SCT->EV[NEO_EVENT_FALL_1].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_1) );
  NEO_SCT->EV[NEO_EVENT_NEXT].STATE = 0xFFFFFFFE;
  NEO_SCT->EV[NEO_EVENT_NEXT].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_PERIOD) | 
    SCT_EV_CTRL_STATELD(0) | SCT_EV_CTRL_STATEV(31));
  NEO_SCT->EV[NEO_EVENT_LAST].STATE = 0x1;
  NEO_SCT->EV[NEO_EVENT_LAST].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_PERIOD) );
  NEO_SCT->EV[NEO_EVENT_RESET].STATE = 0xFFFFFFFF;
  NEO_SCT->EV[NEO_EVENT_RESET].CTRL = (
    kSCTIMER_MatchEventOnly | SCT_EV_CTRL_MATCHSEL(NEO_MATCH_RESET) );


  NEO_SCT->LIMIT = (1 << NEO_EVENT_NEXT) | (1 << NEO_EVENT_RESET);
  NEO_SCT->HALT = (1 << NEO_EVENT_LAST) | (1 << NEO_EVENT_RESET);
  NEO_SCT->START = 0x0;

  NEO_SCT->OUT[NEO_SCT_OUTPUT].SET = (1 << NEO_EVENT_RISE);
  NEO_SCT->OUT[NEO_SCT_OUTPUT].CLR = (1 << NEO_EVENT_FALL_0) | (1 << NEO_EVENT_FALL_1) | (1 << NEO_EVENT_LAST);
  
  NEO_SCT->OUTPUT = 0x0;
  NEO_SCT->RES = SCT_RES_O6RES(0x2);
  NEO_SCT->EVEN = (1 << NEO_EVENT_LAST) | (1 << NEO_EVENT_RESET);
  EnableIRQ(SCT0_IRQn);

  neopixel_setPixel(0, 0x101000);
  neopixel_setPixel(1, 0x101000);
  neopixel_refresh();
}
