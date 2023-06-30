/*
 * vdmaTest.c
 *
 *  Created on: Apr 9, 2020
 *      Author: VIPIN
 */
// Remember to choose empty application
#include "xparameters.h" // The MMIO address space of your HW design.
#include "xaxivdma.h" // Headers with drivers needed for VDMA
#include "xscugic.h"  // Headers with drivers for Interrupts
#include "sleep.h"
#include <stdlib.h>
#include "xil_cache.h"
#include "xil_cache.h"

#define HSize 1920 // Size of the frame
#define VSize 1080
#define FrameSize HSize*VSize*3 // RGB and VGA size

static XScuGic Intc; // Interrupt controller
static int SetupIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId);

unsigned char Buffer[FrameSize]; // The video frame buffer inside the DDR memory.

int main(){

	// unsigned char Buffer[FrameSize]; Adding here would not work, since the stack size is not enough for you to allocate the memory!
	// Thus one must increase the stack size from lscript.ld
	// You need to calculate the memory you need, withni the main function system stack is used.
	// For allocating memory, heap is used.

	int status;
	int Index;
	u32 Addr;
    // VDMA instantiation
	XAxiVdma myVDMA;
	XAxiVdma_Config *config = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	// This is the struct used to configure the frame for Frame transfer.
    XAxiVdma_DmaSetup ReadCfg;
	status = XAxiVdma_CfgInitialize(&myVDMA, config, config->BaseAddress);
    if(status != XST_SUCCESS){
    	xil_printf("DMA Initialization failed");
    }
    // The settings for config of the VDMA device, this is the extra config.
    ReadCfg.VertSizeInput = VSize;
    ReadCfg.HoriSizeInput = HSize*3; // RGB thus 3
    ReadCfg.Stride = HSize*3; // RGB thus 3
    ReadCfg.FrameDelay = 0;
    ReadCfg.EnableCircularBuf = 1;
    ReadCfg.EnableSync = 1;
    ReadCfg.PointNum = 0;
    ReadCfg.EnableFrameCounter = 0;
    ReadCfg.FixedFrameStoreAddr = 0;
    status = XAxiVdma_DmaConfig(&myVDMA, XAXIVDMA_READ, &ReadCfg);
    if (status != XST_SUCCESS) {
    	xil_printf("Write channel config failed %d\r\n", status);
    	return status;
    }

    // The starting address of the video buffer.
    Addr = (u32)&(Buffer[0]);

    // Since we have only 1 frame buffer, this is used to initilize the starting address of the frame buffer.
	for(Index = 0; Index < myVDMA.MaxNumFrames; Index++) {
		ReadCfg.FrameStoreStartAddr[Index] = Addr;
		Addr +=  FrameSize;
	}

    // Passing info into the frame buffer, this specify the direction of DMA reading operation
	status = XAxiVdma_DmaSetBufferAddr(&myVDMA, XAXIVDMA_READ,ReadCfg.FrameStoreStartAddr);
	if (status != XST_SUCCESS) {
		xil_printf("Read channel set buffer address failed %d\r\n", status);
		return XST_FAILURE;
	}

    //  Poiter of DMA and the IRQ number, the initilization of interrupt controller
	XAxiVdma_IntrEnable(&myVDMA, XAXIVDMA_IXR_COMPLETION_MASK, XAXIVDMA_READ);

	SetupIntrSystem(&myVDMA, XPAR_FABRIC_AXI_VDMA_0_MM2S_INTROUT_INTR);

	//Fill the data
	// This stores the RGB strides into the buffers. Note having RGB thus *3
	/*for(int i=0;i<VSize;i++){
		for(int j=0;j<HSize*3;j=j+3){
			if(j>=0 && j<640*3){
				Buffer[(i*HSize*3)+j] = 0xff;
			    Buffer[(i*HSize*3)+j+1] = 0x00;
			    Buffer[(i*HSize*3)+j+2] = 0x00;
			}
			else if(j>=640*3 && j<1280*3){
				Buffer[(i*HSize*3)+j]   = 0x00;
			    Buffer[(i*HSize*3)+j+1] = 0xff;
			    Buffer[(i*HSize*3)+j+2] = 0x00;
			}
			else {
				Buffer[(i*HSize*3)+j]   = 0x00;
			    Buffer[(i*HSize*3)+j+1] = 0x00;
			    Buffer[(i*HSize*3)+j+2] = 0xff;
			}
		}
	}*/

	Xil_DCacheFlush();

	// The function to start the DMA
	status = XAxiVdma_DmaStart(&myVDMA,XAXIVDMA_READ);
	if (status != XST_SUCCESS) {
		if(status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
		return XST_FAILURE;
	}

	// Note the system should not get shut down!
    while(1){
    }
}


/*****************************************************************************/
 /* Call back function for read channel
******************************************************************************/

static void ReadCallBack(void *CallbackRef, u32 Mask)
{
	// This is a very useful function for displaying video.
	static int i=0;
	/* User can add his code in this call back function */
	xil_printf("Read Call back function is called\r\n");

	// Now we want to make the screen flashing in white and black.
	if(i==0){
		// Starting address and the size of frame for us to access.
		memset(Buffer,0x00,FrameSize);
		i=1;
	}
	else{
		memset(Buffer,0xff,FrameSize);
		i=0;
	}
	// Remember to directly update the data within the Cache.
	Xil_DCacheFlush();
	// If we dont add the sleep, since running in 60Hz, changing every frame, we would not notice any difference.
	sleep(1);
}

/*****************************************************************************/
/*
 * The user can put his code that should get executed when this
 * call back happens.
 *
*
******************************************************************************/
static void ReadErrorCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
	xil_printf("Read Call back Error function is called\r\n");

}


static int SetupIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId)
{
	int Status;
	XScuGic *IntcInstancePtr =&Intc;

	/* Initialize the interrupt controller and connect the ISRs */
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	Status =  XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS){
		xil_printf("Interrupt controller initialization failed..");
		return -1;
	}

	// In this interrupt service routine, this is configured and provided by Xilinx.
	Status = XScuGic_Connect(IntcInstancePtr,ReadIntrId,(Xil_InterruptHandler)XAxiVdma_ReadIntrHandler,(void *)AxiVdmaPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed read channel connect intc %d\r\n", Status);
		return XST_FAILURE;
	}

	XScuGic_Enable(IntcInstancePtr,ReadIntrId);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)IntcInstancePtr);
	Xil_ExceptionEnable();

	/* Register call-back functions
	 */
	XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL, ReadCallBack, (void *)AxiVdmaPtr, XAXIVDMA_READ);

	XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR, ReadErrorCallBack, (void *)AxiVdmaPtr, XAXIVDMA_READ);

	return XST_SUCCESS;
}
