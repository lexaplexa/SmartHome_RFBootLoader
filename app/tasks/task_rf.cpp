/*************************************************************************
 * task_rf.cpp
 *
 * Created: 9.8.2016 11:40:07
 * Revised: 24.6.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

uint16_t unFlashPageAddress;
uint16_t unFlashPageCnt;
uint8_t aFlashBuffer[FLASH_PAGE_SIZE + RF_PAYLOAD_MAX_BYTES];

void taskRf()
{
    uint8_t unRfMsgSize;
    
    if (cRf.m_eStatus != RF_STATUS_RECEIVE_OK) {return;}
        
    /* Copy received message */
    memcpy(RFData.reg, cRf.m_sPayload.punBuffer, cRf.m_sPayload.unCounter);
    
    /* Any received message will stop timeout to run application program */
    cMTask.Stop(taskRunApp);
    
    switch(RFData.eRfCommand)
    {
        case RF_COMM_RESET:
            CCP = CCP_IOREG_gc;
            RST.CTRL = RST_SWRST_bm;
            break;
        
        
        case RF_COMM_STATUS:
            RFData.Slave.sStatusBoot.eDeviceType = DEVICE_TYPE_BOOT_LOADER;
            RFData.Slave.sStatusBoot.unFlashPageSize = FLASH_PAGE_SIZE;
            unRfMsgSize = 1+sizeof(RFData.Slave.sStatusBoot);
            break;
        
        
        case RF_COMM_BOOT_ERASE_APP:
            nvm_flash_erase_app();
            nvm_wait_until_ready();
            RFData.Slave.eError = ERROR_OK;
            unRfMsgSize = 2;
            break;
            
        
        case RF_COMM_BOOT_PAGE_ADDRESS:
            if (RFData.Master.unFlashAddress > (FLASH_SIZE - FLASH_PAGE_SIZE)) 
            {
                RFData.eRfCommand = RF_COMM_ERROR;
                RFData.Slave.eError = ERROR_OUT_OF_RANGE;
            }
            else
            {
                RFData.Slave.eError = ERROR_OK;
                unFlashPageAddress = RFData.Master.unFlashAddress;
                unFlashPageCnt = 0;
            }           
            unRfMsgSize = 2;
            break;
            
            
        case RF_COMM_BOOT_LOAD_PAGE:
            memcpy(&aFlashBuffer[unFlashPageCnt],&cRf.m_sPayload.punBuffer[1],cRf.m_sPayload.unCounter-1);
            unFlashPageCnt += cRf.m_sPayload.unCounter-1;
            RFData.Slave.eError = ERROR_NEXT;
            if (unFlashPageCnt >= FLASH_PAGE_SIZE)
            {
                nvm_flash_erase_and_write_buffer(unFlashPageAddress,aFlashBuffer,FLASH_PAGE_SIZE,true);                
                unFlashPageCnt = 0;
                RFData.Slave.eError = ERROR_OK;
            }
            unRfMsgSize = 2;
            break;
            
        
        case RF_COMM_BOOT_GET_DATASET:
            memcpy(RFData.Slave.GetDataset.reg, sDataset.reg, sizeof(DATASET_RF));
            unRfMsgSize = 1+sizeof(DATASET_RF);
            break;  
            
            
        case RF_COMM_BOOT_SET_DATASET:
            if (cRf.m_sPayload.unCounter == (sizeof(DATASET_RF)+1))
            {
                /* Copy data to Dataset */
                memcpy(sDataset.reg, RFData.Slave.GetDataset.reg, sizeof(DATASET_RF));
                /* Copy Dataset to EEPROM */
                eeprom_write_block(&RFData.Slave.GetDataset.reg, &sDatasetEE.reg, sizeof(DATASET_RF));
                RFData.Slave.eError = ERROR_OK;
            }
            else
            {
                RFData.eRfCommand = RF_COMM_ERROR;
                RFData.Slave.eError = ERROR_OUT_OF_RANGE;
            }
            unRfMsgSize = 2;
            break;
        
        
        case RF_COMM_BOOT_VERIFY_APP:
            uint32_t unCRC;
            NVM.CMD = NVM_CMD_APP_CRC_gc;
            CCP = CCP_IOREG_gc;
            NVM.CTRLA = NVM_CMDEX_bm;
            unCRC = (uint32_t)(NVM.DATA0)|(uint32_t)NVM.DATA1<<8|(uint32_t)NVM.DATA2<<16;
            if (unCRC == sDataset.unAppChecksum) {RFData.Slave.eError = ERROR_OK;}
            else {RFData.eRfCommand = RF_COMM_ERROR, RFData.Slave.eError = ERROR_VERIFICATION_FAILED;}
            unRfMsgSize = 2;
            break;  
            
            
        default:
            RFData.eRfCommand = RF_COMM_ERROR;
            RFData.Slave.eError = ERROR_UNKNOWN_COMMAND;
            unRfMsgSize = 2;
            break;        
    }
    cRf.SendMsg(RFData.reg,unRfMsgSize);
}


SIGNAL(RF_IRQ_vect)
{
    cRf.InterruptHandler();
    if (cRf.m_eBaud == RF_BAUD_250kbps_gc)      {cMTask.Delay(taskRf,   TASK_TOUT_MS(10));}
    else if (cRf.m_eBaud == RF_BAUD_1Mbps_gc)   {cMTask.Delay(taskRf,   TASK_TOUT_MS(5));}
    else                                        {cMTask.Delay(taskRf,   TASK_TOUT_MS(1));}
}