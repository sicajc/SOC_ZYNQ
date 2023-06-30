`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 11.02.2019 17:25:12
// Design Name:
// Module Name: Weight_Memory
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
`include "include.v" // This is like the header file, if added, some types of action would be taken on the directive.

// This type of coding style helps instantiate the Block RAM
module Weight_Memory #(parameter numWeight = 3, neuronNo=5,layerNo=1,addressWidth=10,dataWidth=16,weightFile="w_1_15.mif")
    (
    input clk,
    input wen, // weight valid
    input ren,
    input [addressWidth-1:0] wadd, //where to store the weight.
    input [addressWidth-1:0] radd,
    input [dataWidth-1:0] win,
    output reg [dataWidth-1:0] wout);

    reg [dataWidth-1:0] mem [numWeight-1:0]; // Width and Size of memory can be configured here

    `ifdef pretrained // This acts as a ROM. Initilizing the memory with the weight.
        initial
		begin
	        $readmemb(weightFile, mem); // This actually reads a mif file, memory instantiation file. Content of the file shall be in binary format.
	    end
	`else           // Otherwise this acts as a RAM
		always @(posedge clk)
		begin
			if (wen)
			begin
				mem[wadd] <= win;
			end
		end
    `endif

    // This is important for block RAM instantiation, if using Combinational circuit, this would be inferred using LUT.
    always @(posedge clk)
    begin
        if (ren)
        begin
            wout <= mem[radd];
        end
    end
endmodule