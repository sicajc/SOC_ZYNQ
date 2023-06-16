https://www.youtube.com/watch?v=LHfm91SThqI&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=25
# Using Xilinx IP core
1. How to use Xilinx IP cores within your VERILOG CODE. Like PLL.
2. Xilinx IP cores are sometimes more optimized and have better performance.

# FIFO IP
1. When data cannot be completly stream based, we must store initially data somewhere. FIFO is very useful.
2. Different flavour of FIFO cores exist for different usages.
3. sEARCH FOR fifo in IP Catalog. FIFO Generator can be used.
4. Usually we go for Native one or AXI stream one. Not AXI MMIO mode.

# Xilinx SOC Block RAM
1. Xilnix On chip memory has BAlock RAM. You can use different HW on FPGA to implement your IP. Distributed RAM uses LUT, Block RAN uses on chip SRAM.
2. Based on different features you choose RAM or Distributed the function matters.

# Common clock and independent clock RAM
1. CDC, need FIFO for solving the clock frequency mismatch problem, independent clock RAM is this kind of Asychrnious FIFO.
2. Read in 150MHZ and write in 200Mhz
3. Common clock FIFO is in the same Clock domain.

# Asymmetric FIFO
1. Input 8 width -> output 32 width. Reading and Writing has different data width.
2. Remember to check Reset FIFO.
3. Output Register increase the Read Latency. Can be added to improve CP.

# Status Flag
1. Almost full flags -> if there's only 1 slot, flag -> 1
2. Programable flag, can raise high for user defined number of datas in within the fifo.

# Sumary
1. Tells you how much resource might get used.

# DCP and XCI file
1. After generating this IP, Xilinx will not give you the actual verilog code, netlist given only.
2. dcp file is generated , this has the netlist of the used IP.
3. IP file, xci file

# Using this IP within code
1. Instantiation template, veo and vho used to instantiating the module.

# Testing FIFO
1. Simply connect using easy module, then try to
2. For commmon IP cores, they have their own maximum frequency restriction, so one should not be using more than 150 Mhz 100 Mhz.

# Commonly used IPs
1. Memories and FIFOs are frequenctly used. We will need to implement a small RAM.
2. PLL is also frequenctly used, Clocking Wizard.
3. In Vivado we should not use hand conversion from 100->10Mhz, we will use the clock wizard IP to convert the clk frequency to lower clock frequency.
4. 1 clk wizard can provide you lots of different clock frequencies.
