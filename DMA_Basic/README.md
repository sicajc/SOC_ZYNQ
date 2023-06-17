https://www.youtube.com/watch?v=Xkpu8BXi3aI&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=17
# DMA transfer(AXI)
0. xc7z020clg484-1
1. AXI consist of a valid,data from master and ready from slave.
2. The Slave must also produce a response telling master that it has received the data successfully.
3. data transmission happens only when valid and ready is both high~
4. AXI stream provides 1 data transfer per cycle in the ideal scenario~

# Creating new AXI Stream
1. Though one can directly use the AXI-4 stream given by Xilinx. However, most of the signals are actually not needed. The only signals needed are valid,ready and data.
2. Thus we would like to code our own AXI-4 stream interface.

# Inversion
1. Simply subtract each pixel with 255 to get the inversion value.

# Slave and Master
1. In AXI-4 each module has 2 character, one as master another one as slave.
2. In each character, they have 3 important signals, valid,ready and data.
3. ready signal is sent out by the receiver to indicate that I am ready to receive the data.
4. data signal,valid is sent by the transmitter
5. Only when valid and ready are both 1 should the receiver take in the data; Otherwise, the data is not valid at all.

# Simluation pros of Vivado
1. In vivado we can actually force value to perform a rapid simulation on modules.
2. Some easy modules can be tested using this kind of method.
3. Otherwise we have to code out some testbench which may sometimes be tedious.

# Create IP
1. It creates the project and saves its info within the xml file.
2. Notice that GUI mix master and slave together. We can adjust the GUI interface into master and slave just like the AXI interfaces.

# Modification to GUI
1. Regrouping the slaves and master.
2. Add the interface in Ports and interfaces through adding.
3. Add the interfaces to your GUI of block design, to make it more readable.
4. Select the interfaces you want from add interface.
5. Do the port mapping, need to map the signal names into standard name used by xilinx.
6. We try to group all the master signals together, and group all the slave signals together.
7. Also we have to perform ports remapping s.t. they are actually grouped.
8. Remember one is slave another one is master, they are different and must choose different mapping selection.

# DMA block design
1. Note there should be no way you can directly connect your IP to PS if your design is stream base
2. Since being stream based, there is no address for it.
3. Thus we have to use DMA.
4. S_AXI_LITE is used to configuring
5. Master interface of DMA should be connected to the slave interface of your IP
6. You master interface of your IP should be connected to the slave interface of DMA. AXI.

# Scatter and gather
1. An advanced feature of DMA, not used for now.

# HP port and connection of DMA
1. All the port of your IP must be connected, connect your reset signal to the axi_resetn
2. The master interfaces shall be connected to the external memory.
3. In PS, we have to connect DMA with PS, need to open the HP port on PS to enable DMA
4. DMA and both PS can accesses the DDR, DMA access DDR through the HP port interfaces.

# Debugging in block design
1. The connections between your block design, can be debugged by right click debug.
2. ILA debugger would be added
