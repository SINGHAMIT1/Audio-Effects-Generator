// dimpulse_intr.c
#include "audio.h"

volatile int16_t audio_chR=0;
volatile int16_t audio_chL=0;
#define LOOP_SIZE 64
int16_t impulse_table[LOOP_SIZE] = {
10000, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0};
 static int impulse_ptr = 0;
void I2S_HANDLER(void) { /****** I2S Interruption Handler*****/
 audio_IN = i2s_rx();
audio_chL = (audio_IN & 0x0000FFFF);
audio_chR = ((audio_IN >>16)& 0x0000FFFF);
audio_chL = impulse_table[impulse_ptr];
audio_chR = impulse_table[impulse_ptr];
impulse_ptr = (impulse_ptr+1) % LOOP_SIZE;
audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_chL & 0x0000FFFF);
i2s_tx(audio_OUT);
 }
int main(void)
 {
audio_init ( hz8000, line_in, intr, I2S_HANDLER);

while(1){}
}
