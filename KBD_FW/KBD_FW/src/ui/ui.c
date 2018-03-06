/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (c) 2012-2015 Atmel Corporation. All rights reserved.
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

#include <asf.h>
#include "ui.h"
#include "key_reader.h"
#include "key_icons.h"
#include "fifo.h"
#include "Bitmaps.h"

#define KEY_ROW_NUM		4
#define KEY_COL_NUM		3
#define KEY_COUNT		KEY_ROW_NUM*KEY_COL_NUM
#define KEY_ICON_MAX_DIM  50

static struct  {
	gfx_coord_t x;
	gfx_coord_t y;
	key_icon_t* icon;
	bool req_update;
} key_icon_array[KEY_COUNT] = {
	{60, 50, NULL, 0},
	{152, 50, NULL, 0},
	{244, 50, NULL, 0},
	{336, 50, NULL, 0},
	{60, 142, NULL, 0},
	{152, 142, NULL, 0},
	{244, 142, NULL, 0},
	{336, 142, NULL, 0},
	{60, 234, NULL, 0},
	{152, 234, NULL, 0},
	{244, 234, NULL, 0},
	{336, 234, NULL, 0}
	};

#define  MOVE_UP     0
#define  MOVE_RIGHT  1
#define  MOVE_DOWN   2
#define  MOVE_LEFT   3

//! Sequence process running each \c SEQUENCE_PERIOD ms
#define SEQUENCE_PERIOD 150

static struct {
	bool b_modifier;
	bool b_down;
	uint8_t u8_value;
} ui_sequence[] = {
	// Display windows menu
	{true,true,HID_MODIFIER_LEFT_UI},
	// Launch Windows Command line
	{false,true,HID_R},
	{false,false,HID_R},
	// Clear modifier
	{true,false,HID_MODIFIER_LEFT_UI},
	// Tape sequence "notepad" + return
	{false,true,HID_N},
	{false,false,HID_N},
	{false,true,HID_O},
	{false,false,HID_O},
	{false,true,HID_T},
	{false,false,HID_T},
	{false,true,HID_E},
	{false,false,HID_E},
	{false,true,HID_P},
	{false,false,HID_P},
	{false,true,HID_A},
	{false,false,HID_A},
	{false,true,HID_D},
	{false,false,HID_D},
	{false,true,HID_ENTER},
	{false,false,HID_ENTER},
	// Delay to wait "notepad" focus
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	{false,false,0}, // No key (= SEQUENCE_PERIOD delay)
	// Display "Adjustable keyboard, coming soon!"
	{true,true,HID_MODIFIER_RIGHT_SHIFT}, // Enable Maj
	{false,true,HID_A},
	{false,false,HID_A},
	{true,false,HID_MODIFIER_RIGHT_SHIFT}, // Disable Maj
	{false,true,HID_D},
	{false,false,HID_D},
	{false,true,HID_J},
	{false,false,HID_J},
	{false,true,HID_U},
	{false,false,HID_U},
	{false,true,HID_S},
	{false,false,HID_S},
	{false,true,HID_T},
	{false,false,HID_T},
	{false,true,HID_A},
	{false,false,HID_A},
	{false,true,HID_B},
	{false,false,HID_B},
	{false,true,HID_L},
	{false,false,HID_L},
	{false,true,HID_E},
	{false,false,HID_E},
	{false,true,HID_SPACEBAR},
	{false,false,HID_SPACEBAR},
	{false,true,HID_K},
	{false,false,HID_K},
	{false,true,HID_E},
	{false,false,HID_E},
	{false,true,HID_Y},
	{false,false,HID_Y},
	{false,true,HID_B},
	{false,false,HID_B},
	{false,true,HID_O},
	{false,false,HID_O},
	{false,true,HID_A},
	{false,false,HID_A},
	{false,true,HID_R},
	{false,false,HID_R},
	{false,true,HID_D},
	{false,false,HID_D},
	{false,true,HID_COMMA},
	{false,false,HID_COMMA},
	{false,true,HID_SPACEBAR},
	{false,false,HID_SPACEBAR},
	{false,true,HID_C},
	{false,false,HID_C},
	{false,true,HID_O},
	{false,false,HID_O},
	{false,true,HID_M},
	{false,false,HID_M},
	{false,true,HID_I},
	{false,false,HID_I},
	{false,true,HID_N},
	{false,false,HID_N},
	{false,true,HID_G},
	{false,false,HID_G},
	{false,true,HID_SPACEBAR},
	{false,false,HID_SPACEBAR},
	{false,true,HID_S},
	{false,false,HID_S},
	{false,true,HID_O},
	{false,false,HID_O},
	{false,true,HID_O},
	{false,false,HID_O},
	{false,true,HID_N},
	{false,false,HID_N},
	{true,true,HID_MODIFIER_RIGHT_SHIFT}, // Enable Maj
	{false,true,HID_1},
	{false,false,HID_1},
	{true,false,HID_MODIFIER_RIGHT_SHIFT}, // Disable Maj
};

