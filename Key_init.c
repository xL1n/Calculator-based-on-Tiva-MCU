#include "PLL.h" 
#include "Key_init.h"
#include "LCD_init.h"

#define SYSCTL_RCG2_R       (*((volatile unsigned long *) 0x400FE108))
#define GPIO_PORTD_DIR_R    (*((volatile unsigned long *) 0x40007400))
#define GPIO_PORTD_AFSEL_R  (*((volatile unsigned long *) 0x40007420))
#define GPIO_PORTD_PUR_R    (*((volatile unsigned long *) 0x40007510))
#define GPIO_PORTD_PDR_R    (*((volatile unsigned long *) 0x40007514))
#define GPIO_PORTD_DEN_R    (*((volatile unsigned long *) 0x4000751C))
#define GPIO_PORTD_LOCK_R   (*((volatile unsigned long *) 0x40007520))
#define GPIO_PORTD_CR_R     (*((volatile unsigned long *) 0x40007524))
#define GPIO_PORTD_AMSEL_R  (*((volatile unsigned long *) 0x40007528))
#define GPIO_PORTD_PCTL_R   (*((volatile unsigned long *) 0x4000752C))
#define GPIO_PORTD_DATA_R   (*((volatile unsigned long *) 0x400073FC))	

#define GPIO_PORTE_DIR_R    (*((volatile unsigned long *) 0x40024400))
#define GPIO_PORTE_AFSEL_R  (*((volatile unsigned long *) 0x40024420))
#define GPIO_PORTE_PUR_R    (*((volatile unsigned long *) 0x40024510))
#define GPIO_PORTE_DEN_R    (*((volatile unsigned long *) 0x4002451C))
#define GPIO_PORTE_LOCK_R   (*((volatile unsigned long *) 0x40024520))
#define GPIO_PORTE_CR_R     (*((volatile unsigned long *) 0x40024524))
#define GPIO_PORTE_AMSEL_R  (*((volatile unsigned long *) 0x40024528))
#define GPIO_PORTE_PCTL_R   (*((volatile unsigned long *) 0x4002452C))	
#define GPIO_PORTE_DATA_R   (*((volatile unsigned long *) 0x400243FC))

#define ROWS                (*((volatile unsigned long *) 0x4002403C))
#define COLS                (*((volatile unsigned long *) 0x4000703C))
	
//col
void PortD_Init(void){ volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x00000008; 
	delay = SYSCTL_RCG2_R;	
	GPIO_PORTD_CR_R = 0x1F;	
	GPIO_PORTD_AMSEL_R = 0x00; 
	GPIO_PORTD_PCTL_R  = 0x00;
	GPIO_PORTD_DIR_R   = 0x00; //all input(read) ports  
	GPIO_PORTD_AFSEL_R = 0x00;  
	GPIO_PORTD_PUR_R   = 0x00; 
	GPIO_PORTD_PDR_R   = 0x0F;  
	GPIO_PORTD_DEN_R   = 0x0F; 
}  

//row
void PortE_Init(void){ volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x00000010; 
	delay = SYSCTL_RCG2_R;	
	GPIO_PORTE_CR_R = 0x1F;
	GPIO_PORTE_AMSEL_R = 0x00;
	GPIO_PORTE_PCTL_R  = 0x00;
	GPIO_PORTE_DIR_R   = 0x1F; //all output(set) ports
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_PUR_R   = 0x00;
	GPIO_PORTE_DEN_R   = 0x0F; 
}  

void keypadInit(void){
	PortE_Init();
	PortD_Init();
}

//     D0  D1  D2  D3 
// E0  [1] [2] [3] [+]      shift->       [1] [2] [3] [x] 
// E1  [4] [5] [6] [-]                    [4] [5] [6] [/]
// E2  [7] [8] [9] [shift S]              [7] [8] [9] [end shift H]
// E3  [.] [0] [=] [delete lats D]        [(] [e] [)] [Ans]

unsigned char keys_n[4][4] = { {'1','2','3','+'},  //non shifted key chars //[row] [col]
                               {'4','5','6','-'},
                               {'7','8','9','S'},
                               {'.','0','=','D'} };

unsigned char keys_y[4][4] = { {'1','2','3','x'},  //shifted key chars
                               {'4','5','6','/'},
                               {'7','8','9','H'},
                               {'(','e',')','A'} };

volatile char shift_f = 'n'; // shift flag when "shift" pressed, = y or "end shift" pressed = n
															 
unsigned char readKeypad(void) {
	int C = 0;
	int R = 0; 
	if(shift_f == 'n'){ //situation when no shift
	for(R = 0; R < 4; R++) {
		ROWS = (0x01 << R); //set each row high and see what's happening to colum 
		Delay_100us();
		for(C = 0; C < 4; C++) { //check(read) colums when specific row is set high 
			if( COLS & (0x01 << C) ) {
				if(keys_n[R][C] != 'S'){
					int slow = 0;
					while(slow < 15){
						slow ++;
						Delay_15ms();
					}
				  return keys_n[R][C];
					
				}else{
				  shift_f = 'y'; //shift is pressed, change flag 
				  Enable_Test_1_left();
					int slow = 0;
					while(slow < 15){
						slow ++;
						Delay_15ms();
					}
				  return keys_n[R][C];					
				}
			}
		}
	}
 }

 	if(shift_f == 'y'){ //situation when shift
	for(R = 0; R < 4; R++) {
		ROWS = (0x01 << R);
		Delay_100us();
		for(C = 0; C < 4; C++) {
			if(COLS & (0x01 << C)) {
				if(keys_y[R][C] != 'H'){
					int slow = 0;
					while(slow < 15){
						slow ++;
						Delay_15ms();
					}
				  return keys_y[R][C];
				}else{
				  shift_f = 'n'; //un-shift is pressed, change flag 
			    Disable_Test_1();
					int slow = 0;
					while(slow < 15){
						slow ++;
						Delay_15ms();
					}
				  return keys_y[R][C];

				}
			}
		}
	}
 }
	
	
	
	
	
	return 'P';
}
    //this is a test part, should be use only in main - while(1) loop - to check the detection function of keypad!
		//input = readKeypad();
		//if(input == '1'){
			//LCD_Write_Command(0x01);
		//}else if(input == 'S'){
		//LCD_Write_Command(0x04);
		//}else if(input == '('){
		//LCD_Write_Command(0x0F);
		//}else{LCD_Write_Command(0x00);}
    // --------------------------------------as so far, the test goes well--------------------

