/*
 * comm.c
 *
 * Created: 3/18/2018 9:03:59 PM
 *  Author: qis4q
 */ 

#include <asf.h>
#include "comm.h"
#include "xmodem.h"

uint8_t file_read_buf[FILE_READ_BUFFER_SIZE];

void comm_init() {
	file_read_buf[0] = 0x00;
}

void comm_process() {
	uint32_t read_result = xmodem_receive_file(file_read_buf);
	
	if (read_result == 0) {
		return;
	}
	else {
		while(1);
	}
}
