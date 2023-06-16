# SDK of DMA
1. Most of the peripherals in system.hdf is in PS
2. Notice DDR, RAM0,RAM1 and DMA's address
3. For initial few bytes, it is not accessible by the Processor.

# Start application
1. Always create an empty application in SDK.
2. creating the source file.

# DMA functions
1. Every peripherals of Xilinx IP has their own drivers, they are within the bsp directory
2. find the include folder, you can find the header file from there.
3. the xaxidma.h has all the functions instantiation needed for DMA.
4. Most the details are within the src code. Comments and reference design for xilinx IPs.


# Including .h file
1. Find the struct within the .h, struct stores lots of infos for use.
2. Still Initilization, write and read functions are 3 most important functions.
3. For DMA, when initilizing, we have to pass amd set the config struct into the Initilization function.
4. Address still stores in xparameter.h

# SDK
1. You need to use SDK terminal to see the print out value, use these print out value for debugging!
2. Select the correct COM port and then program fpga.
3. Select standalone configuration, run ps_config etc...
4. Then remember to program from main vivado, programming the fpga.

# CLK of PS
1. In SDK, we have to give the clk from PS7_INIT in Run of SDK, otherwise warning is given in Vivado.
2. Then Application should also be check.
3. Remember you have marked some wires as debug, thus you can see the debug waveform after running SDK.

# Debugging
0. Whenever you modify SDK, tell the debug core to wait for trigger.
1. You need to setup the debug trigger, choose the signal you are familiar with to debug.
2. Remember to check whether it is rising or falling
3. Then set the sample rate, to capture the samples before the trigger and after the trigger.
4. Rerun the SDK to see the trigger.
5. Notice we are getting random value? Why? We are not getting 4,2,6,1,2,3,5,5

# Yellow slots
1. Interesting triggering signals are marked in yellow region.
2. Look for Valid and ready signals intersections, are the data you want to send met your expectation

# DMA controller control(Problem1)
- The Internal buffer of DMA went full! So we are no longer able to send data to DMA.
1. We need to write transfer to memory to peripheral & Peripheral to the memory.
2. The internal buffer of DMA got full! So we must first transfer the data from DMA to DEVICE. Deadlock!!
```
    Need to allow DMA to transfer data to Memory.
```

# Verilog IP(Problem2)
1. Note the problem in your design can be spotted through the waveform
2. Sometimes it is the problem of SDK, sometimes the problem lies in your own design!
```
    Check if the valid signal of AXI stream in pulled up and pulled down in a correct manner.
```

# Data Mismatching when sending data Problem3
1. The data sending out does not match the data we try to send.
2. From CA, there are caches within CPU. Since the data trying to write to caches does not get immediately transfer to DDR.
3. Since we try to first push the data into cache, yet we are calling the data from DDR, thus we get the random data from DDR which is not the correct value stored within caches.
4. Thus WE MUST FLUSH THE CACHE MEMORY! To invalidate and push the data from cache into the DDR.
5. Use the Dcache_flush function
```
    This is only needed when you are directly sending the data from you PS to your DEVICE and to DDR.
```
6. Remember to check the data you try to send in by printing out the new value located within the array.

# User Manual & Data Sheet
1. Check the user manuanl of your IP to the what is workable, what is not.
2. Then perform experiment on the manual info you got.
3. If the register number of a certain register is needed it has already been writtern within the datasheet.
4. TLAST would be on when sending out the last data signal.


# Block design modification for image processing
1. Width of buffer length register matters, if we want to increase the performance, must increase the width.
2. Changing the maximum burst size increase performance too. 32
3. Remember to check the allowing unaligned transfers to prevent the DMA from leading the incorrect data address.
4. If you use byte addressable, 0,1,2,3. using u8 this might cause problem. Since 0,4,8,16 is its expected addressing way, so remember to use unalinged transfer if you are not guarantee the data is aligned within your SDK. Occurs a lot in something like PCI-E transfer.
5. Remember in SDK, lcscript, the stack size should allow you to transfer enough data into PS stack.

# UART
1. To add UART into your SDK, look at the following video and modify the code.
2. After adding UART, it seems like PS does not play a role, that is not true, UART still need to pass through PS, thus we still need to flush DCache.

# Debugging tips
1. Whenever you create or use function in Embedded system, remember to check if the return value is correct, by adding early return and checking code, for debugging.
2. To transfer only from the headerSize part, use &imgData[headerSize]
3. Status checking is a important feature.
