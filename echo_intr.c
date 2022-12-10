// echo_intr.c
#include "audio.h"

volatile int16_t audio_chR=0;
volatile int16_t audio_chL=0;
static int emode = 0u;
static int pflag = 0u;
static int ButtonPressCheckFlag = 1;
#define DELAY_BUF_SIZE 16000
#define DELAY_BUF_FLANG 800
#define DELAY_BUF_SB 2000  
#define DELAY_BUF_CH1 2500
#define DELAY_BUF_CH2 3000 
#define GAIN 0.6f
#define LOOP_SIZE 1024
double sine_table[LOOP_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,}; //0, 7, 10, 7, 0, -7, -10, -7
double gain_table[9]={0.5,0.5,0.5, 1, 1,1,1,1,1};
double gain_table2[9]={1, 1, 1, 1, 1, 1, 0.5, 0.5, 0.5};
static int sine_ptr = 0;
int16_t buffer[DELAY_BUF_SIZE];
int16_t buffer2[DELAY_BUF_SB];
int16_t buffer3[DELAY_BUF_CH1];
int16_t buffer4[DELAY_BUF_CH2];
int16_t buf_ptr = 0;
int16_t buf_ptr2 = 0;
int16_t buf_ptr3 = 0;
int16_t buf_ptr4 = 0;
void I2S_HANDLER(void) {
int16_t delayed_sample;
int16_t audio_out_chL = 0;
int16_t audio_out_chR = 0;
int flangerflag = 0 ;
float vargain=0.1;	

audio_IN = i2s_rx();
audio_chL = (audio_IN & 0x0000FFFF);
audio_chR = ((audio_IN >>16)& 0x0000FFFF);
//delayed_sample = buffer[buf_ptr];

	switch(emode)
	{
		case 0: // Echo
			delayed_sample = buffer[buf_ptr];
			audio_out_chL = delayed_sample + audio_chL;
			buffer[buf_ptr] = audio_chL + delayed_sample*GAIN;
			buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
			audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		break;
		case 1: // Flanger
		 /*if(flangerflag==0)
     {
		  flangerflag = 1;
			 delayed_sample = buffer[buf_ptr];
			audio_out_chL = delayed_sample + audio_chL;
		  buffer[buf_ptr] = audio_chL + delayed_sample;
		  buf_ptr = (buf_ptr+1)%DELAY_BUF_SB;
		  //audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		 }
		 else 
		 {
			flangerflag = 0;
			 delayed_sample = buffer2[buf_ptr2];
			audio_out_chL = delayed_sample + audio_chL;
			 buffer2[buf_ptr2] = audio_chL + delayed_sample*GAIN;
		  buf_ptr2 = (buf_ptr2+1)%DELAY_BUF_FLANG;
		  //audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		 }	*/
			delayed_sample = buffer[buf_ptr];
			audio_out_chL = delayed_sample + audio_chL;
			buffer[buf_ptr] = audio_chL + delayed_sample*GAIN;
			buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
			audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		break;
		case 2: // Tremelo
		delayed_sample = buffer[buf_ptr];
		audio_out_chL = delayed_sample*sine_table[sine_ptr];	
		audio_chR =  delayed_sample*sine_table[sine_ptr];	
		buffer[buf_ptr] = audio_chL ;
		buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
		//--audio_out_chL = audio_chL*vargain;
		//--vargain= vargain+2;
		sine_ptr = (sine_ptr+1) % LOOP_SIZE;
		audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		//if (vargain>=10)
		//{ vargain =0.1;}
		break;
		case 3: //slapback
		delayed_sample = buffer2[buf_ptr2];
		audio_out_chL = delayed_sample + audio_chL;	
		buffer2[buf_ptr2] = audio_chL + delayed_sample*0.4;
		buf_ptr2 = (buf_ptr2+1)%DELAY_BUF_CH1;
		audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		break;
		case 4: //Chorus Audio
		delayed_sample = buffer2[buf_ptr2]+buffer3[buf_ptr3];
		audio_out_chL = delayed_sample + audio_chL;	
		buffer2[buf_ptr2] = audio_chL + delayed_sample*0.4;
		buffer3[buf_ptr3] = audio_chL + delayed_sample*0.4;
		buf_ptr2 = (buf_ptr2+1)%DELAY_BUF_SB;
		buf_ptr3 = (buf_ptr3+1)%DELAY_BUF_CH1;
		audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);
		break;	
		case 5: //Pan Audio
		if(pflag==0u)	{
		delayed_sample = buffer[buf_ptr];
		audio_out_chL = 0.1*delayed_sample;
		audio_out_chR = 1.5*delayed_sample;
		buffer[buf_ptr] = audio_chL;
		buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
		audio_OUT = ((audio_out_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x00000000);
		pflag =1u;	
		}
		else if(pflag == 1u){
		delayed_sample = buffer[buf_ptr];
		audio_out_chL = delayed_sample; 	
		buffer[buf_ptr] = audio_chL;
		buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
		audio_OUT = ((audio_out_chR<<16 & 0xFFFF0000)) + (audio_out_chL& 0x0000FFFF);	
		pflag = 2u;
		}
		else if(pflag ==2u){
		 //Pan Audio 2
		delayed_sample = buffer[buf_ptr];
		audio_out_chL = 1.5*delayed_sample;
		audio_out_chR = 0.1*delayed_sample;
		buffer[buf_ptr] = audio_chL;
		buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
		audio_OUT = ((audio_out_chR<<16 & 0x00000000)) + (audio_out_chL& 0x0000FFFF);
		pflag =0u;	
		}
		break;
		case 7: //Reverb
			delayed_sample = buffer[buf_ptr]+buffer2[buf_ptr2];
			audio_out_chL = delayed_sample + audio_chL;
			buffer[buf_ptr] = (audio_chL + delayed_sample*GAIN);
			buf_ptr = (buf_ptr+1)%DELAY_BUF_SIZE;
		  buffer2[buf_ptr2] = delayed_sample*0;
		  buf_ptr2 = (buf_ptr2+1)%DELAY_BUF_SB;
			audio_OUT = (((audio_chR)<<16 & 0xFFFF0000)) + ((audio_out_chL) & 0x0000FFFF);
		
		break;	
	}
	i2s_tx(audio_OUT);	
}
int main(void)
{
audio_init ( hz48000, dmic_in, intr, I2S_HANDLER);
gpio_set_mode(BUTTON,Input );
gpio_set_mode(LEDR,Output );
gpio_set_mode(LEDG,Output );
gpio_set_mode(LEDB,Output );
while(1){
		
		switch (emode)
		{
			case 0 : // Echo
				gpio_set(LEDR,1u);
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =1;
				}
				else {ButtonPressCheckFlag = 1;}
			  }
				break;
			case 1: // Flanger
				gpio_set(LEDG,1u);
			if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =2;
				}
				else {ButtonPressCheckFlag = 1;}
			 }
				delay_ms(1000*sine_table[sine_ptr]);
				break;
			case 2: // Tremelo
				gpio_set(LEDB,1u);
			if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =3;
				}
				else {ButtonPressCheckFlag = 1;}
			}
				break;
			case 3: //slapback
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =4;
				}
				else {ButtonPressCheckFlag = 1;}
			}
			
			break; 
			case 4: //Chorus
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =5;
				}
				else {ButtonPressCheckFlag = 1;}
			 }
			 delay_cycles(3);
			break; 
			case 5: //Pan Audio 1
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =6;
				}
				else {ButtonPressCheckFlag = 1;}
			}
			
			break; 
			case 6: //Pan Audio 2
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =7;
				}
				else {ButtonPressCheckFlag = 1;}
			}
			
			break; 
			case 7: //Reverb
				if (!gpio_get(BUTTON))
				{
					if (1 == ButtonPressCheckFlag)
				{ 
				ButtonPressCheckFlag = 0;
					emode =0;
				}
				else {ButtonPressCheckFlag = 1;}
			}
			break; 
	
	}
 }
}