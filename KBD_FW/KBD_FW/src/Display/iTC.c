/*
 * iTC.c
 *
 * Created: 2/8/2018 5:30:07 PM
 *  Author: David Ma
 */ 

#include "conf_iTC.h"
#include "iTC.h"
#include "iTC_regs.h"
#include <sysclk.h>
#include <ioport.h>
#include <delay.h>
#include <pdc.h>

#if defined(CONF_ITC_SPI) && 0 // Chip doesn't support DMA
#  define ITC_DMA_ENABLED
#  define ITC_DMA_CHUNK_SIZE   16
#endif

/**
 * \internal
 * \brief Helper function to select the CS of the controller on the bus
 */
__always_inline static void itc_select_chip(void)
{
	ioport_set_pin_level(CONF_ITC_CS_PIN, false);
}

/**
 * \internal
 * \brief Helper function to de-select the CS of the controller on the bus
 */
__always_inline static void itc_deselect_chip(void)
{
	ioport_set_pin_level(CONF_ITC_CS_PIN, true);
}

/**
 * \internal
 * \brief Helper function to select command byte transmission mode
 */
__always_inline static void itc_select_command_mode(void)
{
	ioport_set_pin_level(CONF_ITC_DC_PIN, false);
}

/**
 * \internal
 * \brief Helper function to select data byte transmission mode
 */
__always_inline static void itc_select_data_mode(void)
{
	ioport_set_pin_level(CONF_ITC_DC_PIN, true);
}

/**
 * \internal
 * \brief Helper function to wait for the last send operation to complete
 */
__always_inline static void itc_wait_for_send_done(void)
{
#if defined(CONF_ITC_SPI)
	/* Wait for TX to complete */
	while (!spi_is_tx_empty(CONF_ITC_SPI)) {
		/* Do nothing */
	}
#endif
}

/**
 * \internal
 * \brief Helper function to wait for the itc controller to be not busy
 */
__always_inline static void itc_wait_for_busy_done(void)
{
#if defined(CONF_ITC_SPI)
	/* Wait busy line to go high */
	while (!ioport_get_pin_level(CONF_ITC_BUSY_PIN)) {
		/* Do nothing */
	}
#endif
}

/**
 * \internal
 * \brief Helper function to send a byte over an arbitrary interface
 *
 * \param data The byte to be transfered
 */
__always_inline static void itc_send_byte(uint8_t data)
{
#if defined(CONF_ITC_SPI)
	/* Wait for any previously running send data */
	itc_wait_for_send_done();

	spi_write_single(CONF_ITC_SPI, data);
#endif
}

/**
 * \internal
 * \brief Helper function to read a byte from an arbitrary interface
 *
 * \retval uint8_t Byte of data read from the display controller
 */
__always_inline static uint8_t itc_read_byte(void)
{
	uint8_t data;

#if defined(CONF_ITC_SPI)
	spi_write_single(CONF_ITC_SPI, 0xFF);

	itc_wait_for_send_done();

	/* Wait for RX to complete */
	while (!spi_is_rx_full(CONF_ITC_SPI)) {
		/* Do nothing */
	}

	spi_read_single(CONF_ITC_SPI, &data);
#endif

	return data;
}

/**
 * \internal
 * \brief Sends a command to the controller, and prepares for parameter transfer
 *
 * Helper function to use for sending a command to the controller.
 *
 * \note After the command is sent, the display remains selected.
 *
 * \param command The command to send
 */
static void itc_send_command(uint8_t command, bool send_data)
{
	delay_us(1);
	itc_select_command_mode();
	delay_us(1);
	itc_select_chip();
	itc_send_byte(command);
	itc_wait_for_send_done();
	itc_deselect_chip();
	if (send_data) {
		delay_us(1);
		itc_select_data_mode();
		delay_us(1);
		itc_select_chip();
	}
}

static itc_coord_t limit_start_x, limit_start_y;
static itc_coord_t limit_end_x, limit_end_y;
uint8_t image_data_buffer[ITC_SCREEN_BUFFER_SIZE] = {0}; // Initialize all to 0
	
/**
 * Helper function that returns the index in the image buffer 
 */
__always_inline static int itc_image_buffer_idx(itc_coord_t x, itc_coord_t y)
{
	return (int) ((ITC_DEFAULT_WIDTH / 8) * y + x / 8);
}
	
/**
 * Helper function to access the bit at a given coordinate (starting top-left) of the image buffer.
 * Returns a uint8_t that is 1 if the bit was high and 0 if not.
 */
