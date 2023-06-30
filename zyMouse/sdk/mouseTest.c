/*
 * mouseTest.c
 *
 *  Created on: Apr 18, 2020
 *      Author: VIPIN
 */

#include "xparameters.h"
#include "xscugic.h"
#include "zyMouse.h"

#define HSize 1920
#define VSize 1080

#define clockFreq 100000000
#define updateRate 0.005 // The update rate of the mouse segment.
#define CounterValue clockFreq*updateRate

int initIntrController(XScuGic *IntcInstancePtr);
void testPressCallBack(void *callBackRef);
void testMoveCallBack(void *callBackRef);

int main(){
	XScuGic Intc;
	zyMouse myMouse;
	u16 xPos=HSize;
	u16 yPos=VSize;
	u32 pos = xPos|(yPos<<16);

	initIntrController(&Intc);
    //Setting up interrupt for mouse, instance of mouse, instance of interrupt and the IRQ number
	setupZymouseInterrupt(&myMouse,&Intc,XPAR_FABRIC_ZYMOUSE_0_O_INTR_INTR);

	initZyMouse(&myMouse,XPAR_ZYMOUSE_0_S00_AXI_BASEADDR);
	setInterruptZyMouse(&myMouse,AllInterruptMask);
    // Setting up the mouse struct and the callback function
    // When interrupt comes, calls the handler then according to the status of the IP register, it calls the callback functions. THIS Must be provided
    // Callback functions has to be provided. Otherwise ISR cannot work properly
	setZymouseCallBack(&myMouse,moveHandler,testMoveCallBack);
	setZymouseCallBack(&myMouse,pressHandler,testPressCallBack);
	setCoordinateZyMouse(&myMouse,pos);
	setTimerZymouse(&myMouse,CounterValue);
	startZymouse(&myMouse);
	while(1){
	}
	return 0;
}

// The two callback functions
void testPressCallBack(void *CallBackRef){
    // For testing simply prints callback
	xil_printf("Press Call back called\n\r");

}

void testMoveCallBack(void *CallBackRef){
    // The callback function
	u16 xPos;
	u16 yPos;
	u32 pos;
	xil_printf("Move Call back called\n\r");
	zyMouse* zyMouseInst;
	zyMouseInst = (zyMouse*)CallBackRef;
    //Returning the position
	pos = Xil_In32(zyMouseInst->baseAddress+posRegOffset);
	//Seperates the position into (x,y)
    xPos = pos&0xffff;
	yPos = (pos&0xffff0000)>>16;
	xil_printf("x pos:%d, y pos:%d\n\r",xPos,yPos);
}


int initIntrController(XScuGic *IntcInstancePtr){
	int Status;
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	Status =  XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS){
		xil_printf("Interrupt controller initialization failed..");
		return -1;
	}
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)IntcInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}
