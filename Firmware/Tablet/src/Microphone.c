#include "Microphone.h"
#include "LPC177x_8x.h"			/* LPC17xx Peripheral Registers */
#include "arm_math_types.h"
#include "FFT.h"
#include "LCD.h"
#include <stdio.h>


volatile uint32_t I2SReadLength = 0;
volatile uint32_t I2SReadLength2 = 0;
volatile uint32_t readings_skipped = 0;
volatile uint8_t I2SRXDone = 0;
volatile int32_t I2SRXBuffer[MEASUREMENTS_TO_TAKE];// = (uint8_t *)(DMA_DST);
volatile uint8_t mic_fft_run_flag = 0;
float32_t fft_buffer[TEST_LENGTH_SAMPLES];
//volatile int32_t I2SRXBuffer[1];// = (uint8_t *)(DMA_DST);

void mic_init(void){
    for (uint32_t i=0; i<MEASUREMENTS_TO_TAKE; i++){
        I2SRXBuffer[i] = 0;
    }
    mic_pwr_on();
    NVIC_EnableIRQ(I2S_IRQn);
}


void mic_pwr_on(void){
    delay_long();
    LPC_IOCON->P5_4 = 0x00;
    LPC_IOCON->P4_21 = 0x00;
    LPC_GPIO5->DIR |= (0x1<<4);
    LPC_GPIO4->DIR |= (0x1<<21);
    LPC_GPIO5->SET |= (0x1<<4);
    LPC_GPIO4->CLR |= (0x1<<21);
}


void mic_pwr_off(void){
    LPC_IOCON->P4_5 = 0x00;
    LPC_IOCON->P4_21 = 0x00;
    LPC_GPIO4->DIR |= (0x1<<5);
    LPC_GPIO4->DIR |= (0x1<<21);
    LPC_GPIO4->CLR |= (0x1<<5);
    LPC_GPIO4->SET |= (0x1<<21);
}

void mic_take_measurements(void){
    I2S_Init();
    I2S_Config(0x01);

    LPC_IOCON->P0_23 = 0x82;
    LPC_GPIO0->DIR |= (0x1 << 23);
    LPC_IOCON->P0_24 = 0x82;
    LPC_GPIO0->DIR |= (0x1 << 24);
    LPC_IOCON->P0_25 = 0x8A;
    LPC_GPIO0->DIR |= (0x0 << 25);
    I2SRXDone = 0;
    
    //printf("Recording on microphone\n");
    I2S_Start();
    delay(100);
    //delay_long();
    //delay_long();
    //delay_long();

    I2S_IRQConfig(0x01, 0x2);
    I2S_IRQCmd(0x01, 1);

    while(!I2SRXDone);

    I2S_Stop();
    
    //printf("Recording finished\n");
    uint16_t temp = 0;
    for (uint32_t i=0; i<MEASUREMENTS_TO_TAKE; i++){

        //temp = I2SRXBuffer[i];
        if (I2SRXBuffer[i] > (131072)){
            //printf("%d,\n ", I2SRXBuffer[i]-262144);
            fft_buffer[i*2] = (((float)I2SRXBuffer[i]-262144)/65536);
        } else {
            //printf("%d,\n", I2SRXBuffer[i]);
            fft_buffer[i*2] = (((float)I2SRXBuffer[i])/65536);
        }
    }
    fft_test();   
    I2SReadLength = 0;
}


