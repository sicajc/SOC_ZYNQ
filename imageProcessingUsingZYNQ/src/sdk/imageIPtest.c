/*
 * imageIpTest.c
 *
 *  Created on: Apr 4, 2020
 *      Author: VIPIN
 */

#include "xaxidma.h"
#include "xparameters.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "xscugic.h"
#include "imageData.h"
#include "xuartps.h"

#define imageSize 512*512

u32 checkHalted(u32 baseAddress,u32 offset);

XScuGic IntcInstance;
// Declaration of ISR is always static void.
static void imageProcISR(void *CallBackRef);  // The ISR for sending requesting more Data into the IP from IP to DMA
static void dmaReceiveISR(void *CallBackRef); // THE ISR for Receiving dma used stop the process, to send the data
int done;

int main(){
    u32 status;
	u32 totalTransmittedBytes=0;
	u32 transmittedBytes = 0;
	XUartPs_Config *myUartConfig;
	XUartPs myUart;

	//Initialize uart
	myUartConfig = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
	status = XUartPs_CfgInitialize(&myUart, myUartConfig, myUartConfig->BaseAddress);
	if(status != XST_SUCCESS)
		print("Uart initialization failed...\n\r");
	status = XUartPs_SetBaudRate(&myUart, 115200);
	if(status != XST_SUCCESS)
		print("Baudrate init failed....\n\r");

	// DMA
	XAxiDma_Config *myDmaConfig;
	XAxiDma myDma;

    //DMA Controller Configuration
	myDmaConfig = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
	status = XAxiDma_CfgInitialize(&myDma, myDmaConfig);
	if(status != XST_SUCCESS){
		print("DMA initialization failed\n");
		return -1;
	}

	//Interrupt by default is not enabled, polling mode is default, thus we must first enable the interrupt.
	// In block design, we are interested in device to DMA interrupt. DEVICE TO DMA interrupt
	// MASK is required for controlling the input output. IO completetion interrupt.
	XAxiDma_IntrEnable(&myDma, XAXIDMA_IRQ_IOC_MASK, XAXIDMA_DEVICE_TO_DMA);

	//Interrupt Controller Configuration
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	status =  XScuGic_CfgInitialize(&IntcInstance, IntcConfig, IntcConfig->CpuBaseAddress);

	if(status != XST_SUCCESS){
		xil_printf("Interrupt controller initialization failed..");
		return -1;
	}

	//Setting the priorty for the 2 interrupt, one for your IP, another for your DMA, 3 means edge triggered interrupt.
	XScuGic_SetPriorityTriggerType(&IntcInstance,XPAR_FABRIC_IMAGEPROCESS_0_O_INTR_INTR,0xA0,3);
	// Linking ISR with an IRQ, whenever ISR is called, myDma, the pointer would be automatically linked to the ISR function
	status = XScuGic_Connect(&IntcInstance,XPAR_FABRIC_IMAGEPROCESS_0_O_INTR_INTR,(Xil_InterruptHandler)imageProcISR,(void *)&myDma);
	if(status != XST_SUCCESS){
		xil_printf("Interrupt connection failed");
		return -1;
	}
	//Enabling the first interrupt
	XScuGic_Enable(&IntcInstance,XPAR_FABRIC_IMAGEPROCESS_0_O_INTR_INTR);

	// Setting the second interrupt, lesser priorty means it get serves first.
	XScuGic_SetPriorityTriggerType(&IntcInstance,XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,0xA1,3);

	status = XScuGic_Connect(&IntcInstance,XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,(Xil_InterruptHandler)dmaReceiveISR,(void *)&myDma);
	if(status != XST_SUCCESS){
		xil_printf("Interrupt connection failed");
		return -1;
	}
	// Enabling the second interrupt
	XScuGic_Enable(&IntcInstance,XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR);

	// This part is always the same.
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)&IntcInstance);
	Xil_ExceptionEnable();


	//Starting to actually sending data, remember to include the imageData inside.
	status = XAxiDma_SimpleTransfer(&myDma,(u32)imageData, 512*512,XAXIDMA_DEVICE_TO_DMA); //512*512 Sent data.
	status = XAxiDma_SimpleTransfer(&myDma,(u32)imageData, 4*512,XAXIDMA_DMA_TO_DEVICE);
	//Since we are streaming the data, we cannot send in the full 512*512 data, we can only send in 4 at a time.
	if(status != XST_SUCCESS){
		print("DMA initialization failed\n");
		return -1;
	}

	// Must not let the processor exit the main code.
	// Done waits for the DMA to completely send the data back into the DDR.
    while(!done){

    }

	// After DMA completely sends data back to DDR, done is pulled up, we can now start sending data from DDR to UART.
	while(totalTransmittedBytes < imageSize){
		transmittedBytes =  XUartPs_Send(&myUart,(u8*)&imageData[totalTransmittedBytes],1);
		totalTransmittedBytes += transmittedBytes;
		usleep(1000);// Used for avoiding data Lost.
	}


}


