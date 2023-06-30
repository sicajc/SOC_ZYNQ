https://www.youtube.com/watch?v=_o4FbVFLbuw&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=39
# ZYNQ video interfacing
1. VGA interface video graphic interface, HDMI we can send video and audio at the same.
2. VGA and HDMI video interfaces is useful

# Resolution
1. Width * Height
2. Maximum supported resolution is 1080 x 1920 . the 1080p is the heigt of the VGA.


# Frame Rate
1. Number of frame per second. Most display runs at 50Hz or 60 Hz. Usual standard is 50 or 60 Hz.
2. So the raw throuhput 1920 x 1080 x 60 x 3(RGB) = 373.248 MB/s

# Synchronization
1. Vsync, hsync, determining where the line starts, where the line ends.
2. For Zedboard, we are going to use 16 bits instead of 256.
3. Strict syncrhonization of timing of the signal exists, pixel clock is not sent.
4. We need to send the syncrhonization signal and data bits. Pixel clock needed is 148.5 Mhz at 60 Hz.
5. One should strictly adhere to the frequency of the Pixel clock!

# AXI VIDEO STREAM VIDEO OUT IP
1. Combine the syncrhonization signal and video signal together and send it out.
2. tuser is same as Start of frame(start of frame)(EOF)
3. tlast is same as (end of line)(EOL)

# Video timing Control
1. Hsync and Vsync infos generation and sent to the Video out IP.

# Clocking Wizard
1. First try testing whether the IP is working fine.
2. If testing IP, simply use PL part. And one has to generate the clock through clocking wizard.
3. Clock frequencty of 148.5 Hz needed to be modified to the viable pixel clock.
4. Choose MMCM or PLL, both viable.
5. The locked means if the clk is locked to the desired frequency. We can actually remove the locked pins.


# Block diagram
1. Clock wizard + video timing controller + axi video stream video out ip
2. Video Timing control resolution can be set within.


# PINS assignment
1. Remember that your PINS can come from external world.
2. Remember to connect the reset signal to system resetn and clk to system clock

# Video timing generation clock enable
1. ctg_ce is also neeed to connect to gen_clken, the syncrhonization signal enable.
2. the clken and vid_io_out_ce and aclken it would be great idea to connect them to a constant 1 to always drive them high.
3. Make video hsync and vid_vsync external, also vid_data need to make external since we are connecting it to HDMI or VGA.
4. Note that since we can only assign 12 ports at once, yet IP signals sends out 24 data at once, thus we have to seperate it into 4 sections. And connection each section into 8 pins.

# Using the Slice IP
1. This 24 data slice data, can be sliced into multiple smaller ports using the slice IP. Each with 4 bits data.
2. We need 4 slices IP to do so. Seperating this 24 bits data, and then connect each of them into the external world.
3. RGB = {[0:3],[8:11],[16:19]} = RGB then assign them to the outer world PINS.
4. Note only the 4 bits within the 8 bits is the RGB signal needed to be sent to the external world.


# Testing video generator
1. v_tpg_0

## Drag and drop
1. We can actually simply drag and drop the design RTL design into the block design. We will not be able to group signals together if we do so.
2. The RTL design gets automatically converted into a block design.like this.
3. Remember to validate after connecting every steps.

## Create Wrapper and simulate the system
1. In the Precense of many IP, simulation would be significantly slower. Thus in this case, synthesis your design onto your FPGA might be an even faster option.
2. Using force clk and force constant can quickly test your design.
3. When using clk Wizard, we must wait for the clk to get locked, which might takes some time for the inner PLL to lock to a certain clock frequency.
4. It takes some time to actually generate the output in the simulation.
5. Sometimes, IP block need some time to actually synchronize with the data. Thus we have to let them run for some time to see the result.
6. If the IP is not working as you expected, look at the debug flag, underflow, overflow and status. These signals can be used simultaneously with dataSheet.
7. Note, whenever there is no valid signal sending out, we must put low on the value, simply create a easy mux in the block design level to do such thing.

## PINS assignment of VGA and other buttons
1. Look at the data sheet of your FPGA. Search for the PIN assignment.
2. Remember to change all of them to 3.3V


# Xilinx VDMA IP Core
1. Transfering data from DDR memory through VDMA into IP.
2. Portion of memory used for storing video ftames is called frame buffers.
3. VDMA is the special IP for sending the video.
4. Having a standard AXI-4 stream interface. It can continuosly send video data to VGA working in loop mode.
5. Note we can only access DDR through PS.
6. Note we have to make all the signal within the same clock domain.

# PS clock
1. We are not able to generate 148.5 Mhz frequency in PS. How to solve this?
2. Given 100 Mhz of PS, then use the clock wizard to convert the signal into 148.5 Mhz.
3. Note that the stable clock only come after a certain period of time after reseting.
4. Remember to let the locked controls the reset signal, s.t. the output is high only after the clock is stable.

# Frame buffer of VDMA
1. Simply give 1 for testing.
2. Remember to change the data stream to the one your IP use for VDMA. Otherwise, transmittion might fail.
3. Still remember to use unalinged data.
4. Access the HP port to enable the data transfering.
5. Also remember to fabric the interrupt on the PS, and connect the interrupt given by VDMA. Connecting it to the PS.
6. Remember we want every clock to be running at 148.5 Mhz. So that the data can be synchronized! Thus must reconnect all input clock of IPs into 148.5Mhz. Input of clk wizard still comes from PS!
7. Also the reset should all be synchronized, everyone should get reset from the peripheral reset. This cannot be made automatically, must disconnect PIN and do it manually.
8. Then do validation to ensure that you connects onto the correct port.

# SDK SW part
1. Need interrupt header, vdma header and cache header.
