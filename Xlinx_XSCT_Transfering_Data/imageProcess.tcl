connect
target 2
fpga rgbGreySystem_wrapper.bit
source ps7_init.tcl
ps7_init
ps7_post_config
dow xsct_test.elf
bpadd -addr &main                               #bpadd (breakpoint add), this add a break point in the main function
con -block -timeout 500                         #(telling the system to continue executing the program until it founds the main function)
dow -data lena_color.bin 0x11C05C               #
con -timeout 5                                  # Time the script should continue the exucution.
mrd -size b -bin -file out.bin 0x10C05C 65536