void I2S_IRQHandler (void){
    uint32_t RxCount = 0;
    uint32_t dummy_reading = 0;
    
    NVIC->ISER[0] &= ~(0x1 << 27);
    if ( LPC_I2S->STATE & 0x01 ){
        if (readings_skipped < MEASUREMENTS_TO_SKIP){
            I2SRXBuffer[I2SReadLength] = LPC_I2S->RXFIFO>>14;
            readings_skipped++;
        } else if (I2SReadLength < MEASUREMENTS_TO_TAKE){
            I2SRXBuffer[I2SReadLength] = LPC_I2S->RXFIFO>>14;
            if (I2SRXBuffer[I2SReadLength]){
                I2SReadLength++;
            }
        } else {
            LPC_I2S->DAI |= ((0x01 << 3) | (0x01 << 4) | 0x3 | (0x1 << 2)) ;
            LPC_I2S->IRQ &= ~(0x01 << 0);	/* Disable RX */	
            I2SRXDone = 1;
        }
    }
    NVIC->ISER[0] |= (0x1 << 27);
    return;
}


static uint8_t i2s_GetWordWidth(  uint8_t TRMode);
static uint8_t i2s_GetChannel(  uint8_t TRMode);

/********************************************************************//**
 * @brief		Get I2S wordwidth value
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is the I2S mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		The wordwidth value, should be: 8,16 or 32
 *********************************************************************/
static uint8_t i2s_GetWordWidth(  uint8_t TRMode) {
	uint8_t value;

	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		value = (LPC_I2S->DAO) & 0x03; /* get wordwidth bit */
	} else {
		value = (LPC_I2S->DAI) & 0x03; /* get wordwidth bit */
	}
	switch (value) {
	case I2S_WORDWIDTH_8:
		return 8;
	case I2S_WORDWIDTH_16:
		return 16;
	default:
		return 32;
	}
}

/********************************************************************//**
 * @brief		Get I2S channel value
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is the I2S mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		The channel value, should be: 1(mono) or 2(stereo)
 *********************************************************************/
static uint8_t i2s_GetChannel(  uint8_t TRMode) {
	uint8_t value;

	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		value = ((LPC_I2S->DAO) & 0x04)>>2; /* get bit[2] */
	} else {
		value = ((LPC_I2S->DAI) & 0x04)>>2; /* get bit[2] */
	}
        if(value == I2S_MONO) return 1;
          return 2;
}

/* End of Private Functions --------------------------------------------------- */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup I2S_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Initialize I2S
 * 					- Turn on power and clock
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_Init(void) {
	////CHECK_PARAM(PARAM_I2Sx(I2Sx));

	// Turn on power and clock
	//CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2S, ENABLE);
        LPC_SC->PCLKSEL = 0x01;
        LPC_SC->PCONP |= (0x1 << 27);
	LPC_I2S->DAI = LPC_I2S->DAO = 0x00;
        LPC_I2S->RXRATE = 0x0101;
}

/********************************************************************//**
 * @brief		Configuration I2S, setting:
 * 					- master/slave mode
 * 					- wordwidth value
 * 					- channel mode
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode transmit/receive mode, should be:
 * 					- I2S_TX_MODE = 0: transmit mode
 * 					- I2S_RX_MODE = 1: receive mode
 * @param[in]	ConfigStruct pointer to I2S_CFG_Type structure
 *              which will be initialized.
 * @return 		none
 *********************************************************************/
void I2S_Config( uint8_t TRMode)
{
	uint32_t bps, config;

	//CHECK_PARAM(PARAM_I2Sx(I2Sx));

	//CHECK_PARAM(PARAM_I2S_WORDWIDTH(ConfigStruct->wordwidth));
	//CHECK_PARAM(PARAM_I2S_CHANNEL(ConfigStruct->mono));
	//CHECK_PARAM(PARAM_I2S_STOP(ConfigStruct->stop));
	//CHECK_PARAM(PARAM_I2S_RESET(ConfigStruct->reset));
	//CHECK_PARAM(PARAM_I2S_WS_SEL(ConfigStruct->ws_sel));
	//CHECK_PARAM(PARAM_I2S_MUTE(ConfigStruct->mute));

	/* Setup clock */
	//bps = (2+1)*8;
	bps = (3+1)*8;

	/* Calculate audio config */
	config = (bps-1)<<6 | (0x0)<<5 | (0x1)<<4 | (0x1)<<3 | (0x1)<<2 | (0x3);

	if(TRMode == I2S_RX_MODE){
		LPC_I2S->DAI = config;
                LPC_I2S->RXMODE |= (0x1)<<3;
	}else{
		LPC_I2S->DAO = config;
	}
}

