/*
 * key_icons.h
 *
 * Created: 3/4/2018 9:41:47 PM
 *  Author: David Ma
 */ 


#ifndef KEY_ICONS_H_
#define KEY_ICONS_H_

#include "gfx.h"

typedef struct key_icon{
	gfx_coord_t x;
	gfx_coord_t y;
	gfx_coord_t default_len;
	struct gfx_bitmap bmp;
	} key_icon_t;
	
	
key_icon_t* key_icon_init(gfx_coord_t x, gfx_coord_t y, gfx_coord_t width, gfx_coord_t height, gfx_coord_t default_len);

// Sets a bitmap to the given icon.
// Only works if bitmap type is GFX_BITMAP_PROGMEM.
// Does not put limits on the dimension of pixmap given.
void key_icon_set(key_icon_t* icon, struct gfx_bitmap* bmp_new);

// Updates the icon to the displays frame buffer, but does not refresh the screen.
// Argument is a pointer to the icon to be updated.
void key_icon_update(key_icon_t* icon);





#endif /* KEY_ICONS_H_ */