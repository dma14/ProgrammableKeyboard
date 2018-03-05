/*
 * gfx_itc.h
 *
 * Created: 2/11/2018 11:31:24 AM
 *  Author: David Ma
 */ 


#ifndef GFX_ITC_H_
#define GFX_ITC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iTC.h"

/**
 * \ingroup gfx_group
 * \defgroup gfx_itc using itc display controller
 *
 * This is hardware specific configuration that configures the graphical
 * service for use with the ITC display controller and the SDT028ATFT
 * display. All the drawing functionality in this configuration is provided
 * by the \ref gfx_generic driver. This configuration includes information
 * about the panel size, data type used for coordinates and color and
 * functionality to convert RGB color to the color format native to the display.
 *
 * @{
 */

typedef itc_color_t gfx_color_t;
typedef itc_coord_t gfx_coord_t;

#define GFX_COLOR(r, g, b)      ITC_COLOR(r, g, b)

/**
 * It is not possible to define a color that is outside the color spectrum for
 * the ITC driver, hence use a dark color as an invalid color.
 */
#define GFX_COLOR_INVALID       GFX_COLOR(5, 5, 5)

/**
 * It is not possible to define a color that is outside the color spectrum for
 * the ITC driver, hence use a very uncommon strong magenta color as
 * transparency mask color.
 */
#define GFX_COLOR_TRANSPARENT   GFX_COLOR(240, 0, 240)

/**
 * \brief Initialize the itc display controller
 *
 * This function will be called when calling \ref gfx_init.
 * It will draw a black background to the display and enable the
 * display backlight if available.
 */
void gfx_itc_init(void);

/**
 * \brief Generate native color value from R/G/B values.
 *
 * Converts 8-bit R/G/B values to a color value native to the display.
 *
 * The macro \ref GFX_COLOR does the same, only allows the preprocessor to
 * handle the calculations.
 *
 * \param r 8-bit red value.
 * \param g 8-bit green value.
 * \param b 8-bit blue value.
 *
 * \return Color value in display native format.
 */
gfx_color_t gfx_itc_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * \brief Get the color of a pixel on the display.
 *
 * \param x X coordinate of the pixel to be sampled.
 * \param y Y coordinate of the pixel to be sampled.
 *
 * \return Color value of the pixel at (x, y) in display native format,
 * or #GFX_COLOR_INVALID if outside the clipping region.
 */
gfx_color_t gfx_itc_get_pixel(gfx_coord_t x, gfx_coord_t y);

/**
 * \brief Draw a single pixel on the screen
 *
 * Draw a single pixel on screen setting top and bottom limits.
 * If outside the clipping region, nothing is drawn.
 *
 * \param x X coordinate of the pixel to be drawn.
 * \param y Y coordinate of the pixel to be drawn.
 * \param color Color value of the pixel in display native format.
 */
void gfx_itc_draw_pixel(gfx_coord_t x, gfx_coord_t y, gfx_color_t color);

/**
 * \brief Draw a single pixel on the screen
 *
 * This function draws a single pixel on screen only setting top limit
 * which makes it more suitable for line drawing.
 * If outside the clipping region, nothing is drawn.
 *
 * \param x X coordinate of the pixel to be drawn.
 * \param y Y coordinate of the pixel to be drawn.
 * \param color Color value of the pixel in display native format.
 */
void gfx_itc_draw_line_pixel(gfx_coord_t x, gfx_coord_t y,
		gfx_color_t color);

/**
 * \brief Set display orientation
 *
 * Set the display orientation
 *
 * \param flags
 */
void gfx_itc_set_orientation(uint8_t flags);

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_horizontal_line
 */
#define gfx_draw_horizontal_line(x, y, length, color)\
	gfx_generic_draw_horizontal_line(x, y, length, color)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_vertical_line
 */
#define gfx_draw_vertical_line(x, y, length, color)\
	gfx_generic_draw_vertical_line(x, y, length, color)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_line
 */
#define gfx_draw_line(x1, y1, x2, y2, color)\
	gfx_generic_draw_line(x1, y1, x2, y2, color)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_rect
 */
#define gfx_draw_rect(x, y, width, height, color)\
	gfx_generic_draw_rect(x, y, width, height, color)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_filled_rect
 */
