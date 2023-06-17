# SOC_ZYNQ
## Usual Design Flow
1. Design your own IP.
2. You can test your IP in 3 ways, one through coding up a formal testbench or formal verification.
3. Another one through some easy built testbench.
4. Or simply rapid forcing the value on the waveform viewer
5. Create your IP and specify the interface you want. Usually AXI is the option.For custom interfaces, reading to do the port mapping.
6. xml file stores all the info of your IP.
7. Define the address space for your IP, follow the address space defined by your IP and modify the slave registers within the AXI_inst.
8. If you want to add additional port to your AXI design, you have to add port in both top and within the wrapper.
9. Build the block design, connect the blocks you want to your design, remember to use 100Mhz on your PS.
10. If you need DMA transfer, HP0 must be turned on.
11. Search for the space of your own IP in SDK.
# SDK
1.  Remember to check the size of the heap.
2.  Remember to create empty project.
3.  For every sdk function. the return value must be guarded with the exception handling code, if the Status is not as expected, return!
4.  In Embedded system, we can not allow the system to go down, thus while(1) or while(condition) is commonly employed.
5.  Read for the documentation for the function you want to use for your design in SDK.
6.  MMIO