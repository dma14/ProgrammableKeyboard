/*
 * key_reader.h
 *
 * Created: 1/4/2018 2:47:18 PM
 *  Author: David Ma
 */ 


#ifndef KEY_READER_H_
#define KEY_READER_H_

#include "board.h"
#include "ASF/common/services/gpio/gpio.h"
#include "fifo.h"
#include "ui.h"

static uint8_t key_event_up = 1;
static uint8_t key_event_down = 0;

// Runs the key polling algorithm and returns the character for the key.
void keyboard_read(fifo_desc_t *fifo, key_info_t key_arr[KEY_ROW_NUM][KEY_COL_NUM]);

#endif /* KEY_READER_H_ */