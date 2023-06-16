/*
 * oled.h
 *
 *  Created on: Feb 20, 2020
 *      Author: VIPIN
 */

#ifndef SRC_OLED_H_
#define SRC_OLED_H_

#include<xil_types.h>

// Predefine the struct, to allow holding the value of baseAddress and enhance readability
typedef struct oledControl{
	u32 baseAddress;
}oledControl;

//3 Most important function, INIT, WRITE & READ
int initOled(oledControl *myOled,u32 baseAddress);
// Write the Char to OLED 1 BY 1
void writeCharOled(oledControl *myOled,char myChar);
// Print out the whole string into OLED
void printOled(oledControl *myOled,char *myString);

void clearOled(oledControl *myOled);


#endif /* SRC_OLED_H_ */