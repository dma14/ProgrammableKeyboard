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

// Runs the key polling algorithm and returns the character for the key.
char keyboard_read(void);

#endif /* KEY_READER_H_ */