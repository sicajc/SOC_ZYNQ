https://www.youtube.com/watch?v=1CfFmKX8uj0&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=44
# ImageProcess
1. DMA and PS
2. DMA remember to use unaligned transfer.
3. Since the data coming out from IP is 8 bit, we must convert the data width into 32 bits. Using Width converter.


## Frequency is 148.5
1. Get the clock wizard for converting the PS clcok into 148.5 Mhz
2. The output of PS is 100Mhz, connect it to the clocking wizard.
3. Then connect the generated clock with the rest of the system.
4. Remember to use lock, on the ps reset, so that the system would stays reseted before the clock is locked onto the clk.

# High performance port.
1. To connect AXI DMA with DDR, must turn on the HP port.
2. Remember to connect the reset of your IP too.


# Video Interfacing part
0. Remember to bring hsync and vsync external. Outputs RGB should also get externalised.
1. Bring video timing controller, disable the AXI lite interface, no need for detection.
2. Connect enable with constant, reset with the rest of the system.
3. Note the output to video interface must stays 0 when it is not available.
4. Bit slicing using xslice. Since RGB slices has only 4 bits, however the total data width sent in is 24 bits.


# Connection of VDMA
1. When connecting you must beware of which clock you conneceted to and the ports allowed within the design.
2. Later adding the interrupts, interrupts must be arbited through the Interrupt IP. Also we have to turn on the IRQ from PS.
3. Note by manually connecting signals, address map is not mapped automatically, thus we must remember to do the auto assignment of MMIO.

# Warning
1. For Width mismatch, it must be solved, changed the width of the streaming data.


# SDK
https://github.com/vipinkmenon/imageProcessingVideoOut/blob/master/sw/imageProcess.c\
- From the SW part, put the test image into the function.


https://github.com/vipinkmenon/vga
