#include <stdio.h>

#include "AT89X52.h"

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */

#define 	ON		0
#define		OFF		1

#define 	Out_DN_RUN	P2_1
#define		Out_UP_RUN	P2_2
#define 	Out_R_CAM	P2_3
#define 	Out_DR_BUZ	P2_4

#define		CAR_CALL	P1

#define		CARG	P1_0
#define		CAR1	P1_1
#define		CAR2	P1_2
#define		CAR3	P1_3
#define		CAR4	P1_4
#define		CAR5	P1_5

#define		I_DR_ILK1	P1_6
#define		I_RCAM_ILK2	P1_7

#define 	FLR_SWITCH	P3

#define		FLRG	P3_0
#define		FLR1	P3_1
#define		FLR2	P3_2
#define		FLR3	P3_3
#define		FLR4	P3_4
#define		FLR5	P3_5

#define		I_STOP 	P3_6
#define		I_AM_MN	P3_7

//************************* LCD intialization .. 4 line * 20 char ***********************

#define		LCD_RS	P2_7
#define		LCD_RW	0
#define		LCD_EN	P2_6

#define		LCD_PORT	P0


#define		LCD_line1_adrs	0x80
#define		LCD_line2_adrs	0xc0
#define		LCD_line3_adrs	0x94
#define		LCD_line4_adrs	0xd4

#define		LCD_config1		0x38				  // 8 bit >2 line
#define 	LCD_config2		0x0C				  // no flash autoincrement
#define		LCD_config3		0x10
#define		LCD_config4		0x06
#define 	LCD_clear		0x01				 	// lcd display clear



//****************** Function prototypes ********************************************************
        
void LCD_command(char );
void LCD_data(char);
void LCD_string( char[]);


void delay(int);		   
void LCD_init(void);
void LCD_welcome_msg(void);
void LCD_default_msg(void);



const char  code line1[] = "      Swami Sriji   " ;
const char  code line2[] = " Texmex Instruments " ;
const char  code line3[] = "     Ahmedabad      " ;
const char  code line4[] = "    M- 9427553718   " ;

const char code msg_stop[] = "STOP";
const char code msg_auto[] = "AUTO";
const char code msg_manual[] = "MAN ";
const char code msg_up[] = " UP ";
const char code msg_down[] = "DOWN";
const char code label_door_close[] = "CLOSE";
const char code label_door_open[] = "OPEN ";

const char code label_company_name[] = " JP Asssociates ";
const char code label_door[] = "DOOR:";
const char code label_mode[] = "MODE";
const char code label_call[] = "CALL:-";
const char code label_lift[] = "LIFT:-";
const char code label_ritz[] = "RITZ:-";

//************************ Main programme startes here ************************************

void main(void){

/*------------------------------------------------
Setup the serial port for 1200 baud at 16MHz.
------------------------------------------------*/
#ifndef MONITOR51
    SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr      */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 221;                /* TH1:  reload value for 1200 baud @ 16MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    TI    = 1;                  /* TI:   set TI to send first char of UART    */
#endif

LCD_init();





while(1){
//printf("Jai swaminarayan\n");

//P1 ^= 0x55;
P1_0 = 0;
delay(10000);
P1_0 = 1;
delay(10000);

}

}

void delay(int k){
int i =0;
for (i =0;i<k;i++) { }
}


void LCD_init(void) {
LCD_command(LCD_config1);
LCD_command(LCD_config2);
LCD_command(LCD_config3);
LCD_command(LCD_config4);
LCD_command(LCD_clear);

LCD_welcome_msg();
delay(1000);
LCD_default_msg();
				  
}

void LCD_welcome_msg(){
LCD_command(LCD_line1_adrs);
LCD_string(line1);
LCD_command(LCD_line2_adrs);
LCD_string(line2);
LCD_command(LCD_line3_adrs);
LCD_string(line3);
LCD_command(LCD_line4_adrs);
LCD_string(line4);
}

void LCD_default_msg(){
LCD_command(LCD_clear);
LCD_string(label_company_name);
LCD_command(LCD_line2_adrs);
LCD_string(label_door);
LCD_command(LCD_line2_adrs + 15);
LCD_string(label_mode);
LCD_command(LCD_line3_adrs);
LCD_string(label_lift);
LCD_command(LCD_line4_adrs);
LCD_string(label_call);
LCD_command(LCD_line4_adrs + 15);
LCD_string(label_ritz);
}


void LCD_string(char *s){
while( *s != '\0'){
 LCD_data(*s++);
}
}

void LCD_data(char d){
LCD_RS = 1;
LCD_PORT = d;
LCD_EN =1;
delay(10);
LCD_EN = 0;
delay(10);
}

void LCD_command(char cmd){
LCD_RS = 0;
LCD_PORT = cmd;
LCD_EN =1;
delay(10);
LCD_EN = 0;
delay(10);
}