static uint8_t itc_get_image_bit(itc_coord_t x, itc_coord_t y)
{
	uint8_t data_byte = (uint8_t) image_data_buffer[itc_image_buffer_idx(x, y)];
	uint8_t bit_idx = 7 - (x % 8); // Position of the bit we want to get in the byte.
	uint8_t data_mask = (uint8_t) (1 << bit_idx);
	return (uint8_t) (data_byte & data_mask) >> bit_idx;
}

/**
 * Helper function to write the bit at a given coordinate in the image buffer.
 * Does not send the bit to the screen! Need to call itc_refresh_screen() for that.
 */
static void itc_set_image_bit(itc_coord_t x, itc_coord_t y, itc_color_t color)
{
	int buf_idx = itc_image_buffer_idx(x, y);
	uint8_t data_byte = (uint8_t) image_data_buffer[buf_idx];
	uint8_t bit_idx = 7 - (x % 8); // Position of the bit we want to set in the byte.
	
	uint8_t data_mask;
	if (color > ITC_THRES_BLACK) {
		// For white color, we clear the bit
		data_mask = ~((uint8_t) 1 << bit_idx);
		image_data_buffer[buf_idx] = data_mask & data_byte;
	}
	else {
		// For black color (classified as anything but white) we set the bit
		data_mask = (uint8_t) 1 <<  bit_idx;
		image_data_buffer[buf_idx] = data_mask | data_byte;
	}
	
	return;
}

/**
 * \internal
 * \brief Helper function to send the drawing limits (boundaries) to the display
 *
 * This function is used to send the currently set upper-left and lower-right
 * drawing limits to the display, as set through the various limit functions.
 *
 * \param send_end_limits  True to also send the lower-right drawing limits
 */
static void itc_send_draw_limits(const bool send_end_limits)
{
	return; // No limits are sent to the controller.
}

/**
 * \brief Set the display top left drawing limit
 *
 * Use this function to set the top left limit of the drawing limit box.
 *
 * \param x The x coordinate of the top left corner
 * \param y The y coordinate of the top left corner
 */
void itc_set_top_left_limit(itc_coord_t x, itc_coord_t y)
{
	limit_start_x = x;
	limit_start_y = y;

	itc_send_draw_limits(false);
}

/**
 * \brief Set the display bottom right drawing limit
 *
 * Use this function to set the bottom right corner of the drawing limit box.
 *
 * \param x The x coordinate of the bottom right corner
 * \param y The y coordinate of the bottom right corner
 */
void itc_set_bottom_right_limit(itc_coord_t x, itc_coord_t y)
{
	limit_end_x = x;
	limit_end_y = y;

	itc_send_draw_limits(true);
}

/**
 * \brief Set the full display drawing limits
 *
 * Use this function to set the full drawing limit box.
 *
 * \param start_x The x coordinate of the top left corner
 * \param start_y The y coordinate of the top left corner
 * \param end_x The x coordinate of the bottom right corner
 * \param end_y The y coordinate of the bottom right corner
 */
void itc_set_limits(itc_coord_t start_x, itc_coord_t start_y,
		itc_coord_t end_x, itc_coord_t end_y)
{
	limit_start_x = start_x;
	limit_start_y = start_y;
	limit_end_x = end_x;
	limit_end_y = end_y;

	itc_send_draw_limits(true);
}

/**
 * \brief Read a single color from the graphical memory
 *
 * Use this function to read a color from the graphical memory of the
 * controller.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	itc_set_top_left_limit(0, 0);
	itc_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \retval itc_color_t The read color pixel
 */
itc_color_t itc_read_gram(void)
{
	if (itc_get_image_bit(limit_start_x, limit_start_y)) {
		return ITC_BLACK;
	}
	else {
		return ITC_WHITE;
	}
}

/**
 * \brief Write the graphical memory with a single color pixel
 *
 * Use this function to write a single color pixel to the controller memory.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	itc_set_top_left_limit(0, 0);
	itc_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \param color The color pixel to write to the screen
 */
void itc_write_gram(itc_color_t color)
{
	itc_set_image_bit(limit_start_x, limit_start_y, color);
}

/**
 * \brief Copy pixels from SRAM to the screen
 *
 * Used to copy a large quantitative of data to the screen in one go.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	itc_set_top_left_limit(0, 0);
	itc_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \param pixels Pointer to the pixel data
 * \param count Number of pixels to copy to the screen
 */
