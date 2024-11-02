module top #(
    parameter WIDTH = 16
)(
    input logic clk,
    input logic en,
    input logic rst,
    input logic [WIDTH-1:0] N,
    output logic [7:0] data_out
);

logic [WIDTH-1:0] count;

clktick myClock (
    .clk (clk),
    .rst (rst),
    .en (en),
    .N (N),
    .tick (count)
);

f1_fsm myF1 (
    .clk (clk),
    .en (count),
    .rst (rst),
    .data_out (data_out)
);

endmodule
