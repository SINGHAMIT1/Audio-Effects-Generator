// sine_intr.c

#include "audio.h"

volatile int16_t audio_chR=0;
volatile int16_t audio_chL=0;

#define SAMPLING_FREQ 8000
float32_t frequency = 1000.0;
float32_t amplitude = 2000.0;
float32_t theta_increment;
float32_t theta = 0.0f;

void I2S_HANDLER(void) {

audio_IN = i2s_rx();
audio_chL = (audio_IN & 0x0000FFFF);
audio_chR = ((audio_IN >>16)& 0x0000FFFF);

theta_increment = 2*PI*frequency/SAMPLING_FREQ;
theta += theta_increment;
if (theta > 2*PI) theta -= 2*PI;
audio_chL = (int16_t)(amplitude*sin(theta));
audio_chR = audio_chL;

audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_chL & 0x0000FFFF);
i2s_tx(audio_OUT);
}

int main(void)
{
audio_init ( hz8000, line_in, intr, I2S_HANDLER);

while(1){}
}
