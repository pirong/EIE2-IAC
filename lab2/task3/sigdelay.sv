module sigdelay #(
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             wr,       // write
  input  logic             rd,       // read
  input  logic [ADDRESS_WIDTH-1:0] offset,
  input  logic [DATA_WIDTH-1:0] mic_signal,
  output logic [DATA_WIDTH-1:0] delayed_signal
);

logic [ADDRESS_WIDTH-1:0] address1;    // interconnect wire
logic [ADDRESS_WIDTH-1:0] address2;
assign address2 = address1 + offset;  

counter myCounter (
  .clk (clk),
  .rst (rst),
  .count (address1)
);

ram sineRAM (
  .clk (clk),
  .readEn (rd),
  .writeEn (wr),
  .writeAddr (address2),
  .readAddr (address1),
  .din (mic_signal),
  .dout (delayed_signal)
);

endmodule
