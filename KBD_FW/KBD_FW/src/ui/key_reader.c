/*
 * key_reader.c
 *
 * Created: 1/4/2018 2:44:50 PM
 *  Author: David Ma
 */ 

#include <asf.h>
#include "key_reader.h"
#include "delay.h"

static int wait_time = 1000;

// key_info is a structure that holds the current status and key code for each of the keys.
// Held in order in the array starting top left and proceeding right along the row, looping back at each new row.

static uint8_t row_io_pins[KEY_ROW_NUM] = {
	GPIO_ROW_0,
	GPIO_ROW_1,
	GPIO_ROW_2,
	GPIO_ROW_3
	};
	
static uint8_t col_io_pins[KEY_COL_NUM] = {
	GPIO_COL_0,
	GPIO_COL_1,
	GPIO_COL_2
	};

void keyboard_read(fifo_desc_t *fifo, key_info_t key_arr[KEY_ROW_NUM][KEY_COL_NUM])
{	
	// Write 1 to all cols
	for (int i = 0; i < KEY_COL_NUM; i++) {
		gpio_set_pin_high(col_io_pins[i]);
	}
	
	// Poll all columns
	for (int col_idx = 0; col_idx < KEY_COL_NUM; col_idx++) {
		// Write 0 to the polled column and wait
		gpio_set_pin_low(col_io_pins[col_idx]);
		delay_us(wait_time);
		
		// Check rows status: 1 = unpressed, 0 = pressed
		for (int row_idx = 0; row_idx < KEY_ROW_NUM; row_idx++) {
			int key_status_idx = ROW_COL_TO_IDX(row_idx, col_idx);
			
			// Check if the switch status id different than the stored previous status
			bool row_status = gpio_pin_is_high(row_io_pins[row_idx]);
			if (row_status ^ key_arr[row_idx][col_idx].pressed) {
				// Add key event up/down and key code to key event queue.
				if (row_status) {
					fifo_push_uint8(fifo, key_event_up);
				} else {
					fifo_push_uint8(fifo, key_event_down);
				}
				fifo_push_uint8(fifo, key_status_idx);
				
				// Update the status of the key in key_info
				key_arr[row_idx][col_idx].pressed = !key_arr[row_idx][col_idx].pressed;
			}
			
			// Write 1 back to the polled column
			gpio_set_pin_high(col_io_pins[col_idx]);
		}
	}
}