u32 checkIdle(u32 baseAddress,u32 offset){
	// Look for the Xilinx AXI IP documents, checking it is IDLE or it is Halted.
	// If DMA is sending transfer from Dynamic allocated area, it would make the Halted 0x0 register high
	// If sending from static allocated area, it make. the idle high, this is strange.
	// So to check for problem check both Halted and Idle of DMA.
	u32 status;
	// Since we are now statically allocating memory, check for Idle
	status = (XAxiDma_ReadReg(baseAddress,offset))&XAXIDMA_IDLE_MASK;
	return status;
}

//Usually use CallBackRef, a pointer to void. Taking only 1 pointer, a pointer to the HW, so that we can access our IP within the ISR.
static void imageProcISR(void *CallBackRef){
	// Finishing one line buffer, now going to sned the new line buffer.
	// Since we have already sent 4 lines in the main code DMA transfer, thus i starts from 4
	static int i=4; // Need to use static, i is initialized only once. Function would remember the value of i.
	int status;
	// When doing the interrupt service routine, other interrupt should not interfere with the current service, thus we would first disable the interrupt
	XScuGic_Disable(&IntcInstance,XPAR_FABRIC_IMAGEPROCESS_0_O_INTR_INTR);

	// Main transfer code.
	// Note we need this guard code, since DMA controller might still be sending the data.
	// If we blindly send data now, we might have conflict with DMA. DATA is lost if this happends.
	// Thus we must use some kind of polling to prevent DMA conflict. Now we must first check whether DMA is sending the data.
	status = checkIdle(XPAR_AXI_DMA_0_BASEADDR,0x4);
	// We can only send data if DMA is in the IDLE state.
	while(status == 0)
		// Keep doing polling, until DMA is actually 1 means IDLE.
		status = checkIdle(XPAR_AXI_DMA_0_BASEADDR,0x4);

	// When DMA is idle, do the transfer.
	if(i<514){ //514 because we need 2 padded rows. Otherwise, img shrinks.
		//	Each time calling ISR, we would send the next row of the imageData. Since 1 line buffer is processed, XAxiDma a pointer to the DMA controller.
		status = XAxiDma_SimpleTransfer((XAxiDma *)CallBackRef,(u32)&imageData[i*512],512,XAXIDMA_DMA_TO_DEVICE);
		i++;
	}

	// Reenabling the interrupt.
	XScuGic_Enable(&IntcInstance,XPAR_FABRIC_IMAGEPROCESS_0_O_INTR_INTR);
}


static void dmaReceiveISR(void *CallBackRef){
	// Disabling the interrupt from DMA
	XAxiDma_IntrDisable((XAxiDma *)CallBackRef, XAXIDMA_IRQ_IOC_MASK, XAXIDMA_DEVICE_TO_DMA);
	// In addition to disabling the interrupt, IP dependent, note we must remember to clear the interrupt status register.
	// Clear the interrupt, otherwise, interrupt would remains 1 forever, we cannot pull it down.
	XAxiDma_IntrAckIrq((XAxiDma *)CallBackRef, XAXIDMA_IRQ_IOC_MASK, XAXIDMA_DEVICE_TO_DMA);
	// Once done, we can send the data to UART, back to the PC
	done = 1;

	// Enabling interrupt
	XAxiDma_IntrEnable((XAxiDma *)CallBackRef, XAXIDMA_IRQ_IOC_MASK, XAXIDMA_DEVICE_TO_DMA);
}
