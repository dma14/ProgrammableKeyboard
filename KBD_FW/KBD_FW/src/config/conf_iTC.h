/*
 * conf_iTC.h
 *
 * Created: 2/6/2018 2:50:38 PM
 *  Author: David Ma
 */
#include <board.h>

#ifndef CONF_ITC_H_
#define CONF_ITC_H_


#define CONF_ITC_SPI SPI

#define CONF_ITC_CLOCK_SPEED 1000000UL


/** \brief Define what MCU pin the ILI9341 chip select pin is connected to */
#define CONF_ITC_CS_PIN        DISPLAY_CS

/** \brief Define what MCU pin the ILI9341 DC pin is connected to */
#define CONF_ITC_DC_PIN        DISPLAY_DC

/** \brief Define what MCU pin the ILI9341 reset is connected to */
#define CONF_ITC_RESET_PIN     DISPLAY_RST

// Define what MCU pin the itc busy pin is connected to
#define CONF_ITC_BUSY_PIN      DISPLAY_BUSY

#define CONF_ITC_PANEL_ON_PIN  DISPLAY_PANEL_ON

#define CONF_ITC_DISCHARGE_PIN DISPLAY_DISCHARGE



#endif /* CONF_ITC_H_ */