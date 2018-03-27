/*
 * comm.c
 *
 * Created: 3/18/2018 9:03:59 PM
 *  Author: qis4q
 */ 

#include <asf.h>
#include "comm.h"
#include "xmodem.h"
#include "gfx.h"

uint8_t file_read_buf[FILE_READ_BUFFER_SIZE];

/* File Format:
	1-2:	Start of key identifier
	3:		Key location Id
	4:		Scancode
	5:		Icon Width
	6:		Icon Height
	7-n:	Bmp Bytestream
	n:		Next key, etc.
*/

void comm_init() {
	file_read_buf[0] = 0x00;
}

void comm_process() {
	uint32_t read_result = xmodem_receive_file(file_read_buf);
	
	if (read_result == 0) {
		return;
	}
	else {
		bool file_handling_finished = false;
		uint32_t idx = 0;
		while (!file_handling_finished) {
			// Read the first two bytes of the section, and make sure it's the start identifier.
			uint8_t identifier_upper = file_read_buf[idx++];
			uint8_t identifier_lower = file_read_buf[idx++];
			int identifier = (((int) identifier_upper) << 8) | ((int) identifier_lower);
			if (identifier != FILE_SECTION_IDENTIFIER) {
				// If it's not mathcing the expected identifier, return with an error;
				return;
			}
			
			// Read physical code, scancode, width, height
			uint8_t key_id = file_read_buf[idx++];
			uint8_t scancode = file_read_buf[idx++];
			uint8_t icon_width = file_read_buf[idx++];
			uint8_t icon_height = file_read_buf[idx++];
			
			// Set the scancode for the key
			ui_set_key_scancode(key_id, scancode);
			
			// Create a bitmap using the byte stream
			struct gfx_bitmap bmp = {.width = icon_width, 
										.height = icon_height, 
										.type = GFX_BITMAP_RAM, 
										.data.pixmap = (gfx_color_t *) &(file_read_buf[idx])};
										
			ui_set_key_icon(key_id, &bmp);
			idx += BYTES_PER_PIXEL*icon_height*icon_width;
			
			// Set the flag for the ui to update the screen
			ui_set_needs_refresh();
			
			// Check if we've read all the data from the file
			if((idx + 1) >= read_result) {
				file_handling_finished = true;
			}
		}
	}
}
