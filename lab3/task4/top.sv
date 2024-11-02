module top #(
    parameter WIDTH = 5
)(
    input logic clk,
    input logic rst,
    input logic cmd_seq,
    input logic cmd_delay,
    input logic trigger,
    input logic [WIDTH-1:0] N,
    output logic [7:0] data_out,
    output logic cmd_seq_output,
    output logic cmd_delay_output
);

logic [7:1] lfsrToDelay;

logic tick;
logic timeout;
logic fsmEnable;
assign fsmEnable = cmd_seq ? tick : timeout;

lfsr_7 myLFSR (
    .clk (clk),
    .rst (rst),
    .data_out (lfsrToDelay)
);

delay myDelay (
    .clk (clk),
    .rst (rst),
    .trigger (cmd_delay),
    .n (lfsrToDelay),
    .time_out (timeout)
);

clktick myClock (
    .clk (clk),
    .rst (rst),
    .en (cmd_seq),
    .N (N),
    .tick (tick)
);

f1_fsm myF1 (
    .clk (clk),
    .en (fsmEnable),
    .rst (rst),
    .trigger (trigger),
    .data_out (data_out),
    .cmd_seq (cmd_seq_output),
    .cmd_delay (cmd_delay_output)
);

endmodule