/********************************************************************//**
 * @brief		DeInitial both I2S transmit or receive
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_DeInit(void) {
	////CHECK_PARAM(PARAM_I2Sx(I2Sx));

	// Turn off power and clock
	//CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2S, DISABLE);
        LPC_SC->PCONP &= ~(0x1 << 27);
}

/********************************************************************//**
 * @brief		Get I2S Buffer Level
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode Transmit/receive mode, should be:
 * 					- I2S_TX_MODE = 0: transmit mode
 * 					- I2S_RX_MODE = 1: receive mode
 * @return 		current level of Transmit/Receive Buffer
 *********************************************************************/
uint8_t I2S_GetLevel(  uint8_t TRMode)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if(TRMode == I2S_TX_MODE)
	{
		return ((LPC_I2S->STATE >> 16) & 0xFF);
	}
	else
	{
		return ((LPC_I2S->STATE >> 8) & 0xFF);
	}
}

/********************************************************************//**
 * @brief		I2S Start: clear all STOP,RESET and MUTE bit, ready to operate
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_Start(void)
{
	//Clear STOP,RESET and MUTE bit
	LPC_I2S->DAO &= ~I2S_DAI_RESET;
	LPC_I2S->DAI &= ~I2S_DAI_RESET;
	LPC_I2S->DAO &= ~I2S_DAI_STOP;
	LPC_I2S->DAI &= ~I2S_DAI_STOP;
	LPC_I2S->DAO &= ~I2S_DAI_MUTE;
        LPC_I2S->RXBITRATE = 0x1F;
}

/********************************************************************//**
 * @brief		I2S Send data
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	BufferData pointer to uint32_t is the data will be send
 * @return 		none
 *********************************************************************/
void I2S_Send(  uint32_t BufferData) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));

	LPC_I2S->TXFIFO = BufferData;
}

/********************************************************************//**
 * @brief		I2S Receive Data
 * @param[in]	I2Sx pointer to LPC_I2S_TypeDef
 * @return 		received value
 *********************************************************************/
uint32_t I2S_Receive(void) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));

	return (LPC_I2S->RXFIFO);

}

/********************************************************************//**
 * @brief		I2S Pause
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Pause(  uint8_t TRMode) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) //Transmit mode
	{
		LPC_I2S->DAO |= I2S_DAO_STOP;
	} else //Receive mode
	{
		LPC_I2S->DAI |= I2S_DAI_STOP;
	}
}

/********************************************************************//**
 * @brief		I2S Mute
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Mute(  uint8_t TRMode) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) //Transmit mode
	{
		LPC_I2S->DAO |= I2S_DAO_MUTE;
	} else //Receive mode
	{
		LPC_I2S->DAI |= I2S_DAI_MUTE;
	}
}

/********************************************************************//**
 * @brief		I2S Stop
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Stop(void) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

        LPC_I2S->DAI |= I2S_DAI_STOP;
        LPC_I2S->DAI |= I2S_DAI_RESET;
        LPC_I2S->RXRATE = 0x0;
        I2S_DeInit();
	
}

/********************************************************************//**
 * @brief		Set frequency for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	Freq is the frequency for I2S will be set. It can range
 * 				from 16-96 kHz(16, 22.05, 32, 44.1, 48, 96kHz)
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		Status: ERROR or SUCCESS
 *********************************************************************/