void itc_copy_pixels_to_screen(const itc_color_t *pixels, uint32_t count)
{
	const itc_color_t *pixel = pixels;

	/* Sanity check to make sure that the pixel count is not zero */
	Assert(count > 0);
	
	// Loop through the pixels to write
	itc_coord_t x = limit_start_x;
	itc_coord_t y = limit_start_y;
	for ( ; count > 0; count--)
	{
		// Write the current bit and increment the pixel pointer
		itc_set_image_bit(x, y, *pixel);
		pixel++;
		
		// Increment to the coordinate for the next pixel.
		if (x < limit_end_x) {
			x++;
		} else {
			x = limit_start_x;
			y++;
		}
	}
}

/**
 * \brief Set a given number of pixels to the same color
 *
 * Use this function to write a certain number of pixels to the same color
 * within a set limit.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	itc_set_top_left_limit(0, 0);
	itc_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \param color The color to write to the display
 * \param count The number of pixels to write with this color
 */
void itc_duplicate_pixel(const itc_color_t color, uint32_t count)
{
	/* Sanity check to make sure that the pixel count is not zero */
	Assert(count > 0);
	
	// Loop through the pixels to write
	itc_coord_t x = limit_start_x;
	itc_coord_t y = limit_start_y;
	for ( ; count > 0; count--)
	{
		// Write the current bit and increment the pixel pointer
		itc_set_image_bit(x, y, color);
		
		// Increment to the coordinate for the next pixel.
		if (x < limit_end_x) {
			x++;
		} else {
			x = limit_start_x;
			y++;
		}
	}
	return;
}

/**
 * \brief Copy pixels from the screen to a pixel buffer
 *
 * Use this function to copy pixels from the display to an internal SRAM buffer.
 *
 * Limits have to be set prior to calling this function, e.g.:
 * \code
	itc_set_top_left_limit(0, 0);
	itc_set_bottom_right_limit(320, 240);
	...
\endcode
 *
 * \param pixels Pointer to the pixel buffer to read to
 * \param count Number of pixels to read
 */
void itc_copy_pixels_from_screen(itc_color_t *pixels, uint32_t count)
{
	/* Sanity check to make sure that the pixel count is not zero */
	Assert(count > 0);
	
	// Loop through the pixels to read
	itc_coord_t x = limit_start_x;
	itc_coord_t y = limit_start_y;
	for ( ; count > 0; count--)
	{
		// Write the current bit and increment the pixel pointer
		*pixels = itc_get_image_bit(x, y);
		pixels++;
		
		// Increment to the coordinate for the next pixel.
		if (x < limit_end_x) {
			x++;
			} else {
			y++;
		}
	}
}

/**
 * \internal
 * \brief Initialize the hardware interface to the controller
 *
 * This will initialize the module used for communication with the controller.
 * Currently supported interfaces by this component driver are the SPI
 * interface through either the SPI module in master mode or the USART in
 * Master SPI mode.  Configuration must be done in the associated
 * conf_itc.h file.
 */
static void itc_interface_init(void)
{
#if defined(CONF_ITC_SPI)
	spi_flags_t spi_flags = SPI_MODE_0;
	board_spi_select_id_t spi_select_id = 0;
#else
	#error Interface for ITC has not been selected or interface not\
	supported, please configure component driver using the conf_itc.h\
	file!
#endif

#if defined(CONF_ITC_SPI)
	struct spi_device device = {
		.id = 0,
	};

	spi_master_init(CONF_ITC_SPI);
	spi_master_setup_device(CONF_ITC_SPI, &device, spi_flags,
			CONF_ITC_CLOCK_SPEED, spi_select_id);
	spi_enable(CONF_ITC_SPI);

	/* Send one dummy byte for the spi_is_tx_ok() to work as expected */
	spi_write_single(CONF_ITC_SPI, 0);
#endif
}

/**
 * \internal
 * \brief Initialize all the display registers
 *
 * This function will set up all the internal registers according the the
 * manufacturer's description.
 */
