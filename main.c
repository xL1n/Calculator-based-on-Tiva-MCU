#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

////////////////////////////////////
#include "PLL.h"
#include "LCD_init.h"
#include "Key_init.h"
//SysTick_Wait(unsigned long delay);
////////////////////////////////////
char input[50];
char result_value_buffers[9]; //a buffer stores converted string value from float value 
//char rd;
float result_value = 0.0;

char output_Q[50][20];
char stack[50];

char all[50][20];
int exp_ct = 0;
		
float stack2[50];

char ANS_str[7];
float cal(char *input);

char rd;
int limit = 0;

//arrays used in function solving 
char fuc_input[50] = "";
int fuc_counter = 0;
char fuc_used[10][20]; //numbers 
char fuc_wate[10]; //separation between each numbers
float fuc_number[10];
int fuc_ncount = 0;
int fuc_wcout = 0;
float X1 = 0;
float X2 = 0;
int next = 0;
int non_real = 0;

void Write_to_function_solving();
void function_solving_mian();
void function_solving_one(char *fuc_express);
void function_solving_mian2();
void function_solving_two(char *fuc_express);

int equal = 0; //is equal sign pressed ? 
int null = 0;  //after pressing equal sign, to you want to restart another calculation ? (back to start)

char password_correct[15] = "666666"; //initial set pass
char password_entered[15];
char sel; //used to select functions  1 = cal 2 = setpass

int password_correct_counter = 0;//count how much sign/number has inputed to password
int password_entered_counter = 0;//count how much sign/number has inputed to password

int correct = 0; //check if password entered is correct

int input_counter = 0; //count how much sign/number has inputed 
void Write_to_Input();
void cal_main(); //calculator process - this function is not about how excat calculator works [cal_func(char expression[]) is the function about how to calculate], 
                 // but above write expression, deliver to caluclator function, after calculation, home the LCD

char cal_symbol[8] = {0x1F,0x11,0x1F,0x1B,0x15,0x1B,0x15,0x1F}; 
char key_symbol[8] = {0x04,0x06,0x07,0x04,0x0A,0x11,0x11,0x0E};
char fuc_symbol[8] = {0x00,0x0E,0x02,0x06,0x04,0x4,0x0,0x04};	
void Write_to_Pass_correct();
void Write_to_Pass_entered();
void start();
void pass_set();
void pass_check();
void pass_start();



int main(){  
//for time init
  PLL_Init();  
	SysTick_Init();
//for keypad and LCD init
	keypadInit();
	PortABInit();
	LCD_init();
  pack_man();
////////////////////////////////////
	while(1){
		
   pass_start(); //start with checking password
   //start(); //start without password checking ---for test 

  }
	
}

void pass_start(){
		Clear_LCD();
	  Delay_15ms();
		LCD_ReturnHome();  
		while(1){
	    pass_check();
	    if(correct == 1){
        start();
	}
 }
}

