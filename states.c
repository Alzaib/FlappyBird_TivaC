#include "states.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include <stdbool.h>
#include <stdlib.h>
#include "image.h"
#include "initialize.h"

#define STATE_START 0
#define STATE_FLAP 1
#define STATE_HIT 2
#define STATE_END 3
#define STATE_password 4

//int g_current_state = 0;


////create 6  pole objects
//PoleObject Pole[6];

////initilize the pole objects to point at their respective bitmapped images
//int i;

////randomize the array to display poles
//int rand_pos [6] = {0, 1, 2, 3, 4, 5};


//void Game_Init(void){

//	Pole[rand_pos[0]].image = pole_0;
//	Pole[rand_pos[1]].image = pole_1;
//	Pole[rand_pos[2]].image = pole_2;
//	Pole[rand_pos[3]].image = pole_3;
//	Pole[rand_pos[4]].image = pole_4;
//	Pole[rand_pos[5]].image = pole_5;
//	

//	
//	for(i=0; i<6; i++){
//		Pole[i].x = 83 + 30*i;
//		Pole[i].y = 47;
//		Pole[i].pole_active = 1;
//	}
//}

//void DisplayPole(void){
//	Nokia5110_ClearBuffer();
//	for (i=0; i<6; i++){
//		if (Pole[i].pole_active>0){
//			Nokia5110_PrintBMP(Pole[i].x, Pole[i].y, Pole[i].image,0);
//		}
//	}
//	Nokia5110_DisplayBuffer();
//}


////*********************************************************************

//void display_start_screen(){
//	//to do: pointer to the memory location, where the highest number is saved
//	//clear the screen
//	Nokia5110_Clear();
//	// Display game logo ie bird
//	Nokia5110_PrintBMP(22, 44, flappy_logo, 0);
//	Nokia5110_DisplayBuffer();
//	Delay100ms(2); 
//	Nokia5110_ClearBuffer();
//	Nokia5110_DisplayBuffer();	
//	
//	
//	//write press X to start and display highest score from the memory
//	Nokia5110_SetCursor(1, 0);
//	Nokia5110_OutString("Flappy Bird");
//	Nokia5110_SetCursor(1, 2);
//	//Nokia5110_OutString("Top Score: ");
//	//Nokia5110_SetCursor(2, 3);
//	//use 23 for now to display highest score
//	//Nokia5110_OutUDec(23);
//	Nokia5110_SetCursor(2, 5);
//	Nokia5110_OutString("Press SW2");
//	Delay100ms(5); 
//	
//	//change current state when button is pressed
//	SW2 = GPIO_PORTF_DATA_R&0x01;
//	
//	if(!SW2){
//		g_current_state = STATE_FLAP;
//	}
//	else{
//		g_current_state = STATE_START;
//	}
//	
//	
//}

//void reset(){
//  pole_x_pos = 83;
//	brid_y_pos = 29;
//	Game_Init();
//	score=0;
//	
//	
//	//random_number = (Random32()>>24)%5 + 1;
//	//g_pole_id = random_number;
//}

//void display_end_screen(int score){
//	//to do: pointer to the memory location, where the highest number is saved
//	//clear the screen
//	Nokia5110_Clear();
//	Nokia5110_ClearBuffer();
//	Nokia5110_DisplayBuffer(); 
//	
//	//display current score, and display highest score from the memory
//	Nokia5110_SetCursor(1, 0);
//	Nokia5110_OutString("Your Score: ");
//	Nokia5110_SetCursor(2, 1);
//	Nokia5110_OutUDec(score);
//	Nokia5110_SetCursor(1, 2);
//	Nokia5110_OutString("Top Score: ");
//	Nokia5110_SetCursor(3, 4);
//	//use 23 for now to display highest score
//	Nokia5110_OutUDec(8);
//	Delay100ms(5); 
//	
//	reset();
//	
//	g_current_state = STATE_START;
//}

//void display_hit_screen(){
//	//to do: pointer to the memory location, where the highest number is saved
//	//clear the screen
//	Nokia5110_Clear();
//	Nokia5110_ClearBuffer();
//	Nokia5110_DisplayBuffer(); 

