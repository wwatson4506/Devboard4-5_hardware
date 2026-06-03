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

#include "imxrt.h"
#include "math.h"
#include <stdbool.h>
#include <stdio.h>
#include "core_pins.h"

//=============================================================================
// SDRAM
//=============================================================================

#include <smalloc.h>

#ifdef __cplusplus
extern "C" {
#endif

struct smalloc_pool sdram_smalloc_pool;

#ifdef __cplusplus
}
#endif

// default = 166MHz
#define SDRAM_CLOCK 206
// default base address
#define SDRAM_BASE 0x80000000
// default SDRAM size (in MBs)
#define SDRAM_SIZE 32
// Dev board with no capacitor on pin EMC_39
#define SDRAM_USEDQS 1

uint8_t _size = 0;
float frequency = 0;

float getFrequency() { return frequency; }

unsigned int ns_to_clocks(float ns, float freq)
{
    float clocks = ceilf(ns * 1.0e-9f * freq);
    if (clocks < 1.0f) return 1;
    return (unsigned int)clocks;
}

void configure_sdram_pins()
{
	// initialize pins
    IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_00 = 0x0110F9;   //SEMC_D0
                                                /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/7
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Enabled */
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_01 = 0x0110F9;   //SEMC_D0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_02 = 0x0110F9;   //SEMC_D1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_03 = 0x0110F9;   //SEMC_D2
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_04 = 0x0110F9;   //SEMC_D3
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_05 = 0x0110F9;   //SEMC_D4
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_06 = 0x0110F9;   //SEMC_D5
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_07 = 0x0110F9;   //SEMC_D6
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_08 = 0x0110F9;   //SEMC_DMO
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_09 = 0x0110F9;   //SEMC_A0 // edited
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_10 = 0x0110F9;   //SEMC_A1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_11 = 0x0110F9;   //SEMC_A2
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_12 = 0x0110F9;   //SEMC_A3
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_13 = 0x0110F9;   //SEMC_A4
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_14 = 0x0110F9;   //SEMC_A5
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_15 = 0x0110F9;   //SEMC_A6
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_16 = 0x0110F9;   //SEMC_A7
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_17 = 0x0110F9;   //SEMC_A8
    
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_18 = 0x0110F9;   //SEMC_A9
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_19 = 0x0110F9;   //SEMC_A11
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_20 = 0x0110F9;   //SEMC_A12
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_21 = 0x0110F9;   //SEMC_BA0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_22 = 0x0110F9;   //SEMC_BA1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_23 = 0x0110F9;   //SEMC_A10
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_24 = 0x0110F9;   //SEMC_CAS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_25 = 0x0110F9;   //SEMC_RAS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_26 = 0x0110F9;   //SEMC_CLK
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_27 = 0x0110F9;   //SEMC_CKE
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_28 = 0x0110F9;   //SEMC_WE
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_29 = 0x0110F9;   //SEMC_CS0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_30 = 0x0110F9;   //SEMC_D8
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_31 = 0x0110F9;   //SEMC_D9
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_32 = 0x0110F9;   //SEMC_D10
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_33 = 0x0110F9;   //SEMC_D11
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_34 = 0x0110F9;   //SEMC_D12
    
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_35 = 0x0110F9;   //SEMC_D13
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_36 = 0x0110F9;   //SEMC_D14
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_37 = 0x0110F9;   //SEMC_D15
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_38 = 0x0110F9;   //SEMC_DM1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_39 = 0x0110F9;   //SEMC_DQS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_40 = 0x0110F9;   //SEMC_MD0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_41 = 0x0110F9;   //SEMC_MD1

// configure pins
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_01 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_02 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_03 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_04 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_05 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_07 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_09 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_10 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_11 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_12 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_13 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_14 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_15 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_16 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_17 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_18 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_19 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_20 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_21 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_22 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_23 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_24 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_25 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_26 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_27 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_28 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_29 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_30 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_33 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_34 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_35 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_36 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_37 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_38 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_39 = 0x10;

}

bool IPCommandComplete()
{
    /* Poll status bit till command is done*/
    while (!(SEMC_INTR & 0x1))
    {
    };

    /* Clear status bit */
    SEMC_INTR |= 0x1;

    /* Check error status */
    if (SEMC_INTR & 0x2)
    {
        SEMC_INTR |= 0x2;
        return false;
    }

    return true;
}


