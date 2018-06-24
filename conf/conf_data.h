/*************************************************************************
 * conf_data.h
 *
 * Created: 24.6.2018 10:53:11
 * Revised:
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#ifndef CONF_DATA_H_
#define CONF_DATA_H_

/************************************************************************/
/* ENUMs                                                                */
/************************************************************************/
enum DEVICE_TYPE_enum {
    DEVICE_TYPE_BOOT_LOADER,
    DEVICE_TYPE_MASTER_CONTROLLER,
    DEVICE_TYPE_LED_CONTROLLER,
    DEVICE_TYPE_METEO_STATION,
};

enum ERROR_enum {
    ERROR_OK,
    ERROR_OUT_OF_RANGE,
    ERROR_OVERVOLTAGE,
    ERROR_UNDERVOLTAGE,
    ERROR_UNKNOWN_COMMAND,
    ERROR_NEXT,
    ERROR_VERIFICATION_FAILED,
};

/************************************************************************/
/* DATASET                                                              */
/************************************************************************/
union DATASET_RF {
    uint8_t reg[16];
    struct {
        uint8_t					unRFChannel;				/* RF channel number													*/
        uint8_t					aTransmitAddress[5];		/* Slave address														*/
        uint8_t					unRFBaud;					/* RF Baud rate (250kbps, 1Mbps, 2Mbps)									*/
        uint32_t				unAppChecksum;				/* Application sector checksum											*/
        uint8_t					aReserved[5];				/* Reserved bytes for future use										*/
    };
};

/************************************************************************/
/* COMMANDS                                                             */
/************************************************************************/
enum RF_COMMAND_enum {
    /* Basic commands */
    RF_COMM_RESET,
    RF_COMM_STATUS,
    RF_COMM_BOOT_GET_DATASET,
    RF_COMM_BOOT_SET_DATASET,
    RF_COMM_BOOT_ERASE_APP,
    RF_COMM_BOOT_VERIFY_APP,    
    RF_COMM_BOOT_PAGE_ADDRESS,
    RF_COMM_BOOT_LOAD_PAGE,
    
    /* Error answer to command. Error code is followed */
    RF_COMM_ERROR = 255,
};

/************************************************************************/
/* RF COMUNICATION STRUCTURE                                            */
/************************************************************************/
union RF_DATA {
    uint8_t reg[32];
    struct 
    {
        /*===== COMMAND =========================================================
         * Command is always in first place
         *=======================================================================*/
        RF_COMMAND_enum eRfCommand;
        
        union 
        {
            /*===== MASTER DATA =====================================================
             * Data send by Master controller
             *=======================================================================*/
            union 
            {
                DATASET_RF				SetDataset;
                uint16_t				unFlashAddress;
            } Master;
            
            /*===== SLAVE DATA ======================================================
             * Data send by Slave controller
             *=======================================================================*/
            union 
            {
                ERROR_enum				    eError;
                struct STATUS_BOOT_struct   {DEVICE_TYPE_enum eDeviceType; uint16_t unFlashPageSize;}sStatusBoot;
                DATASET_RF				    GetDataset;
            } Slave;
        };
    };
};

#endif /* CONF_DATA_H_ */