#include "LCD.h"
#include "Speech.h"
//#include "Speech_signals.h"
#include "Microphone.h"
#include "Colors.h"
#include <string.h>
#include <stdio.h>

//extern const audio_signal_r[16000];
//extern I2S_array_t *I2S_arr;

//int speech_find_audio_start(char color[]){
//    int *signal;
//    if (strcmp(color, "red") == 0){
//        printf("checking red signal\n");
//        signal = (int *)audio_signal_r;
//    } else if (strcmp(color, "green") == 0){
//        printf("checking green signal\n");
//        signal = (int *)audio_signal_g;
//    } else if (strcmp(color, "blue") == 0){
//        printf("checking blue signal\n");
//        signal = (int *)audio_signal_b;
//    } else if (strcmp(color, "red test") == 0){
//        printf("checking red test signal\n");
//        signal = (int *)audio_signal_r_test;
//    } else if (strcmp(color, "green test") == 0){
//        printf("checking green test signal\n");
//        signal = (int *)audio_signal_g_test;
//    } else if (strcmp(color, "blue test") == 0){
//        printf("checking blue test signal\n");
//        signal = (int *)audio_signal_b_test;
//    } else if (strcmp(color, "test") == 0){
//        printf("checking user test signal\n");
//        signal = I2S_arr;
//        //for (int i=0; i<8000; i++){
//        //    printf("%d\n", signal[i]);
//        //    delay(5000);
//        //}
//        //delay_short();
//        //for (int i=8000; i<16000; i++){
//        //    printf("%d\n", signal[i]);
//        //    delay(5000);
//        //}
//    };
    
//    int signal_previous = signal[200];
//    int signal_current = signal[200];
//    for (int i=1000; i<16000; i++){
//        signal_current = signal[i];
//        if (signal_previous - signal_current > 300){
//            printf("jump at %d: %d\n", i, signal_previous-signal_current);
//            return i;
//        }
//        signal_previous = signal[i];
//    }
//    return 500;
//    printf("done\n");
//}



void speech_detection_app(void){
    //lcd_clear_screen();
    lcd_draw_speech_models();
    lcd_speech_draw_buttons();
    //fft_speech_models();
    //delay_long();
    //lcd_speech_draw_answer(RED);
    //delay_long();
    //lcd_speech_clear_answer();
    //lcd_speech_draw_answer(YELLOW);
    //delay_long();
    //lcd_speech_clear_answer();
    //lcd_speech_draw_answer(BLUE);
    //delay_long();
    //lcd_speech_clear_answer();
    //lcd_speech_draw_answer(0);
    //delay_long();
    //lcd_speech_clear_answer();
    while(1){    
        while (1){
            touch_check();
        }
    }
}