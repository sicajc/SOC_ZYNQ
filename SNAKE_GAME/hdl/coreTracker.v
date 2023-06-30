`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 04/15/2020 12:06:42 PM
// Design Name:
// Module Name: coreTracker
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
`define MaxSnakeLength 1600 // The length of snake, we do not want the size to be too large, otherwise lead to timing violation of 148.5 Mhz

module coreTracker(
input        i_clk,
input        i_reset_rdPointer,
input        i_reset,
input  [15:0] i_start_pos,
output reg [15:0] o_cellPos,
output reg    o_valid_cellPos,
input  [15:0] i_preyPos,
input  [15:0] i_snake_size,
input        i_rd_pos,
// These check whether a certain button is pressed
input  wire  i_down,
input  wire  i_up,
input  wire  i_left,
input  wire  i_right,
input  wire  i_move,
output reg   o_game_over,
output reg   o_preyInBody
);

// The SW reads this snakeBody to allow it to be printing onto the screen
reg [15:0] snakeBody [`MaxSnakeLength-1:0];
// This 1600 size of snake body would always stays, everything is fixed. A huge memory allocated for snake body.
reg [15:0] rdPointer;
reg [1:0] direction;
reg buttonPress;
//00 right
//01 left
//10 up
//11 down

localparam right = 'b00,
           left  = 'b01,
           up    = 'b10,
           down  = 'b11;


integer i;

/*always @(posedge i_clk)
begin
    if(i_reset_rdPointer)
        o_valid_cellPos <= 1'b0;
    else
        o_valid_cellPos <= i_rd_pos;
end*/

always @(posedge i_clk)
begin
    // SW reading the snakeBody.
    o_cellPos <= snakeBody[rdPointer];
end

always @(posedge i_clk)
begin
    if(i_reset_rdPointer)
        rdPointer <= 0;
    else
    begin
        if(i_rd_pos)//& (rdPointer != i_snake_size)
            rdPointer <= rdPointer+1;
        //else
        //    rdPointer <= 0;
    end
end


always @(posedge i_clk)
begin
    if(i_reset)
    begin
        direction <= right;
        buttonPress <= 1'b0;
    end
    else
    begin
        buttonPress <= 1'b0;
        if(i_down & direction!=up &  direction!=down)
        begin
            direction <= down;
            buttonPress <= 1'b1;
        end
        else if(i_up & direction!=up & direction!=down)
        begin
            direction <= up;
            buttonPress <= 1'b1;
        end
        else if(i_left & direction!=right & direction!=left)
        begin
            direction <= left;
            buttonPress <= 1'b1;
        end
        else if(i_right & direction!=left & direction!=right)
        begin
            direction <= right;
            buttonPress <= 1'b1;
        end
    end
end

// The shift registers.
always @(posedge i_clk)
begin
    if(i_reset)
    begin
        // Initial position provided by software. Initial position controlled by software
        snakeBody[0] = i_start_pos;
    end
    else if(i_move|buttonPress)
    begin
        case(direction)
            down:begin
                // The new coordinates are calculated
                snakeBody[0][15:8] <= snakeBody[0][15:8]+1;
                snakeBody[0][7:0]  <= snakeBody[0][7:0];
            end
            up:begin
                snakeBody[0][15:8] <= snakeBody[0][15:8]-1;
                snakeBody[0][7:0]  <= snakeBody[0][7:0];
            end
            right:begin
                snakeBody[0][15:8] <= snakeBody[0][15:8];
                snakeBody[0][7:0] <= snakeBody[0][7:0]+1;
            end
            left:begin
                snakeBody[0][15:8] <= snakeBody[0][15:8];
                snakeBody[0][7:0]  <= snakeBody[0][7:0]-1;
            end
        endcase
    end
end


always @(posedge i_clk)
begin
    if(i_reset)
    begin
        // Initialization of snake body.
        snakeBody[1] = i_start_pos-1;
        snakeBody[2] = i_start_pos-2;
        snakeBody[3] = i_start_pos-3;
        snakeBody[4] = i_start_pos-4;
    end
    else if(i_move|buttonPress)
    begin
        for(i=1;i<`MaxSnakeLength-1;i=i+1)
        begin
            snakeBody[i] <= snakeBody[i-1];
        end
    end
end

// Game Over logic
always @(posedge i_clk)
begin
    if(i_reset)
    begin
        o_game_over <= 1'b0;
    end
    // While SW reading , check if the snake bite itself.
    else if((snakeBody[0] == o_cellPos) & (rdPointer!=0) & (rdPointer<i_snake_size) & i_rd_pos)
        o_game_over <= 1'b1;
end

// Checking whether the position of the pray is within the snake.
always @(posedge i_clk)
begin
    if(i_reset)
        o_preyInBody <= 1'b0;
    else if((i_preyPos == o_cellPos)&& (rdPointer!=0) && (rdPointer<i_snake_size) & i_rd_pos)
        // This checks, if the body is generated within the body, regenerate the pray. Since this would be complicated if purely generated in HW.
        // Thus we would need SW to aid us in checking.
        o_preyInBody <= 1'b1;
    else
        o_preyInBody <= 1'b0;
end


endmodule