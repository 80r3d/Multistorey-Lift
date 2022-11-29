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

#define		CAR_CALL_IP	P1

#define		CARG_I	P1_0
#define		CAR1_I	P1_1
#define		CAR2_I	P1_2
#define		CAR3_I	P1_3
#define		CAR4_I	P1_4
#define		CAR5_I	P1_5

#define		I_DR_ILK1	P1_6
#define		I_RCAM_ILK2	P1_7

#define  INSPECT_DN	  P1_0	
#define  INSPECT_UP	  P1_1



#define 	FLR_SWITCH_IP	P3


#define		FLRG_I	P3_0
#define		FLR1_I	P3_1
#define		FLR2_I	P3_2
#define		FLR3_I	P3_3
#define		FLR4_I	P3_4
#define		FLR5_I	P3_5

#define		I_STOP 	P3_6
#define		I_AT_MN	P3_7


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

void read_ip(void);
void floor_disp(void);

void init_lift(void);
void process_call(void);

void negl_on_floor(void);
void disp_call_no(char);
void check_car_call(void);
void chk_floor(void);

void disp_door_close_msg(void);
void disp_door_open_msg(void);
void disp_stop_msg(void);
void disp_up_msg(void);
void disp_down_msg(void);

void floor_door_open(void);
void car_door_open(void);

void lift_status_update(void);
void clear_all_status(void);
void car_pos_ok(void);



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


char bdata FLR_SWITCH _at_	0x20;

sbit FLRG	= FLR_SWITCH^0;
sbit FLR1	= FLR_SWITCH^1;
sbit FLR2	= FLR_SWITCH^2;
sbit FLR3	= FLR_SWITCH^3;
sbit FLR4 	= FLR_SWITCH^4;
sbit FLR5	= FLR_SWITCH^5;
sbit FLR6	= FLR_SWITCH^6;
sbit FLR7	= FLR_SWITCH^7;

char bdata CAR_CALL  _at_	0x21;
sbit CARG	= CAR_CALL^0;
sbit CAR1	= CAR_CALL^1;
sbit CAR2	= CAR_CALL^2;
sbit CAR3	= CAR_CALL^3;
sbit CAR4	= CAR_CALL^4;
sbit CAR5	= CAR_CALL^5;
sbit CAR6	= CAR_CALL^6;
sbit CAR7	= CAR_CALL^7;

char bdata FN_CALL	_at_	0x22;

sbit FCG	= FN_CALL^0;
sbit FC1	= FN_CALL^1;
sbit FC2	= FN_CALL^2;
sbit FC3	= FN_CALL^3;
sbit FC4	= FN_CALL^4;
sbit FC5	= FN_CALL^5;
sbit FC6	= FN_CALL^6;
sbit FC7	= FN_CALL^7;

char bdata	FLR_FLAG	_at_ 	0x23;

sbit b_FLG0	= FLR_FLAG^0 ;
sbit b_FLG1	= FLR_FLAG^1 ;
sbit b_FLG2	= FLR_FLAG^2 ;
sbit b_FLG3	= FLR_FLAG^3 ;
sbit b_FLG4	= FLR_FLAG^4 ;
sbit b_FLG5	= FLR_FLAG^5 ;
sbit b_FLG6	= FLR_FLAG^6 ;
sbit b_FLG7	= FLR_FLAG^7 ;


char bdata status1	_at_	0x24;

sbit f_PWR_ON	= status1^0;
sbit f_car_pos	= status1^1;
sbit f_no_call	= status1^2;
sbit f_idle		= status1^3;
sbit f_ilk1_ok	= status1^4;
sbit f_run		= status1^5;
sbit f_stop		= status1^6;
sbit f_ilk2_on	= status1^7;

char bdata status2	_at_	0x25;

sbit f_up_dir	= status2^0;
sbit f_dn_dir	= status2^1;
sbit f_fl_reach	= status2^2;
sbit f_lift_sts	= status2^3;
sbit f_auto	=	  status2^4;
sbit f_manual	= status2^5;
sbit f_cam_on	= status2^6;
sbit f_buz_on	= status2^7;


unsigned char data floor_value = 0,FL_DISP = '-', SEG_DATA ='-',GET_FLR_NO = 0,FLR_NUMBER = 0x0;	   

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

FLR_SWITCH = 0;
CAR_CALL = 0;
FN_CALL = 0;

FLR_SWITCH_IP = 0xff;
CAR_CALL_IP = 0xff;
read_ip();
floor_disp();
f_PWR_ON = 1;
init_lift();