#define gfx_draw_filled_rect(x, y, width, height, color)\
	gfx_generic_draw_filled_rect(x, y, width, height, color)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_circle
 */
#define gfx_draw_circle(x, y, radius, color, octant_mask)\
	gfx_generic_draw_circle(x, y, radius, color, octant_mask)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_filled_circle
 */
#define gfx_draw_filled_circle(x, y, radius, color, quadrant_mask)\
	gfx_generic_draw_filled_circle(x, y, radius, color,\
		quadrant_mask)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_get_pixmap
 */
#define gfx_get_pixmap(pixmap, map_width, map_x, map_y, x, y, width, height)\
	gfx_generic_get_pixmap(pixmap, map_width, map_x, map_y, x, y,\
		width, height)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_put_pixmap
 */
#define gfx_put_pixmap(pixmap, map_width, map_x, map_y, x, y, width, height)\
	gfx_generic_put_pixmap(pixmap, map_width, map_x, map_y, x, y,\
		width, height)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_put_bitmap
 */
#define gfx_put_bitmap(bmp, map_x, map_y, x, y, width, height)\
	gfx_generic_put_bitmap(bmp, map_x, map_y, x, y, width, height)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_bitmap_tiled
 */
#define gfx_draw_bitmap_tiled(bmp, x1, y1, x2, y2, tile_origin_x,\
			tile_origin_y)\
	gfx_generic_draw_bitmap_tiled(bmp, x1, y1, x2, y2,\
		tile_origin_x, tile_origin_y)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_draw_bitmap
 */
#define gfx_draw_bitmap(bmp, x, y) gfx_generic_draw_bitmap(bmp, x, y)

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_set_ext_handler
 */
#define gfx_set_ext_handler(ext_draw_handler)\
	gfx_generic_set_ext_handler(ext_draw_handler)

/**
 * ITC display driver specific function, see
 * \ref itc_duplicate_pixel
 */
#define gfx_duplicate_pixel(color, count) itc_duplicate_pixel(color, count)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_get_pixel
 */
#define gfx_get_pixel(x, y) gfx_itc_get_pixel(x, y)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_draw_pixel
 */
#define gfx_draw_pixel(x, y, color) gfx_itc_draw_pixel(x, y, color)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_draw_line_pixel
 */
#define gfx_draw_line_pixel(x, y, color) gfx_itc_draw_line_pixel(x, y,\
		color)

/**
 * ITC display driver specific function, see
 * \ref itc_set_top_left_limit
 */
#define gfx_set_top_left_limit(x, y) itc_set_top_left_limit(x, y)

/**
 * ITC display driver specific function, see
 * \ref itc_set_bottom_right_limit
 */
#define gfx_set_bottom_right_limit(x, y) itc_set_bottom_right_limit(x, y)

/**
 * ITC display driver specific function, see
 * \ref itc_set_limits
 */
#define gfx_set_limits(x1, y1, x2, y2) itc_set_limits(x1, y1, x2, y2)

/**
 * ITC display driver specific function, see
 * \ref itc_copy_pixels_to_screen
 */
#define gfx_copy_pixels_to_screen(pixels, count)\
	itc_copy_pixels_to_screen(pixels, count)

/**
 * ITC display driver specific function, see
 * \ref itc_copy_pixels_from_screen
 */
#define gfx_copy_pixels_from_screen(pixels, count)\
	itc_copy_pixels_from_screen(pixels, count)

/**
 * ITC display driver specific function available for ATmega and ATXmega
 * devices, see \ref itc_copy_progmem_pixels_to_screen
 */
#define gfx_copy_progmem_pixels_to_screen(pixels, count)\
	itc_copy_progmem_pixels_to_screen(pixels, count)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_color
 */
#define gfx_color(r, g, b) gfx_itc_color(r, g, b)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_set_orientation
 */
#define gfx_set_orientation(flags) gfx_itc_set_orientation(flags)

/**
 * ITC display driver specific function, see
 * \ref gfx_itc_init
 */
#define gfx_init() gfx_itc_init()

/**
 * ITC display driver uses generic gfx implementation for this function. See
 * \ref gfx_generic_sync
 */
#define gfx_sync() gfx_generic_sync()

/** @} */

#ifdef __cplusplus
}
#endif



#endif /* GFX_ITC_H_ */