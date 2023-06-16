// The size of header and image of bmp file
#include <stlib.h>
#include "xil_type.h"
#include "xuartps.h"
#include <sleep.h>

#define imageSize 512*512
#define headerSize 1000


int main()
{
    // Use dynamic memory allocation
    u8 *imageData;
    u32 receivedByte = 0;
    u32 totalReceivedByte = 0;
    imageData = malloc(sizeof(u8)*(imageSize+headerSize)); // Taking in sizeof(u8) to allocate the memory allocation
    XUartPs_Config *myUartConfig;
    u32 status;

    // Remember we have a heap memory in CPU, however it might not be able to handle this much data at once.
    //This might not work and sent through UART. Ideally, this should work, however, in reality the data might get loss during transmission.
    // Large amount of data sending might leads to data Loss.
    for (int i = 0; i < fileSize; i++)
    {
        // scanf("%c",&imageData[i]);
    }

    // Thus we must use the low level UART interface to transmit data.
    // The driver is provided within the bsp file! uartps.h
    // Must first config the uart initilization first
    // You can find the device ID in xparameter file.
    myUartConfig =XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
    status = XUartPs_CfgInitilize(&myUart,myUartConfig,myUartConfig->BaseAddress);

    // Error handling
    if(status != XST_SUCCESS)
    {
        print("UART Initilization Failed!\n")
        return -1;
    }

    // Baud rate it important for synchronization
    // Call the baudRate is better setted, on default it is 115200
    status = XUartPs_SetBaudRate(&myUart,115200)

    // Error handling
    if(status != XST_SUCCESS)
    {
        print("Set BaudRate Fail!\n");
        return -1;
    }

    // From the xuartps, we need receiver, uartreceive
    //This works in a polling mode, a non-blocking, if data available, this uart would take the value, if not available ignored.

    // This cannot work properly, since receiver sometimes available sometimes not, need a while loop to check for success transfer.
    // for (int i = 0; i < fileSize; i++)
    // {
    //     u32 XUartPs_Recv(&myUart,imageData,fileSize);
    // }

    // Data transfer from computer to DDR
    // This is the correct way of doing UART
    while(total_ReceviedBytes < fileSize)
    {   // This loop would be working, only jump out if I get the correct amount of data.
        receivedBytes = XUartPS_Recv(&myUart,(u8*)&imageData[totalReceivedBytes],fileSize);
        totalReceivedBytes += receivedBytes;
    }

    //Check whether really get Transfered, this print the first ten values of your image if successfully tranfered data
    for(int i = 0;i<10;i++)
    {
        xil_print("%0x",imageData[i]);
    }

    //Do the data processing. Reading data from DDR, process it then store it back to DDR
    for (int i = headerSize; i < fileSize ; i++)
    {
        imageData[i] = 255-imageData[i];
    }

    // Send data back to computer,XUartPS_Send, this is also non-blocking
    while (totaltransmittedBytes < fileSize)
    {
        transmittedByte = XUartPs_Send(myUart,(u8*)&imageData[totalTransmittedBytes],1);
        totaltransmittedBytes += tranmittedByte;
        // Note some data is loss during transmission, to solve the wait for some time during each transfer. Thus add the sleep(1)
        usleep(2000);
    }

    // Now we want to saved the received data on PC, define the file name and then do the transaction.
    // Modify it on the teraterm. However, the recieving process would take a lot longer.

}