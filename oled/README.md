# OLED
1. To display , we must put the data into the GDRAM
2. We read the pixel value column wise, writing the data into GDRAM, we must store a particular sequence into the GDRAM for the OLED to display the correct information
3. One must find their own data representation to write into GDRAM for the display of the string.
4. One can use tool like GLCD Font creator, we can create the bit map for the correspondent character.

# OLED Interface
1. We need data and column to OLED.
2. Also we have to tell the OLED what kind of data we are sending? Whether we are sending data or we are sending the commands!


# Sharing source file
1. When sharing your design in xilinx, you only need to give other the source file and constraint file, it would suffice.
2. Note when adding source, use copying file to prevent destruction of your own file, however you might not be able to modify it in the same file you just created.

# Hierarchy of simulation
1. Note when there are multiple modules, we must specify the correct top module for simulation.
2. By pressing the setting button, simulation, one can know which top module to be simluated from.
3. When simulating multiple modules, we usually use Divider to seperate multiple data ways.
4. Also using force value and force clk can help you quickly verify your circuit.


# HW/SW Co-design
1. Using PS and PL at the same time. Using the IP of Xilinx.
2. Remember to connect the necessary ports.
3. Beware whether the processor reset and the AXI interconnection is connected, are ARESETN
4. Also remember the port connection, check for the I/O port, is it connecting to the correct external port?

# After modifying
1. Program the FPGA from SDK.
2. Remember to export the bistream into SDK to enable the program of FPGA.
3. Then run configuration, use standalone debugging, runps7 config.
4. We then have to program from vivado.
5. Then do the software run from SDK.

# Notice