module spiControl(
    input  clock, //On-board Zynq clock (100 MHz)
    input  reset,
    input [7:0] data_in, // Inserting 1 byte of data, reading for the processor.
    input  load_data, //Signal indicates new data for transmission

    output reg done_send,//Signal indicates data has been sent over spi interface, handshaking tell the processor the data has been sent
    output     spi_clock,//10MHz max
    output reg spi_data // 1 bit of data sent
  );

  // Initilizae the counter
  reg [2:0] counter=0;
  reg [2:0] dataCount;
  reg [7:0] shiftReg;
  // This line is used specifically in vivado
  (*KEEP = "true"*)
  reg [1:0] state;
  reg clock_10;
  reg CE;

  // Enable the spi_clock only when the communication happens.
  // This is the gated clock, s.t. the clk is enable only when you are sending the data.
  assign spi_clock = (CE == 1) ? clock_10 : 1'b1;

  // Since we want to have 10Mhz, however processor clock is of 100
  // Use counter for clock division.
  always @(posedge clock)
  begin
    if(counter != 4)
      counter <= counter + 1;
    else
      counter <= 0;
  end

  // This is used for simultation, if you dont add this, you must add a reset to your signal.
  // initial
  //   clock_10 <= 0;

  //This generated clock is of 10 Mhz
  always @(posedge clock)
  begin //
    if(reset)
    begin
      clock_10 <= 0;
    end
    else if(counter == 4)
    begin
      clock_10 <= ~clock_10;
    end
  end

  // In order to now when to send data, when not, we need a small state machine.
  localparam IDLE = 'd0,
             SEND = 'd1,
             DONE = 'd2;

  always @(negedge clock_10)
  begin
    if(reset)
    begin
      state <= IDLE;
      dataCount <= 0;
      done_send <= 1'b0;
      CE <= 0;
      spi_data <= 1'b1;
    end
    else
    begin
      case(state)
        IDLE:
        begin
          // If the master wants to send data, load_data -> 1
          if(load_data)
          begin
            // If load data, we have a valid data in data_in, so we send it into the shift register.
            shiftReg <= data_in;
            // Start sending the data, so we have to change the state
            state <= SEND;
            dataCount <= 0;
          end
        end
        SEND:
        begin
          // Send out the most significant bit, specified in datasheet
          spi_data <= shiftReg[7];
          // The shift register shifting, try to shift out the data in a serial manner
          shiftReg <= {shiftReg[6:0],1'b0};

          CE <= 1;
          if(dataCount != 7)
          begin
            // Since I have 8 bit of data to serialize, the counter is of 0~7
            dataCount <= dataCount + 1;
          end
          else
          begin
            state <= DONE;
          end
        end
        DONE:
        begin
          CE <= 0;
          // If finish sending, slave should tell the master, I have done sending.
          done_send <= 1'b1;
          //The master receive the done_send, it pulls the load_data signal->0
          if(!load_data)
          begin
            // I also has to change the done_send signal down.
            done_send <= 1'b0;
            // Returning state to IDLE state
            state <= IDLE;
          end
        end
      endcase
    end
  end



endmodule
