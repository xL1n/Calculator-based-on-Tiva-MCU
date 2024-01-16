// header file of PLL 
#ifndef PLL_H
#define PLL_H
////PLL is now set to have Freqeucny of 80MHz -- 1/80*10^6 s / cycle

void SysTick_Init(void); 

void PLL_Init(void);
	
void SysTick_Wait(unsigned long delay);

//delays
void Delay_40ms(void);
void Delay_15ms(void);
void Delay_4P1ms(void);
void Delay_100us(void);
void Delay_10s(void);
#endif