/*
 * dmaTest.c
 *
 *  Created on: Mar 1, 2020
 *      Author: VIPIN
 */


#include "xaxidma.h" // The DMA header file storing all important drivers of DMA
#include "xparameters.h" // the ADDRESS of all DMA accesses generated by your IP is within here
#include "sleep.h" // The OS sleep function to allow running empty clock
#include "xil_cache.h" // The DCache_flush function of PS

u32 checkHalted(u32 baseAddress,u32 offset); // First perform forward declaration to allow creating the new function

int main(){

    //Store 8 data in array a with each data 32 bits, we have to give the starting data of a
	u32 a[] = {1,2,3,4,5,6,7,8};
	u32 b[8];
	// Status flag to indicate the event is successful or not.
    u32 status;

    // The config structure pointer, used to configure the DMA
	XAxiDma_Config *myDmaConfig;
    //The DMA itself.
	XAxiDma myDma;

    //This config structure is used to initilize the DMA
    //Look for function provided by xilinx, each functions requires different passing variables.
	myDmaConfig = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR); // The BASE addr is within the xparameter.
    // DMA initialization
    status = XAxiDma_CfgInitialize(&myDma, myDmaConfig);
    // Initilization also needs an error handling function, first check if it is a success transfer.
	if(status != XST_SUCCESS){
		print("DMA initialization failed\n");
		return -1;
	}
	//This get printed to SDK terminal
	print("DMA initialization success..\n");
	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	xil_printf("Status before data transfer %0x\n",status);
	//This is from cache. used to invalidate cache and push data into DDR for maintaining data validity of memory hierarchy!
	//This is needed only if the data is generated from the PS!!
	Xil_DCacheFlushRange((u32)a,8*sizeof(u32));
	// The function used for simple data transfer, press f3 to jump to definition of the function in SDK.
    // The function transfer to where you want to store the Data. Which is an ADDRESS.
	// Transfering from device peripheral to memory.
	// We need this line of code s.t. we allow the DMA controller to actually transfer to the memory, otherwise DMA went full.
	// Now we must first allow the DMA to actually accept the Data from DMA now. So we can transfer data from DMA to our IP later
	// Receive data would come to b. DEVICE(YOUR IP) -> DMA
    status = XAxiDma_SimpleTransfer(&myDma, (u32)b, 8*sizeof(u32),XAXIDMA_DEVICE_TO_DMA);
    //1. DMA pointer
    //2. simply write (u32)a, remember to use typeCasting, serach for how it works, though it works perfectly fine.(Represent the address of a's first char)
    //3. The size of data in byte, better use size of function, or use sizeof(a)/sizeof(u32) = 8
    //4. Transfering from memory to device, as the direction.
	// This function does not wait until all data is sent out! I.e. it only send 3, thus only sent successfuly, not transfer successfuly!
	// DMA -> YOUR IP
	status = XAxiDma_SimpleTransfer(&myDma, (u32)a, 8*sizeof(u32),XAXIDMA_DMA_TO_DEVICE);//typecasting in C/C++
	if(status != XST_SUCCESS){
        //Error handling code, see if the return value is XST_SUCCESS
        //XST_SUCCESS is written within the function documentation. Otherwise, success.
		print("DMA initialization failed\n");
		return -1;
	}
	// THE POLLING!
	// From memory to device.Check if successully, this 0x4 is the halt register address, can be found in AXIDMA documentation
	// https://docs.xilinx.com/r/en-US/pg021_axi_dma/MM2S_DMASR-MM2S-DMA-Status-Register-Offset-04h
    status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
    while(status != 1){
		// Otherwise keeps on  waiting, doing Polling. Wait For the DMA truly halts, status bit would get sent out.
    	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
    }
	// The other way around, from device to memory.
    status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
    while(status != 1){
		// From datasheet we know that 0x34 is the device to memory register.
		// https://docs.xilinx.com/r/en-US/pg021_axi_dma/S2MM_DMASR-S2MM-DMA-Status-Register-Offset-34h
    	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
    }
	//After getting out these 2 while loop, truly transfer successfully, you can check for success through the debug core.
	print("DMA transfer success..\n");
	//Try to see if the received data to DMA is truly correct.
	for(int i=0;i<8;i++)
		xil_printf("%0x\n",b[i]);
}

// This function can be seen from the AXIDMA manual. It is at the base address of 4,uses print or xil_printf to see what is happening.
// Give a start and finish signal! Also you can uses sleep(1) function.
// This used to check DMA has halted.
u32 checkHalted(u32 baseAddress,u32 offset){
	u32 status;
	// https://docs.xilinx.com/r/en-US/pg021_axi_dma/MM2S_DMASR-MM2S-DMA-Status-Register-Offset-04h
	// This masking can be understood if seeing the DMA manual, since we are only interested in the first bit of MM2S_DMASR Register.
	// Thus we have to give a Mask, by using and operator on it. We can actually simply & 0x1 to get the halt status bit.
	// From the header file, we can actually find the info of Bit position of HALT_BIT.
	// If you didnt do this, you would get more infos from the DMA status registers.
	status = (XAxiDma_ReadReg(baseAddress,offset)) & XAXIDMA_HALTED_MASK;
	return status;
}