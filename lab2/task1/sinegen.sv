module sinegen #(
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             en,       // enable
  input  logic [ADDRESS_WIDTH-1:0] incr,
  output logic [DATA_WIDTH-1:0] dout
);

logic [ADDRESS_WIDTH-1:0] address;    // interconnect wire

counter myCounter (
  .clk (clk),
  .rst (rst),
  .en (en),
  .incr (incr),
  .count (address)
);

rom sineROM (
  .clk (clk),
  .addr (address),
  .dout (dout)
);

endmodule
