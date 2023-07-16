#include "I2C.h"
#include <stdio.h>
#include <stdlib.h>
volatile unsigned int I2C_MasterState[I2C_PORT_NUM] = {I2C_IDLE,I2C_IDLE,I2C_IDLE};
volatile unsigned int I2C_Timeout[I2C_PORT_NUM] = {0, 0, 0};

volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
volatile unsigned int I2C_Count[I2C_PORT_NUM] = {0, 0, 0};
volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];

volatile unsigned int RdIndex0 = 0, RdIndex1 = 0, RdIndex2 = 0;
volatile unsigned int WrIndex0 = 0, WrIndex1 = 0, WrIndex2 = 0;

/*****************************************************************************
** Function name:		I2C Init
**
** Descriptions:		Initialize I2C controller as a master
**
** parameters:			Fast mode enable/disable
** Returned value:	None
** 
*****************************************************************************/

void I2C0_Init(BOOL fastMode) {
	LPC_SC->PCONP |= (0x1 << 7);

	/* set PIO0.27 and PIO0.28 to I2C0 SDA and SCL */
	/* function to 01 on both SDA and SCL. */
	
        LPC_IOCON->P0_28 = 0x001;
        LPC_IOCON->P0_27 = 0x001;
 
	/*--- Clear flags ---*/
	LPC_I2C0->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

	/*--- Reset registers ---*/
	if(fastMode)
	{
                //LPC_SC->
		//LPC_SC->PCONP |= ((0x1<<1)|(0x1<<2));
                //LPC_PINCON->I2CPADCFG |= ((0x1<<0)|(0x1<<2));
		LPC_I2C0->SCLL   = I2SCLL_SCLL;
		LPC_I2C0->SCLH   = I2SCLH_SCLH;
	}

	/* Install interrupt handler */
	NVIC_EnableIRQ(I2C0_IRQn);

	LPC_I2C0->CONSET = I2CONSET_I2EN;
}

void I2C1_Init(BOOL pinsSelect) {
    LPC_SC->PCONP |= (1 << 19);
    /* set PIO0.0 and PIO0.1 to I2C1 SDA and SCL */
    /* function to 11 on both SDA and SCL. */
    //LPC_PINCON->PINSEL0 &= ~((0x3<<0)|(0x3<<2));
    //LPC_PINCON->PINSEL0 |= ((0x3<<0)|(0x3<<2));
    //LPC_PINCON->PINMODE0 &= ~((0x3<<0)|(0x3<<2));
    //LPC_PINCON->PINMODE0 |= ((0x2<<0)|(0x2<<2));	/* No pull-up no pull-down */
    //LPC_PINCON->PINMODE_OD0 |= ((0x01<<0)|(0x1<<1));	/* Open drain */

    LPC_IOCON->P0_0 = 0x3;
    LPC_IOCON->P0_1 = 0x3;

 
    /*--- Clear flags ---*/
    LPC_I2C1->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

    /*--- Reset registers ---*/
    LPC_I2C1->SCLL   = I2SCLL_SCLL;
    LPC_I2C1->SCLH   = I2SCLH_SCLH;

    /* Install interrupt handler */
    NVIC_EnableIRQ(I2C1_IRQn);

    LPC_I2C1->CONSET = I2CONSET_I2EN;
}


unsigned int I2C_Start(char portNum) {
	unsigned int retVal = FALSE;
 
    I2C_Timeout[portNum] = 0;

          /*--- Issue a start condition ---*/
    LPC_I2C[portNum]->CONSET = I2CONSET_STA;	/* Set Start flag */
        
    /*--- Wait until START transmitted ---*/
    while(1)
    {
            if(I2C_MasterState[portNum] == I2C_STARTED)
            {
                    printf("success i2c0\n");
                    retVal = TRUE;
                    break;	
            }
            if(I2C_Timeout[portNum] >= I2C_MAX_TIMEOUT)
            {
                    printf("timeout i2c0\n");
                    retVal = FALSE;
                    break;
            }
            I2C_Timeout[portNum]++;
    }
      
    return(retVal);
}

/*****************************************************************************
** Function name:		I2C Stop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or never return
** 
*****************************************************************************/

unsigned int I2C_Stop(char portNum) {
	LPC_I2C[portNum]->CONSET = I2CONSET_STO;      /* Set Stop flag */ 
	LPC_I2C[portNum]->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
						
	/*--- Wait for STOP detected ---*/
	while(LPC_I2C[portNum]->CONSET & I2CONSET_STO){};

  return TRUE;
}

