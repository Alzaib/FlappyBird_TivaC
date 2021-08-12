//MSE 402 Demo: Project Release 2.0
//Game: Flappy Bird

//Team #10 Members: 
//Alzaib Karovalia 301315878
//Peter Kwan 301302763 
//Syed Salman haider 301277622
 
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include <stdbool.h>
#include <stdlib.h>
#include "ADC.h"
#include "UART.h"
#include "image.h"
#include "initialize.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void UART_example(void); // An example to help you with Task 5 :)
void UART_password(void);


// *************************** Functions ***************************

#define POLE_CLEARANCE 15
#define POLE_WIDTH 6
#define SCREEN_TOP 0
#define SCREEN_BOT 48
#define BIRD_HEIGHT 10
#define BIRD_WIDTH 10
#define BIRD_X_POS 30


#define STATE_START 0
#define STATE_FLAP 1
#define STATE_HIT 2
#define STATE_END 3
#define STATE_password 4


int gravity;
int brid_y_pos;
int pole_x_pos;
int g_pole_id;
int score; 
bool key_press;
int g_current_state = 0;
int current_pole_id;
int SW2;
int random_number;
int score = 0;
int topscore = 0;
//*********************************************************************

//create 6  pole objects
PoleObject Pole[6];

//initilize the pole objects to point at their respective bitmapped images
int i;

//randomize the array to display poles
int rand_pos [6] = {0, 1, 2, 3, 4, 5};


void Game_Init(void){
	//register 6 poles with 6 different clearance height
	Pole[rand_pos[0]].image = pole_0;
	Pole[rand_pos[1]].image = pole_1;
	Pole[rand_pos[2]].image = pole_2;
	Pole[rand_pos[3]].image = pole_3;
	Pole[rand_pos[4]].image = pole_4;
	Pole[rand_pos[5]].image = pole_5;
	

	//space out each pole in x-axis
	for(i=0; i<6; i++){
		Pole[i].x = 83 + 30*i;
		Pole[i].y = 47;
		Pole[i].pole_active = 1;
	}
}

//display pole on screen
void DisplayPole(void){
	Nokia5110_ClearBuffer();
	for (i=0; i<6; i++){
		if (Pole[i].pole_active>0){
			Nokia5110_PrintBMP(Pole[i].x, Pole[i].y, Pole[i].image,0);
		}
	}
	Nokia5110_DisplayBuffer();
}


//*********************************************************************

void display_start_screen(){
	//to do: pointer to the memory location, where the highest number is saved
	//clear the screen
	Nokia5110_Clear();
	// Display game logo ie bird
	Nokia5110_PrintBMP(22, 44, flappy_logo, 0);
	Nokia5110_DisplayBuffer();
	Delay100ms(2); 
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();	
	
	//write press X to start and display highest score from the memory
	Nokia5110_SetCursor(1, 0);
	Nokia5110_OutString("Flappy Bird");
	Nokia5110_SetCursor(1, 2);
	//Nokia5110_OutString("Top Score: ");
	//Nokia5110_SetCursor(2, 3);
	//use 23 for now to display highest score
	//Nokia5110_OutUDec(23);
	Nokia5110_SetCursor(2, 5);
	Nokia5110_OutString("Press SW2");
	Delay100ms(5); 
	
	//change current state when button is pressed
	SW2 = GPIO_PORTF_DATA_R&0x01;
	
	if(!SW2){
		g_current_state = STATE_FLAP;
	}
	else{
		g_current_state = STATE_START;
	}
	
	
}

void reset(){
	//far right of the screen (just outside)
  pole_x_pos = 83;
	brid_y_pos = 29;
	Game_Init();
	score=0;
}

// End screen 
void display_end_screen(int score){
	
	
	// replace the score to top score 
	if (score > topscore)
	{
		topscore = score; 
	}
	
	//clear the screen
	Nokia5110_Clear();
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer(); 
	
	//display current score, and display highest score from the memory
	Nokia5110_SetCursor(1, 0);
	Nokia5110_OutString("Your Score: ");
	Nokia5110_SetCursor(2, 1);
	Nokia5110_OutUDec(score);
	Nokia5110_SetCursor(1, 2);
	Nokia5110_OutString("Top Score: ");
	Nokia5110_SetCursor(3, 4);
	//use 23 for now to display highest score
	Nokia5110_OutUDec(topscore);
	Delay100ms(5); 
	
	reset();

	g_current_state = STATE_START;
}


// Hit scren 
void display_hit_screen(){
	GPIO_PORTB_DATA_R=0x00;
	//clear the screen
	Nokia5110_Clear();
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer(); 

	//display current score, and display highest score from the memory
	Nokia5110_SetCursor(2, 2);
	Nokia5110_OutString("*********");
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("***HIT***");
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("*********");
	Delay100ms(5); 	
	g_current_state = STATE_END;
}

