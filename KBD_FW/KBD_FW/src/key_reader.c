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

char keyboard_read(void)
{
	// Write 1 to all cols
	gpio_set_pin_high(GPIO_COL_0);
	gpio_set_pin_high(GPIO_COL_1);
	gpio_set_pin_high(GPIO_COL_2);
	
	// Write a 0 to the first col
	gpio_set_pin_low(GPIO_COL_0);
	delay_us(wait_time);
	
	// Check rows for a 0
	if (gpio_pin_is_low(GPIO_ROW_0)) {return '1';}
	if (gpio_pin_is_low(GPIO_ROW_1)) {return '2';}
	if (gpio_pin_is_low(GPIO_ROW_2)) {return '3';}
		
	// Write a 0 to the second col
	gpio_set_pin_high(GPIO_COL_0);
	gpio_set_pin_low(GPIO_COL_1);
	delay_us(wait_time);
	
	// Check rows for a 0
	if (gpio_pin_is_low(GPIO_ROW_0)) {return '4';}
	if (gpio_pin_is_low(GPIO_ROW_1)) {return '5';}
	if (gpio_pin_is_low(GPIO_ROW_2)) {return '6';}
		
	// Write a 0 to the third col
	gpio_set_pin_high(GPIO_COL_1);
	gpio_set_pin_low(GPIO_COL_2);
	delay_us(wait_time);
	
	// Check rows for a 0
	if (gpio_pin_is_low(GPIO_ROW_0)) {return '7';}
	if (gpio_pin_is_low(GPIO_ROW_1)) {return '8';}
	if (gpio_pin_is_low(GPIO_ROW_2)) {return '9';}
		
	return '0';
}