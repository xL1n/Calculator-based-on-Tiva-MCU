#ifndef LCDinit_H
#define LCDinit_H

void PortB_Init(void);
void PortA_Init(void);
void PortABInit(void);

void LCD_init(void);

void LCD_load(void);

void LCD_Write_Command(unsigned char in);

void LCD_Write_4_DATA(unsigned char in);

void LCD_Write_8_DATA(unsigned char in);

void LCD_locate(unsigned int R, unsigned int C);

void Clear_LCD(void);

void LCD_Write_Str(char y, char x, char *str);
void LCD_CursorSet(char row,char col);

void Enable_Test_1_all(void);
void Enable_Test_1_right(void);
void Enable_Test_1_left(void); 
void Disable_Test_1(void);
void LCD_Write_custom(char r, char c, unsigned char RAM, char *custum);
void LCD_Write_custom_general(char r, char c, unsigned char RAM, char *custum);
void pack_man();
void LCD_ReturnHome(void);

#endif