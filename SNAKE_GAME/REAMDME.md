https://www.youtube.com/watch?v=_rxgdctIv9U&list=PLXHMvqUANAFOviU0J8HSp0E91lLJInzX1&index=46
# Snake game
1. Software HW Co-design of snake game.


# Reading from other prebuilt bitstream and elf file
1. From the run configuration, we can select the bitstream file and the elf file, for running the software design.
2. NO need to rerun the whole process again!!!

# Concept
1. First consider the whole display as grids.
2. When displaying the snake , each body part is a grid.
3. The snake and prey will be always in the multiples of grids.
4. Listing out the algorithm by text and your text description.
5. Arrays of snake size stores the position, shifting operation is easily implemented in HW but complex in software which is not that efficient.

# Debounce
1. Simply drag and drop into the design


# The video interface is the same


# Debouncing
1. Debouncing circuit is added s.t. the up down right left can be controlled smoothyl
2. Requires 4 debounce logic, simply drag and drop into the block design.


# Clk Wizard
1. We can generate multiple clk frequency from the clokcing wizard, since debounce circuit needs different clk.
2. The button pins should be made external using the GPIO of ZYNQ.


# Slave AXI of snakeTracker
1. It should be connected to the GP port of PS.
2. Note that we cannot run automation here, otherwise the design get messed up.


# Syncrhonization Reset
1. Note since the design of PS's reset is not synchornized with the system I used.
2. Thus copy another reset block, connect the clk to this newly created reset. S.t. we can make the system work,
3. Then when running automation, the clk of autoation should choose 100Mhz. Might get connected to 148.5 which may get messed up.

# GPeripheral
1. Whenever adding a new IP, we have to add another port for the AXI interconnection.

# Validate the design
1. Note the clk domain should be connected in the correct way, otherwise the design get messed up.
2. Reconnect the PINS.


# Sepration of Clk domain
1. We want the VDMA domain at 148.5 Mhz
2. The control domain and HW domain runs at 100 Mhz, check for the correspondent design's reset and clk, whether you connect them to the correct clk domain.

# Constraints
1. Remember to add the button constraint.

# AXI GPIOs
1. Center push botton to control the reset of the game, make this GPIO port external, center button gets controlled.


# SDK
1. https://github.com/vipinkmenon/Znake
