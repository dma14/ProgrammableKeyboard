/*
 * key_icons.c
 *
 * Created: 3/4/2018 7:15:10 PM
 *  Author: qis4q
 */ 

#include <asf.h>
#include <string.h>
#include "key_icons.h"

key_icon_t* key_icon_init(gfx_coord_t x, gfx_coord_t y, gfx_coord_t width, gfx_coord_t height, gfx_coord_t default_len) {
	key_icon_t *icon = malloc(sizeof(key_icon_t));
	icon->x = x;
	icon->y = y;
	icon->default_len = default_len;
	(icon->bmp).width = width;
	(icon->bmp).height = height;
	(icon->bmp).type = GFX_BITMAP_PROGMEM;
	(icon->bmp).data.progmem = malloc(sizeof(gfx_color_t) * width * height);
	
	return icon;
}
	
void key_icon_set(key_icon_t* icon, struct gfx_bitmap* bmp_new) {
	if (bmp_new->type == GFX_BITMAP_PROGMEM) {
		(icon->bmp).width = bmp_new->width;
		(icon->bmp).height = bmp_new->height;
		(icon->bmp).type = GFX_BITMAP_PROGMEM;
		memcpy((icon->bmp).data.progmem, bmp_new->data.progmem, sizeof(gfx_color_t) * (bmp_new->width) * (bmp_new->height));
	}
}

void key_icon_update(key_icon_t* icon) {
	// Adjust the coordinates of the top left corner based on the new dimensions compared to the default, to centre the image.
	gfx_coord_t adjusted_x = ((icon->default_len) - ((icon->bmp).width))/2 + (icon->x);
	gfx_coord_t adjusted_y = ((icon->default_len) - ((icon->bmp).height))/2 + (icon->y);;
	
	gfx_draw_bitmap(&(icon->bmp), adjusted_x, adjusted_y);
}