/**
 * \file
 *
 * \brief Common User Interface for USB application
 *
 * Copyright (c) 2009-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _UI_H_
#define _UI_H_

#include "gfx.h"

#define KEY_ROW_NUM		4
#define KEY_COL_NUM		3
#define KEY_COUNT		KEY_ROW_NUM*KEY_COL_NUM
#define KEY_ICON_MAX_DIM  50

#define ROW_COL_TO_IDX(ROW, COL)	ROW+COL*KEY_ROW_NUM
#define IDX_TO_ROW(IDX)				IDX%KEY_ROW_NUM
#define IDX_TO_COL(IDX)				IDX/KEY_ROW_NUM

typedef struct key_info {
	uint8_t key_id;
	uint8_t key_code;
	gfx_coord_t centre_x;
	gfx_coord_t centre_y;
	gfx_coord_t max_dim;
	bool pressed;
	} key_info_t;

//! \brief Initializes the user interface
void ui_init(void);

// Refreshes the ui's screen. Clears the flag indicating an update is required.
void ui_refresh_screen(void);

// Sets a key icon - should this be in a separate place?
void ui_set_key_icon(uint8_t index, struct gfx_bitmap* bmp);

// Set the scancode for a key at the given index.
void ui_set_key_scancode(uint8_t index, uint8_t scancode);

// Set the internal flag for the ui to update the screen
void ui_set_needs_refresh(void);

// Gets the internal flag for the ui to update the screen
bool ui_get_needs_refresh(void);

//! \brief Enters the user interface in power down mode
void ui_powerdown(void);

//! \brief Enables the asynchronous interrupts of the user interface
void ui_wakeup_enable(void);

//! \brief Disables the asynchronous interrupts of the user interface
void ui_wakeup_disable(void);

//! \brief Exits the user interface of power down mode
void ui_wakeup(void);

//! \name Callback to show the MSC read and write access
//! @{
void ui_start_read(void);
void ui_stop_read(void);
void ui_start_write(void);
void ui_stop_write(void);
//! @}

/*! \brief Called when communication port is opened
 */
void ui_com_open(uint8_t port);

/*! \brief Called when communication port is closed
 */
void ui_com_close(uint8_t port);

/*! \brief Called when a data is received on CDC
 */
void ui_com_rx_start(void);

/*! \brief Called when a data is received on port com
 */
void ui_com_tx_start(void);

/*! \brief Called when all data pending are sent on port com
 */
void ui_com_rx_stop(void);

/*! \brief Called when all data pending are sent on CDC
 */
void ui_com_tx_stop(void);

/*! \brief Called when a communication error occur
 */
void ui_com_error(void);

/*! \brief Called when a overflow occur
 */
void ui_com_overflow(void);

/*! \brief This process is called each 1ms
 * It is called only if the USB interface is enabled.
 *
 * \param framenumber  Current frame number
 */
void ui_process(uint16_t framenumber);

/*! \brief Turn on or off the keyboard LEDs
 */
void ui_kbd_led(uint8_t value);

#endif // _UI_H_
