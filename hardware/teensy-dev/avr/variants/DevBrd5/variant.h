/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2018 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Tell most of the system for now you are a MICROMOD
//#define ARDUINO_TEENSY_MICROMOD
#include "smalloc.h"

#ifdef __cplusplus
extern "C" {
#endif

void *sdram_malloc(size_t size);
void sdram_free(void *ptr);
void *sdram_calloc(size_t nmemb, size_t size);
void *sdram_realloc(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------
// Serial objects overwrides.
// Serial2 - Note this update should be done to MMOD itself as 44 and 45 are valid on these boards as well
#define SERIAL2_UART_ADDR IMXRT_LPUART3_ADDRESS
#define SERIAL2_LPUART   IRQ_LPUART3, CCM_CCGR0, CCM_CCGR0_LPUART3(CCM_CCGR_ON), XBARA1_OUT_LPUART3_TRG_INPUT
#define SERIAL2_CTS_PIN 0xff, 0 
#define SERIAL2_RX_PINS  {{16,2, &IOMUXC_LPUART3_RX_SELECT_INPUT, 0}, {45, 3, &IOMUXC_LPUART3_RX_SELECT_INPUT, 2}}
#define SERIAL2_TX_PINS  {{17,2, &IOMUXC_LPUART3_TX_SELECT_INPUT, 0}, {44, 3, &IOMUXC_LPUART3_TX_SELECT_INPUT, 2}}

// Serial7 The default pins on MMOD are EMC pins, but we added Serial 5 pins, used as Serial8 on T4.1...
#define SERIAL7_UART_ADDR IMXRT_LPUART5_ADDRESS
#define SERIAL7_LPUART IRQ_LPUART5, CCM_CCGR3, CCM_CCGR3_LPUART5(CCM_CCGR_ON), XBARA1_OUT_LPUART5_TRG_INPUT
#define SERIAL7_CTS_PIN 0xff, 0 
#define SERIAL7_RX_PINS {{61,1, &IOMUXC_LPUART5_RX_SELECT_INPUT, 1}, {0xff, 0xff, nullptr, 0}}
#define SERIAL7_TX_PINS {{60,1, &IOMUXC_LPUART5_TX_SELECT_INPUT, 1}, {0xff, 0xff, nullptr, 0}}
