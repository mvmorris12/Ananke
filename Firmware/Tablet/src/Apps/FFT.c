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
extern fft_buffer_t *speech_fft_model_r;
extern fft_buffer_t *speech_fft_model_g;
extern fft_buffer_t *speech_fft_model_b;
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
float32_t *fft_bin_output_speech_model_r = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R;
float32_t *fft_bin_output_speech_model_g = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G;
float32_t *fft_bin_output_speech_model_b = (float32_t*) ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B;
//float32_t fft_bin_output_old[TEST_LENGTH_SAMPLES/2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = TEST_LENGTH_SAMPLES/2;
uint32_t fftSize_speech = SPEECH_TEST_LENGTH_SAMPLES;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
arm_cfft_instance_f32 varInstCfftF32;
arm_cfft_instance_f32 varInstCfftF32_speech;
arm_rfft_fast_instance_f32 varInstRfftF32_speech;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;

/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */

float32_t maxValue;
volatile uint8_t fft_ready = 0;

int32_t fft_test(void)
{

    arm_status status;

    status = ARM_MATH_SUCCESS;
    //for (uint16_t i=0; i<2048; i++){
    //    printf("%f,\n", fft_buffer[i]);
    //    delay(10000);
    //    //i++;
    //}
    status=arm_cfft_init_f32(&varInstCfftF32,fftSize);

    /* Process the data through the CFFT/CIFFT module */
    arm_cfft_f32(&varInstCfftF32, fft_buffer, ifftFlag, doBitReverse);

    /* Process the data through the Complex Magnitude Module for
    calculating the magnitude at each bin */
    arm_cmplx_mag_f32(fft_buffer, fft_bin_output, fftSize);

    /* Calculates maxValue and returns corresponding BIN value */
    arm_max_f32(fft_bin_output, fftSize, &maxValue, &testIndex);

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
    for (int i=0; i< fftSize_speech; i++){
        fft_bin_output_speech_model_r[i] = 0.0;
        fft_bin_output_speech_model_g[i] = 0.0;
        fft_bin_output_speech_model_b[i] = 0.0;
    }
    arm_status status;
    status = ARM_MATH_SUCCESS;
    status=arm_cfft_init_f32(&varInstCfftF32_speech,fftSize_speech);
    arm_cfft_f32(&varInstCfftF32_speech, speech_fft_model_r, ifftFlag, doBitReverse);
    arm_cmplx_mag_f32(speech_fft_model_r, fft_bin_output_speech_model_r, fftSize_speech);
    arm_max_f32(fft_bin_output_speech_model_r, fftSize_speech, &maxValue, &testIndex);
    status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;
    
    status = ARM_MATH_SUCCESS;
    status=arm_cfft_init_f32(&varInstCfftF32_speech,fftSize_speech);
    arm_cfft_f32(&varInstCfftF32_speech, speech_fft_model_g, ifftFlag, doBitReverse);
    arm_cmplx_mag_f32(speech_fft_model_g, fft_bin_output_speech_model_g, fftSize_speech);
    arm_max_f32(fft_bin_output_speech_model_g, fftSize_speech, &maxValue, &testIndex);
    status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;
    
    status = ARM_MATH_SUCCESS;
    status=arm_cfft_init_f32(&varInstCfftF32_speech,fftSize_speech);
    arm_cfft_f32(&varInstCfftF32_speech, speech_fft_model_b, ifftFlag, doBitReverse);
    arm_cmplx_mag_f32(speech_fft_model_b, fft_bin_output_speech_model_b, fftSize_speech);
    arm_max_f32(fft_bin_output_speech_model_b, fftSize_speech, &maxValue, &testIndex);
    status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;

    printf("Speech Model FFTs done\n");
}

