/*
 * gfx_itc.c
 *
 * Created: 2/11/2018 11:31:48 AM
 *  Author: David Ma
 */ 

#include "gfx.h"
#include "gfx_itc.h"
#include "iTC.h"
#include "iTC_regs.h"

gfx_coord_t gfx_height, gfx_width;
gfx_coord_t gfx_min_x, gfx_min_y;
gfx_coord_t gfx_max_x, gfx_max_y;

void gfx_itc_set_orientation(uint8_t flags)
{
	itc_set_orientation(flags);

	/* Switch width and height if XY is switched. */
	if ((flags & GFX_SWITCH_XY) != 0x00) {
		gfx_width = ITC_SWITCH_XY_WIDTH;
		gfx_height = ITC_SWITCH_XY_HEIGHT;
		} else {
		gfx_width = ITC_DEFAULT_WIDTH;
		gfx_height = ITC_DEFAULT_HEIGHT;
	}

	#ifdef CONF_GFX_USE_CLIPPING
	/* Reset clipping region. */
	gfx_set_clipping(0, 0, gfx_width - 1, gfx_height - 1);
	#endif
}

gfx_color_t gfx_itc_color(uint8_t r, uint8_t g, uint8_t b)
{
	return GFX_COLOR(r, g, b);
}

gfx_color_t gfx_itc_get_pixel(gfx_coord_t x, gfx_coord_t y)
{
	gfx_color_t color;

	#ifdef CONF_GFX_USE_CLIPPING
	if ((x < gfx_min_x) || (x > gfx_max_x) ||
	(y < gfx_min_y) || (y > gfx_max_y)) {
		return GFX_COLOR_INVALID;
	}
	#endif

	/* Set up draw area and read the three bytes of pixel data. */
	gfx_set_limits(x, y, x, y);
	color = itc_read_gram();

	return color;
}

void gfx_itc_draw_pixel(gfx_coord_t x, gfx_coord_t y, gfx_color_t color)
{
	#ifdef CONF_GFX_USE_CLIPPING
	if ((x < gfx_min_x) || (x > gfx_max_x) ||
	(y < gfx_min_y) || (y > gfx_max_y)) {
		return;
	}
	#endif

	/* Set up draw area and write the two bytes of pixel data. */
	gfx_set_limits(x, y, x, y);
	itc_write_gram(color);
}

void gfx_itc_draw_line_pixel(gfx_coord_t x, gfx_coord_t y,
gfx_color_t color)
{
	#ifdef CONF_GFX_USE_CLIPPING
	if ((x < gfx_min_x) || (x > gfx_max_x) ||
	(y < gfx_min_y) || (y > gfx_max_y)) {
		return;
	}
	#endif

	/* Set up top left corner of area and write the two bytes of */
	/* pixel data.  Bottom left corner is already set to max_x/y. */
	gfx_set_top_left_limit(x, y);
	itc_write_gram(color);
}

void gfx_itc_init(void)
{
	/* initialize globals */
	gfx_width = ITC_DEFAULT_WIDTH;
	gfx_height = ITC_DEFAULT_HEIGHT;

	itc_init();

	/* Set clipping area to whole screen initially */
	gfx_set_clipping(0, 0, gfx_width, gfx_height);

	gfx_draw_filled_rect(0, 0, gfx_width, gfx_height,
	GFX_COLOR_WHITE);
}