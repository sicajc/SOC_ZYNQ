https://www.youtube.com/watch?v=lzQ9hJ-wevg&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=21
# Data UART
1. To do big data transfer from computer, we use ethernet in LINUX.
2. We can use the UART interface to transfer large amount of data from Computer to your board.
3. Later we will compare the Sole SW  and HW/SW Codesigned result.


# Inversion
1. We would send the image from computer to Zedboard through UART.
2. Processed in ZEDBOARD'
3. Sent it back


# bmp format
1. We want to see the picture information stored within the the image.
2. HxD can see the Hex information stored within the image.
3. Notice the head value is BM, computer understand this is BMP.
4. When processing the image, we should not make any change to the header file!

# procedures(Software)
1. PC-> UART -> ZYNQ DDR -> PS -> DDR -> PC
2. Heap might not be able to handle the large memory sent by UART
3. lscript.ld has the stack and heap size information
4. You can find the size of the memory you want to allocate by calculating the size of an image in HEX.

# Perform memory allocation
1. Since default heap size cannot handle your image, we have to change the size of heap allowance
2. Edit the lscrpit.ld file and change the heap size to the desired size you want.

# TerraTerm
1. Uses TerraTerm to transfer your data through UART onto your ZYNQ board.
2. The baudRate must be set to the correct value you specified. And Connect them to the available COM port
3. TerraTerm is able to send the whole image
4. REMEMBER TO CHECK BINARY FILE!!

# HxD
1. Helps you examine your img file data, and spot the headerSize of your img.
2. Usually img has 2 parts, header Part and the data part.


# DataLoss during UART(Dummy bits)
1. Some data might get lost during UART transfer.
2. You must know ,headerSize,your image size beforehead to send the data.
3. Do a comparing, see how much data is loss during transmission.
4. In-order to reopen the received image, one must add dummy bytes in TerraTerm, pad the lost data, using 00

# Why do we have Dataloss when using UART?
1. Since in UART interface, no handshaking mechanism between transmitor and receiver.
2. Now we only have the USB interface, since the data is sent through USB to UART bridege.
3. Converting the USB interface into UART interface.
4. When we are sending at very high rate from ZYNQ to PC, the internal buffer of bridge between UART -> USB might overflow, so we give it sometime using sleep(1000) s.t. the data can be sent out.
