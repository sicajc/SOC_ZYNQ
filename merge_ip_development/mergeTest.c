/*
 * mergeTest.c
 *
 *  The result should be noticed that, IP merging is faster than printing a single string. You can see it on TerraTerm COM port.
 */

#include"xparameters.h"
#include "xil_io.h"
// This is the header file for GIC, which controls the interrupt controller.
#include "xscugic.h"


XScuGic IntcInstancePtr; // This needed to be declared as global varialbe since myISR is also using it.
static void myISR(); // Function pre-defined

int main(){
	// If coding an IP these a,b,c would be part of your IP structure.
	u32 a[] = {1,5,6,9,16,25,32};
	u32 b[] = {3,5,7,10,12,20};
	u32 Status;
	// Must first declare a pointer for instantiation of interrupt controller
	XScuGic_Config *IntcConfig;

	// Initilization of the interrupt controller
	// Search for the device ID in the header file xparameters.h
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {

			return XST_FAILURE;
	}
	//The return status to check if the interrupt is a success or not.
	// CPU based address.
	Status = XScuGic_CfgInitialize(&IntcInstancePtr, IntcConfig,IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
			// Initilization failed.
			return XST_FAILURE;
	}
	//----------------------------------------------------------------
	//--------------------This Implement ONE INTERRUPT only, FOR MORE INTERRUPT, YOU HAVE TO DO THESE STEPS FOR EACH INTERRUPT-----------------//
	//XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR
	// Setting the priority IRQ for our IP
	// func(Interrupt, IRP#, Priority of interrupt and Trigger time), whether it is a level or edge trigger.
	// Important if we have multiple interrupt within the system.
	XScuGic_SetPriorityTriggerType(&IntcInstancePtr, XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR, 0xA0, 0x3);
	// Connecting ISR with the Interrupt.
	//(Xil_InterruptHandler) type casting , myISR is a pointer to my writtern ISR function.
	// Most function has a status, use it to check whether it return successfully
	Status = XScuGic_Connect(&IntcInstancePtr, XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR,(Xil_InterruptHandler)myISR,0);
	if (Status != XST_SUCCESS) {
		return Status;
	}
	//Use this function to invoke a certain interrupt.
	XScuGic_Enable(&IntcInstancePtr, XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR);

	//-------------------------------------------------------------------

	// We have to link GIC with PS also, this is a standard way of doing so. Init->handler->enable
	Xil_ExceptionInit();
	// This is a standard way of linking GIC with PS.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)&IntcInstancePtr);
	Xil_ExceptionEnable();


	// The merging operation for our merge IP.
	for(int i=0;i<7;i++){
		//Base address for your slv_reg3 for writing value.
		Xil_Out32(XPAR_MERGE_V1_0_0_BASEADDR+0xC,a[i]);
	}

	for(int i=0;i<6;i++){
		Xil_Out32(XPAR_MERGE_V1_0_0_BASEADDR+0x10,b[i]);
	}

	// Setting the control register
	Xil_Out32(XPAR_MERGE_V1_0_0_BASEADDR,0x1);


	//---------------WITHOUT INTERRUPT----------------//
	/*Status = Xil_In32(XPAR_MERGE_V1_0_0_BASEADDR+4);

	while(!Status)
		Status = Xil_In32(XPAR_MERGE_V1_0_0_BASEADDR+4);

	for(int i=0;i<13;i++){
		c[i] = Xil_In32(XPAR_MERGE_V1_0_0_BASEADDR+0x8);
		xil_printf("%d\n\r",c[i]);
	}*/
	//-------------------------------------------------
	while(1){
		xil_printf("I am working..\n\r");
	}

	return 0;
}

// The return type of ISR should be a void type, should not return any value,they are system call,should not return anything.
// static s.t. only in this source code can see this function
// Interrupt service routine
static void myISR(){
	//Disable the corresponding interrupt.
	XScuGic_Disable(&IntcInstancePtr, XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR);
	//Read the status register s.t. the interrupt signal get reasserted.
	Xil_Out32(XPAR_MERGE_V1_0_0_BASEADDR+4,0x0); // Edge active-low sensitive interrupt signal, so we want to


	// In the previous example, we code our IP as a struct
	// When coding for ISR, and we usually put our IP into a struct s.t. we can access the pointer to that address.
	xil_printf("Got Interrupt\n\r");
	for(int i=0;i<13;i++){
		//Printing the merge number
		xil_printf("%d\n\r",Xil_In32(XPAR_MERGE_V1_0_0_BASEADDR+0x8));
	}
	//Enable the interrupt
	XScuGic_Enable(&IntcInstancePtr, XPAR_FABRIC_MERGE_V1_0_0_INTR_INTR);
}