ITStatus I2S_FreqConfig(  uint32_t Freq, uint8_t TRMode) {

	uint32_t i2sMclk;
	uint8_t bitrate, channel, wordwidth;

	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PRAM_I2S_FREQ(Freq));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	//set i2s reference is i2s_pclk/2 as default
	i2sMclk = 0x8;
	LPC_I2S->TXRATE = 1  | (1<<8);
	LPC_I2S->RXRATE = 1  | (1<<8);
	if(TRMode == I2S_TX_MODE)
	{
		channel = i2s_GetChannel(I2S_TX_MODE);
		wordwidth = i2s_GetWordWidth(I2S_TX_MODE);
	}
	else
	{
		channel = i2s_GetChannel(I2S_RX_MODE);
		wordwidth = i2s_GetWordWidth(I2S_RX_MODE);
	}
	bitrate = i2sMclk/(Freq * channel * wordwidth) - 1;
	if (TRMode == I2S_TX_MODE)// Transmitter
	{
		LPC_I2S->TXBITRATE = bitrate;
	} else //Receiver
	{
		LPC_I2S->RXBITRATE = bitrate;
	}
	return 1;
}

/********************************************************************//**
 * @brief		I2S set bitrate
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	bitrate value will be set
 * 				bitrate value should be in range: 0 .. 63
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_SetBitRate(  uint8_t bitrate, uint8_t TRMode)
{
    LPC_I2S->RXBITRATE = bitrate;	
}

/********************************************************************//**
 * @brief		Configuration operating mode for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	ModeConfig pointer to I2S_MODEConf_Type will be used to
 * 				configure
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_ModeConfig(  I2S_MODEConf_Type* ModeConfig,
		uint8_t TRMode)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_CLKSEL(ModeConfig->clksel));
	//CHECK_PARAM(PARAM_I2S_4PIN(ModeConfig->fpin));
	//CHECK_PARAM(PARAM_I2S_MCLK(ModeConfig->mcena));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		LPC_I2S->TXMODE &= ~0x0F; //clear bit 3:0 in I2STXMODE register
		if (ModeConfig->clksel == I2S_CLKSEL_MCLK) {
			LPC_I2S->TXMODE |= 0x02;
		}
		if (ModeConfig->fpin == I2S_4PIN_ENABLE) {
			LPC_I2S->TXMODE |= (1 << 2);
		}
		if (ModeConfig->mcena == I2S_MCLK_ENABLE) {
			LPC_I2S->TXMODE |= (1 << 3);
		}
	} else {
		LPC_I2S->RXMODE &= ~0x0F; //clear bit 3:0 in I2SRXMODE register
		if (ModeConfig->clksel == I2S_CLKSEL_MCLK) {
			LPC_I2S->RXMODE |= 0x02;
		}
		if (ModeConfig->fpin == I2S_4PIN_ENABLE) {
			LPC_I2S->RXMODE |= (1 << 2);
		}
		if (ModeConfig->mcena == I2S_MCLK_ENABLE) {
			LPC_I2S->RXMODE |= (1 << 3);
		}
	}
}

/********************************************************************//**
 * @brief		Configure DMA operation for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	DMAConfig pointer to I2S_DMAConf_Type will be used to configure
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_DMAConfig(  I2S_DMAConf_Type* DMAConfig, uint8_t TRMode)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_I2S_DMA(DMAConfig->DMAIndex));
	//CHECK_PARAM(PARAM_I2S_DMA_DEPTH(DMAConfig->depth));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_RX_MODE) {
		if (DMAConfig->DMAIndex == I2S_DMA_1) {
			LPC_I2S->DMA1 = (DMAConfig->depth) << 8;
		} else {
			LPC_I2S->DMA2 = (DMAConfig->depth) << 8;
		}
	} else {
		if (DMAConfig->DMAIndex == I2S_DMA_1) {
			LPC_I2S->DMA1 = (DMAConfig->depth) << 16;
		} else {
			LPC_I2S->DMA2 = (DMAConfig->depth) << 16;
		}
	}
}

/********************************************************************//**
 * @brief		Enable/Disable DMA operation for I2S
 * @param[in]	I2Sx: I2S peripheral selected, should be: LPC_I2S
 * @param[in]	DMAIndex chose what DMA is used, should be:
 * 				- I2S_DMA_1 = 0: DMA1
 * 				- I2S_DMA_2 = 1: DMA2
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	NewState is new state of DMA operation, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void I2S_DMACmd(  uint8_t DMAIndex, uint8_t TRMode,
		FunctionalState NewState)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
	//CHECK_PARAM(PARAM_I2S_DMA(DMAIndex));
	//CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_RX_MODE) {
		if (DMAIndex == I2S_DMA_1) {
			if (NewState == ENABLE)
				LPC_I2S->DMA1 |= 0x01;
			else
				LPC_I2S->DMA1 &= ~0x01;
		} else {
			if (NewState == ENABLE)
				LPC_I2S->DMA2 |= 0x01;
			else
				LPC_I2S->DMA2 &= ~0x01;
		}
	} else {
		if (DMAIndex == I2S_DMA_1) {
			if (NewState == ENABLE)
				LPC_I2S->DMA1 |= 0x02;
			else
				LPC_I2S->DMA1 &= ~0x02;
		} else {
			if (NewState == ENABLE)
				LPC_I2S->DMA2 |= 0x02;
			else
				LPC_I2S->DMA2 &= ~0x02;
		}
	}
}

/********************************************************************//**
 * @brief		Configure IRQ for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	level is the FIFO level that triggers IRQ request
 * @return 		none
 *********************************************************************/
