/************************************************************************************
 * configs/sam4l-xplained/src/sam4l-xplained.h
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __CONFIGS_SAM4L_XPLAINED_SRC_SAM4L_XPLAINED_H
#define __CONFIGS_SAM4L_XPLAINED_SRC_SAM4L_XPLAINED_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

#include <arch/irq.h>
#include <nuttx/irq.h>

#include "chip/sam_pinmap.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* LEDs: There are three LEDs on board the SAM4L Xplained Pro board:  The EDBG
 * controls two of the LEDs, a power LED and a status LED.  There is only
 * one user controllable LED, a yellow LED labeled LED0 near the SAM4L USB
 * connector.
 *
 * This LED is controlled by PC07 and LED0 can be activated by driving the
 * PC07 to GND.
 *
 * When CONFIG_ARCH_LEDS is defined in the NuttX configuration, NuttX will
 * control LED0 as follows:
 *
 *   SYMBOL              Meaning                 LED0
 *   ------------------- ----------------------- ------
 *   LED_STARTED         NuttX has been started  OFF
 *   LED_HEAPALLOCATE    Heap has been allocated OFF
 *   LED_IRQSENABLED     Interrupts enabled      OFF
 *   LED_STACKCREATED    Idle stack created      ON
 *   LED_INIRQ           In an interrupt         N/C
 *   LED_SIGNAL          In a signal handler     N/C
 *   LED_ASSERTION       An assertion failed     N/C
 *   LED_PANIC           The system has crashed  FLASH
 *
 * Thus is LED0 is statically on, NuttX has successfully  booted and is,
 * apparently, running normmally.  If LED0 is flashing at approximately
 * 2Hz, then a fatal error has been detected and the system has halted.
 */

#define GPIO_LED0     (GPIO_OUTPUT | GPIO_PULL_NONE | GPIO_OUTPUT_SET | \
                       GPIO_PORTC | GPIO_PIN7)

/* QTouch button: The SAM4L Xplained Pro kit has one QTouch button.  The connection
 * to the SAM4L is:
 *
 *   PC13 CATB_SENSE15
 *   PC14 CATB_DIS
 */

/* Mechanical buttons:
 *
 * The SAM4L Xplained Pro contains two mechanical buttons. One button is the
 * RESET button connected to the SAM4L reset line and the other is a generic user
 * configurable button. When a button is pressed it will drive the I/O line to GND.
 *
 *   PC24 SW0
 */

#define GPIO_SW0      (GPIO_INTERRUPT | GPIO_PULL_UP | GPIO_GLITCH_FILTER | \
                       GPIO_PORTC | GPIO_PIN24)
#define IRQ_SW0       SAM_IRQ_PC24

/* I/O1
 *
 * Support for the microSD card slot on the I/O1 module.  The I/O1 requires
 * SPI support and two GPIOs.  These two GPIOs will vary if the
 *
 *
 *   PIN EXT1           EXT2            Description
 *   --- -------------- --------------- -------------------------------------
 *   15  PC03 SPI/NPCS0 PB11 SPI/NPCS2  Active low chip select OUTPUT, pulled
 *                                      high on board.
 *   10  PB13 SPI/NPCS1 10 PC09 GPIO    Active low card detect INPUT, must
 *                                      use internal pull-up.
 */

#ifdef CONFIG_SAM4L_XPLAINED_IOMODULE

#  ifndef CONFIG_SAM34_SPI
#    error CONFIG_SAM34_SPI is required to use the I/O1 module
#  endif

#  if defined(CONFIG_SAM4L_XPLAINED_IOMODULE_EXT1)

#    define GPIO_SD_CD (GPIO_INTERRUPT | GPIO_INT_CHANGE | GPIO_PULL_UP | \
                        GPIO_GLITCH_FILTER | GPIO_PORTB | GPIO_PIN13)
#    define IRQ_SD_CD   SAM_IRQ_PB13

#    define GPIO_SD_CS (GPIO_OUTPUT | GPIO_PULL_NONE | GPIO_OUTPUT_SET | \
                        GPIO_PORTC | GPIO_PIN3)
#    define SD_CSNO    0

#  elif defined(CONFIG_SAM4L_XPLAINED_IOMODULE_EXT2)
#    define GPIO_CD   (GPIO_INTERRUPT | GPIO_INT_CHANGE | GPIO_PULL_UP | \
                       GPIO_GLITCH_FILTER | GPIO_PORTC | GPIO_PIN9)
#    define IRQ_CD     SAM_IRQ_PC9

#    define GPIO_SD_CS (GPIO_OUTPUT | GPIO_PULL_NONE | GPIO_OUTPUT_SET | \
                        GPIO_PORTB | GPIO_PIN11)
#    define SD_CSNO    2

#  else
#    error Which connector is the I/O1 module installed in?
#  endif
#endif

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: sam_spiinitialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the SAM3U-EK board.
 *
 ************************************************************************************/

void weak_function sam_spiinitialize(void);

/************************************************************************************
 * Name: sam_sdinitialize
 *
 * Description:
 *   Initialize the SPI-based SD card.  Requires CONFIG_SAM4L_XPLAINED_IOMODULE=y,
 *   CONFIG_DISABLE_MOUNTPOINT=n, CONFIG_MMCSD=y, and CONFIG_SAM34_SPI=y
 *
 ************************************************************************************/

#ifdef CONFIG_SAM4L_XPLAINED_IOMODULE
int sam_sdinitialize(int minor);
#endif

/************************************************************************************
 * Name: up_ledinit
 ************************************************************************************/

#ifdef CONFIG_ARCH_LEDS
void up_ledinit(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __CONFIGS_SAM4L_XPLAINED_SRC_SAM4L_XPLAINED_H */

