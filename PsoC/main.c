/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#define TRUE 1
#define FALSE 0
#define RAM_LENGTH 64
int flag1=TRUE;
int flag2=TRUE;
int src1Flag1=FALSE;
int src1Flag2=FALSE;
int src2Flag1=FALSE;
int src2Flag2=FALSE;
int timeFlag=FALSE;
/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define IN2_EP_NUM    (2u)
#define OUT_EP_NUM    (3u)

/* Size of SRAM buffer to store endpoint data. */
#define BUFFER_SIZE   (64u)

#define RD_BUFFER_SIZE        (2u)
#define WR_BUFFER_SIZE        (1u)

uint8 buffer[BUFFER_SIZE];

CY_ISR(end){
    if (flag1){    
        src1Flag1=TRUE; //set the secondary array low and primary high when DMA is done
        src1Flag2=FALSE;
        flag1=FALSE;
    }
    else{
        src1Flag2=TRUE;
        src1Flag1=FALSE;
        flag1=TRUE;
    }
}
CY_ISR(end1){
    if (flag2){
        src2Flag1=TRUE;
        src2Flag2=FALSE;
        flag2=FALSE;
    }
    else{
        src2Flag2=TRUE;
        src2Flag1=FALSE;
        flag2=TRUE;
    }
        
}
CY_ISR(time){
    timeFlag=TRUE;
    LCD_ClearDisplay();
    Timer_ReadStatusRegister();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_Start();
    ADC_StartConvert();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    
    isr_StartEx(end);
    isr_1_StartEx(end1);
    Onems_StartEx(time);
    
    LCD_Start();
    Timer_Start();
    AMux_Init();
    
    uint8 src1RAM1[BUFFER_SIZE];
    uint8 src1RAM2[BUFFER_SIZE];
    uint8 src2RAM1[BUFFER_SIZE];
    uint8 src2RAM2[BUFFER_SIZE];
    int i=0;
    int potFlag=TRUE;
    //clear the arrays with 0's
    for (i = 0; i < RAM_LENGTH; i++) {
        src1RAM1[i] = 0;
        src1RAM2[i] = 0;
        src2RAM1[i] = 0;
        src2RAM2[i] = 0;
    }

    /* Defines for DMA */
    #define DMA_BYTES_PER_BURST 1
    #define DMA_REQUEST_PER_BURST 1
    #define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for DMA */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_Chan;
    uint8 DMA_TD[2];

    /* DMA Configuration for DMA */
    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
        HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));

    DMA_TD[0] = CyDmaTdAllocate();
    DMA_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_TD[0], RAM_LENGTH, DMA_TD[1], DMA__TD_TERMOUT_EN  | CY_DMA_TD_INC_DST_ADR );
    CyDmaTdSetConfiguration(DMA_TD[1], RAM_LENGTH, DMA_TD[0], DMA__TD_TERMOUT_EN  | CY_DMA_TD_INC_DST_ADR );
    
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)ADC_SAR_SAR_WRK0_PTR), LO16((uint32)src1RAM1));
    CyDmaTdSetAddress(DMA_TD[1], LO16((uint32)ADC_SAR_SAR_WRK0_PTR), LO16((uint32)src1RAM2));
    
    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    CyDmaChEnable(DMA_Chan, 1);

    /* Defines for DMA_1 */
    #define DMA_1_BYTES_PER_BURST 1
    #define DMA_1_REQUEST_PER_BURST 1
    #define DMA_1_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for DMA_1 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[2];

    /* DMA Configuration for DMA_1 */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
    HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_1_TD[1] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_1_TD[0], RAM_LENGTH, DMA_1_TD[1], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_1_TD[1], RAM_LENGTH, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)src2RAM1));
    CyDmaTdSetAddress(DMA_1_TD[1], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)src2RAM2));
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    CyDmaChEnable(DMA_1_Chan, 1);

    
    /* Start USBFS operation with 5V operation. */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);

    /* Wait until device is enumerated by host. */
    while (0u == USBFS_GetConfiguration())
    {
    }
    
    /* Enable OUT endpoint to receive data from host. */
    USBFS_EnableOutEP(OUT_EP_NUM);
    uint8 read_buffer[RD_BUFFER_SIZE] = {0};
    uint8 write_buffer[WR_BUFFER_SIZE] = {0};
    uint8 readbyteCount = 0u;
    
    uint8 command_reg = 0; // Command register to capture writes from I2C

    // Set up slave write data buffer
    I2CS_SlaveInitWriteBuf((uint8 *) write_buffer, WR_BUFFER_SIZE);   
    // Set up slave read data buffer
    I2CS_SlaveInitReadBuf((uint8 *) read_buffer, RD_BUFFER_SIZE);
    // Start I2C slave component
    I2CS_Start();
    if(I2CS_SlaveStatus() & I2CS_SSTAT_WR_CMPLT)
    {
        // Copy command into command register
        command_reg = write_buffer[0]; //Pi to PSoC
        //divide the DMA clock to the specified sample rate
        Clock_2_SetDividerValue(command_reg);
        //LCD_Position(0u, 10u);
        //LCD_PrintNumber(command_reg);
        // Clear status
        I2CS_SlaveClearWriteStatus();
        I2CS_SlaveClearWriteBuf();
        
    } 
    for(;;)
    {
     /* Check if the command register has been written */
  
    
        /* Check if the slave buffer has been read */
     if(I2CS_SlaveStatus() & I2CS_SSTAT_RD_CMPLT)
        {
         readbyteCount = I2CS_SlaveGetReadBufSize();
         /* If both bytes of the read buffer have been read */
          if(readbyteCount == RD_BUFFER_SIZE) 
            {
             // Clear read status
              I2CS_SlaveClearReadStatus();
              I2CS_SlaveClearReadBuf();

              if (timeFlag){
                if (potFlag){
                    AMux_Select(0x0);//switch to pin0.3 to grab 
                    int adc1=ADC_GetResult16();
                    if (adc1<0) adc1=0; 
                    if (adc1>254) adc1=255;
                    read_buffer[0] = adc1;
                    potFlag=FALSE;
                }
                else{
                    AMux_Select(0x1);//switch to pin 0.4 to grab value
                    int adc2=ADC_GetResult16();
                    if (adc2<0) adc2=0; 
                    if (adc2>254) adc2=255;
                    read_buffer[1] = adc2;
                    potFlag=TRUE;
                }
                timeFlag=FALSE; //reset flag for next isr
              }

           }
        }
        //TRANSMISSION SIDE
        
        /* Trigger DMA to copy data into IN endpoint buffer.
        * After data has been copied, IN endpoint is ready to be read by the
        * host.
        */
        
        if (src1Flag1){ //fill in the desired array based on the ping pong buffer of DMA
            if (USBFS_IN_BUFFER_EMPTY == USBFS_GetEPState(IN_EP_NUM))
            {
                USBFS_LoadInEP(IN_EP_NUM, src1RAM1, RAM_LENGTH);
                src1Flag1=FALSE;
            }
        }
        else if (src1Flag2){
            if (USBFS_IN_BUFFER_EMPTY == USBFS_GetEPState(IN_EP_NUM))
            {
                USBFS_LoadInEP(IN_EP_NUM, src1RAM2, RAM_LENGTH);
                src1Flag2=FALSE;
            }
        }
        if (src2Flag1){
            if (USBFS_IN_BUFFER_EMPTY == USBFS_GetEPState(IN2_EP_NUM))
            {            
                USBFS_LoadInEP(IN2_EP_NUM, src2RAM1, RAM_LENGTH);
                src2Flag1=FALSE;
            }

        }
        else if (src2Flag2){
            if (USBFS_IN_BUFFER_EMPTY == USBFS_GetEPState(IN2_EP_NUM))
            {            
                USBFS_LoadInEP(IN2_EP_NUM, src2RAM2, RAM_LENGTH);
                src2Flag2=FALSE;
            }

        }
        
        //wait till the Pi recieves the byte
        
        /* Check if configuration is changed. */
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        LCD_Position(0u, 0u);
        //check the destination array make sure it emptied 
        LCD_PrintNumber(read_buffer[0]);
        
        LCD_Position(1u, 0u);
        //check the destination array make sure it emptied 
        LCD_PrintNumber(read_buffer[1]);
        

        /*
        LCD_Position(0u, 10u);
        //check the destination array make sure it emptied 
        LCD_PrintNumber(src1RAM[0]);
            
        //LCD_Position(1u, 10u);
        //check the source array make sure it emptied 
        LCD_PrintNumber(src2RAM[0]);
        */
        
    }
}


/* [] END OF FILE */
