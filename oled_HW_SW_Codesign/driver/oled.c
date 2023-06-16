/*
 * oled.c
 *
 *  Created on: Feb 20, 2020
 *      Author: VIPIN
 */


#include "oled.h"
#include "xil_io.h" // The library for supporting the Xil_Out32 and Xil_In32 functions

int initOled(oledControl *myOled,u32 baseAddress){
	myOled->baseAddress = baseAddress;
	return 0;
}


void writeCharOled(oledControl *myOled,char myChar){
	// Status register set as 0, indicating the HW is still running.
    u32 status=0;
    // Because of slvreg2, +8, write data onto slvreg2
	Xil_Out32(myOled->baseAddress+8,myChar);
    // The slvreg0 , thus no offset, modify the register to 1
    // Meaning sendValid signal.
	Xil_Out32(myOled->baseAddress,0x1);
	while(!status){
        //sendDone signal is of slvreg1, thus polling and waiting for the HW to send back
        //the sendDone signal, writing out to the output slave register.
		status = Xil_In32(myOled->baseAddress+4); //polling mode
	}
    // Must remember to clear the status register after running, otherwise the value would get stored
    // within the HW register. slvreg1
	Xil_Out32(myOled->baseAddress+4,0x0);
}


void printOled(oledControl *myOled,char *myString){
	while(*myString != 0){
        // Keep writing the oled, myString is the pointer contains the starting addres of STRING
		writeCharOled(myOled,*myString);
        //Pointer pointing to the next datam increment data pointer ++
		myString++;
	}
}


void clearOled(oledControl *myOled){
    // Since we do not know what kind of value are stored within the GDRAM initially, we would
    // like to clear the GDRAM first. This function clears the GDRAM
	u32 i;
	for(i=0;i<64;i++)
        // Replacing the GDRAM with the empty string.
		writeCharOled(myOled,' ');
}