void I2S_IRQConfig(  uint8_t TRMode, uint8_t level) {
    //CHECK_PARAM(PARAM_I2Sx(I2Sx));
    //CHECK_PARAM(PARAM_I2S_TRX(TRMode));
    //CHECK_PARAM(PARAM_I2S_IRQ_LEVEL(level));

    if (TRMode == I2S_RX_MODE) {
            LPC_I2S->IRQ |= ((level << 8) | (0x1));
    } else {
            LPC_I2S->IRQ |= (level << 16);
    }
    NVIC->ISER[0] |= (0x1 << 27);
}

/********************************************************************//**
 * @brief		Enable/Disable IRQ for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	NewState is new state of DMA operation, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void I2S_IRQCmd(uint8_t TRMode, uint8_t NewState) {
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	//CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (TRMode == I2S_RX_MODE) {
		if (NewState == 1)
			LPC_I2S->IRQ |= 0x01;
		else
			LPC_I2S->IRQ &= ~0x01;
		//Enable DMA

	} else {
		if (NewState == 1)
			LPC_I2S->IRQ |= 0x02;
		else
			LPC_I2S->IRQ &= ~0x02;
	}
}

/********************************************************************//**
 * @brief		Get I2S interrupt status
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		FunctionState	should be:
 * 				- ENABLE: interrupt is enable
 * 				- DISABLE: interrupt is disable
 *********************************************************************/
FunctionalState I2S_GetIRQStatus(uint8_t TRMode)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	if(1 == I2S_TX_MODE)
		return ((LPC_I2S->IRQ >> 1)&0x01);
	else
		return ((LPC_I2S->IRQ)&0x01);
}

/********************************************************************//**
 * @brief		Get I2S interrupt depth
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		depth of FIFO level on which to create an irq request
 *********************************************************************/
uint8_t I2S_GetIRQDepth(uint8_t TRMode)
{
	//CHECK_PARAM(PARAM_I2Sx(I2Sx));
	if(1 == I2S_TX_MODE)
		return (((LPC_I2S->IRQ)>>16)&0xFF);
	else
		return (((LPC_I2S->IRQ)>>8)&0xFF);
}
/**
 * @}
 */

 void mic_start_fft(void){
    lcd_draw_fft_graph(1);
    lcd_draw_audio_graph(1);
    lcd_fft_draw_buttons();
    mic_init();
    while(1){    
        mic_take_measurements();
        
        //delay_ms(500);
    }
}


