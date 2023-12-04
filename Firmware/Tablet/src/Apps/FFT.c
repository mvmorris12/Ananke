/* ----------------------------------------------------------------------
* Copyright (C) 2010-2012 ARM Limited. All rights reserved.
*
* $Date:         17. January 2013
* $Revision:     V1.4.0
*
* Project:       CMSIS DSP Library
* Title:	     arm_fft_bin_example_f32.c
*
* Description:   Example code demonstrating calculation of Max energy bin of
*                frequency domain of input signal.
*
* Target Processor: Cortex-M4/Cortex-M3
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------- */

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup FrequencyBin Frequency Bin Example
 *
 * \par Description
 * \par
 * Demonstrates the calculation of the maximum energy bin in the frequency
 * domain of the input signal with the use of Complex FFT, Complex
 * Magnitude, and Maximum functions.
 *
 * \par Algorithm:
 * \par
 * The input test signal contains a 10 kHz signal with uniformly distributed white noise.
 * Calculating the FFT of the input signal will give us the maximum energy of the
 * bin corresponding to the input frequency of 10 kHz.
 *
 * \par Block Diagram:
 * \image html FFTBin.gif "Block Diagram"
 * \par
 * The figure below shows the time domain signal of 10 kHz signal with
 * uniformly distributed white noise, and the next figure shows the input
 * in the frequency domain. The bin with maximum energy corresponds to 10 kHz signal.
 * \par
 * \image html FFTBinInput.gif "Input signal in Time domain"
 * \image html FFTBinOutput.gif "Input signal in Frequency domain"
 *
 * \par Variables Description:
 * \par
 * \li \c fft_buffer points to the input data
 * \li \c fft_bin_output points to the output data
 * \li \c fftSize length of FFT
 * \li \c ifftFlag flag for the selection of CFFT/CIFFT
 * \li \c doBitReverse Flag for selection of normal order or bit reversed order
 * \li \c refIndex reference index value at which maximum energy of bin ocuurs
 * \li \c testIndex calculated index value at which maximum energy of bin ocuurs
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_cfft_f32()
 * - arm_cmplx_mag_f32()
 * - arm_max_f32()
 *
 * <b> Refer  </b>
 * \link arm_fft_bin_example_f32.c \endlink
 *
 */


/** \example arm_fft_bin_example_f32.c
  */
#include "FFT.h"
#include "LCD.h"
#include <stdio.h>
#include "Microphone.h"
#include "Colors.h"

#include "arm_math.h"
#include "arm_const_structs.h"

#if defined(SEMIHOSTING)
#include <stdio.h>
#endif


/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern fft_buffer_t *fft_buffer;
extern fft_buffer_t *speech_fft_test;
extern fft_buffer_t *speech_fft_model_r0;
extern fft_buffer_t *speech_fft_model_r1;
extern fft_buffer_t *speech_fft_model_r2;
extern fft_buffer_t *speech_fft_model_r3;
extern fft_buffer_t *speech_fft_model_r4;
extern fft_buffer_t *speech_fft_model_r5;
extern fft_buffer_t *speech_fft_model_r6;
extern fft_buffer_t *speech_fft_model_r7;
extern fft_buffer_t *speech_fft_model_g0;
extern fft_buffer_t *speech_fft_model_g1;
extern fft_buffer_t *speech_fft_model_g2;
extern fft_buffer_t *speech_fft_model_g3;
extern fft_buffer_t *speech_fft_model_g4;
extern fft_buffer_t *speech_fft_model_g5;
extern fft_buffer_t *speech_fft_model_g6;
extern fft_buffer_t *speech_fft_model_g7;
extern fft_buffer_t *speech_fft_model_b0;
extern fft_buffer_t *speech_fft_model_b1;
extern fft_buffer_t *speech_fft_model_b2;
extern fft_buffer_t *speech_fft_model_b3;
extern fft_buffer_t *speech_fft_model_b4;
extern fft_buffer_t *speech_fft_model_b5;
extern fft_buffer_t *speech_fft_model_b6;
extern fft_buffer_t *speech_fft_model_b7;
//extern fft_buffer_t *fft_buffer;
//extern fft_buffer_t *fft_bin_output;
float32_t fft_bin_output[TEST_LENGTH_SAMPLES/2];
float32_t *fft_bin_output_speech_test = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_TEST;
//float32_t *fft_bin_input_xcorr_r = (float32_t*) 0xA0262E00;
//float32_t *fft_bin_input_xcorr_g = (float32_t*) 0xA0272E00;
//float32_t *fft_bin_input_xcorr_b = (float32_t*) 0xA0282E00;
////float32_t *fft_bin_output_xcorr_r = (float32_t*) 0xA02C2E00;
//float32_t *fft_bin_output_xcorr_r = (FFT2);
//float32_t *fft_bin_output_xcorr_g = (float32_t*) 0xA0302E00;
//float32_t *fft_bin_output_xcorr_b = (float32_t*) 0xA0342E00;
//float32_t *fft_bin_output_speech_model_r = (float32_t*) 0xA0382E00;
//float32_t *fft_bin_output_speech_model_g = (float32_t*) 0xA03C2E00;
//float32_t *fft_bin_output_speech_model_b = (float32_t*) 0xA0402E00;

