`define clockFreq 100000000 //In Hertz, since the clk runs in 100Mhz, yet video interface is in 148.5Mhz, thus 2 clock domain.
`define minPulseWidth 0.01  // If 0.01 second duration, it is consider as a valid press.
//`define clockCount `minPulseWidth*`clockFreq

module debounce(
input       i_clk,
input       i_button,
output reg  o_press
);
// Generating a single pulse.

integer counter=0;
parameter clockCount = 1000000; // clock counts 0.01 seconds result, after reaching that maximum value, it holds there.

always @(posedge i_clk)
begin
    if((counter < clockCount)&i_button)
        counter <= counter+1;
    else if(!i_button)
        counter <= 0;
end

always @(posedge i_clk)
begin
    if(counter == clockCount-1)
        o_press <= 1'b1;
    else
        o_press <= 1'b0;
end


endmodule