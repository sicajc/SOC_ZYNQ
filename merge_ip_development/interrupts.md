# Interrupts
1. Since processor has to process multiple peripherals at the same time instance, thus we need interrupts to tell the processor one needs immediate use. Processor starts other peripherals.
2. Polling is much slower than interrupts. Processor keep on checking if the peripherals are completed or not.
3. Interrupt signals are dedicated wires to the processor.
4. Once interrupts received, PC pushed into stack, services the interrupts, goes into ISR(interrupt sub-routine), automatically called , OS takes over.
5. When receive interrupt, we will go into ISR. Also we hope that when interrupt is being handled, no new interrupt is being sent in into the processor.
6. We would like to finish the ISR as soon as possible.

# IVT(interrupt vector table)
1. Linking ISR with a specific interrupt.


# PIC(programmable interrupt Control)
1. An arbiter to allow lots of peripheral to sent in the interrupt signals.
2. Interrupt request number(IRN), these are assigned during the boot operation, they must determine which interrupt raise the interrupt.
3. IRN is assiged based on priority, smaller IRN number means higher priority for processing.
4. One can set the interrupt sensitivity, edge sensitivity or level sensitivity etc...

# Modifying design to be compatible with interrupt
1. Add intr output signal to your module to enable interrupt

# GIC(Generate Interrupt controller)(PS)
1. After adding interrupt signal to your IP, you have to modify the GIC, beside the Snoop Control Unit.
2. GIC is also a memory mapped I/O peripheral,thus you can see it in the address map.
3. Interrupt, fabric interrupt to enable the PL-> PS interrupt.
4. IRQ_F2P.
5. Single interrupt line from the merge IP to ZYNQ processer connection can be done using dragging.
6. However, if you have multiple interrupts, you need a concatenation IP s.t. multiple interrupts can be connected and grouped into a single bus.


# Major Change in SW
1. There is a general way of coding interrupt ISR.
2. First need to add the header file of GIC. The driver managing the interrupts.
3. See the C code.