`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/31/2020 10:09:05 PM
// Design Name:
// Module Name: conv
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


module conv(
input        i_clk,
input [71:0] i_pixel_data, // Feeding in 9 pixel of data
input        i_pixel_data_valid,
output reg [7:0] o_convolved_data,
output reg   o_convolved_data_valid
    );

integer i;
reg [7:0] kernel [8:0];     // Kernal is of size 3x3
reg [15:0] multData[8:0];   // The bit extension after mult must be considered
reg [15:0] sumDataInt;      // Note one must take care of the arithmetic bit extension
reg [15:0] sumData;
reg multDataValid;
reg sumDataValid;
reg convolved_data_valid;

initial
begin
    for(i=0;i<9;i=i+1)
    begin
        kernel[i] = 1;
    end
end

always @(posedge i_clk)
begin
    for(i=0;i<9;i=i+1)
    begin
        // Convolving for one cycle, architecture pipeline.
        multData[i] <= kernel[i]*i_pixel_data[i*8+:8];
    end
    multDataValid <= i_pixel_data_valid;
end


always @(*)
begin
    // ADDER TREE, this is a valid coding style. Synthesizable
    sumDataInt = 0;
    for(i=0;i<9;i=i+1)
    begin
        sumDataInt = sumDataInt + multData[i];
    end
end

always @(posedge i_clk)
begin
    // 1 cycle for sending to the output data.
    sumData <= sumDataInt;
    // Second pipeline stages.
    sumDataValid <= multDataValid;
end

always @(posedge i_clk)
begin
    // Divide by 3, third pipeline stages.
    // Note valid signal is being propogated along side with the data.
    o_convolved_data <= sumData/9;
    o_convolved_data_valid <= sumDataValid;
end

endmodule