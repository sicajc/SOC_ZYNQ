`define clockFreq 100000000 //In Hertz
`define minPulseWidth 0.01
//`define clockCount `minPulseWidth*`clockFreq

module singlePulseDebounce(
input       i_clk,
input       i_button,
output reg  o_press
);

integer counter=0;
parameter clockCount = 1000000;

always @(posedge i_clk)
begin
    if((counter < clockCount)&i_button)
        counter <= counter+1;
    else if(!i_button)
        counter <= 0;
end

// Once running for 10 milliseconds, 1 would stays high.
always @(posedge i_clk)
begin
    if(counter == clockCount-1) // This generates only a signel pulse
        o_press <= 1'b1;
    else
        o_press <= 1'b0;
end


endmodule