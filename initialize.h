//create a struct to hold x, y coordinates of the pole, and image pointer
typedef struct {
	unsigned long x; // x coordinate of the pole
	unsigned long y; // y coordinate of the pole = 47 
	unsigned int pole_active; //0= passed, greater than 0= active
	const unsigned char *image;
}PoleObject;


//initializing port F to read switch
void PortF_Init(void);  //for sw2
void PortF_Init(void);  //for led
void PortB_Init(void);

//wait delay
void Delay100ms(unsigned long count);

//modifiying gravity based on ADC slider (potentiometer value)
int change_gravity (void);