float32_t *fft_bin_input_xcorr_r = (float32_t*) ADDR_FFT_BIN_INPUT_XCORR_R;
float32_t *fft_bin_input_xcorr_g = (float32_t*) ADDR_FFT_BIN_INPUT_XCORR_G;
float32_t *fft_bin_input_xcorr_b = (float32_t*) ADDR_FFT_BIN_INPUT_XCORR_B;
float32_t *fft_bin_output_xcorr_r = (float32_t*) ADDR_FFT_BIN_OUTPUT_XCORR_R;
float32_t *fft_bin_output_xcorr_g = (float32_t*) ADDR_FFT_BIN_OUTPUT_XCORR_G;
float32_t *fft_bin_output_xcorr_b = (float32_t*) ADDR_FFT_BIN_OUTPUT_XCORR_B;
float32_t *fft_bin_output_speech_model_r0 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R0;
float32_t *fft_bin_output_speech_model_r1 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R1;
float32_t *fft_bin_output_speech_model_r2 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R2;
float32_t *fft_bin_output_speech_model_r3 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R3;
float32_t *fft_bin_output_speech_model_r4 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R4;
float32_t *fft_bin_output_speech_model_r5 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R5;
float32_t *fft_bin_output_speech_model_r6 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R6;
float32_t *fft_bin_output_speech_model_r7 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R7;
float32_t *fft_bin_output_speech_model_g0 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G0;
float32_t *fft_bin_output_speech_model_g1 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G1;
float32_t *fft_bin_output_speech_model_g2 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G2;
float32_t *fft_bin_output_speech_model_g3 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G3;
float32_t *fft_bin_output_speech_model_g4 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G4;
float32_t *fft_bin_output_speech_model_g5 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G5;
float32_t *fft_bin_output_speech_model_g6 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G6;
float32_t *fft_bin_output_speech_model_g7 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G7;
float32_t *fft_bin_output_speech_model_b0 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B0;
float32_t *fft_bin_output_speech_model_b1 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B1;
float32_t *fft_bin_output_speech_model_b2 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B2;
float32_t *fft_bin_output_speech_model_b3 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B3;
float32_t *fft_bin_output_speech_model_b4 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B4;
float32_t *fft_bin_output_speech_model_b5 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B5;
float32_t *fft_bin_output_speech_model_b6 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B6;
float32_t *fft_bin_output_speech_model_b7 = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B7;