void start(){	
	while(1){
		LCD_Write_Str(0x00,0,"Select Functions");
		LCD_Write_Str(0x01,0,"1calculate");
		LCD_Write_Str(0x02,0,"2ResetPass");
		LCD_Write_Str(0x03,0,"3FuncCalcu");
		LCD_Write_custom_general(1,0x0B,0,cal_symbol);
		LCD_Write_custom_general(2,0x0F,1,key_symbol);
		LCD_Write_custom_general(3,0x0F,2,fuc_symbol);
		sel = readKeypad();
		//sel = '1';
		switch(sel){
			case '1':
				memset(input,0,sizeof(input));
				Clear_LCD();
			  Delay_15ms();
			  LCD_ReturnHome();
			  equal = 0;
			  limit = 0;
			  input_counter = 0;
			  Delay_40ms();
			  while(1){
				  cal_main();
			  }
			break;
			
			case '2':
			  memset(password_correct,0,sizeof(password_correct));
			  Clear_LCD();
			  LCD_ReturnHome();
     	  password_correct_counter = 0;
			  equal = 0;
			  while(1){
				  pass_set();
				}
			break;
				
			case '3':
			 	  memset(fuc_input,0,sizeof(fuc_input));
			    Clear_LCD();
			    LCD_ReturnHome();
			    equal = 0;
			    Delay_40ms();
		 while(1){
			 LCD_Write_Str(0x00,0x00,"Select Order:");
			 LCD_Write_Str(0x01,0x00,"1:ax+b=0");
			 LCD_Write_Str(0x02,0x00,"2:ax^2+bx+c=0");
			 char order = readKeypad();
			 X1 = 0;
			 X2 = 0;
			  if(order == '1'){
			 	  memset(fuc_input,0,sizeof(fuc_input));
			    Clear_LCD();
			    LCD_ReturnHome();
			    equal = 0;
			    Delay_40ms();
			    while(1){
				    function_solving_mian();
				  }
				}else if(order == '2'){
			 	  memset(fuc_input,0,sizeof(fuc_input));
			    Clear_LCD();
			    LCD_ReturnHome();
			    equal = 0;
			    Delay_40ms();
			    while(1){
				    function_solving_mian2();
				  }
				}
			}
			break;				
		}
	}	
	
}

//reading function, store the read value from keyboard to input string, and keep storing until = pressed
void Write_to_Input(){
	char read = readKeypad();
	if(read == '1'){
		strcat(input,"1");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '2'){
		strcat(input,"2");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '3'){
		strcat(input,"3");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '4'){
		strcat(input,"4");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '5'){
		strcat(input,"5");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '6'){
		strcat(input,"6");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '7'){
		strcat(input,"7");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '8'){
		strcat(input,"8");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '9'){
		strcat(input,"9");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '0'){
		strcat(input,"0");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '+'){
		strcat(input,"+");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '-'){
		strcat(input,"-");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '.'){
		strcat(input,".");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '='){ //when equal sign is pressed, then change the equal flag to get the value instead of keep reading
		equal = 1;
		input_counter = 0; //when calcualted, restart input counter for next caculation
		Delay_40ms();
		
	}else if(read == 'D'){ //delate last element in that string
		input[input_counter - 1] = '\0';
		LCD_Write_Str(0x00,0x00,input);
		Delay_10s();
		input_counter--;
		
	}else if(read == 'x'){
		strcat(input,"x");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '/'){
		strcat(input,"/");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == '('){
		strcat(input,"(");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == 'e'){
		strcat(input,"e");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
		
	}else if(read == ')'){
		strcat(input,")");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
	
	}else if(read == 'A'){
		strcat(input,"A");
		LCD_Write_Str(0x00,0x00,input);
		Delay_40ms();
		input_counter++;
	}
		
	
	
}


