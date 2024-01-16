#include "PLL.h"
#include "LCD_init.h"
#include <string.h>

#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *) 0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *) 0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *) 0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *) 0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *) 0x4000452C))

#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *) 0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *) 0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *) 0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *) 0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *) 0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *) 0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *) 0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *) 0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *) 0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *) 0x400FE108))
	
// bit specific 
#define LCD_RS                  (*((volatile unsigned long *) 0x40004020)) //RS = PA3 --> 1000 --> 0x08
#define LCD_EN                  (*((volatile unsigned long *) 0x40004010)) //EN = PA2 --> 0100 --> 0x04
#define LCD_DB                  (*((volatile unsigned long *) 0x4000503C)) //DB4-7(databus) = PORT B 0, 1, 2, 3
//LCD_RS = 0
#define TEST_POINT              (*((volatile unsigned long *) 0x40004300)) //PA6 and PA7 --> set to be output port for any test indicators 1100 0000

//current cursor location record: 
unsigned char cursor_r; //which row the cursor locate
unsigned char cursor_c; //which colum the cursor locate

void PortA_Init(void){unsigned long delay; 
  SYSCTL_RCGC2_R |= 0x00000001;
  delay = SYSCTL_RCGC2_R;            
  GPIO_PORTA_CR_R = 0x0F; 
  GPIO_PORTA_AMSEL_R = 0; 
  GPIO_PORTA_PCTL_R = 0;   
  GPIO_PORTA_DIR_R = 0xFF;          //PA2-3 are OUTPUTS
  GPIO_PORTA_AFSEL_R = 0;
	//no pulling R	
  GPIO_PORTA_DEN_R = 0xFF;   
}

void PortB_Init(void){unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002;  
  delay = SYSCTL_RCGC2_R;          
  GPIO_PORTB_CR_R = 0x3C;              
  GPIO_PORTB_AMSEL_R = 0;        
  GPIO_PORTB_PCTL_R = 0;    
  GPIO_PORTB_DIR_R = 0xFF;          //PB0-PB3 are OUTPUTS
  GPIO_PORTB_AFSEL_R = 0;  
  //no pulling R	
  GPIO_PORTB_DEN_R = 0xFF;   
}

void PortABInit(void){
	PortA_Init();
	PortB_Init();
}

void LCD_init(void){// initialize LCD and portA and B
	PortABInit();	
	cursor_c = 0;
	cursor_r = 0;
	Delay_40ms(); // wait time > 40ms after VDD > 2.7

	LCD_Write_Command(0x03);
	Delay_4P1ms();
	
  LCD_Write_Command(0x03);
	Delay_100us();
	
  LCD_Write_Command(0x03);

  LCD_Write_Command(0x02);

  LCD_Write_Command(0x02);
	
	
  LCD_Write_Command(0x08); //set display of line = 2; front size = 5x8 dots(as only 5x8 can be displayed two lines)
	
  LCD_Write_Command(0x00);
	LCD_Write_Command(0x0F);
  //LCD_Write_Command(0x0F);

  LCD_Write_Command(0x00);
  LCD_Write_Command(0x01);

  LCD_Write_Command(0x00);
  LCD_Write_Command(0x06);
	
  //LCD_Write_Command(0x00);
  //LCD_Write_Command(0x0F);
	
}

void LCD_load(void){ // enable is triggered to load data into LCD
	LCD_EN = 0x04;
	Delay_100us(); // give some time for loading 
	LCD_EN = 0;
	Delay_100us();
}

void LCD_Write_Command(unsigned char in){ //write single command into register (in = xxxx bbbb)
	LCD_RS = 0;
	LCD_DB = in & 0x0F;
	LCD_load(); // load command into LCD
}

void LCD_Write_Command_8bit(unsigned char in_8){ //write single command into register (in_8 = aaaa bbbb)
	LCD_RS = 0;
	LCD_Write_Command((in_8) >> 4); //first 4 bits in 8bit command (aaaa)
	LCD_Write_Command((in_8)); //last 4 bits in 8bit command (bbbb)
}

