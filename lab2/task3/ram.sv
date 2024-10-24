module ram #(
    parameter ADDRESS_WIDTH = 9,
              DATA_WIDTH = 8
)(
    input logic clk,
    input logic readEn,
    input logic writeEn,
    input logic [ADDRESS_WIDTH-1:0] writeAddr,
    input logic [ADDRESS_WIDTH-1:0] readAddr,
    input logic [DATA_WIDTH-1:0] din,
    output logic [DATA_WIDTH-1:0] dout
);

logic [DATA_WIDTH-1:0] ram_array [2**ADDRESS_WIDTH-1:0];

always_ff @(posedge clk) begin
    if (writeEn == 1'b1)
        ram_array[writeAddr] <= din;
    if (readEn == 1'b1)
        dout <= ram_array[readAddr];
    end

endmodule
