https://www.youtube.com/watch?v=Zm3KzhahbUg&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=29
# Point processing
1. Input image, do some operation on image then get the output image.
2. Output image is only depends on the function and the input image.

# Neighborhood processing
1. The pixel value depends on the neighboring pixels too.
2. Take in an image, perform kernal operation onto the image using a mask depending on what you are trying to accomplish. MAC

# Data transfer problem
1. Pure streaming based approach is not possible for this image processing core! We must buffer the data within the IP.
2. Not practical to buffer the entire image within the IP. BRAMS on quite limited.
3. Practically, we would only buffered a part of image within the IP.

# Line Buffers
1. Small RAMs for storing 1 line of image pixels.
2. Size of Line buffers is determined by the width of image.
3. In convolution, same line buffer is used for multiple times.
4. We can improve performance by adding 4th line buffer, while convolution is in progress, we can store the image pixel into the 4th line buffer, originally 3.
5. Make assumptions, writing one pixel into the line buffer at a time.
6. Remember we have to flatten the data in verilog I/O port.
7. Line buffer has read and write part. We try to read 24 bits = 3*8bits data at once to increase the throughput. Reading out 3 bytes in one shot.

```verilog
    always@(*)
    begin
        for(i=0;i<9;i=i+1)
        begin
            // This is the combinational circuit of parrallel adder tree.
            // We have to consider the maximum value of sumData.
            sumData = sumData + multiData[i];
        end
    end
```

# Implementation note
1. Use a calculator toe check for value, to allow better analysis of bit width.


# Control Logic
1. pixelCounter are used to calculate the total receivedPixel, whenever one line buffer is full, we store the next pixel into the next line buffer.

2. ShortCut ways of declaring.
```verilog

    always@(*)
    begin
        //line buffer is a 4 bit value, others remains 0.
        lineBufferDataValid = 4'h0;
        lineBufferDataValid[currentWrLineBuffer] = i_pixel_data_valid;
    end
```
3. Note we now have 4 line buffers, 3 used for processing at a time.

# Packaging the IP
1. We must first add the AXI interface and add interrupt to your IP.
2. Note every AXI interface is just a handshaking protocal. One Master port and one slave port for handshaking, each with  valid and ready.
3. Adding FIFO to the output buffer can help preventing the mismatch of data in of AXI and data ready out of AXI. Thus FIFO is usually used to handle the mismatch of this data transfer sceheme.

# FIFO generator
1. Find the IP in fifo generator
2. We are using the AXI stream fifo.
3. Threshold assert value, if I have 8 data iwthin the FIFO, axis_prog_full = half full, telling the other circuit, I am already half full, once half full, start giving the IP data, sending out data.
4. Prevent the data lost.

## Interrupt
1. When should we send the interrupt? Once finish processing 1 line buffer, we send out the interrupt to the PS. Telling PS, you can send more data in now.
2. Same do the port mapping after you create the IP.

# Simulation
1. Writing a simple testbench, aim to simply check if my DUT works, instead of forcing value.
2. Beware of the headerSize, must first read in the header to the image byte by byte, otherwise we cannot open the value.
3. Using %c to read a byte in.
4. To enable the opening in window image viewer, header part must also be included.

# Problem of dimensionality reduction
1. To overcome this, we would send in two dummy lines,0. Without doing so, input image size is not the same as output image size.
2. Thus one must combat this by sending in dummy rows to combat the lost of dimensionality.
3. The lost of the first row and the lost row must be combat.

# Interrupt
1. Once one line processing is over, the convolutioned result would be sent through DMA using interrupt to the DDR.
2. Interrupt is on only if one of the line buffer is empty!

# Stream interface
1. Read interface use 8 bit, since DMA data read is of 8 bit port.
2. Note in Block design, remember to do the width conversion for width matching. Otherwise, width mismatch.
3. width converter, take 4 datas with 8 bit using 4 consecutive cycles, convert it into 32 bits, data then send it to DMA.

# HP port and ACP port
1. In addition to HP port, High performance port, there is another port called AXI ACP port for usage.
2. Accelerator coherance port, since data would be cached after taking it from processor.
3. Using ACP port, cache coheracny can be maintained, thus we no longer need to use the DCacheFlush.


# DMA in block
1. Notice 2 interrupts exist on DMA and also interrupt exists on imageProcess, thus we have to use concat IP.


# Block design
1. DMA controller
2. Interrupt controller
3. UART


# Image data
1. Instead of sending in the array, simply make the image a part of your software code, store it as a char array inside your code.
2. Bring the imageData itself.
# SDK
1. Now we are now using interrupt, not the polling mode, also we do not need the cache Flush since we are now using ACP port not the HP port.


# Adding back the Header Part
1. Remember after sending the image through the UART, your received data does not has the header part.
2. One must remember to restore the image by re-concatinating the header part using HxD, otherwise you cannot re-open the image.