/*****************************************************************************
** Function name:		I2C Engine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermittenT
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			I2C port number
** Returned value:	master state of current I2C port.
** 
*****************************************************************************/

unsigned int I2C_Engine(char portNum) {
	if(portNum == 0) {
		RdIndex0 = 0;
		WrIndex0 = 0;
	} else if(portNum == 1) {
		RdIndex1 = 0;
		WrIndex1 = 0;
	} else {
		RdIndex2 = 0;
		WrIndex2 = 0;
	}

	/*--- Issue a start condition ---*/
  LPC_I2C[portNum]->CONSET = I2CONSET_STA;	/* Set Start flag */

  I2C_MasterState[portNum] = I2C_BUSY;	

  while(I2C_MasterState[portNum] == I2C_BUSY)
  {
      if(I2C_Timeout[portNum] >= I2C_MAX_TIMEOUT)
      {
              I2C_MasterState[portNum] = I2C_TIME_OUT;
              break;
      }
      I2C_Timeout[portNum]++;
  }
  LPC_I2C[portNum]->CONCLR = I2CONCLR_STAC;

  return (I2C_MasterState[portNum]);
}

/*****************************************************************************
** Function name:		I2C IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:	None
** 
*****************************************************************************/

extern void I2C0_IRQHandler(void) {
	unsigned char StatValue;

  I2C_Timeout[0] = 0;
	
  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C0->STAT;
	
  switch(StatValue){
    case 0x08:			/* A Start condition is issued. */
            //printf("start\n");
            WrIndex0 = 0;
            LPC_I2C0->DAT = I2C_MasterBuffer[0][WrIndex0++];
            LPC_I2C0->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
    break;

    case 0x10:			/* A repeated started is issued */
            //printf("repeated start\n");
            RdIndex0 = 0;
            /* Send SLA with R bit set, */
            LPC_I2C0->DAT = I2C_MasterBuffer[0][WrIndex0++];
            LPC_I2C0->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
    break;

    case 0x18:			/* Regardless, it's a ACK */
            if(I2C_WriteLength[0] == 1)
            {
                    LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
                    I2C_MasterState[0] = I2C_NO_DATA;
            }
            else
            {
                    LPC_I2C0->DAT = I2C_MasterBuffer[0][WrIndex0++];
            }
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
            //printf("data sent\n");
            if(WrIndex0 < I2C_WriteLength[0])
            {   
                    LPC_I2C0->DAT = I2C_MasterBuffer[0][WrIndex0++]; /* this should be the last one */
            } else {
                    if(I2C_ReadLength[0] != 0)
                    {
                            LPC_I2C0->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
                    } else {
                            LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
                            I2C_MasterState[0] = I2C_OK;
                    }
            }
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x30:
            LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
            I2C_MasterState[0] = I2C_NACK_ON_DATA;
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x40:	/* Master Receive, SLA_R has been sent */
            if((RdIndex0 + 1) < I2C_ReadLength[0])
            {
                    /* Will go to State 0x50 */
                    LPC_I2C0->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
            }
            else
            {
                    /* Will go to State 0x58 */
                    LPC_I2C0->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
            }
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
            //printf("data received 0x50\n");
            I2C_SlaveBuffer[0][RdIndex0++] = LPC_I2C0->DAT;
            if((RdIndex0 + 1) < I2C_ReadLength[0])
            {   
                    LPC_I2C0->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
            }
            else
            {
                    LPC_I2C0->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
            }
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x58:
            I2C_SlaveBuffer[0][RdIndex0++] = LPC_I2C0->DAT;
            I2C_MasterState[0] = I2C_OK;
            LPC_I2C0->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
            LPC_I2C0->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
    break;

    case 0x20:		/* regardless, it's a NACK */
    case 0x48:
            LPC_I2C0->CONSET = I2CONSET_STO;      /* Set Stop flag */
            I2C_MasterState[0] = I2C_NACK_ON_ADDRESS;
            LPC_I2C0->CONCLR = I2CONCLR_SIC;
    break;

    case 0x38:		/* Arbitration lost, in this example, we don't
                                    deal with multiple master situation */
    default:
            I2C_MasterState[0] = I2C_ARBITRATION_LOST;
            //printf("arb lost\n");
            LPC_I2C0->CONCLR = I2CONCLR_SIC;	
    break;
    }
}

