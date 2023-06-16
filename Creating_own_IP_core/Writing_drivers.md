# Drivers for your custom IP
1. APP -> OS -> DRIVER -> HW
2. Driver knows all the detail of your hardware, driver must provide access to read and write to the registers of HW.
3. INIT, WRITING_TO_IP & READING_FROM_IP basic 3 types of these drivers must be implemented.
4. Vivado has already created template for your driver which can be further configured.

# SDK
1. Then we export the Custom HW and launch the SDK.
2. Create an empty application project.
3. Usually 2 files for drivers. .h and .c file
4. So we must create a header file.
5. H_ means the header definition which are used to avoid the circular reference.
6. In C, we usually use structure, and we must extends data type from struct
```C
    typedef struct gpioControl{

    }gpioControl;
```
7. We try to define important info within this struct, one of the most important parameter for your struct object is the baseAddr.
8. include the <xil_types.h> to enable the use of u32 u16 u8 data types.
```C
    typedef struct gpioControl{
        u32 baseAddress; // u32 type is defined within <xil_types.h>
    }gpioControl;
```
9. We can also store different important registers within the struct.

# 3 important driver functions
- include<xil_io.h> to enable the use of Xil_out32 & Xil_in32
## INIT function
1. We have to initilize our HW, thus a initilization function is added
```C
     int initGPIO(gpioControl* myGpio, u32 baseAddress){
        myGpio->baseAddress = baseAddress;
        return 0;
     }
```
## WRITE function
```C
    void writeGpio(gpioControl* myGpio, u32 writeData){
        Xil_Out32(myGpio->baseAddress, writeData);
    }
```
## Read Function
```C
u32 readGpio(gpioControl* myGpio){
    return Xil_In32(myGpio->baseAddress+4); // Increment by 4 since we are reading from the second registers, i.e. slvreg1, and since it is byte address, offest is +4
}
```

## Header and .c
1. We only have the declaration in h and implementation in the .c file.

## Test
```C
int main()
{
    gpioControl myGpio; // Declare a defined gpio type
    initGpio(&myGpio,XPAR_GPIO_CONTROL_0_S00_AXI_BASEADDR); // Pass in the data type and the base address parameter
    while(1) // The embbeded system should not turn off, thus use while(1), if turn off, the system fails
    {
        writeGpio(&myGpio,readGpio(&myGpio));
    }
}

```
## Change driver folder
1. After modifying and creating the driver, remember to replace the files within the driver folder of your own IP.

## Back to packaging IP
1. The file group is modified and give warning after you replace and add your own file to the driver folder.
2. One must remove the old file directory in the xml file and add the new file directory to the xml file to enable vivado of finding the new added file.
3. However, the included file comes in with the ABSOLUTE path, which is not portable between different platforms, thus we must make further modification toward the xml file.
4. Remember to update, i.e. Repackage the IP first. So that the info can get updated.
5. Edit the xml file and try to modify it in detail. Search for driver in the xml, edit the absoluate path into relative path.

```C
 //Original absoluate path
 D:/VivadoProject/ip_repo/GPIO_Control/Gpio_/drivers/test.c
 //Turn it into
 drivers/test.c
```
6. After doing so, repackage the IP, we can see the directory is changed.
7. Afterall, upgrade the IP status to allow the upgrade of IP.
8. Then we export HW and open SDK, so that we can check whether the driver is being added and exported into the SDK.
9. Driver software = .h + .c~
10. Application software engineer simply need to know the driver declaration and know how to use the driver to develop their code.
11. Remember to add the .h file, to enable the use of drivers.

## Vitis
1. Note after the version of 2019, launch sdk is replaced by vitis