// Wakeup pin is PA15 (fast wakeup 14)
#define  WAKEUP_PMC_FSTT (PMC_FSMR_FSTT14)
#define  WAKEUP_PIN      (PIO_PA15_IDX)
#define  WAKEUP_PIO      (PIOA)
#define  WAKEUP_PIO_ID   (ID_PIOA)
#define  WAKEUP_PIO_MASK (PIO_PA15)
#define  WAKEUP_PIO_ATTR (PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_LOW_LEVEL)

// Key event FIFO
#define KEY_EVENT_FIFO_SIZE 512
fifo_desc_t key_event_fifo_desc;
uint8_t     key_event_buf[KEY_EVENT_FIFO_SIZE];

// Interrupt on "pin change" from PA15 to do wakeup on USB
// Note:
// This interrupt is enable when the USB host enable remotewakeup feature
// This interrupt wakeup the CPU if this one is in idle mode
static void ui_wakeup_handler(uint32_t id, uint32_t mask)
{
	if (WAKEUP_PIO_ID == id && WAKEUP_PIO_MASK == mask) {
		// It is a wakeup then send wakeup USB
		udc_remotewakeup();
	}
}

void ui_init(void)
{
	// Enable PIO clock for button inputs
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);
	// Set handler for PA15
	pio_handler_set(WAKEUP_PIO, WAKEUP_PIO_ID, WAKEUP_PIO_MASK, WAKEUP_PIO_ATTR, ui_wakeup_handler);
	// Enable IRQ for button (PIOA)
	NVIC_EnableIRQ((IRQn_Type) WAKEUP_PIO_ID);
	// Initialize LEDs
	LED_On(LED0_GPIO);
	LED_Off(LED1_GPIO);
	
	// Initialize the key even FIFO queue
	fifo_init(&key_event_fifo_desc, key_event_buf, KEY_EVENT_FIFO_SIZE);
	
	// Initialize Graphics Driver
	gfx_init();
	
	/* Clear the screen */
	gfx_draw_filled_rect(0, 0, gfx_get_width(), gfx_get_height(), GFX_COLOR_WHITE);
	itc_refresh_screen();
	
	// Initialize Key Icons
	for (int i = 0; i < KEY_COUNT; i++) {
		key_icon_array[i].icon = key_icon_init(key_icon_array[i].x, key_icon_array[i].y, KEY_ICON_MAX_DIM, KEY_ICON_MAX_DIM, KEY_ICON_MAX_DIM);
	}
	
	// Set Initial Key Icons
	for (int i = 0; i < KEY_COUNT; i++) {
		key_icon_set(key_icon_array[i].icon, &testText);
		key_icon_update(key_icon_array[i].icon);
	}
	itc_refresh_screen();
}

void ui_powerdown(void)
{
	LED_Off(LED0_GPIO);
	LED_Off(LED1_GPIO);
}

void ui_wakeup_enable(void)
{
	// Configure PA15 as PIO input
	pio_configure_pin(WAKEUP_PIN, WAKEUP_PIO_ATTR);
	// Enable interrupt for PA15
	pio_enable_pin_interrupt(WAKEUP_PIN);
	// Enable fast wakeup for button pin (WKUP14 for PA15)
	pmc_set_fast_startup_input(WAKEUP_PMC_FSTT);
}

void ui_wakeup_disable(void)
{
	// Disable interrupt for button pin
	pio_disable_pin_interrupt(WAKEUP_PIN);
	// Disable fast wakeup for button pin (WKUP14 for PA15)
	pmc_clr_fast_startup_input(WAKEUP_PMC_FSTT);
}

void ui_wakeup(void)
{
	LED_On(LED0_GPIO);
}

void ui_com_open(uint8_t port)
{
	UNUSED(port);
}