//
void Write_to_Pass_correct(){
	char read = readKeypad();
	if(read == '1'){
		strcat(password_correct,"1");
		LCD_Write_Str(1,0,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '2'){
		strcat(password_correct,"2");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '3'){
		strcat(password_correct,"3");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '4'){
		strcat(password_correct,"4");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '5'){
		strcat(password_correct,"5");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '6'){
		strcat(password_correct,"6");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '7'){
		strcat(password_correct,"7");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '8'){
		strcat(password_correct,"8");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '9'){
		strcat(password_correct,"9");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
		
	}else if(read == '0'){
		strcat(password_correct,"0");
		LCD_Write_Str(1,0x00,password_correct);
		Delay_40ms();
		password_correct_counter++;
 }else if(read == '='){ //when equal sign is pressed, then change the equal flag to get the value instead of keep reading
		equal = 1;
		password_correct_counter = 0; //when calcualted, restart input counter for next caculation
 }
}




//
void Write_to_Pass_entered(){
	char read = readKeypad();
	if(read == '1'){
		strcat(password_entered,"1");
		LCD_Write_Str(1,0,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '2'){
		strcat(password_entered,"2");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '3'){
		strcat(password_entered,"3");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '4'){
		strcat(password_entered,"4");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '5'){
		strcat(password_entered,"5");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '6'){
		strcat(password_entered,"6");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '7'){
		strcat(password_entered,"7");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '8'){
		strcat(password_entered,"8");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '9'){
		strcat(password_entered,"9");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
		
	}else if(read == '0'){
		strcat(password_entered,"0");
		LCD_Write_Str(1,0x00,password_entered);
		Delay_40ms();
		password_entered_counter++;
 }else if(read == '='){ //when equal sign is pressed, then change the equal flag to get the value instead of keep reading
		equal = 1;
		password_entered_counter = 0; //when calcualted, restart input counter for next caculation
 }
}






//
void Write_to_function_solving(){
	char read = readKeypad();
	if(read == '1'){
		strcat(fuc_input,"1");
		LCD_Write_Str(0x02,0,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '2'){
		strcat(fuc_input,"2");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '3'){
		strcat(fuc_input,"3");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '4'){
		strcat(fuc_input,"4");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '5'){
		strcat(fuc_input,"5");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '6'){
		strcat(fuc_input,"6");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '7'){
		strcat(fuc_input,"7");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '8'){
		strcat(fuc_input,"8");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '9'){
		strcat(fuc_input,"9");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
		
	}else if(read == '0'){
		strcat(fuc_input,"0");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
 }else if(read == '='){ //when equal sign is pressed, then change the equal flag to get the value instead of keep reading
		equal = 1;
		fuc_counter = 0; //when calcualted, restart input counter for next caculation
 }else if(read == '+'){ //next parameter
		strcat(fuc_input,";");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
 }else if(read == '.'){
		strcat(fuc_input,".");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
	}else if(read == '-'){
		strcat(fuc_input,"-");
		LCD_Write_Str(0x02,0x00,fuc_input);
		Delay_40ms();
		fuc_counter++;
	}
}
void cal_main(){
  if(equal == 0){
			Write_to_Input();
			Delay_40ms(); //evey time you read from keypad, add bit delay so avoid keep pressing 
		}else{
			rd = readKeypad();
			Delay_40ms();
			if(rd == 'D'){
				equal = 0;
				limit = 0;
				Clear_LCD();
				LCD_ReturnHome();
				input_counter = 0;
				
				Disable_Test_1();
				
				start();
			}else{
				 if(limit == 0){
					 Clear_LCD();
					 Delay_15ms();
					 LCD_ReturnHome();

					 int i = 0;

			     float value = cal(input); //get final value
			     //char result_value_buffers[9] = ""; //a buffer stores converted string value from float value 

					 //sprintf(result_value_buffers, "%.5f", value);
           snprintf(result_value_buffers, 9, "%.5f", value); //converted string value into string
					 
					 LCD_Write_Str(0x00,0x00,input); //still be able to see your expression 
			     LCD_Write_Str(0x03,0x03,"=");
			     LCD_Write_Str(0x03,0x09, result_value_buffers); //display final calculated value at bottom 
					 
					 //store previous ANS into another array used for next cal
					 for(i = 0; i < 7; i++){
						 ANS_str[i] = result_value_buffers[i];
					 }
					
					 
					 //clear all the storages used in cal function which preper for next calculation ----->
					 memset(input,0,sizeof(input)); //clear input array (expression)

					 memset(result_value_buffers,0,sizeof(result_value_buffers)); //clear output string
					 

					 for(i = 0; i < 50; i++){
						 output_Q[i][0] = '\0';	 
					 }
					 
					 for(i = 0; i < 50; i++){
						 stack[i] = '\0';
					 }			

					 for(i = 0; i < 50; i++){
						 stack2[i] = 0;
					 }	
					 
					 for(i = 0; i < 50; i++){
						 all[i][0] = '\0';
					 }
					 
           exp_ct = 0;
					 
					 
		       limit = 1; //turn limit to 1 which means no more calculation if 'D' not pressed					 
					 
			   }
		 
				 
			}

		}		
}

//
void pass_set(){
		if(equal == 0){	
      LCD_Write_Str(0,0,"ReSet your Password");
      Delay_40ms();			
			Write_to_Pass_correct();
			Delay_40ms(); //evey time you read from keypad, add bit delay so avoid keep pressing 
		}else if (equal == 1){ //password set
			equal = 0;
			correct = 0;
			pass_start(); //back to main 
			
		}
}


//
void pass_check(){
    
		if(equal == 0){
			LCD_Write_Str(0,0,"PleaseEnter Password");

			Write_to_Pass_entered();
			Delay_40ms(); //evey time you read from keypad, add bit delay so avoid keep pressing 

		}else if (equal == 1){ //password set

			int i = 0;
			int correct_count = 0;
			for(i = 0; i < sizeof(password_correct); i++){
				if(password_entered[i] == password_correct[i]){
					correct_count ++;
				}
			}
			if(correct_count == 15){
		    memset(password_entered,0,sizeof(password_entered));
			  Clear_LCD();
			  Delay_4P1ms();
     	  password_correct_counter = 0;
				correct = 1;
				start(); //back to main 
			 }else{
				correct = 0;
				LCD_Write_Str(3,0,"WRONG!");
				Delay_10s();
				Delay_10s();
		    memset(password_entered,0,sizeof(password_entered));
			  Clear_LCD();
				LCD_ReturnHome();
			  Delay_4P1ms();
     	  password_correct_counter = 0;
				equal = 0;
				pass_start();
				 
			 }
			
		}
}
//
void function_solving_mian(){
	if(equal == 0){
		LCD_Write_Str(0x00,0x00,"ax + b = 0");
		Write_to_function_solving();
		Delay_40ms();
	}else{
			rd = readKeypad();
			Delay_40ms();
			if(rd == 'D'){
				equal = 0;
				limit = 0;
				X1 = 0;
				Clear_LCD();
				LCD_ReturnHome();
				start();
			}else{
				 if(limit == 0){
					 Clear_LCD();
					 LCD_ReturnHome();
					 function_solving_one(fuc_input);

			     char X1_buf[9] = ""; //a buffer stores converted string value from float value 
           snprintf(X1_buf, 8, "%f", X1); //converted string value into string
					 
			     LCD_Write_Str(1,0,"Result is:");
					 LCD_Write_Str(0x02,0,"x = ");
			     LCD_Write_Str(0x02,0x07, X1_buf); //display final calculated value 
					 
					 
					 //clear all the storages used in cal function which preper for next calculation ----->
					 memset(fuc_input,0,sizeof(fuc_input)); //clear input array (expression)

					 memset(X1_buf,0,sizeof(X1_buf)); //clear output string
					 
           for(int i = 1; i < sizeof(fuc_wate) / sizeof(fuc_wate[0]); ++i){ //clear operators
                fuc_wate[i] = '\0';
           }
					 
           for (int i = 0; i < sizeof(fuc_used) / sizeof(fuc_used[0]); ++i) {
                memset(fuc_used[i], 0, sizeof(fuc_used[i])); // clear numbers string 
           }				
					 
           for (int i = 0; i < sizeof(fuc_number) / sizeof(fuc_number[0]); ++i) { //clear array of float numbers
                fuc_number[i] = 0.0;
           }
           fuc_ncount = 0; //reset numCount for next cal
           fuc_wcout = 0;	 //reset opCount for next cal
					 
		       limit = 1; //turn limit to 1 which means no more calculation if 'D' not pressed 
					 X1 = 0;
			   }
				 
				 
			}

		}	
			
}
void function_solving_one(char *fuc_express){
    int i = 0;
    for (i = 0; i < strlen(fuc_express); i++) {
        if (isdigit(fuc_express[i])|| fuc_express[i] == '.'|| fuc_express[i] == '-') {
            int j = 0;
            while (isdigit(fuc_express[i]) ||  fuc_express[i] == '.'|| fuc_express[i] == '-') {
                fuc_used[fuc_ncount][j] = fuc_express[i];
						    j++;
						    i++;
            }
            fuc_ncount++;
            i--;
        }
    }
    i = 0;	
		//convert values into float 
    for(i = 0; i < 10 ; i++){
        fuc_number[i] = atof(fuc_used[i]);
    }
    i = 0;
		X1 = - (fuc_number[1] / fuc_number[0]);
}

void function_solving_mian2(){
	if(equal == 0){
		LCD_Write_Str(0x00,0x00,"ax^2 + bx + c = 0");
		Write_to_function_solving();
		Delay_40ms();
	}else{
			rd = readKeypad();
			Delay_40ms();
			if(rd == 'D'){
				equal = 0;
				limit = 0;
				X1 = 0;
				X2 = 0;
				Clear_LCD();
				LCD_ReturnHome();
				Delay_15ms();
				start();
			}else{
				 if(limit == 0){
					 Clear_LCD();
					 LCD_ReturnHome();
					 Delay_15ms();
					 function_solving_two(fuc_input);
			     char X1_buf[9] = ""; //a buffer stores converted string value from float value 
					 char X2_buf[9] = ""; //a buffer stores converted string value from float value 
					 if(non_real == 0){
             snprintf(X1_buf, 8, "%f", X1); //converted string value into string
					   snprintf(X2_buf, 8, "%f", X2); //converted string value into string
					   LCD_Write_Str(0x00,0x02,"x1 = ");
			       LCD_Write_Str(0x00,0x09, X1_buf); //display final calculated value 
					   LCD_Write_Str(0x02,0x02,"x2 =");
			       LCD_Write_Str(0x02,0x09, X2_buf); //display final calculated value 

					 }else if(non_real == 1){
						 Clear_LCD();
					   LCD_ReturnHome();
						 Delay_15ms();
						 LCD_Write_Str(0x00,0x00,"No");
						 LCD_Write_Str(0x01,0x00,"Real");
						 LCD_Write_Str(0x02,0x00,"Solutions");
					 }
					memset(X1_buf,0,sizeof(X1_buf)); //clear output string
					memset(X2_buf,0,sizeof(X2_buf)); //clear output string					 
					 //clear all the storages used in cal function which preper for next calculation ----->
					 memset(fuc_input,0,sizeof(fuc_input)); //clear input array (expression)


					 
           for(int i = 1; i < sizeof(fuc_wate) / sizeof(fuc_wate[0]); ++i){ //clear operators
                fuc_wate[i] = '\0';
           }
					 
           for (int i = 0; i < sizeof(fuc_used) / sizeof(fuc_used[0]); ++i) {
                memset(fuc_used[i], 0, sizeof(fuc_used[i])); // clear numbers string 
           }				
					 
           for (int i = 0; i < sizeof(fuc_number) / sizeof(fuc_number[0]); ++i) { //clear array of float numbers
                fuc_number[i] = 0.0;
           }
           fuc_ncount = 0; //reset numCount for next cal
           fuc_wcout = 0;	 //reset opCount for next cal
					 
		       limit = 1; //turn limit to 1 which means no more calculation if 'D' not pressed 
					 X1 = 0;
					 X2 = 0;
					 non_real = 0;
			   }
				 
				 
			}

		}	
			
}
void function_solving_two(char *fuc_express){
    int i = 0;
    for (i = 0; i < strlen(fuc_express); i++) {
        if (isdigit(fuc_express[i])|| fuc_express[i] == '.'|| fuc_express[i] == '-') {
            int j = 0;
            while (isdigit(fuc_express[i])|| fuc_express[i] == '.'|| fuc_express[i] == '-') {
                fuc_used[fuc_ncount][j] = fuc_express[i];
						    j++;
						    i++;
            }
            fuc_ncount++;
            i--;
        }
    }
    i = 0;	
		//convert values into float 
    for(i = 0; i < 10 ; i++){
        fuc_number[i] = atof(fuc_used[i]);
    }
    i = 0;
		float a = fuc_number[0];
		float b = fuc_number[1];
		float c = fuc_number[2];
		float checking = pow(b,2) - 4*a*c;
		if(checking < 0){
			non_real = 1;
		}else{
			X1 = (-b + sqrt((pow(b,2) - 4*a*c))) / (2*a);
			X2 = (-b - sqrt((pow(b,2) - 4*a*c))) / (2*a);
		}
}






float cal(char *input){

    float result;

    int i = 0;

    for (i = 0; i < 50; i++) {
			//case if is number or a dot
        if (isdigit(input[i]) || input[i] == '.') {
            int j = i;
            while (isdigit(input[j]) || input[j] == '.') {
                j++; // Count how many characters belong to one "number" (start from i th to j th)
            }

            int k;
            for (k = 0; i < j; i++) { //current element in all[] = from i th to j th element in expression str
                all[exp_ct][k] = input[i];
							  k++; //next char in that string (still in same element)
            }
            all[exp_ct][k] = '\0'; //full stop of current element
            exp_ct++;//next element in expres
            i--;  //minus one, otherwise one char missing 
        } 
				//case if is a operator or "A" (only one char)
        else if (input[i] == 'A' || input[i] == '+' || input[i] == '-' || input[i] == 'x' || input[i] == '/' || input[i] == 'e' || input[i] == '(' || input[i] == ')') {
            all[exp_ct][0] = input[i];
            all[exp_ct][1] = '\0'; 
            exp_ct++;
        } 
    }
		i = 0;

    int output_ct = 0;
    int stack_ct = 0;
		int stack2_ct = 0;
    //first is a number that can be directly stored into output
    strcpy(output_Q[0], all[0]);
    output_ct++;


    //first stack is a an opterator -- any, so that coming ones in the future can have comparison
    stack[stack_ct] = all[1][0];
    stack_ct ++;    

    
 //Reverse Polish Notation:
    for (i = 2; i < 50; i++) {
        //if number, go to output queue
        if (isdigit(all[i][0])) { //if any element's first element is a number (digit), then it means this element is a number (as the other case, operators do not have digit)
					   strcpy(output_Q[output_ct], all[i]);
             output_ct++;
        } 
				
				//if contains A (ANS), use previous converted ANS and as it's a "number", hence treat as normal number
				else if(all[i][0] == 'A'){
					strcpy(output_Q[output_ct], ANS_str);
					output_ct++;
				}
        
        //if "(", go to top of stack 
        else if (all[i][0] == '(') {
            stack[stack_ct] = '(';
            stack_ct++;
        } 
        
        
        //if ")", bring all the ops out until meets "(" in stack 
        else if (all[i][0] == ')') {
            while (stack[stack_ct - 1] != '(') {
                output_Q[output_ct][0] = stack[stack_ct - 1]; //bring ops from stack to output 
                stack[stack_ct - 1] = '\0'; //clear current stored op (as it's gone to output)
                output_ct++;
                
                if(stack_ct >= 0){
                   stack_ct--; //keep on down searching if not meet "("
                }else{stack_ct = 0;}
            }

            if (stack[stack_ct - 1] == '(') { //meet "("
                stack[stack_ct - 1] = '\0'; // remove '('

                if(stack_ct >= 0){ //ensure stack will not be negative as there is no space in:   stack [-1]
                   stack_ct--; //make the counter down to the place where "(" used to locate (as it's empty now for next op)
                }else{stack_ct = 0;}
            }
        } 
        
        
        
        //if operators: 
        //case 1: lower operators ---- if meets higher or equal level op on top of stack, higher order op comes to output, after move, keep on comparing with next op, until condition not successful 
        else if (all[i][0] == '+' || all[i][0] == '-') {
            while (stack[stack_ct - 1] == 'x' || stack[stack_ct - 1] == '/' ||stack[stack_ct - 1] == '+' ||stack[stack_ct - 1] == 'e' || stack[stack_ct - 1] == '-') { //if "+ -", all ops needs to move as it's lowest output_Q[output_ct][0] = stack[stack_ct - 1];
            output_Q[output_ct][0] = stack[stack_ct - 1];   
						stack[stack_ct - 1] = '\0';
                output_ct++;

                if(stack_ct >= 0){
                   stack_ct--; 
                }else{stack_ct = 0;}

            }
            stack[stack_ct] = all[i][0]; //until condition not successful (no more move), push this to the stack
            stack_ct++; //next location of stack for next op comes 
        } 
        
        //case 2: Higher opertors ---- if meets lower, go to stack, meets equal level, move to out, keep on doing .....
        else if (all[i][0] == 'x' || all[i][0] == '/'|| all[i][0] == 'e') {
            while (stack[stack_ct - 1] == 'x' || stack[stack_ct - 1] == '/'|| stack[stack_ct - 1] == 'e') {
                output_Q[output_ct][0] = stack[stack_ct - 1];
                stack[stack_ct - 1] = '\0';
                output_ct++;

                if(stack_ct >= 0){
                   stack_ct--; 
                }else{stack_ct = 0;}

            }
            stack[stack_ct] = all[i][0];
            stack_ct++;
        }
    }
		i = 0;


    //after all management, then detect if there are any left in stack, move out by order to output queue
    while (stack_ct > 0) {
        output_Q[output_ct][0] = stack[stack_ct - 1];
        stack[stack_ct - 1] = '\0';
        output_ct++;
        stack_ct--;
    }

    // Reset stack2 and stack2_ct for RPN calculation

   for(i = 0; i < 50; i ++){

    if(isdigit(output_Q[i][0])){
        stack2[stack2_ct] = atof(output_Q[i]);
        stack2_ct++;				
    }
    else if(output_Q[i][0] == '+'||output_Q[i][0] == '-'||output_Q[i][0] == 'x'||output_Q[i][0] == '/'||output_Q[i][0] == 'e'){
        if(output_Q[i][0] == '+'){
            float buf = stack2[stack2_ct - 1] + stack2[stack2_ct - 2]; //bring last 2 numbers stored int stack out for calculation 
            stack2_ct --; //2 number's being calculated, index is now pointing to the next number that will be coming to stack, hence for current case, moved down by 1.
            stack2[stack2_ct] = '\0'; //as moved one down, this number is used for calculation, hence clear it.
            stack2_ct --;//move down one more
            stack2[stack2_ct] = buf; //replace current element (which is one of the operated number by the buffered result)
            stack2_ct++; //index move to next position in stack2
        }

        else if(output_Q[i][0] == '-'){
            float buf = stack2[stack2_ct - 2] - stack2[stack2_ct - 1];
            stack2_ct --; //2 number's being calculated, index moved down by 1, and replace one with buffer
            stack2[stack2_ct] = '\0';
            stack2_ct --;
            stack2[stack2_ct] = buf; 
            stack2_ct++; 
        }

        else if(output_Q[i][0] == 'x'){
            float buf = stack2[stack2_ct - 2] * stack2[stack2_ct - 1]; 
            stack2_ct --; //2 number's being calculated, index moved down by 1, and replace one with buffer
            stack2[stack2_ct] = '\0';
            stack2_ct --;
            stack2[stack2_ct] = buf;
            stack2_ct++;
        }

        else if(output_Q[i][0] == '/'){
            float buf = stack2[stack2_ct - 2] / stack2[stack2_ct - 1]; 
            stack2_ct --; //2 number's being calculated, index moved down by 1, and replace one with buffer
            stack2[stack2_ct] = '\0';
            stack2_ct --;
            stack2[stack2_ct] = buf;
            stack2_ct++; 
        }

        else if(output_Q[i][0] == 'e'){
            float buf = stack2[stack2_ct - 2] * pow(10, stack2[stack2_ct - 1]);
            stack2_ct --; //2 number's being calculated, index moved down by 1, and replace one with buffer
            stack2[stack2_ct] = '\0';
            stack2_ct --;
            stack2[stack2_ct] = buf; 
            stack2_ct++;
        }

    }


 }
  i = 0;
 result = stack2[0];   
 return result;

}
