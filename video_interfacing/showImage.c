/*
 * vdmaTest.c
 *
 *  Created on: Apr 9, 2020
 *      Author: VIPIN
 */
// We would store the whole image as a header .h file!
// If we directly use the bmp and show the video, things would get inverted. This can be easily done through python or matlab.
#include "xparameters.h"
#include "xaxivdma.h"
#include "xscugic.h"
#include "sleep.h"
#include <stdlib.h>
#include "xil_cache.h"
#include "xil_cache.h"
#include "imageData.h" // The image.h needed for displaying. The image itself.

#define HSize 1920
#define VSize 1080
#define FrameSize HSize*VSize*3
#define imgHSize 512
#define imgVSize 512

static XScuGic Intc;


static int SetupIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId);
int drawImage(u32 displayHSize,u32 displayVSize,u32 imageHSize,u32 imageVSize,u32 hOffset, u32 vOffset,char *imagePointer);

// Note in most of the cases, the resolution of image and VGA is different, if directly copying the image into the frame buffer, the image would be
// 4 times smaller than the original one one the VGA, thus one must copy the image to the correct position on VGA
unsigned char Buffer[FrameSize];

int main(){
	int status;
	int Index;
	u32 Addr;
	XAxiVdma myVDMA;
	XAxiVdma_Config *config = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	XAxiVdma_DmaSetup ReadCfg;
	status = XAxiVdma_CfgInitialize(&myVDMA, config, config->BaseAddress);
    if(status != XST_SUCCESS){
    	xil_printf("DMA Initialization failed");
    }
    ReadCfg.VertSizeInput = VSize;
    ReadCfg.HoriSizeInput = HSize*3;
    ReadCfg.Stride = HSize*3;
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

    Addr = (u32)&(Buffer[0]);


	for(Index = 0; Index < myVDMA.MaxNumFrames; Index++) {
		ReadCfg.FrameStoreStartAddr[Index] = Addr;
		Addr +=  FrameSize;
	}

	status = XAxiVdma_DmaSetBufferAddr(&myVDMA, XAXIVDMA_READ,ReadCfg.FrameStoreStartAddr);
	if (status != XST_SUCCESS) {
		xil_printf("Read channel set buffer address failed %d\r\n", status);
		return XST_FAILURE;
	}

	XAxiVdma_IntrEnable(&myVDMA, XAXIVDMA_IXR_COMPLETION_MASK, XAXIVDMA_READ);

	SetupIntrSystem(&myVDMA, XPAR_FABRIC_AXI_VDMA_0_MM2S_INTROUT_INTR);

	//Fill the data
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
	drawImage(HSize,VSize,imgHSize,imgVSize,(HSize-imgHSize)/2,(VSize-imgVSize)/2,imageData);


	status = XAxiVdma_DmaStart(&myVDMA,XAXIVDMA_READ);
	if (status != XST_SUCCESS) {
		if(status == XST_VDMA_MISMATCH_ERROR)
			xil_printf("DMA Mismatch Error\r\n");
		return XST_FAILURE;
	}

    while(1){
    }
}


/*****************************************************************************/
 /* Call back function for read channel
******************************************************************************/

static void ReadCallBack(void *CallbackRef, u32 Mask)
{
	/* User can add his code in this call back function */
	xil_printf("Read Call back function is called\r\n");
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


int drawImage(u32 displayHSize,u32 displayVSize,u32 imageHSize,u32 imageVSize,u32 hOffset, u32 vOffset,char *imagePointer){
    //0,1 Size of frame buffers
    //2,3 Size of your image.
    //4  The Offset, from which position should we start displaying the image.
	for(int i=0;i<VSize;i++){
		for(int j=0;j<HSize;j++){
			if(i<vOffset || i >= vOffset+imageVSize){
				Buffer[(i*HSize*3)+(j*3)]   = 0x00;
			    Buffer[(i*HSize*3)+(j*3)+1] = 0x00;
			    Buffer[(i*HSize*3)+(j*3)+2] = 0x00;
			}
			else if(j<hOffset || j >= hOffset+imageHSize){
				Buffer[(i*HSize*3)+(j*3)]   = 0x00;
			    Buffer[(i*HSize*3)+(j*3)+1] = 0x00;
			    Buffer[(i*HSize*3)+(j*3)+2] = 0x00;
			} // Upper two if conditions, put black pixel to the outer image part.
			else {
                // The actual image data copied to the frame buffer, if we want to display to RGB, we must do 3 channels of assignments.
                // Note the Pixel value eats in 4 bits, however our image is 8 bits, thus one must subtract 16 away to get the correct value.
                // Thus we have to uniformly shift right all the pixel by 4 bit.
				Buffer[(i*HSize*3)+j*3]     = *imagePointer/16;
			    Buffer[(i*HSize*3)+(j*3)+1] = *imagePointer/16;
			    Buffer[(i*HSize*3)+(j*3)+2] = *imagePointer/16;
			    imagePointer++;
			}
		}
	}

    // Updating the cache in constant.
	Xil_DCacheFlush();
	return 0;
}
