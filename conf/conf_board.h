/*************************************************************************
 * conf_board.h
 *
 * Created: 11.2.2016 13:01:57
 * Revised: 24.6.2018
 * Author: LeXa
 * BOARD: 
 *
 * ABOUT:
 * Configuration of board
 *************************************************************************/

#ifndef CONF_BOARD_H_
#define CONF_BOARD_H_

/************************************************************************/
/* BOARD                                                                */
/************************************************************************/
#define GENERIC_RF_BOARD				/* Board name and version */


/************************************************************************/
/* SYSTEM CLOCK OPTIONS                                                 */
/************************************************************************/
#define F_CPU						CONF_SOURCE_FREQ*CONF_FREQ_MULTIPLICATION
#define CONF_SOURCE_FREQ			32000000
#define CONF_FREQ_MULTIPLICATION	1

/* Define source for PLL if used. Uncomment the right one */
//#define CONF_PLL_SOURCE			OSC_PLLSRC_XOSC_gc
//#define CONF_PLL_SOURCE			OSC_PLLSRC_RC2M_gc
//#define CONF_PLL_SOURCE			OSC_PLLSRC_RC32M_gc


    /*===== RF MODULE nRF24L01+ ============================================
	 *======================================================================*/
	#define RF_PORT							PORTC
	#define RF_SPI							SPIC
	#define RF_BAUD_SPI						1000000
	#define RF_IRQ_vect						PORTC_INT0_vect
	#define RF_IRQ_PIN						PIN2_bp
	#define RF_CE_PIN						PIN3_bp
	#define ENABLE_AUTOACK					false
	#define DYNAMIC_PAYLOAD					true
	#define STATIC_PAYLOAD_BUFFER_LENGTH	32
	#define DATA_PIPE0_EN					1

#endif /* CONF_BOARD_H_ */