extern void I2C1_IRQHandler(void) {
	unsigned char StatValue;

  I2C_Timeout[1] = 0;
	
  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C1->STAT;

  //printf("statvalue: %02X\n", StatValue);
	
  switch(StatValue){
    case 0x08:			/* A Start condition is issued. */
            //printf("start\n");
            WrIndex1 = 0;
            LPC_I2C1->DAT = I2C_MasterBuffer[1][WrIndex1++];
            LPC_I2C1->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
    break;

    case 0x10:			/* A repeated started is issued */
            //printf("repeated start\n");
            RdIndex1 = 0;
            /* Send SLA with R bit set, */
            LPC_I2C1->DAT = I2C_MasterBuffer[1][WrIndex1++];
            LPC_I2C1->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
    break;

    case 0x18:			/* Regardless, it's a ACK */
            //printf("0x18\n");
            if(I2C_WriteLength[1] == 1)
            {
                    LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
                    I2C_MasterState[1] = I2C_NO_DATA;
            }
            else
            {
                    LPC_I2C1->DAT = I2C_MasterBuffer[1][WrIndex1++];
            }
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
            //printf("0x28\n");
            if(WrIndex1 < I2C_WriteLength[1])
            {   
                    LPC_I2C1->DAT = I2C_MasterBuffer[1][WrIndex1++]; /* this should be the last one */
            } else {
                    if(I2C_ReadLength[1] != 0)
                    {
                            LPC_I2C1->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
                    } else {
                            LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
                            I2C_MasterState[1] = I2C_OK;
                    }
            }
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x30:
            //printf("nack 0x30\n");
            LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
            I2C_MasterState[1] = I2C_NACK_ON_DATA;
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x40:	/* Master Receive, SLA_R has been sent */
            //printf("master rec\n");
            if((RdIndex1 + 1) < I2C_ReadLength[1])
            {
                    /* Will go to State 0x50 */
                    LPC_I2C1->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
            }
            else
            {
                    //printf("got nack\n");
                    /* Will go to State 0x58 */
                    LPC_I2C1->CONCLR = I2CONCLR_AAC;	/* assert NACK after data is received */
            }
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
            //printf("data rec\n");
            I2C_SlaveBuffer[1][RdIndex1++] = LPC_I2C1->DAT;
            if((RdIndex1 + 1) < I2C_ReadLength[1])
            {   
                    LPC_I2C1->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
            }
            else
            {
                    LPC_I2C1->CONCLR = I2CONCLR_AAC;	/* assert NACK on last byte */
            }
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x58:
            //printf("data in\n");
            I2C_SlaveBuffer[1][RdIndex1++] = LPC_I2C1->DAT;
            I2C_MasterState[1] = I2C_OK;
            LPC_I2C1->CONSET = I2CONSET_STO;	/* Set Stop flag */ 
            LPC_I2C1->CONCLR = I2CONCLR_SIC;	/* Clear SI flag */
    break;

    case 0x20:		/* regardless, it's a NACK */
            //printf("nack 0x20\n");
    case 0x48:
            LPC_I2C1->CONSET = I2CONSET_STO;      /* Set Stop flag */
            I2C_MasterState[1] = I2C_NACK_ON_ADDRESS;
            LPC_I2C1->CONCLR = I2CONCLR_SIC;
    break;

    case 0x38:		/* Arbitration lost, in this example, we don't
                                    deal with multiple master situation */
    default:
            I2C_MasterState[1] = I2C_ARBITRATION_LOST;
            LPC_I2C1->CONCLR = I2CONCLR_SIC;	
            //printf("arb1 lost\n");
    break;
    }
}




void i2c_scan_devices(void){
    
    I2C_ReadLength[1] = 2;
    I2C_WriteLength[1] = 2;
    uint8_t address = 0;
    for (address = 0; address<255; address++){
        I2C_MasterBuffer[0][0] = address;
        I2C_MasterBuffer[0][1] = 0x04;
        I2C_MasterBuffer[0][2] = (0x51<<1) | 0x01;
        I2C_Engine(0);
        delay_short();
    }
    //for (uint32_t i = 0; i < 0x200000; i++ );	/* Delay after write */
    //for (uint32_t i = 0; i < I2C_BUFSIZE; i++ ){
    //    I2C_SlaveBuffer[1][i] = 0x00;
    //}
    //printf("%d\t%d\n\t\t\t%d\n", *I2C_MasterBuffer[1], *I2C_SlaveBuffer[1], (*ptr)++);
}