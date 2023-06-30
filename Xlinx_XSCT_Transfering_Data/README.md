https://www.youtube.com/watch?v=mdRYY-mpMYI&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=55
# Xilinx Software Command line tool(XSCT)
1. In long term, to transfer data from Board to Zedboard, usually uses UART or SD card.
2. But this is good for debug though it is hard for distributing to other system.

# Usage
1. Transfer the image from PC to board using JTAG interface. XSCT Console exists in SDK
2. XSCT can transfer data to ANY location in DDR.
3. In this tutorial, polling is used. However, interrupt can also be used.
4. Remember to print out the image buffer address and the end address of DDR. Here it is 0x130320
5. Later we can wait for the user to transfer the data. By inserting a scanf to continue.
6. Insert the following command in XSCT TO TRANSFER the data to DDR
```C
    dow -data lena_color.bin 0x130320
```
7. To send the image back to PC from Board use the following command, the starting address of system memory which would get printed in SDK if you specified correctly.
8. 0x120320 is the output buffer address of DDR system memory. 65536 is the size of your image.
```C
    mrd -size b -bin -file out.bin 0x120320 65536
```
9. To see the image, you have to run the matlab script for the correct image conversion.

# Usual case
- If we have access to bit file,elf file and tcl file how to program my code? We can also use XSCT to initiate the design.

1. Move the the xsct folder

## Connect
- To connect onto the board
```
    connect
```

## Targets
- Lists all the support targets on the JTAG chains. It would display the states of your board devices.
```
    targets
```

## Program
1. Program bit stream
```
    fpga rgbSystemWrapper.bit
```
2. This fpga actually is the same as program fpga on sdk

## Select your target core
1. To select the PS you want on your zedboard
```
    target 2 or 1
```
2. Then source the tcl
```
    source ps7_init.tcl
```
3. This actually is the same as init fpga in sdk.
```
    ps7_init
```
4. Also we need to ps7_post_config, which enables the barrael shifter to transfer the clock from PS -> PL


# elf
```
    dow xsct_test.elf
```

1. Processor goes into suspended state.

```
    stop
```
2. The program always start at the first program of DDR, 0x00100000 is the first address of DDR in the zedboard. This can be viewed in the elf file in SDK.

```
    con -addr 0x00100000
```

# Downloading the image file
```
    dow -data lena_color.bin 0x130320
```

# Readback for image checking
```
    mrd -size b - bin -file out.bin 0x120320 655536
```
- later remember to convert the transfered data using the matlab script for display.

# Combining all of them together into one scripts tcl file
1. Configure the environment variable.
2. Include the xsct.bat within the bin of SDK to the environment variable to directly run it on the command prompt.
3. The imageProcess.tcl


# RST
```
    connect
```

```
    rst
```