while(1){
//P1_0 = 0;
//delay(10000);
//P1_0 = 1;
//delay(10000);
read_ip();
floor_disp();
init_lift();

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

//**************************************** Read inputs ***********************************************
void read_ip(){
unsigned char temp,c;

temp = FLR_SWITCH;
temp &=0x3f;
FLR_SWITCH = temp;

temp = FLR_SWITCH_IP;
temp = !temp;
floor_value = temp;
temp |= FLR_SWITCH;
FLR_SWITCH = temp;

temp = CAR_CALL;
temp &=0x3f;

if( temp == 0) {  temp = CAR_CALL_IP; temp = ~temp; temp |= CAR_CALL; CAR_CALL = temp;}

if( f_stop == 0) { negl_on_floor();} 

temp = CAR_CALL& 0x3f;

if(temp == 0) { c = '-';
				disp_call_no(c);
				f_no_call = 1; f_idle = 1;
				if(I_AT_MN ) { 
					 f_manual = 0;
					 if(!f_auto){ f_auto = 1; LCD_command(LCD_line3_adrs+15); LCD_string(msg_auto);} }
			    else { f_auto = 0; f_manual = 1;LCD_command(LCD_line3_adrs+15); LCD_string(msg_manual);}
			}
else {		
		f_no_call = 0;
		if(f_stop) { check_car_call();}
		FN_CALL = CAR_CALL & 0x3f;  
    }
}

//*******************************************************************************************
void disp_call_no(char c){
	 LCD_command(LCD_line4_adrs +5); 
	 LCD_data(c);
}
void disp_up_msg(){
	 LCD_command(LCD_line4_adrs +9); 
	 LCD_string(msg_up);
  }
void disp_down_msg(){
	 LCD_command(LCD_line4_adrs +9); 
	 LCD_string(msg_down);
  }
void disp_stop_msg(){
	 LCD_command(LCD_line4_adrs +9); 
	 LCD_string(msg_stop);
  }
void disp_door_close_msg(){
	 LCD_command(LCD_line2_adrs +5); 
	 LCD_string(label_door_close);
  }
void disp_door_open_msg(){
	 LCD_command(LCD_line2_adrs +5); 
	 LCD_string(label_door_open);
  }


//******************************************************************************************
void check_car_call(){
	if(FLRG) { if (CARG) { FLRG = 0; FLR_FLAG = 0; f_PWR_ON =1; FCG =0; CARG = 0;}}
	if(FLR1) { if (CAR1) { FLR1 = 0; FLR_FLAG = 0; f_PWR_ON =1; FC1 =1; CAR1 = 1;}}
	if(FLR2) { if (CAR2) { FLR2 = 0; FLR_FLAG = 0; f_PWR_ON =1; FC2 =2; CAR2 = 2;}}
	if(FLR3) { if (CAR3) { FLR3 = 0; FLR_FLAG = 0; f_PWR_ON =1; FC3 =3; CAR3 = 3;}}
	if(FLR4) { if (CAR4) { FLR4 = 0; FLR_FLAG = 0; f_PWR_ON =1; FC4 =4; CAR4 = 4;}}
	if(FLR5) { if (CAR5) { FLR5 = 0; FLR_FLAG = 0; f_PWR_ON =1; FC5 =5; CAR5 = 5;}}
}
//********************************************************************************
void negl_on_floor(){
		if (!f_run) {
				 	if(FLRG){ FCG = 0; CARG=0;}																999
				 	if(FLR1){ FC1 = 0; CAR1=0;}
				 	if(FLR2){ FC2 = 0; CAR2=0;}
				 	if(FLR3){ FC3 = 0; CAR3=0;}
				 	if(FLR4){ FC4 = 0; CAR4=0;}
					if(FLR5){ FC5 = 0; CAR5=0;}
					}
 }
//**********************************************************************************
void floor_disp() { 
 		FL_DISP = '-';
		chk_floor();
		SEG_DATA = FL_DISP;
		LCD_command(LCD_line3_adrs + 5);
		LCD_data(SEG_DATA);
		LCD_command(LCD_line4_adrs + 19);
		LCD_data(SEG_DATA);
 } 
//****************************************************************************
void chk_floor(){
 	unsigned char c,temp ;
	c = floor_value & 0x3f;	
	if(c == 0x00) { FL_DISP = SEG_DATA;}
	else {
		  temp = FLR_SWITCH & 0xc0;	
		  if( c == 0x01) { GET_FLR_NO = 1;FL_DISP = 'G'; temp |= 0x01; FLR_SWITCH =temp;}
		  if( c == 0x02) { GET_FLR_NO = 2;FL_DISP = '1'; temp |= 0x02; FLR_SWITCH =temp;}
		  if( c == 0x04) { GET_FLR_NO = 3;FL_DISP = '2'; temp |= 0x04; FLR_SWITCH =temp;}
		  if( c == 0x08) { GET_FLR_NO = 4;FL_DISP = '3'; temp |= 0x08; FLR_SWITCH =temp;}
		  if( c == 0x10) { GET_FLR_NO = 5;FL_DISP = '4'; temp |= 0x10; FLR_SWITCH =temp;}
		  if( c == 0x20) { GET_FLR_NO = 6;FL_DISP = '5'; temp |= 0x20; FLR_SWITCH =temp;}
		  }
}
//**************************************************************************************
void clear_all_status(){
if(!f_stop) { f_stop = 1; 
			  f_lift_sts = 1;  
			  if(Out_DN_RUN) { lift_status_update();}
			  Out_DN_RUN = 1;
			  f_lift_sts = 0;
			}
}

void lift_status_update(){
Out_UP_RUN = 1;
Out_DN_RUN = 1;
Out_R_CAM = 1;
f_up_dir = 0;
f_dn_dir = 0;
f_run = 0;
FN_CALL = 0x0;
CAR_CALL &= 0xc0;
f_no_call = 1;
f_fl_reach = 0;
disp_stop_msg();
}

void inspection(){
	if (INSPECT_UP) { if(!I_DR_ILK1){ Out_DN_RUN = 1; Out_UP_RUN = 0; Out_R_CAM = 0;} disp_up_msg(); }
	if (INSPECT_DN) { if(!I_DR_ILK1){ Out_UP_RUN = 1; Out_DN_RUN = 0; Out_R_CAM = 0;} disp_down_msg(); }
	lift_status_update();
}

//***************************************************************************************************
void floor_door_open(){
	 disp_door_open_msg();
	 Out_UP_RUN = 1;
	 Out_DN_RUN = 1;
	 Out_R_CAM = 1;
	 f_run = 0;
	 f_fl_reach = 0;
 	 disp_stop_msg();
}
void init_lift() {
		if(I_DR_ILK1){floor_door_open(); return;}
//		if(I_RCAM_ILK2) {car_door_open(); return;}
		disp_door_close_msg();
		 if(f_PWR_ON == 1) process_call();
		 if(f_run == 1) process_call();
}

void get_floor_position(){
		if(FLRG) { FLR_NUMBER = 0x01; GET_FLR_NO = 1; car_pos_ok(); }
		else if(FLR1) { FLR_NUMBER = 0x02; GET_FLR_NO = 2; car_pos_ok(); }
		else if(FLR2) { FLR_NUMBER = 0x04; GET_FLR_NO = 3; car_pos_ok(); }
		else if(FLR3) { FLR_NUMBER = 0x08; GET_FLR_NO = 4; car_pos_ok(); }
		else if(FLR4) { FLR_NUMBER = 0x10; GET_FLR_NO = 5; car_pos_ok(); }
		else if(FLR5) { FLR_NUMBER = 0x20; GET_FLR_NO = 6; car_pos_ok(); }
		else
			if(f_run) return ;
			if(f_fl_reach) return;
			f_car_pos = 1;
			Out_R_CAM = 0;
			FN_CALL = 0x0;
			CAR_CALL &= 0xc0;
			f_run = 1;
		if( f_stop) { 
						if(f_lift_sts) {Out_DN_RUN = 0; f_dn_dir = 1; f_up_dir =0; f_stop = 0; disp_down_msg(); }
			  			 else {Out_UP_RUN = 0; f_dn_dir = 0; f_up_dir = 1; f_stop = 0; disp_up_msg(); }
						 }
			else  											   
				{Out_DN_RUN = 0; f_dn_dir = 1; f_up_dir =0;f_stop = 0; disp_down_msg();	}			
		 
		//	get_car_position();
}	
void car_pos_ok(){
   unsigned char temp;
   temp = FLR_SWITCH;
   temp &= 0xc0;
   temp |= FLR_NUMBER;
   FLR_SWITCH = temp;
   f_PWR_ON= 0;
   CAR_CALL &= 0xc0;
   FN_CALL = CAR_CALL;
   f_stop = 0;	
   f_car_pos = 0;
   Out_UP_RUN = 1;
   Out_DN_RUN = 1;
   Out_R_CAM = 1;
   f_up_dir = 0;
   f_dn_dir = 0;
   f_run = 0;
   f_fl_reach = 1;
   floor_disp();
   //off_delay_100ms(); not defined
   disp_stop_msg();
}


void process_call(){
		if(f_manual) { inspection();return;}
		if(!I_STOP) { clear_all_status(); return;}
		if(f_PWR_ON) { get_floor_position(); return;}
		if ( f_no_call) { FLR_FLAG =0x0; return;}
		
		if(FLRG) { GET_FLR_NO = 1; b_FLG0 = 1; b_FLG1 = 0;
					if( b_FLG2) { b_FLG2 = 0; }
					if( b_FLG3)	{ b_FLG3 = 0; }

}
}
