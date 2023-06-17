https://www.youtube.com/watch?v=I0eu_Y3pMmM&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=14
# IP core customizations
0. xc7z020clg484-1
1. The interfaces should be specified first!
2. Your IP is usually the Master for DDR and SRAM. The slave of PS.
3. There are a number of registers within your IP, PS would write into the Registers within your IP.
4. We must choose the AXI peripheral for your IP s.t. you can communicate with other modules.
5. Remember to store it into the global IP repo folder
6.  We would like to edit the IP, so choose edit IP

# IP folder
1. The most important file within is the component.xml file!
2. The xml file has all the information of your IP!
3. It has drivers and block design within the ip folders for testing and integrating your IP.

# IP compatibility
1. The xml file has all the info needed and it would be shown as a package IP interface.
2. The axi-inst, it automatically implements the AXI-4 interface for your desired IP.
3. The package IP parts has all the information needed for GPIO.

### Controlling I/O registers
1. slvreg = slave registers for the PS to control the inner part of circuit through the I/O port.
2. We have address and data registers.
3. Look for the src files, and pay attention to the slv_reg, Module connection part and Parameter part.
4. If you want to add new port, you must add it in both top module, your inner instantiation module, and the register to control your new port. i.e. slv_reg must be edited! And it must not have conflict with the original slv_reg.

```C
    slv_reg_wren tells when the PS can access the register with the IP.

```

## Adding the port to your IP
1. We now want to access the leds by adding more I/O ports to the IPs.
2. Go to the user add ports, add the desired port you want onto your self defined IP.
3. Add the port in the user port section of the top module of your own IP.
4. Since leds update its signal, thus we have to add it into the your instantiation module also.

## Registers I/O
1. Connect the port your want to change into the Slave registers that will controlled by PS.
2. We have to connect register with the port your want within your module file.
3. If you want the to add new master to your IP. You must modify your own code.
4. I want to always connect switches to slave1

## Updating xml file and Repackaging your IP
1. Whenever you modify your source code, you must refresh your xml file!
2. You can then see the ports is added to your own IP
3. Remember to tell the vivado settting to NOT DELETE your project after packaging!
4. Uncheck it whenever you want to package the IP!

## After editing
1. The info you just created would all get saved within the ip repo folder.
2. Note if the called project does not have the same GUI  in your design, try to restart vivado.
3. Note by default, IP would be displayed in your IP catalog list! You must tell vivado where to search your own IPs.
4. You can do so by going into Settings -> IP -> Repo
5. Your user defined IPs output must be make external manually.
6.  Wrap your design then synthesize it!

## After completing block design
1. I/O planning after synthesis, we have to help ZYNQ to connect the I/O to the correct PINS on the ZYNQ board.

## Address space
1. Note when you write into your IP register, to write to led, write to the base addr. However, if we want to read switches, we have to read from BASE_ADDR+4 since we add the switches to slv_reg1. slv_reg0 sits in the BASE_ADDR.

## Further modify your IP
0. Since we want to reuse the IP, make this IP parametrizable. Go to your IP repo, and try to modify the IP, add the additional ports and parameter onto it making it parametrizable.
1. When you are porting your IP to other board, we have to modify the number of ports on IP and number of I/O ports on your IP.
2. To do that we must edit our original IP.
3. Later update the the xml, you will see your parameters hidden, change them and make those visible in the xml file.
4. Remember to upgrade the design after modifying the configuration.

## Design
1. In the Design -> Hierarchy -> IP sources, you can find the IP source file.
2. After changing any configuration in the GUI, you have to resynthesize your design!
