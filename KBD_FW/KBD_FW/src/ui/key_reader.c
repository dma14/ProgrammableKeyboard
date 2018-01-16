/*
 * key_reader.c
 *
 * Created: 1/4/2018 2:44:50 PM
 *  Author: David Ma
 */ 

#include <asf.h>
#include "key_reader.h"
#include "delay.h"

#define KEY_ROWS 3
#define KEY_COLS 3

static int wait_time = 1000;

// key_info is a structure that holds the current status and key code for each of the keys.
// Held in order in the array starting top left and proceeding right along the row, looping back at each new row.
struct {
	bool b_pressed;
	uint8_t key_code;
} key_info[KEY_ROWS * KEY_COLS] = {
	{false, HID_1},
	{false, HID_2},
	{false, HID_3},
	{false, HID_4},
	{false, HID_5},
	{false, HID_6},
	{false, HID_7},
	{false, HID_8},
	{false, HID_9}
};

static uint8_t row_io_pins[KEY_ROWS] = {
	GPIO_ROW_0,
	GPIO_ROW_1,
	GPIO_ROW_2
	};
	
static uint8_t col_io_pins[KEY_COLS] = {
	GPIO_COL_0,
	GPIO_COL_1,
	GPIO_COL_2
	};

void keyboard_read(fifo_desc_t *fifo)
{	
	// Write 1 to all cols
	for (int i = 0; i < KEY_COLS; i++) {
		gpio_set_pin_high(col_io_pins[i]);
	}
	
	// Poll all columns
	for (int col_idx = 0; col_idx < KEY_COLS; col_idx++) {
		// Write 0 to the polled column and wait
		gpio_set_pin_low(col_io_pins[col_idx]);
		delay_us(wait_time);
		
		// Check rows status: 1 = unpressed, 0 = pressed
		for (int row_idx = 0; row_idx < KEY_ROWS; row_idx++) {
			int key_status_idx = row_idx + col_idx * KEY_COLS;
			
			// Check if the switch status id different than the stored previous status
			bool row_status = gpio_pin_is_high(row_io_pins[row_idx]);
			if (row_status ^ key_info[key_status_idx].b_pressed) {
				// Add key event up/down and key code to key event queue.
				if (row_status) {
					fifo_push_uint8(fifo, key_event_up);
				} else {
					fifo_push_uint8(fifo, key_event_down);
				}
				fifo_push_uint8(fifo, key_info[key_status_idx].key_code);
				
				// Update the status of the key in key_info
				key_info[key_status_idx].b_pressed = !key_info[key_status_idx].b_pressed;
			}
			
			// Write 1 back to the polled column
			gpio_set_pin_high(col_io_pins[col_idx]);
		}
	}
}