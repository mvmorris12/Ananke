
//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#include <string.h>
#include <stdint.h>
#include "Microphone.h"

//#define TEST_LENGTH_SAMPLES 4096
#define TEST_LENGTH_SAMPLES 2048
////#define TEST_LENGTH_SAMPLES 1400
#define SPEECH_TEST_LENGTH_SAMPLES 4096




#define ADDR_FFT_BIN_INPUT_XCORR_R ADDR_SPEECH_FFT_MODEL_B + 2 * 64 * 1000
#define ADDR_FFT_BIN_INPUT_XCORR_G ADDR_FFT_BIN_INPUT_XCORR_R + 64 * 4096
#define ADDR_FFT_BIN_INPUT_XCORR_B ADDR_FFT_BIN_INPUT_XCORR_G + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_XCORR_R ADDR_FFT_BIN_INPUT_XCORR_B + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_XCORR_G ADDR_FFT_BIN_OUTPUT_XCORR_R + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_XCORR_B ADDR_FFT_BIN_OUTPUT_XCORR_G + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R ADDR_FFT_BIN_OUTPUT_XCORR_B + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_R + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_G + 64 * 4096
#define ADDR_FFT_BIN_OUTPUT_SPEECH_TEST ADDR_FFT_BIN_OUTPUT_SPEECH_MODEL_B + 64 * 4096

int32_t fft_test(void);
void fft_speech_test(void);
void fft_speech_models(void);
void fft_compare_models_test(void);
void speech_cross_correlation_test(void);

extern void delay(uint32_t i);