bool SendIPCommand(uint32_t address, uint16_t command, uint32_t write, uint32_t *read)
{
    uint32_t cmdMode;
    bool readCmd = 0;
    bool writeCmd = 0;

    /* Clear status bit */
    SEMC_INTR |= 0x1U;
    /* Set address. */
    SEMC_IPCR0= address;

    /* Check command mode. */
    cmdMode = command & 0xFU;

    readCmd = (cmdMode == 0x08);    /*  SDRAM memory read. */
    writeCmd = (cmdMode == 0x09) || (cmdMode == 0x0a);  /* SDRAM memory write, SDRAM MODE SET */

    if (writeCmd)
    {
        /* Set data. */
        SEMC_IPTXDAT = write;
    }

    /* Set command code. */
    SEMC_IPCMD = command | (((uint32_t)(((uint32_t)(0xA55A)) << 16)) & 0xFFFF0000U);
    
    /* Wait for command done. */
    bool result = IPCommandComplete();

    if (result != true)
    {
        return result;
    }

    if (readCmd)
    {
        /* Get the read data */
        *read = SEMC_IPRXDAT;
    }

    return true;
}

bool  check_fixed_pattern(uint32_t pattern)
{
	volatile uint32_t *p;
    uint32_t *memory_begin, *memory_end;
    memory_begin = (uint32_t *)(0x80000000);
	memory_end = (uint32_t *)(0x80000000 + _size * 1048576);
    
	//Serial.printf("testing with fixed pattern %08X\n", pattern);
	for (p = memory_begin; p < memory_end; p++) {
		*p = pattern;
	}
	arm_dcache_flush_delete((void *)memory_begin,
		(uint32_t)memory_end - (uint32_t)memory_begin);
	for (p = memory_begin; p < memory_end; p++) {
		uint32_t actual = *p;
		if (actual != pattern) return false;
	}
	return true;
}