// display screen 
void display_game_screen(int brid_y_pos){
	Nokia5110_ClearBuffer();
	for (i=0; i<6; i++){
			Nokia5110_PrintBMP(Pole[i].x, Pole[i].y, Pole[i].image,0);
	}
	//display bird
	Nokia5110_PrintBMP(BIRD_X_POS, brid_y_pos, flappy, 0);

	Nokia5110_DisplayBuffer(); 
	Delay100ms(1); 	

}

//choose a pole and check the clearence height of that pole
int get_clearance(int pole_id){
	if (pole_id == 0){
		return 4;
	}
	else if(pole_id == 1){
		return 9;
	}
	else if (pole_id == 2){
		return 12;
	}
	else if (pole_id == 3){
		return 14;
	}
	else if (pole_id == 4){
		return 18;
	}
	else{
		return 22;
	}
}

// collision state
void check_collision(int bird_y_pos, int pole_id, int pole_x_pos){
	int clearance_up = get_clearance(pole_id);
	int clearance_down = clearance_up + POLE_CLEARANCE;
	
	//check if bird hits the top
	if ((bird_y_pos - BIRD_HEIGHT) <= SCREEN_TOP){
		g_current_state = STATE_HIT;
	}
	
	//check if bird hits the bot
	if (bird_y_pos >= SCREEN_BOT){
		g_current_state = STATE_HIT;
	}
	
	if((pole_x_pos < BIRD_X_POS + BIRD_WIDTH) && (pole_x_pos>BIRD_X_POS) ){
		//if bird is between clearance up and down ---> state=flap
		//else state =hit
		if((bird_y_pos - BIRD_HEIGHT>=clearance_up) && (bird_y_pos<=clearance_down)){
//			GPIO_PORTB_DATA_R=0x30;
			g_current_state = STATE_FLAP;
		}
		else{
			g_current_state = STATE_HIT;
		}
	}
}



void move_bird_pole(){
	GPIO_PORTB_DATA_R=0x10;
	gravity = change_gravity();
	//move the pole to left. 
	for (i=0; i<6; i++){
		if(Pole[i].x == 0){
			Pole[i].x = 180; //put it back in the array
			Pole[i].pole_active = 1;
		}
		else{
			Pole[i].x -= 1;
		}
		
		if (Pole[i].x< BIRD_X_POS){
			Pole[i].pole_active = 0; //pole is not active 
		}
		
	}
	
	//get current pole_x_pos for the next active pole
	for (i=0; i<6; i++){
		if (Pole[i].pole_active == 1 && Pole[i].x <= BIRD_X_POS + BIRD_WIDTH){
			pole_x_pos = Pole[i].x;
			g_pole_id = rand_pos[i];
		}
		if(Pole[i].x + POLE_WIDTH == BIRD_X_POS){
			score ++;
			GPIO_PORTB_DATA_R=0x30;
		}
	}
	
	
	if(GPIO_PORTE_DATA_R&0x01){
		brid_y_pos = brid_y_pos -2;
	}
	else{
		brid_y_pos = brid_y_pos + gravity;
	}
	
	check_collision(brid_y_pos, g_pole_id, pole_x_pos);
	display_game_screen(brid_y_pos);

}


//void Control_LED(void){   		
//	if (!SW2) {
//	GPIO_PORTF_DATA_R=0x10; 
//	} else if (SW2) {
//	GPIO_PORTF_DATA_R=0x00;
//	}
//}

int main(void){
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Nokia5110_Init(); //initilize nokia display
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();   			   // draw buffer

	gravity = 1;  //default drop of bird is 1 pixel/frame

	PortF_Init();   
	PortB_Init();
	ADC0_Init();

	Nokia5110_Clear();
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer(); 

	reset();
	Game_Init();
	DisplayPole();
	UART_password();
	while(1){
		//at different states, the game with show different screens
			switch(g_current_state){
				case STATE_START: //start screen, ask to push switch to start game
					display_start_screen();
					break;
				case STATE_FLAP: //in-game, refresh screen constantly and update position of bird (based on user input)and pole
					move_bird_pole();
					break;
				case STATE_HIT:  //bird either hit the top, bottom, or pole. Hit screen will show up
					display_hit_screen();
					break;
				case STATE_END:  //display final score and maximum available score
					display_end_screen(score);
					break;
				case STATE_password:  //ask user for password before start screen appears (PW: "awesome")
					UART_password();
					break;
				default:
					display_start_screen();			
			}
  }

}


