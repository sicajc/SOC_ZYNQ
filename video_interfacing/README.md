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


# Test video generator
1. v_tpg_0

# Drag and drop
1. We can actually simply drag and drop the design RTL design into the block design. We will not be able to group signals together if we do so.
