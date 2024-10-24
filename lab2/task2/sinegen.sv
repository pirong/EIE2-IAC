module sinegen #(
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
)(
  // interface signals
  input  logic             clk,      // clock
  input  logic             rst,      // reset
  input  logic             en,       // enable
  input  logic [ADDRESS_WIDTH-1:0] incr,
  input  logic [ADDRESS_WIDTH-1:0] offset,
  output logic [DATA_WIDTH-1:0] dout1,
  output logic [DATA_WIDTH-1:0] dout2
);

logic [ADDRESS_WIDTH-1:0] address1;    // interconnect wire
logic [ADDRESS_WIDTH-1:0] address2;
assign address2 = address1 + offset;  

counter myCounter (
  .clk (clk),
  .rst (rst),
  .en (en),
  .incr (incr),
  .count (address1)
);

rom sineROM (
  .clk (clk),
  .addr1 (address1),
  .addr2 (address2),
  .dout1 (dout1),
  .dout2 (dout2)
);

endmodule
