/*
 * vdmaTest.c
 *
 *  Created on: Apr 9, 2020
 *      Author: VIPIN
 */
#include "xparameters.h"
#include "znake.h"
#include "video.h" // The initialization code for video controller.

char Buffer[FrameSize]; //Video Frame
XScuGic Intc; //Interrupt Controller Instance


int main(){
    // All other structure uses this struct.
	znake myZnake;
	u8 preyX;
	u8 preyY;
	u16 pos;
	u16 preyPos;
	u8 xPos;
	u8 yPos;
	u8 hxPos;
	u8 hyPos;
	u32 score;
	u32 delayValue;
	u32 snakeSize;
    // Initilization of our IPs.
	initIntrController(XPAR_PS7_SCUGIC_0_DEVICE_ID,&Intc);
	initVideo(XPAR_AXI_VDMA_0_DEVICE_ID,Buffer,&Intc);
	initZnake(&myZnake,XPAR_SNAKETRACKER_0_S00_AXI_BASEADDR,XPAR_TIMER_0_S00_AXI_BASEADDR,XPAR_AXI_GPIO_0_BASEADDR,Buffer);
	// Main loops, embedded system shall not stop.
    while(1){
		showWelcome(&myZnake);
		score = 0;
        // Note in HW, we define the initial snake size to be 1,2,3,4,5, one of the size. Thus here we use size of 3
		snakeSize = 3;
        // Sending the snake size back to HW.
		updateSnakeSize(&myZnake,snakeSize);
		drawFrame(HSize,VSize,gridSize,frameVOffset,frameHOffset,borderWidth,frameColor,Buffer);//Draw the Frame
		printString(HSize,VSize,"Score:0",ScorePos,10,5,whiteColor,Buffer);//Show the initial Score
		// This value would get sent to our HW delay value of Timer in HW
        delayValue = initDelayValue;
		hxPos = HSize/(2*gridSize);
		hyPos = VSize/(2*gridSize);
        // Combining x and y position in to a single pos
		pos = hxPos|(hyPos<<8);
        // send the initial position in.
		updateSnakePosition(&myZnake,pos); //Set the initial position of the snake as the centre of screen
		loadTimer(&myZnake,delayValue);
		startEventTimer(&myZnake); //Start the even timer
		// Reset the snake tracker core HW
        resetSnakeTracker(&myZnake);
		// Generating a random number for the pray position
        preyPos=getPreyPosition(&myZnake);
		preyX = preyPos&0xff;
		preyY = (preyPos&0xff00)>>8;
		//drawPrey(&myZnake,preyX,preyY); // used for debugging. Check if we successfully draw the prey.
		while(1){
			restartSnakeTracker(&myZnake); //reset the internal read pointer
			pos = getSnakeSegment(&myZnake); //Get the snake head position
			hxPos = pos&0xff;
			hyPos = (pos&0xff00)>>8;
			if(checkGameOver(hxPos,hyPos)){//check whether head hit the wall
				break;
			}
			drawSquare(hxPos,hyPos,gridSize,HSize,Buffer,redColor); //draw the head
			// We read through register of AXI of HW, reading the head value of SNAKE in the HW.
            for(int i=1;i<snakeSize;i++){
                // Displaying all pos of the snake within. Read segment by segment, drawing out the square on the screen.
				pos = getSnakeSegment(&myZnake); //draw the body
				xPos = pos&0xff;
				yPos = (pos&0xff00)>>8;
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,snakeColor);
			}
            // Note the checkpreyInbody is checked by HW, thus we have to check the value sent out by HW.
			if(checkPreyInBody(&myZnake)){//check whether prey in the body, we must prevent the RNG generates prey within the body of snake.
                preyPos=getPreyPosition(&myZnake);
				preyX = preyPos&0xff;
				preyY = (preyPos&0xff00)>>8;
				//drawPrey(&myZnake,preyX,preyY);
				clearPreyInBody(&myZnake); //Clear the bit in the snake tracker
			}
            // This is also implemented and check in HW, check whether the snake hits the body.
			if(headHitBody(&myZnake)){ //Check whether head of snake hit the body
                // GameOver
				break;
			}
            // If the game does not ends, get the tail,last position of the snake
			pos = getSnakeSegment(&myZnake);
			// This is not exactly the tail position, it is one position after the tail. Because we run the entire snake in the for loop above.
            xPos = pos&0xff;
			yPos = (pos&0xff00)>>8;
            // Check if the snake swallow the prey.
			if((hxPos==preyX) && (hyPos==preyY)){//Check whether snake swallowed the prey
                // Once swalloed we grow the snake.
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,snakeColor);
				snakeSize++;                                     //Increase snake size
				updateSnakeSize(&myZnake,snakeSize);             //Update the snake size in snake tracker
				// Find the new position of the snake.
                preyPos=getPreyPosition(&myZnake);
				preyX = preyPos&0xff;
				preyY = (preyPos&0xff00)>>8;
				//drawPrey(&myZnake,preyX,preyY);
				score++;                                         //Increment the score
				updateScore(&myZnake,score);                     //Update the score on screen
				if(score%scoreIncrementSize == 0){               //Once snake catches 10 preys double the speed
					delayValue /= speedIncrementFactor;
					loadTimer(&myZnake,delayValue);              //Load the new value to the event timer
				}
			}
			else{
                // If the snake does not swallow the prey, one needs to draw the black segment onto the tail, every other position gets a new position.
                // If removing this line, white trail appears on the screen, this only moves the snake by 1 position
				#ifdef whiteMode
					drawSquare(xPos,yPos,gridSize,HSize,Buffer,whiteColor); //remove the previous tail
				#else
                    // Drawing the black segment to the tail of snake.
					drawSquare(xPos,yPos,gridSize,HSize,Buffer,blackColor);
				#endif
			}
            // Since it is possible, when the game is running, snake move from event of timer also I press the button
            // Both the event might happen at the same time. In this case head and tail moves by 2 position.
			// READING one more segment from the HW.
            pos = getSnakeSegment(&myZnake); //additional cell to solve the issue of move and button between 2 reads
			xPos = pos&0xff;
			yPos = (pos&0xff00)>>8;
			#ifdef whiteMode
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,whiteColor);
			#else
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,blackColor);
			#endif
			drawPrey(&myZnake,preyX,preyY);
            // Check whether you have the center button press, this pauses the game. Center button signal sent in through the GPIO.
			checkPause(&myZnake);
		}
        // Showing game Over onto the screen.
		showGameOver(&myZnake);
        // Waiting for the player to push the center button sent in by the GPIO.
		waitRestart(&myZnake);
	}
}

// Notice there is no syncrhonization between HW and SW. In ideal, we should only do all these things when we press the button.
// We do so since we check whether the prey is in snake is in HW. The snake position is within the HW!!
// Note the speed HW sends signal back is a lot faster than SW. HW sends around 300 times signals back to PS, SW can always checks the conditions
// sends back by the HW.