//	//display current score, and display highest score from the memory
//	Nokia5110_SetCursor(2, 2);
//	Nokia5110_OutString("*********");
//	Nokia5110_SetCursor(2, 3);
//	Nokia5110_OutString("***HIT***");
//	Nokia5110_SetCursor(2, 4);
//	Nokia5110_OutString("*********");
//	Delay100ms(5); 	
//	g_current_state = STATE_END;
//}


//void display_game_screen(int brid_y_pos){
//	Nokia5110_ClearBuffer();
//	//Nokia5110_PrintBMP(pole_x_pos, 47, pole_pointer, 0);
//	for (i=0; i<6; i++){
//			Nokia5110_PrintBMP(Pole[i].x, Pole[i].y, Pole[i].image,0);
//	}
//	//display bird
//	Nokia5110_PrintBMP(BIRD_X_POS, brid_y_pos, flappy, 0);

//	Nokia5110_DisplayBuffer(); 
//	Delay100ms(1); 	

//}


//int get_clearance(int pole_id){
//	if (pole_id == 0){
//		return 4;
//	}
//	else if(pole_id == 1){
//		return 9;
//	}
//	else if (pole_id == 2){
//		return 12;
//	}
//	else if (pole_id == 3){
//		return 14;
//	}
//	else if (pole_id == 4){
//		return 18;
//	}
//	else{
//		return 22;
//	}
//}

//void check_collision(int bird_y_pos, int pole_id, int pole_x_pos){
//	int clearance_up = get_clearance(pole_id);
//	int clearance_down = clearance_up + POLE_CLEARANCE;
//	
//	//check if bird hits the top
//	if ((bird_y_pos - BIRD_HEIGHT) <= SCREEN_TOP){
//		g_current_state = STATE_HIT;
//	}
//	
//	//check if bird hits the bot
//	if (bird_y_pos >= SCREEN_BOT){
//		g_current_state = STATE_HIT;
//	}
//	
//	if((pole_x_pos < BIRD_X_POS + BIRD_WIDTH) && (pole_x_pos>BIRD_X_POS) ){
//		//if bird is between clearance up and down ---> state=flap
//		//else state =hit
//		if((bird_y_pos - BIRD_HEIGHT>=clearance_up) && (bird_y_pos<=clearance_down)){
//			g_current_state = STATE_FLAP;
//		}
//		else{
//			g_current_state = STATE_HIT;
//		}
//	}
//}



//void move_bird_pole(){
//	gravity = change_gravity();
////	change_gravity();
//	//move the pole to left. 
//	for (i=0; i<6; i++){
//		if(Pole[i].x == 0){
//			Pole[i].x = 180; //put it back in the array
//			Pole[i].pole_active = 1;
//		}
//		else{
//			Pole[i].x -= 1;
//		}
//		
//		if (Pole[i].x< BIRD_X_POS){
//			Pole[i].pole_active = 0; //pole is not active 
//		}
//		
//	}
//	
//	//get current pole_x_pos for the next active pole
//	for (i=0; i<6; i++){
//		if (Pole[i].pole_active == 1 && Pole[i].x <= BIRD_X_POS + BIRD_WIDTH){
//			pole_x_pos = Pole[i].x;
//			g_pole_id = rand_pos[i];
//		}
//		if(Pole[i].x + POLE_WIDTH == BIRD_X_POS){
//			score ++;
//		}
//	}
//	
//	
//	if(GPIO_PORTE_DATA_R&0x01){
//		brid_y_pos = brid_y_pos -2;
//	}
//	else{
//		brid_y_pos = brid_y_pos + gravity;
//	}
//	
//	check_collision(brid_y_pos, g_pole_id, pole_x_pos);
//	display_game_screen(brid_y_pos);
//}


//void run () {
//				switch(g_current_state){
//				case STATE_START:
//					display_start_screen();
//					break;
//				case STATE_FLAP:
//					move_bird_pole();
//					break;
//				case STATE_HIT:
//					display_hit_screen();
//					break;
//				case STATE_END:
//					display_end_screen(score);
//					break;
//				case STATE_password:
//					UART_password();
//					break;
//				default:
//					display_start_screen();			
//			}
//}