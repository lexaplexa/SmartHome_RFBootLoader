/*
 * task_startupapp.cpp
 *
 * Created: 24.6.2018 10:37:08
 * Revised:
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

DATASET_RF sDataset;
DATASET_RF sDatasetEE EEMEM = {
    /* Default EEPROM Dataset Values ---------------------------------------*/
    125,                        /* RF channel number                        */
    0xAA,0x55,0xAA,0x55,0xAA,   /* Slave address                            */
    RF_BAUD_250kbps_gc,         /* RF Baud rate (250kbps, 1Mbps, 2Mbps)     */
    0x00000000                  /* CRC application sector checksum          */
};
RF cRf;
RF_DATA RFData;

void taskStartUpApp()
{
    /* Load Datasets from EEPROM */
    eeprom_read_block(sDataset.reg, sDatasetEE.reg, sizeof(DATASET_RF));
    
    /* RF initialization */
    cRf.Init();
    cRf.SetBaud((RF_BAUD_enum)sDataset.unRFBaud);
    cRf.SetChannel(sDataset.unRFChannel);
    cRf.SetTXaddress(sDataset.aTransmitAddress,5);
    cRf.SetRXaddress(sDataset.aTransmitAddress,5,0);
    cRf.WakeUp();
    
    /* Boot loader message after start */
    RFData.eRfCommand = RF_COMM_STATUS;
    cRf.m_eStatus = RF_STATUS_RECEIVE_OK;
    taskRf();
    
    /* Verify Application checksum */
    NVM.CMD = NVM_CMD_APP_CRC_gc;
    CCP = CCP_IOREG_gc;
    NVM.CTRLA = NVM_CMDEX_bm;
    uint32_t unCRC = (uint32_t)(NVM.DATA0)|(uint32_t)NVM.DATA1<<8|(uint32_t)NVM.DATA2<<16;
    if (sDataset.unAppChecksum == unCRC) {cMTask.Delay(taskRunApp,TASK_TOUT_MS(100));}
}