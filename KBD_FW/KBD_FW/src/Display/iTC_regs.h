/*
 * iTC_regs.h
 *
 * Created: 2/8/2018 4:08:15 PM
 *  Author: qis4q
 */ 


#ifndef ITC_REGS_H_
#define ITC_REGS_H_

/* Register definitions from data sheet */
/* Sizes in Bytes */
# define ITC_CMD_PANEL_SETTINGS				0x00
# define ITC_PANEL_SETTINGS_SIZE			0x01

# define ITC_CMD_DC_TOGGLE					0x02
# define ITC_DC_TOGGLE_SIZE					0x00

# define ITC_CMD_POWER_ON					0x04
# define ITC_POWER_ON_SIZE					0x00

# define ITC_CMD_SS_SETTINGS				0x06
# define ITC_SS_SETTINGS_SIZE				0x03

# define ITC_CMD_BLACK_FRAME_DATA			0x10
# define ITC_BLACK_FRAME_DATA_SIZE			15000

# define ITC_CMD_RED_FRAME_DATA				0x13
# define ITC_RED_FRAME_DATA_SIZE			15000

# define ITC_CMD_REFRESH					0x12
# define ITC_REFRESH_SIZE					0x00

# define ITC_CMD_RED_FRAME_DATA				0x13
# define ITC_RED_FRAME_DATA_SIZE			15000

# define ITC_CMD_VCOM_DATA_INTERVAL			0x50
# define ITC_VCOM_DATA_INTERVAL_SIZE		0x01

# define ITC_CMD_RES_SETTINGS				0x61
# define ITC_RES_SETTINGS_SIZE				0x04

# define ITC_CMD_ACTIVE_TEMP				0xe0
# define ITC_ACTIVE_TEMP_SIZE				0x01

# define ITC_CMD_POWER_SAVING				0xe3
# define ITC_POWER_SAVING_SIZE				0x01

# define ITC_CMD_INPUT_TEMP					0xe5
# define ITC_INPUT_TEMP_SIZE				0x01

#endif /* ITC_REGS_H_ */