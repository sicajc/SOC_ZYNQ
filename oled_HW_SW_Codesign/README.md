## IP generation and software driver
0. xc7z020clg484-1
1. Since if we develop solely using HW, if we want to modify something, we must comlpetly change the original design and recompile the whole design which is an extremely inefficient manner of doing things.
2. We will try to modify the stateMachine into Register, to allow the PS part to control the OLED Control through writing into the registers.

## Replacing state machine into Control Registers
0. The data register is needed to sendData
1. Connect the 2 status register, dataValid signal and sendDone signal are 2 status registers needed to connect to the slave register, s.t. we can control the HW using the PS. By writing into these slave registers.

## Packaging your design into IP
1. Since the number of registers, data + sendValid + readvalid , 3 registers are needed, thus we would keep the number of registers during instantiation to be 4 setted as default.
2. Remember to use the edit IP.
3. Remember when creating the IP, you had better put it into the shared IP_REPO of your whole project.
4. Remember to check copy sources!

## Plan
1. Move the data to the registers.


## Calling IP
1. After calling out IP and connect it onto PS, remember to make the output port external.
2. The external ports are I/O of your ZYNQ board. Remember to connect them.
3. Then we have to specify them in Synthesis I/O planning!
4. create HDL wrapper

## Generating output products!(MINIMAL STEPS FOR Exporting into SDK)
1. Before exporting HW, if you dont have bitstream, use generate output product in sources->IP
2. This Generate output products step is the minimal step required to send your design and shared it with SDK.

# SDK
1. Driver -> Header -> Implementation -> Application.
2. Remember to use struct!


# Note
1. Whenever you run the program or tell HW to work, the previous result would be stored within the RAM.
2. In order to prevent the OLED  from displaying the past data, you must first clear the data this can be done by adding additional functions to SDK, using PS to clear everything first.
3. Moreover you can connect the SPI controller to SW side, s.t. we can reset and modify the SPI controller using PS.