void LCD_Write_4_DATA(unsigned char in){ //write single 4 bit data to LCD  ----DDDD
	LCD_RS = 0x08;
	LCD_DB = in & 0x0F; //we use 4-bit scheme, we intrested in DB4-7 --> PB0-3 (----DDDD)
	LCD_load(); // load data into LCD output ports;
}

void LCD_Write_8_DATA(unsigned char in){ //write 8bit data together to LCD DDDDDDDD
	// as we still using 4-bit scheme, therefore, this "in" need to be send in separate 2 times, 
	//first we will send it's higher 4 bit to DB4-7
	LCD_Write_4_DATA((in) >> 4); // D8 D7 D6 D5 D4 D3 D2 D1 -> 0000 D8 D7 D6 D5
	LCD_Write_4_DATA(in); // D8 D7 D6 D5 D4 D3 D2 D1 ->(due to mask in Write4 function that only D1-4 are got) --> 0000 D4 D3 D2 D1
}


void LCD_CursorSet(char row, char col){

   if(row == 0x00){
		 LCD_Write_Command_8bit(0x80 | 0x00 | col);
	 }else if(row == 0x01){
		 LCD_Write_Command_8bit(0x80 | 0x40 | col);
	 }else if(row == 0x02){
		 LCD_Write_Command_8bit(0x80 | 0x14 | col);
	 }else if(row == 0x03){
		 LCD_Write_Command_8bit(0x80 | 0x54 | col);
	 }

}

void LCD_Write_Str(char r, char c, char *str){ // display a string from sel-defined row/col
	LCD_CursorSet(r,c); //locate cursor to the defined position 
	cursor_c = c;
	cursor_r = r;
	int len = strlen(str);
	int i = 0;
	if(len < 20){ //one line need to dispay
		LCD_Write_Command_8bit(0x0F);	
		LCD_CursorSet(r,c);
		for(i = 0; i < len; i++){
			LCD_Write_8_DATA(str[i]);
		}
	}else if(len == 20){
		LCD_Write_Command_8bit(0x0F);	
		LCD_CursorSet(r,c);
		for(i = 0; i < 20; i++){
			LCD_Write_8_DATA(str[i]);
		}
    LCD_Write_Command_8bit(0x0C);			
	}
	else if(len > 20 && len <= 40){ //2 lines need to display
		LCD_CursorSet(r,c);
		LCD_Write_Command_8bit(0x0F);	
		for(i = 0; i < 20; i++){
			LCD_Write_8_DATA(str[i]);
		}
		//deal with switching part:
		LCD_Write_Command_8bit(0x0C); //if exceed, do not show cursor during switching rows
		cursor_r++;
		cursor_c = 0;
		LCD_CursorSet(cursor_r,cursor_c);
		Delay_4P1ms();
		LCD_Write_8_DATA(str[20]);
		Delay_4P1ms();
		//deal with remianing part:
		for(i = 21; i < len; i++){
			LCD_Write_Command_8bit(0x0F);	
			LCD_Write_8_DATA(str[i]);
		}			
	}

}
	



void LCD_Write_custom(char r, char c, unsigned char RAM, char *custum){ //display custum pack man 
	//5x8 only 5 bits can be used per char per row; 8 bits per col eg: 
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//{0,0,0,0,0}
	//RAM is CGRAM address

	int i;
	//write custum data into CGRAM
	for(i = 0; i < 8; i++){
		LCD_Write_Command_8bit(0x40 | (RAM*8) | i); //select address of CGRAM 
		LCD_Write_8_DATA(custum[i]);
	}
	
	//print custum data stored in CRRAM
	LCD_CursorSet(r,c);
	LCD_Write_8_DATA(RAM);
	//also write some initail words
	LCD_Write_Str(2,0,"Xianglin'Calculator");
	
	
}

void LCD_Write_custom_general(char r, char c, unsigned char RAM, char *custum){ //same with presviou case, but in general use
	//address of CGRAM: -00 0xxx, -00 1xxx, -01 0xxx, -01 1xxx   .....	
  // '-' = not need; xxx = 000->111 (8 lines) for one custum; 
	// therefore, first ensure which CGRAM (000) are used which is RAM*8 
	// then select which sub address for each line of custum "i" (i = 0 - 8[111])
	// overall: address of CGRAM for a specfic line used to store custum = 0x40 + RAM*8 + i 
	int i;
	for(i = 0; i < 8; i++){
		LCD_Write_Command_8bit(0x40 | (RAM*8) | i);
		LCD_Write_8_DATA(custum[i]);
	}
	LCD_CursorSet(r,c);
	LCD_Write_8_DATA(RAM);
}