struct RGB_results_t rgb_results;
//float32_t fft_bin_output_old[TEST_LENGTH_SAMPLES/2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = TEST_LENGTH_SAMPLES/2;
uint32_t fftSize_speech = SPEECH_TEST_LENGTH_SAMPLES;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
arm_cfft_instance_f32 varInstCfftF32;
//arm_cfft_instance_f32 varInstCfftF32_speech;
arm_rfft_fast_instance_f32 varInstCfftF32_speech;
arm_rfft_fast_instance_f32 varInstRfftF32_speech;
uint16_t speech_analysis_count = 0;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;

/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */

float32_t maxValue;
volatile uint8_t fft_ready = 0;

const weights_red[]   = {1.0, 5.0, 5.0, 1.0, 1.0, 1.0, 1.0, 1.0};
const weights_green[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 80.0, 100.0};
const weights_blue[]  = {1.0, 1.0, 10.0, 5.0, 1.0, 1.0, 5.0, 1.0};


int32_t fft_test(void){

    arm_status status;

    status = ARM_MATH_SUCCESS;
    //for (uint16_t i=0; i<2048; i++){
    //    printf("%f,\n", fft_buffer[i]);
    //    delay(10000);
    //    //i++;
    //}
    //status=arm_cfft_init_f32(&varInstCfftF32,fftSize);

    ///* Process the data through the CFFT/CIFFT module */
    //arm_cfft_f32(&varInstCfftF32, fft_buffer, ifftFlag, doBitReverse);

    ///* Process the data through the Complex Magnitude Module for
    //calculating the magnitude at each bin */
    //arm_cmplx_mag_f32(fft_buffer, fft_bin_output, fftSize);



    status=arm_rfft_fast_init_f32(&varInstCfftF32,fftSize/2);
    arm_rfft_fast_f32(&varInstCfftF32, fft_buffer, fft_bin_output, 0);




    /* Calculates maxValue and returns corresponding BIN value */
    //arm_max_f32(fft_bin_output, fftSize, &maxValue, &testIndex);

    status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;
    
    //printf("maxValue:\t%0.4f\nBin\t\t%d\nFrequency:\t%0.3f\n\n", maxValue, testIndex, testIndex*16000/(TEST_LENGTH_SAMPLES/2)*1.004651162790697674);
    
    //lcd_clear_fft_graph();
    //lcd_draw_fft_bins(maxValue);
    fft_ready = 1;

    for (uint32_t i=0; i<TEST_LENGTH_SAMPLES; i++){
        fft_buffer[i] = 0.0;
    }


    //for (uint32_t i=0; i<TEST_LENGTH_SAMPLES/2; i++){
    //    printf("%0.4f\n", fft_bin_output[i]);
    ////    fft_bin_output_old[i] = fft_bin_output[i];
    ////    fft_bin_output[i] = 0.0;
    //}
}


void fft_speech_models(void){
    printf("Running speech FFT\n");
    arm_status status;
    status = ARM_MATH_SUCCESS;




    //arm_rfft_fast_instance_f32 varInstRfftF32_speech;
    //uint16_t fftSize_speech = 512;
    //uint8_t inverse_flag = 0;
    //arm_status status;
    status = arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r0, fft_bin_output_speech_model_r0, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r1, fft_bin_output_speech_model_r1, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r2, fft_bin_output_speech_model_r2, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r3, fft_bin_output_speech_model_r3, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r4, fft_bin_output_speech_model_r4, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r5, fft_bin_output_speech_model_r5, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r6, fft_bin_output_speech_model_r6, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_r7, fft_bin_output_speech_model_r7, 0);
    
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g0, fft_bin_output_speech_model_g0, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g1, fft_bin_output_speech_model_g1, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g2, fft_bin_output_speech_model_g2, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g3, fft_bin_output_speech_model_g3, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g4, fft_bin_output_speech_model_g4, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g5, fft_bin_output_speech_model_g5, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g6, fft_bin_output_speech_model_g6, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_g7, fft_bin_output_speech_model_g7, 0);
    
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b0, fft_bin_output_speech_model_b0, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b1, fft_bin_output_speech_model_b1, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b2, fft_bin_output_speech_model_b2, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b3, fft_bin_output_speech_model_b3, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b4, fft_bin_output_speech_model_b4, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b5, fft_bin_output_speech_model_b5, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b6, fft_bin_output_speech_model_b6, 0);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_model_b7, fft_bin_output_speech_model_b7, 0);


    //for (int i=0; i<fftSize_speech; i++){
    //    fft_bin_output_speech_model_r[i] = fft_bin_output_speech_model_r[i];
    //    fft_bin_output_speech_model_g[i] = fft_bin_output_speech_model_g[i];
    //    fft_bin_output_speech_model_b[i] = fft_bin_output_speech_model_b[i];
    //}

    //for (int i=0; i<fftSize_speech*2; i++){
    //    printf("%0.5f\n", fft_bin_output_speech_model_r0[i]);
    //    delay(5000);
    //}
    //for (int i=0; i<fftSize_speech*2; i++){
    //    printf("%0.5f\n", fft_bin_output_speech_model_g0[i]);
    //    delay(5000);
    //}
    //for (int i=0; i<fftSize_speech*2; i++){
    //    printf("%0.5f\n", fft_bin_output_speech_model_b0[i]);
    //    delay(5000);
    //}

    printf("Speech Model FFTs done\n");
}