static void itc_controller_init_registers(void)
{
	itc_send_command(ITC_CMD_INPUT_TEMP, true);
	itc_send_byte(0x19); // Expect 25 degrees celsius
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_ACTIVE_TEMP, true);
	itc_send_byte(0x02);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_PANEL_SETTINGS, true);
	itc_send_byte(0x0F);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_SS_SETTINGS, true);
	itc_send_byte(0x17);
	itc_send_byte(0x17);
	itc_send_byte(0x27);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_RES_SETTINGS, true);
	itc_send_byte(0x01);
	itc_send_byte(0x90);
	itc_send_byte(0x01);
	itc_send_byte(0x2C);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_VCOM_DATA_INTERVAL, true);
	itc_send_byte(0x87);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_send_command(ITC_CMD_POWER_SAVING, true);
	itc_send_byte(0x88);
	itc_wait_for_send_done();
	itc_deselect_chip();

	itc_set_orientation(0);
	itc_set_limits(0, 0, ITC_DEFAULT_WIDTH,
			ITC_DEFAULT_HEIGHT);
}

/**
 * \internal
 * \brief Reset the display using the digital control interface
 *
 * Controls the reset pin of the display controller to reset the display.
 */
static void itc_reset_display(void)
{
	ioport_set_pin_level(CONF_ITC_DISCHARGE_PIN, true);
	delay_ms(5);
	ioport_set_pin_level(CONF_ITC_DISCHARGE_PIN, false);
	ioport_set_pin_level(CONF_ITC_PANEL_ON_PIN, false);
	ioport_set_pin_level(CONF_ITC_RESET_PIN, false);
	ioport_set_pin_level(CONF_ITC_DC_PIN, false);
	delay_ms(10);
	ioport_set_pin_level(CONF_ITC_PANEL_ON_PIN, true);
	delay_ms(5);
	ioport_set_pin_level(CONF_ITC_RESET_PIN, true);
	delay_ms(1);
	itc_deselect_chip();
}

static void itc_power_off(void) {
	itc_send_command(ITC_CMD_DC_TOGGLE, false);
	itc_wait_for_send_done();
	itc_deselect_chip();
	
	itc_wait_for_busy_done();
	
	ioport_set_pin_level(CONF_ITC_PANEL_ON_PIN, false);
	ioport_set_pin_level(CONF_ITC_RESET_PIN, false);
	ioport_set_pin_level(CONF_ITC_DC_PIN, false);
	ioport_set_pin_level(CONF_ITC_DISCHARGE_PIN, true);
	itc_select_chip();
	
}

/**
 * \brief Initialize the controller
 *
 * Used to initialize the ITC display controller by setting up the hardware
 * interface, and setting up the controller according to the manufacturer's
 * description. It also set up the screen orientation to the default state
 * (portrait).
 */
void itc_init(void)
{
	/* Initialize the communication interface */
	itc_interface_init();
}
/** 
 * Sens an update to the display screen, pushing any changes made since the last refresh.
 */
void itc_refresh_screen(void)
{

	/* Reset the display */
	itc_reset_display();

	/* Write all the controller registers with correct values */
	itc_controller_init_registers();
	
	
	itc_wait_for_busy_done();
	// Send the actual data to the display controllers frame data register
	itc_send_command(ITC_CMD_BLACK_FRAME_DATA, true);
	// Send each byte from our buffer
	for (int i = 0; i < ITC_SCREEN_BUFFER_SIZE; i++)
	{
		itc_send_byte((uint8_t) image_data_buffer[i]);
	}
	itc_wait_for_send_done();
	itc_deselect_chip();
	
	// Send the red frame (all 0s)
	itc_send_command(ITC_CMD_RED_FRAME_DATA, true);
	// Send each byte from our buffer
	for (int i = 0; i < ITC_SCREEN_BUFFER_SIZE; i++)
	{
		itc_send_byte((uint8_t) 0);
	}
	itc_wait_for_send_done();
	itc_deselect_chip();
	
	// Process for sending an update command
	itc_wait_for_busy_done();
	itc_send_command(ITC_CMD_POWER_ON, false);
	itc_wait_for_send_done();
	itc_deselect_chip();
	
	itc_wait_for_busy_done();
	itc_send_command(ITC_CMD_REFRESH, false);
	itc_wait_for_send_done();
	itc_deselect_chip();
	
	itc_wait_for_busy_done();
	itc_power_off();
	
	return;
}

/**
 * \brief Sets the orientation of the display data
 *
 * Configures the display for a given orientation, including mirroring and/or
 * screen rotation.
 *
 * \param flags Orientation flags to use, see \ref ITC_FLIP_X, \ref ITC_FLIP_Y
 *        and \ref ITC_SWITCH_XY.
 */
void itc_set_orientation(uint8_t flags)
{
	// TODO: Implement orientation flipping.
	return;
}