void Clear_LCD(void){
	LCD_Write_Command_8bit(0x01);
}
void LCD_ReturnHome(void){
	LCD_Write_Command_8bit(0x02);
}


//follwoing function for LEDs connected to PA6 and PA7 are used for TEST ---- may be used for other purpose!
void Enable_Test_1_all(void){ //light up all PA6 and PA7
	TEST_POINT = 0xC0;
}
void Enable_Test_1_left(void){ //light up PA7
	TEST_POINT = 0x80;
}
void Enable_Test_1_right(void){ //light up PA6
	TEST_POINT = 0x40;
}
void Disable_Test_1(void){
	TEST_POINT = 0x00;
}

//patterns used for custum display 
char closed_halfface1[8] = {0x01,0x04,0x09,0x11,0x10,0x10,0x08,0x07};
char closed_halfface2[8] = {0x18,0x04,0x02,0x11,0x01,0x0F,0x02,0x1C};
//char open_halfface1[8] = {0x01,0x04,0x09,0x11,0x10,0x10,0x08,0x07};
char open_halfface2[8] = {0x18,0x04,0x02,0x16,0x08,0x0C,0x06,0x1C};
char pack_dot[8] = {0,0,0,0,0x0E,0x0E,0x0E,0}; 


void pack_man(){
	LCD_Write_custom(0,0,0,closed_halfface1);
	LCD_Write_custom(0,1,1,open_halfface2);
	LCD_Write_custom(0,2,3,pack_dot);
	LCD_Write_custom(0,3,3,pack_dot);
	LCD_Write_custom(0,4,3,pack_dot);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,1,0,closed_halfface1);
	LCD_Write_custom(0,2,2,closed_halfface2);
	LCD_Write_custom(0,3,3,pack_dot);
	LCD_Write_custom(0,4,3,pack_dot);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,1,0,closed_halfface1);
	LCD_Write_custom(0,2,1,open_halfface2);
	LCD_Write_custom(0,3,3,pack_dot);
	LCD_Write_custom(0,4,3,pack_dot);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,2,0,closed_halfface1);
	LCD_Write_custom(0,3,2,closed_halfface2);
	LCD_Write_custom(0,4,3,pack_dot);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,2,0,closed_halfface1);
	LCD_Write_custom(0,3,1,open_halfface2);
	LCD_Write_custom(0,4,3,pack_dot);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,3,0,closed_halfface1);
	LCD_Write_custom(0,4,2,closed_halfface2);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,3,0,closed_halfface1);
	LCD_Write_custom(0,4,1,open_halfface2);
	LCD_Write_custom(0,5,3,pack_dot);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,4,0,closed_halfface1);
	LCD_Write_custom(0,5,2,closed_halfface2);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,4,0,closed_halfface1);
	LCD_Write_custom(0,5,1,open_halfface2);
	LCD_Write_custom(0,6,3,pack_dot);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,5,0,closed_halfface1);
	LCD_Write_custom(0,6,2,closed_halfface2);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,5,0,closed_halfface1);
	LCD_Write_custom(0,6,1,open_halfface2);
	LCD_Write_custom(0,7,3,pack_dot);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,6,0,closed_halfface1);
	LCD_Write_custom(0,7,2,closed_halfface2);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,6,0,closed_halfface1);
	LCD_Write_custom(0,7,1,open_halfface2);
	LCD_Write_custom(0,8,3,pack_dot);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,7,0,closed_halfface1);
	LCD_Write_custom(0,8,2,closed_halfface2);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,7,0,closed_halfface1);
	LCD_Write_custom(0,8,1,open_halfface2);
	LCD_Write_custom(0,9,3,pack_dot);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
	LCD_Write_custom(0,8,0,closed_halfface1);
	LCD_Write_custom(0,9,2,closed_halfface2);
	Delay_10s();
	Clear_LCD();
	LCD_ReturnHome();
}
	


