module double_axi(
  input         ACLK,
  input         ARESETn,
  output        TREADY_IN,
  input         TVALID_IN,
  input  [31:0] TDATA_IN,
  input         TREADY_OUT,
  output        TVALID_OUT,
  output [31:0] TDATA_OUT
);

double DUT(
  .clock(ACLK),
  .reset(ARESETn),
  .io_in(TDATA_IN),
  .io_out(TDATA_OUT)
);

assign TREADY_IN  = 1'b1;
assign TVALID_OUT = 1'b1;

endmodule