void ui_com_close(uint8_t port)
{
	UNUSED(port);
}

void ui_com_rx_start(void)
{
}

void ui_com_rx_stop(void)
{
}

void ui_com_tx_start(void)
{
}

void ui_com_tx_stop(void)
{
}

void ui_com_error(void)
{
}

void ui_com_overflow(void)
{
}

void ui_start_read(void)
{
	LED_On(LED1_GPIO);
}

void ui_stop_read(void)
{
	LED_Off(LED1_GPIO);
}

void ui_start_write(void)
{
	LED_On(LED1_GPIO);
}

void ui_stop_write(void)
{
	LED_Off(LED1_GPIO);
}

void ui_process(uint16_t framenumber)
{
	bool b_btn_state, sucess, success;	
	static bool btn_last_state = false;
	static bool sequence_running = false;
	static uint8_t u8_sequence_pos = 0;
	uint8_t u8_value;
	static uint16_t cpt_sof = 0;
	char key_result;
	
	if (framenumber % 100 == 0) {
		// Check for a key press
		keyboard_read(&key_event_fifo_desc);
		
		// If the key event queue has elements in it, send the first event.
		if (!fifo_is_empty(&key_event_fifo_desc)) {
			uint8_t direction;
			fifo_pull_uint8(&key_event_fifo_desc, &direction);
			uint8_t keycode;
			fifo_pull_uint8(&key_event_fifo_desc, &keycode);
			
			if (direction == key_event_up) {
				success = udi_hid_kbd_up(keycode);
			} else if (direction == key_event_down) {
				success = udi_hid_kbd_down(keycode);
			}
			
			// If it didn't work, re-add the event to the queue to try again
			if (!success) {
				fifo_push_uint8(&key_event_fifo_desc, direction);
				fifo_push_uint8(&key_event_fifo_desc, keycode);
				printf("Failed key event.");
			}
		}
	}

	if ((framenumber % 1000) == 0) {
		LED_On(LED0_GPIO);
	}
	if ((framenumber % 1000) == 500) {
		LED_Off(LED0_GPIO);
	}
	// Scan process running each 2ms
	cpt_sof++;
	if ((cpt_sof % 2) == 0) {
		return;
	}

	// Button down to send keys sequence
	b_btn_state = (!gpio_pin_is_high(GPIO_PUSH_BUTTON_1));
	if (b_btn_state != btn_last_state) {
		btn_last_state = b_btn_state;
		sequence_running = true;
	}

	// Sequence process running each period
	if (SEQUENCE_PERIOD > cpt_sof) {
		return;
	}
	cpt_sof = 0;

	if (sequence_running) {
		// Send next key
		u8_value = ui_sequence[u8_sequence_pos].u8_value;
		if (u8_value!=0) {
			if (ui_sequence[u8_sequence_pos].b_modifier) {
				if (ui_sequence[u8_sequence_pos].b_down) {
					sucess = udi_hid_kbd_modifier_down(u8_value);
				} else {
					sucess = udi_hid_kbd_modifier_up(u8_value);
				}
			} else {
				if (ui_sequence[u8_sequence_pos].b_down) {
					sucess = udi_hid_kbd_down(u8_value);
				} else {
					sucess = udi_hid_kbd_up(u8_value);
				}
			}
			if (!sucess) {
				return; // Retry it on next schedule
			}
		}
		// Valid sequence position
		u8_sequence_pos++;
		if (u8_sequence_pos >=
			sizeof(ui_sequence) / sizeof(ui_sequence[0])) {
			u8_sequence_pos = 0;
			sequence_running = false;
		}
	}
}

void ui_kbd_led(uint8_t value)
{
	UNUSED(value);
	/*
	if (value & HID_LED_NUM_LOCK) {
	} else {
	}
	if (value & HID_LED_CAPS_LOCK) {
	} else {
	}
	*/
}

/**
 * \defgroup UI User Interface
 *
 * Human interface on SAM4S-Xplained:
 * - LED D9 blinks when USB host has checked and enabled HID and MSC interface
 * - LED D10 is on during read/write operation
 * - No mouse buttons are linked
 * - SW1(BOARD REV A) or BP2(BOARD REV B) down opens a notepad application on Windows O.S.
 *   and sends key sequence "Atmel ARM"
 * - Only a low level on J13.PA15 will generate a wakeup to USB Host in remote wakeup mode.
 *
 */
