`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 02/08/2020 08:07:20 PM
// Design Name:
// Module Name: delayGen
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module delayGen(
    input clock,
    input delayEn,       // After enabling start counting delay,
    output reg delayDone // The hand shaking between modules, telling the master, delay is completed.
    );

reg [17:0] counter; // Since we need 200,000 cycles, 17 ~= log200000

always @(posedge clock)
begin
    if(delayEn & counter!=200000)
        counter <= counter+1;
    else
        counter <= 0;
end

always @(posedge clock)
begin
    if(delayEn & counter==200000)
        delayDone <= 1'b1; // Once delay is compelted, make delay done low.
    else
        delayDone <= 1'b0;
end

endmodule
