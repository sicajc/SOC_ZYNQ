### https://www.youtube.com/watch?v=V8jW81VaLOg&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=8

### https://www.youtube.com/watch?v=kTjeTOf6ACI&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=9

# SPI Controller note
0. xc7z020clg484-1
1. SCLK and SDIN these clock for SPI comes from the master, like OLED and so on
2. CS#, # means chip select active low to use OLED. N/C means it is always selected. 0 to mean selected.
3. clock frequency supported by this spi-controller is 10MHZ.
4. SPI sends in a byte of data, then send it out in a serial manner.
5. Whenever we want to use SPI, we would have SCLK oscillating; otherwise, it is high, indicating we are not using the SPI.
6. Use the FORCE value for rapid simulation, we can save you some time of coding for the testbench.

# ILA note
1. Synthesis the design!
2. Setup Debugging
3. Find the signal you want to debug by searching in a linux fashion. like *spi*. Specially noted that the signal you found are origniated from the netlist, not from your src code.
4. Remember to connect the I/O pins to your design, otherwise debugging cannot be made! In order to connect to the correct I/O port, one must read the document to enable the correct I/O port of the design!
5. We must specify the sampling clock for every signal! Thus we have to add a clock domain for every signal! Simply assign all clk domain.
6. The main clk signal cannot be captured.
7. You must specify the PIN you want to use for your signal, for debugging. Also, the working voltage of I/O must be specified, you cannot leave it as default!


# ILA debug window
1. Must add the trigger condition.
2. Must know when a certain event happens, in this scenario, when load_data is 1, we know that the spi starts sending value.
3. When the signal goes from low -> high, we would know how data transfer in SPI.
4. Trigger position in window: 1024, means how much we should captures before the condition happens.
5. Remember to change the value of trigger condition to R. meaning 0->1
6. The T window is where the condition happens.
7. To connect the trigger onto the board, you must remember how you manage your I/O port.


# Note
1. Vivado might synthesized away some of your signal.
2. In debug, you can actually prevent it from optimizaing away your signals.
3. After adding the debug core for the first time, you can press the debug again
4. Notably, fsm state will get optimized to one-hot assignment!
5. State variable is completely changed after synthesis.
6. We would like to preserve the debug pin, we dont want the signals to be optimized.

```verilog
    (*KEEP = "true"*)
    reg[1:0] state;
```
- This is only understood by Vivado, thus it would preserve the original state without optimizing it.
