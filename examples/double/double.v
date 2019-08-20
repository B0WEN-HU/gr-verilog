module double(
  input         clock,
  input         reset,
  input  [31:0] io_in,
  output [31:0] io_out
);
  // wire [32:0] temp;
  // assign temp = 32'b1 + io_in;
  assign io_out = io_in << 1;//temp[31:0];
endmodule