void fft_speech_test(void){
    printf("Running speech test FFT\n");
    for (int i=0; i< fftSize_speech; i++){
        fft_bin_output_speech_test[i] = 0.0;
    }
    arm_status status;
    status = ARM_MATH_SUCCESS;
    status=arm_cfft_init_f32(&varInstCfftF32_speech,fftSize_speech);
    arm_cfft_f32(&varInstCfftF32_speech, speech_fft_test, ifftFlag, doBitReverse);
    arm_cmplx_mag_f32(speech_fft_test, fft_bin_output_speech_test, fftSize_speech);
    arm_max_f32(fft_bin_output_speech_test, fftSize_speech, &maxValue, &testIndex);
    status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;

    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
        fft_bin_output_xcorr_r[i] = 0.0;
        fft_bin_output_xcorr_g[i] = 0.0;
        fft_bin_output_xcorr_b[i] = 0.0;
    }
    
    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
        fft_bin_input_xcorr_r[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_r[i];
        fft_bin_input_xcorr_g[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_g[i];
        fft_bin_input_xcorr_b[i] = fft_bin_output_speech_test[i]*fft_bin_output_speech_model_b[i];
    }
//TODO try fftSize_speech/2 ? ifft might not be right. Try *2 also
    status=arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_r, fft_bin_output_xcorr_r, 1);
    
    status=arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_g, fft_bin_output_xcorr_g, 1);
    
    status=arm_rfft_fast_init_f32(&varInstRfftF32_speech,fftSize_speech);
    arm_rfft_fast_f32(&varInstRfftF32_speech, fft_bin_input_xcorr_b, fft_bin_output_xcorr_b, 1);
    
    printf("Speech test FFT done\n");
}


void fft_compare_models_test(void){
    float total_r, total_g, total_b;
    total_r = 0.0;
    total_g = 0.0;
    total_b = 0.0;

    for (int i=0; i<1024; i++){
        total_r += abs(fft_bin_output_speech_test[i]-fft_bin_output_speech_model_r[i]);
        total_g += abs(fft_bin_output_speech_test[i]-fft_bin_output_speech_model_g[i]);
        total_b += abs(fft_bin_output_speech_test[i]-fft_bin_output_speech_model_b[i]);
    }

    float min = total_r;
    if (min > total_g){
        min = total_g;
    }
    if (min > total_b){
        min = total_b;
    }
    
    //printf("red: \t%0.1f\ngreen: \t%0.1f\nblue: \t%0.1f\nxxxxxx\n\n", total_r, total_g, total_b);
   
    if (min == total_r){
        printf("min = red\n");
    } else if (min == total_g){
        printf("min = green\n");
    } else if (min == total_b){
        printf("min = blue\n");
    }
    //printf("red: \t%0.1f\ngreen: \t%0.1f\nblue: \t%0.1f\nxxxxxx\n\n", total_r, total_g, total_b);
    printf("red: \t%0.3f\ngreen: \t%0.3f\nblue: \t%0.3f\n\n", total_r/min, total_g/min, total_b/min);
}



void speech_cross_correlation_test(void){
    float max_r, max_g, max_b, max_t;
    max_r = 0.0;
    max_g = 0.0;
    max_b = 0.0;    

    //for (int i=0; i<4096; i++){
    //    printf("%0.5f\n", fft_bin_output_xcorr_r[i]);
    //    delay(3000);
    //}

    //for (int i=0; i<4096; i++){
    //    printf("%0.5f\n", fft_bin_output_xcorr_g[i]);
    //    delay(3000);
    //}

    //for (int i=0; i<4096; i++){
    //    printf("%0.5f\n", fft_bin_output_xcorr_b[i]);
    //    delay(6000);
    //}

    for (int i=0; i<1000; i++){
        if (fft_bin_output_xcorr_r[i] > max_r){
            max_r = fft_bin_output_xcorr_r[i];
        }
        if (fft_bin_output_xcorr_g[i] > max_g){
            max_g = fft_bin_output_xcorr_g[i];
        }
        if (fft_bin_output_xcorr_b[i] > max_b){
            max_b = fft_bin_output_xcorr_b[i];
        }
    }
    printf("cross correlation done\nmax_r: %0.2f\nmax_g: %0.2f\nmax_b: %0.2f\n", max_r, max_g, max_b);
    if (max_r > max_g && max_r > max_b){
        printf("max is red\n");
    } else if (max_g > max_r && max_g > max_b){
        printf("max is green\n");
    } else if (max_b > max_r && max_b > max_g){
        printf("max is blue\n");
    }
}



 /** \endlink */