void fft_speech_test(int iteration){
    //printf("Running speech test FFT\n");
    for (int i=0; i< fftSize_speech; i++){
        fft_bin_output_speech_test[i] = 0.0;
    }
    arm_status status;
    status = ARM_MATH_SUCCESS;
    //status=arm_cfft_init_f32(&varInstCfftF32_speech,fftSize_speech);
    //arm_cfft_f32(&varInstCfftF32_speech, speech_fft_test, ifftFlag, doBitReverse);
    //arm_cmplx_mag_f32(speech_fft_test, fft_bin_output_speech_test, fftSize_speech);
    //arm_max_f32(fft_bin_output_speech_test, fftSize_speech, &maxValue, &testIndex);
    //status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;


    //for (int i=0; i<fftSize_speech*2; i++){
    //    printf("%0.5f\n", speech_fft_test[i]);
    //    delay(3000);
    //}
    status=arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, speech_fft_test, fft_bin_output_speech_test, 0);

    
    //for (int i=0; i<fftSize_speech*2; i++){
    //    printf("%0.5f\n", fft_bin_output_speech_test[i]);
    //    delay(3000);
    //}

    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
        fft_bin_output_xcorr_r[i] = 0.0;
        fft_bin_output_xcorr_g[i] = 0.0;
        fft_bin_output_xcorr_b[i] = 0.0;
    }
    
    float32_t* fft_bin_output_speech_model_r;
    float32_t* fft_bin_output_speech_model_g;
    float32_t* fft_bin_output_speech_model_b;


    switch(iteration){
        case 0:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r0;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g0;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b0;
            break;
        case 1:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r1;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g1;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b1;
            break;
        case 2:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r2;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g2;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b2;
            break;
        case 3:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r3;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g3;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b3;
            break;
        case 4:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r4;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g4;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b4;
            break;
        case 5:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r5;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g5;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b5;
            break;
        case 6:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r6;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g6;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b6;
            break;
        case 7:
            fft_bin_output_speech_model_r = fft_bin_output_speech_model_r7;
            fft_bin_output_speech_model_g = fft_bin_output_speech_model_g7;
            fft_bin_output_speech_model_b = fft_bin_output_speech_model_b7;
            break;
    }

    
    //for (int i=0; i<fftSize_speech; i++){
    //    printf("%0.5f\n", fft_bin_output_speech_model_g[i]);
    //    delay(5000);
    //}

    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
        fft_bin_input_xcorr_r[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_r[i];
        fft_bin_input_xcorr_g[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_g[i];
        fft_bin_input_xcorr_b[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_b[i];
    }



    status=arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_r, fft_bin_output_xcorr_r, 1);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_g, fft_bin_output_xcorr_g, 1);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_b, fft_bin_output_xcorr_b, 1);
    
    //printf("Speech test FFT done\n");
}


