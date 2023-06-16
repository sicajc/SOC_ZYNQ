`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 02/25/2020 01:13:15 PM
// Design Name:
// Module Name: inverter
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


module inverter #(parameter DATA_WIDTH=32)
   (
    // The AXI clk
    input    axi_clk,
    input    axi_reset_n,
    //AXI4-S slave i/f, now slave is IP.
    input    s_axis_valid,              // Sends from master (PS) -> IP slave
    input [DATA_WIDTH-1:0] s_axis_data, // Sends from master data -> IP slave
    output   s_axis_ready,              // The IP should tell PS it is ready.
    //AXI4-S master i/f, now master is IP.
    output reg  m_axis_valid,               // Master is IP telling PS that its data is valid
    output reg [DATA_WIDTH-1:0] m_axis_data,// Master is IP sending out its data
    input    m_axis_ready                   // This ready signal is sent by slave, but receive by IP as master.
    );

    integer i;

    // Since we don't need to buffer the data, simply send ready as the requester is ready.
    assign s_axis_ready = m_axis_ready;

    always @(posedge axi_clk)
    begin
       if(s_axis_valid & s_axis_ready)
       // Following the protocal, when valid and ready are 1, we can send data in
       begin
           for(i=0;i<DATA_WIDTH/8;i=i+1)
           begin
              // Inversion main logic, each pixel subtracted by 255 invert the pixel
              m_axis_data[i*8+:8] <= 255-s_axis_data[i*8+:8];
           end
       end
    end

    always @(posedge axi_clk)
    begin
        //Telling the requester when the data is ready, since we are not trying to buffer
        // any data, simply propogate the valid signal
        m_axis_valid <= s_axis_valid;
    end


endmodule