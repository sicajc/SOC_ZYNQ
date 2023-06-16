#　Merge Opeartion of two arrays
1. Given two arrays, merge them into a large arrays.
2. When doing compare and reading datas, one must set the maximum size of the arrays.

# Address mapping
1. You must first declared the correct mapping address for your control registers.


# Simulation in AXI.
1. We would not try to code our module directly into the automatically generated AXI_inst since it would be very hard to simluate from there, testbench is required to do a rapid simulation on it.
2. So we would code the module outside, it is always better to keep a Hierarchical design for easier debug and testing.

# FIFO IP
1. First Word Fall thorugh, FIFO would prefetch the next data, read latency vanishes which makes implementation simpler.

# Building IP core
1. Build your IP core seperate from the main AXI, then test it by injecting some basic signals.
2. After testing it is working in a fine way, add the modules into the 0_S00_AXI instantiation.
3. Remember ARESETN is negative trigger active low.


# Modifying Control Registers in AXI interfaces
1. SET BY SW CLEAR BY HW.
2. slv_reg0 ~slv_reg3 can actually changed name, as long as you follow the control rule of AXI.
3. Define your slave registers according to the memory map you just defined.

# After modification
1. Remember to add the Vivado's IP xci file into your IP packager. Vivado would not add those files into your IP automatically.
2. Tools to create package IP and include the xci file if it does not include the xci files for your own design.