void speech_cross_correlation_test(int iteration){
    float max_r, max_g, max_b, max_t;
    float total_r, total_g, total_b;
    max_r = 0.0;
    max_g = 0.0;
    max_b = 0.0;      

    if (iteration == 0){
        for (uint8_t i=0; i<MAX_ITERATIONS; i++){
            rgb_results.red.max[i] = -1000.0;
            rgb_results.green.max[i] = -1000.0;
            rgb_results.blue.max[i] = -1000.0;
        }
        rgb_results.red.total = 0.0;
        rgb_results.green.total = 0.0;
        rgb_results.blue.total = 0.0;
    }

    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
        total_r += fft_bin_output_xcorr_r[i];
        total_g += fft_bin_output_xcorr_g[i];
        total_b += fft_bin_output_xcorr_b[i];
        
        if (fft_bin_output_xcorr_r[i] > rgb_results.red.max[iteration]){
            rgb_results.red.max[iteration] = fft_bin_output_xcorr_r[i];
        }
        if (fft_bin_output_xcorr_g[i] > rgb_results.green.max[iteration]){
            rgb_results.green.max[iteration] = fft_bin_output_xcorr_g[i];
        }
        if (fft_bin_output_xcorr_b[i] > rgb_results.blue.max[iteration]){
            rgb_results.blue.max[iteration] = fft_bin_output_xcorr_b[i];
        }

    }
    //printf("RESULTS:\nred: \t%0.3f\ngreen: \t%0.3f\nblue: \t%0.3f\n\n", total_r, total_g, total_b);
    
    rgb_results.red.total += total_r;
    rgb_results.green.total += total_g;
    rgb_results.blue.total += total_b;

    //printf("cross correlation done\n");

    if (iteration == MAX_ITERATIONS-1){
        float32_t max_r_tot, max_g_tot, max_b_tot;

        for (uint8_t i=0; i<MAX_ITERATIONS; i++){
            rgb_results.red.max[i] *= weights_red[i];
            rgb_results.green.max[i] *= weights_green[i];
            rgb_results.blue.max[i] *= weights_blue[i];
        }

        for (uint8_t i=0; i<MAX_ITERATIONS; i++){
            max_r_tot += rgb_results.red.max[i];
            max_g_tot += rgb_results.green.max[i];
            max_b_tot += rgb_results.blue.max[i];
        }
        if (speech_analysis_count++){
            printf("red   : [");
            for (uint8_t i=0; i<MAX_ITERATIONS-1; i++){
                printf("%0.2f ", rgb_results.red.max[i]);
            }
            printf("%0.2f] = %0.2f\n", rgb_results.red.max[MAX_ITERATIONS-1], max_r_tot);
            printf("green : [");
            for (uint8_t i=0; i<MAX_ITERATIONS-1; i++){
                printf("%0.2f ", rgb_results.green.max[i]);
            }
            printf("%0.2f] = %0.2f\n", rgb_results.green.max[MAX_ITERATIONS-1], max_g_tot);
            printf("blue  : [");
            for (uint8_t i=0; i<MAX_ITERATIONS-1; i++){
                printf("%0.2f ", rgb_results.blue.max[i]);
            }
            printf("%0.2f] = %0.2f\n", rgb_results.blue.max[MAX_ITERATIONS-1], max_b_tot);

            if (max_r_tot >= max_b_tot && max_r_tot >= max_g_tot){
                lcd_speech_draw_answer(RED);
            } else if (max_b_tot >= max_r_tot && max_b_tot >= max_g_tot){
                lcd_speech_draw_answer(BLUE);
            } else if (max_g_tot >= max_r_tot && max_g_tot >= max_b_tot){
                lcd_speech_draw_answer(YELLOW);
            }
        }

        //delay_ms(1000);
    }
    



}



 /** \endlink */