bool sdram_begin(uint8_t external_sdram_size, uint8_t clock, uint8_t useDQS)
{
    static bool doneOnce=false;
    _size = external_sdram_size;
    unsigned int clockdiv;
    float base_frequency;
    bool use_pll2_pfd2 = false;
    bool use_pll3_pfd1 = false;
    bool use_pll3_custom_pfd1 = false;
    
    if (clock > 360 || clock < 100) return false;
    
    switch(clock) {
        case 133:
            clockdiv = 5;
            use_pll3_pfd1 = true;
            break;
        case 166:
            clockdiv = 4;
            use_pll3_pfd1 = true;
            break;
        case 221:
            clockdiv = 3;
            use_pll3_pfd1 = true;
            break;
        case 198:
            clockdiv = 2;
            use_pll2_pfd2 = true;
            break;
        default:
            clockdiv = (clock < 124) ? 3 : 2;
            use_pll3_custom_pfd1 = true;
            break;
    }
            
        
    /* Experimental note (see https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/post-335619):
    *  if you want to try 198 MHz overclock
    *  const unsigned int clockdiv = 2; // PLL2_PFD2 / 2 = 396 / 2 = 198 MHz
    */
    if (use_pll2_pfd2) {
        CCM_CBCDR = (CCM_CBCDR & ~(CCM_CBCDR_SEMC_PODF(7) | CCM_CBCDR_SEMC_ALT_CLK_SEL)) |
        CCM_CBCDR_SEMC_CLK_SEL | CCM_CBCDR_SEMC_PODF(clockdiv-1);  
        base_frequency = 396.0e6;
    /* If it doesn't work, maybe try soldering a 5pF or 10pF capacitor at C29 
    */
    } else if (use_pll3_pfd1) {
    // use PLL3 PFD1 664.62 divided by 4 or 5, for 166 or 133 MHz
    // 5 = 133mhz
    // 4 = 166mhz - SDRAM rated,  see post #60
    // 3 = 221mhz
        CCM_CBCDR = (CCM_CBCDR & ~(CCM_CBCDR_SEMC_PODF(7))) |
            CCM_CBCDR_SEMC_CLK_SEL | CCM_CBCDR_SEMC_ALT_CLK_SEL |
            CCM_CBCDR_SEMC_PODF(clockdiv-1);
        base_frequency = 664.6154e6;
    } else if (use_pll3_custom_pfd1) {
        // custom PLL3 PDF1: 173,180,187,196,206,216,227,240,254,270,288,etc
        CCM_ANALOG_PFD_480_SET = 0x80 << 8;
        CCM_ANALOG_PFD_480_CLR = 0x7F << 8;
        unsigned int frac = roundf(8640.0f / (float)(clock * clockdiv));
        if (frac < 12 || frac > 35) return false; // should never happen...
        CCM_ANALOG_PFD_480_TOG = (0x80 | frac) << 8;
        CCM_CBCDR = (CCM_CBCDR & ~(CCM_CBCDR_SEMC_PODF(7))) |
            CCM_CBCDR_SEMC_CLK_SEL | CCM_CBCDR_SEMC_ALT_CLK_SEL |
            CCM_CBCDR_SEMC_PODF(clockdiv-1);
        base_frequency = 8640.0e6f / (float)frac;
    } else {
        return false;
    }
    
    delayMicroseconds(1);
    const float freq = base_frequency / (float)clockdiv;
    frequency = freq / 1.0e6f;
    //Serial.printf("Clock set %.2f MHz\n", freq / 1.0e6f);
    CCM_CCGR3 |= CCM_CCGR3_SEMC(CCM_CCGR_ON);
    
    // software reset
    SEMC_BR0 = 0;
    SEMC_BR1 = 0;
    SEMC_BR2 = 0;
    SEMC_BR3 = 0;
    SEMC_BR4 = 0;
    SEMC_BR5 = 0;
    SEMC_BR6 = 0;
    SEMC_BR7 = 0;
    SEMC_BR8 = 0;
    SEMC_MCR = SEMC_MCR_SWRST;
    uint32_t timeout = micros();
    while (SEMC_MCR & SEMC_MCR_SWRST)
    {
      if (micros() - timeout > 1500)
      {
        return false;
      }
    }
    configure_sdram_pins();

    if(useDQS == 1) {
        SEMC_MCR |= SEMC_MCR_MDIS | SEMC_MCR_CTO(0xFF) | SEMC_MCR_BTO(0x1F) | SEMC_MCR_DQSMD;
    } else  { // enable SEMC_MCR_DQSMD (EMC_39
        SEMC_MCR |= SEMC_MCR_MDIS | SEMC_MCR_CTO(0xFF) | SEMC_MCR_BTO(0x1F);
    }

    // TODO: reference manual page 1364 says "Recommend to set BMCR0 with 0x0 for
    // applications that require restrict sequence of transactions", same on BMCR1
    //SEMC_BMCR0 = SEMC_BMCR0_WQOS(5) | SEMC_BMCR0_WAGE(8) |
    //    SEMC_BMCR0_WSH(0x40) | SEMC_BMCR0_WRWS(0x10);
    //SEMC_BMCR1 = SEMC_BMCR1_WQOS(5) | SEMC_BMCR1_WAGE(8) |
    //    SEMC_BMCR1_WPH(0x60) | SEMC_BMCR1_WRWS(0x24) | SEMC_BMCR1_WBR(0x40);
    //Per NXP Forum: i.MXRT1060 SEMC SDRAM Data Corruption,
    // https://community.nxp.com/t5/i-MX-RT/i-MXRT1060-SEMC-SDRAM-Data-Corruption/m-p/1172919/highlight/true#M10887
    SEMC_BMCR0 = 0x81;
    SEMC_BMCR1 = 0x81;

    SEMC_MCR &= ~SEMC_MCR_MDIS;

    // configure SDRAM parameters
    SEMC_BR0 = 0x80000000 | SEMC_BR_MS(13 /*13 = 32 Mbyte*/) | SEMC_BR_VLD;
    SEMC_SDRAMCR0 = SEMC_SDRAMCR0_CL(3) |
        SEMC_SDRAMCR0_COL(3) |  // 3 = 9 bit column
        SEMC_SDRAMCR0_BL(3) |   // 3 = 8 word burst length
        SEMC_SDRAMCR0_PS;       // use 16 bit data
    SEMC_SDRAMCR1 =
        SEMC_SDRAMCR1_ACT2PRE((ns_to_clocks(42, freq)-1)) | // tRAS: ACTIVE to PRECHARGE
        SEMC_SDRAMCR1_CKEOFF((ns_to_clocks(42, freq)-1)) |  // self refresh
        SEMC_SDRAMCR1_WRC((ns_to_clocks(12, freq)-1)) |     // tWR: WRITE recovery
        SEMC_SDRAMCR1_RFRC((ns_to_clocks(67, freq)-1)) |    // tRFC or tXSR: REFRESH recovery
        SEMC_SDRAMCR1_ACT2RW((ns_to_clocks(18, freq)-1)) |  // tRCD: ACTIVE to READ/WRITE
        SEMC_SDRAMCR1_PRE2ACT((ns_to_clocks(18, freq)-1));  // tRP: PRECHARGE to ACTIVE/REFRESH
    SEMC_SDRAMCR2 = 0; // TODO... page 1425
    SEMC_SDRAMCR3 = 0; // TODO...page 1426
    SEMC_SDRAMCR2 =
        SEMC_SDRAMCR2_SRRC((ns_to_clocks(67, freq) - 1)) |
        SEMC_SDRAMCR2_REF2REF(ns_to_clocks(60, freq)) |           /* No Minus one to keep with RM */
        SEMC_SDRAMCR2_ACT2ACT(ns_to_clocks(60, freq)) | /* No Minus one to keep with RM */
        SEMC_SDRAMCR2_ITO(0);  //sdk has: idle = config->tIdleTimeout_Ns / config->tPrescalePeriod_Ns


    uint32_t prescaleperiod = 160 * (1000000000 / freq);
    uint16_t prescale = prescaleperiod / 16 / (1000000000 / freq);

    if (prescale > 256)
    {
        //Serial.println("Invalid Timer Setting");
        while(1){}
    }

    uint16_t refresh = (64 * 1000000 / 8192)/ prescaleperiod;
    uint16_t urgentRef = refresh;
    //uint16_t idle = 0 / prescaleperiod;
    
    SEMC_SDRAMCR3 = SEMC_SDRAMCR3_REBL((1 - 1)) |
                     /* N * 16 * 1s / clkSrc_Hz = config->tPrescalePeriod_Ns */
                     SEMC_SDRAMCR3_PRESCALE(prescale) | SEMC_SDRAMCR3_RT(refresh) | SEMC_SDRAMCR3_UT(urgentRef);

    SEMC_IPCR1 = 2; // IP commadns, data is 16 bits wide
    SEMC_IPCR2 = 0;
if (doneOnce) return true;
doneOnce=true;

    //  send IP commands to initialize SDRAM chip:
    //  precharge all
    //  auto refresh (NXP SDK sends this twice, why?)
    //  mode set
    bool result_cmd = SendIPCommand(0x80000000, 0x0f, 0, NULL);  //Prechargeall
    if (result_cmd != true)
    {
        return result_cmd;
    }
    result_cmd = SendIPCommand(0x80000000, 0x0c, 0, NULL);        //AutoRefresh
    if (result_cmd != true)
    {
        return result_cmd;
    }
    result_cmd = SendIPCommand(0x80000000, 0x0c, 0, NULL);         //AutoRefresh
    if (result_cmd != true)
    {
        return result_cmd;
    }
    /* Mode setting value. */
    uint16_t mode = (uint16_t)3| (uint16_t)(3 << 4);
    result_cmd = SendIPCommand(0x80000000, 0x0a, mode, NULL);       //Modeset
    if (result_cmd != true)
    {
        return result_cmd;
    }
    // enable refresh
    SEMC_SDRAMCR3 |= SEMC_SDRAMCR3_REN;

    if(result_cmd == false) return false;
    
    sm_set_pool(&sdram_smalloc_pool, (void *)0x80000000, external_sdram_size * 1024 *1024, 1, NULL);

//    if(!check_fixed_pattern(0x5A698421))
//        return false;


    return true; // hopefully SDRAM now working at 80000000 to 81FFFFFF
}

void startup_middle_hook(void)
{
    sdram_begin(SDRAM_SIZE, SDRAM_CLOCK, SDRAM_USEDQS);

}

#include <stdlib.h>
#include "wiring.h"
#define IS_SDRAM(addr) (((uint32_t)addr >> 28) == 8)

void *sdram_malloc(size_t size)
{
    void *ptr = sm_malloc_pool(&sdram_smalloc_pool, size);
    if (ptr) return ptr;
    return malloc(size);
}

void sdram_free(void *ptr)
{
    if (IS_SDRAM(ptr)) {
        sm_free_pool(&sdram_smalloc_pool, ptr);
        return;
    }
    free(ptr);
}

void *sdram_calloc(size_t nmemb, size_t size)
{
    // Note: It is assumed that the pool was created with do_zero set to true
    void *ptr = sm_malloc_pool(&sdram_smalloc_pool, nmemb*size);
    if (ptr) return ptr;
    return calloc(nmemb, size);
}

void *sdram_realloc(void *ptr, size_t size)
{
    if (IS_SDRAM(ptr)) {
        return sm_realloc_pool(&sdram_smalloc_pool, ptr, size);
    }
    return realloc(ptr, size);
}

