`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 02/08/2020 07:55:40 PM
// Design Name:
// Module Name: oledControl
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


module oledControl(
    input  clock, //100MHz onboard clock
    input  reset,
    //oled interface, these signals can be found in OLED data sheet
    output wire oled_spi_clk,
    output wire oled_spi_data,
    output reg oled_vdd,  //Voltage high
    output reg oled_vbat,
    output reg oled_reset_n, // Active low reset
    output reg oled_dc_n,

    // Inputs from other modules
    input [6:0] sendData,      // This tells which ASCII char I want to display
    input       sendDataValid, // Sends only when valid.
    output reg  sendDone       // Handshaking, telling the master you are done.
  );


  reg [4:0] state;
  reg [4:0] nextState;
  reg startDelay;
  reg [7:0] spiData;
  reg spiLoadData;
  wire delayDone;
  wire spiDone;
  reg [1:0] currPage;
  wire [63:0] charBitMap;
  reg [7:0] columnAddr;
  reg [3:0] byteCounter;

  // Since building an OLED need a sequence of operations, we have to build a state machine for it!
  localparam  IDLE  = 'd0,
              DELAY = 'd1,
              INIT  = 'd2,
              RESET = 'd3,
              CHRG_PUMP = 'd4,
              CHRG_PUMP1 = 'd5,
              WAIT_SPI = 'd6,
              PRE_CHRG = 'd7,
              PRE_CHRG1 = 'd8,
              VBAT_ON = 'd9,
              CONTRAST = 'd10,
              CONTRAST1 = 'd11,
              SEG_REMAP = 'd12,
              SCAN_DIR = 'd13,
              COM_PIN = 'd14,
              COM_PIN1 = 'd15,
              DISPLAY_ON = 'd16,
              FULL_DISPLAY = 'd17,
              DONE = 'd18,
              PAGE_ADDR = 'd19,
              PAGE_ADDR1 = 'd20,
              PAGE_ADDR2 = 'd21,
              COLUMN_ADDR = 'd22,
              SEND_DATA = 'd23;


  always @(posedge clock)
  begin
    if(reset)
    begin
      state <= IDLE;
      nextState <= IDLE;
      // These initial values are from the data Sheets
      oled_vdd <= 1'b1;
      oled_vbat <= 1'b1;
      oled_reset_n <= 1'b1;
      oled_dc_n <= 1'b1;
      //
      startDelay <= 1'b0;
      spiData <= 8'd0;
      spiLoadData <= 1'b0;
      currPage <= 0;
      sendDone <= 0;
      columnAddr <= 0;
    end
    else
    begin
      case(state)
        IDLE:
        begin
          oled_vbat <= 1'b1;
          oled_reset_n <= 1'b1;
          oled_dc_n <= 1'b0;
          oled_vdd <= 1'b0;
          state <= DELAY;
          nextState <= INIT;
        end
        DELAY:
        begin
          // Delay states, to helps delay for 200,000 cycles. 2ms elapsed as specified in the dataSheet.
          startDelay <= 1'b1;
          if(delayDone)
          begin
            state <= nextState;
            startDelay <= 1'b0;
          end
        end
        INIT:
        begin
          // To initiliaze, sends in the 0xAE to tell OLED to initilize! First send it to SPI controller.
          spiData <= 'hAE;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            // Once done sending, waiting for SPI to be complete, pull the load date down.
            spiLoadData <= 1'b0;
            oled_reset_n <= 1'b0;
            // Next state we delay for another 2ms,
            state <= DELAY;
            nextState <= RESET;
          end
        end
        RESET:
        begin
          oled_reset_n <= 1'b1;
          state <= DELAY;
          nextState <= CHRG_PUMP;
        end
        CHRG_PUMP:
        begin
          spiData <= 'h8D;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= CHRG_PUMP1;
          end
        end
        WAIT_SPI:
        begin
          if(!spiDone)
          begin
            state <= nextState;
          end
        end
        CHRG_PUMP1:
        begin
          spiData <= 'h14;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= PRE_CHRG;
          end
        end
        PRE_CHRG:
        begin
          spiData <= 'hD9;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= PRE_CHRG1;
          end
        end
        PRE_CHRG1:
        begin
          spiData <= 'hF1;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= VBAT_ON;
          end
        end
        VBAT_ON:
        begin
          oled_vbat <= 1'b0;
          state <= DELAY;
          nextState <= CONTRAST;
        end
        CONTRAST:
        begin
          spiData <= 'h81;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= CONTRAST1;
          end
        end
        CONTRAST1:
        begin
          spiData <= 'hFF;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= SEG_REMAP;
          end
        end
        SEG_REMAP:
        begin
          spiData <= 'hA0;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= SCAN_DIR;
          end
        end
        SCAN_DIR:
        begin
          spiData <= 'hC0;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= COM_PIN;
          end
        end
        COM_PIN:
        begin
          spiData <= 'hDA;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= COM_PIN1;
          end
        end
        COM_PIN1:
        begin
          spiData <= 'h00;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= DISPLAY_ON;
          end
        end
        DISPLAY_ON:
        begin
          spiData <= 'hAF;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= PAGE_ADDR;//FULL_DISPLAY;
          end
        end
        PAGE_ADDR:
        begin
          spiData <= 'h22;
          spiLoadData <= 1'b1;
          oled_dc_n <= 1'b0;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= PAGE_ADDR1;
          end
        end
        PAGE_ADDR1:
        begin
          spiData <= currPage;//start page address
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            currPage <= currPage+1;
            nextState <= PAGE_ADDR2;
          end
        end
        PAGE_ADDR2:
        begin
          spiData <= currPage;//start page address
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= COLUMN_ADDR;
          end
        end
        COLUMN_ADDR:
        begin
          spiData <= 'h10;
          spiLoadData <= 1'b1;
          if(spiDone)
          begin
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            nextState <= DONE;
          end
        end
        /*FULL_DISPLAY:begin
           spiData <= 'hA5;
           spiLoadData <= 1'b1;
           if(spiDone)
           begin
               spiLoadData <= 1'b0;
               state <= WAIT_SPI;
               nextState <= DONE;
           end
        end*/
        DONE:
        begin
          sendDone <= 1'b0;
          if(sendDataValid & columnAddr != 128 & !sendDone)
          begin
            state <= SEND_DATA;
            byteCounter <= 8;
          end
          else if(sendDataValid & columnAddr == 128 & !sendDone)
          begin
            state <= PAGE_ADDR;
            columnAddr <= 0;
            byteCounter <= 8;
          end
        end
        SEND_DATA:
        begin
          //Here we make a small memory inside fpga, later send these data
          //We can only send 8 bits per spi-transfer.
          // The lower index is of 8 width lower.
          spiData <= charBitMap[(byteCounter*8-1)-:8]; // Lower index is smaller than the upper index by 8

          spiLoadData <= 1'b1;
          oled_dc_n <= 1'b1;    // Now when sending to the GDRAM, this should be 1.
          if(spiDone)
          begin
            // We have to do 8 full spi-transfer to send a character
            columnAddr <= columnAddr + 1;
            spiLoadData <= 1'b0;
            state <= WAIT_SPI;
            if(byteCounter != 1)
            begin
              byteCounter <= byteCounter - 1;
              nextState <= SEND_DATA;
            end
            else
            begin
              nextState <= DONE;
              sendDone <= 1'b1;
            end
          end
        end
      endcase
    end
  end


  delayGen DG(
             .clock(clock),
             .delayEn(startDelay),
             .delayDone(delayDone)
           );


  //The spi contoller you just built before.
  spiControl SC(
               .clock(clock), //On-board Zynq clock (100 MHz)
               .reset(reset),
               .data_in(spiData),
               .load_data(spiLoadData), //Signal indicates new data for transmission
               .done_send(spiDone),//Signal indicates data has been sent over spi interface
               .spi_clock(oled_spi_clk),//10MHz max
               .spi_data(oled_spi_data)
             );


  // The ROM which stores the bit pattern for 1 character.
  charROM CR(
            .addr(sendData),
            .data(charBitMap)
          );

endmodule
