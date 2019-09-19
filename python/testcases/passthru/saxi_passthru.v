module saxi_passthru(ACLK, ARESETn, TREADY_IN, TVALID_IN, TDATA_IN, TREADY_OUT, TVALID_OUT, TDATA_OUT);
    input ACLK;
    input ARESETn;
    // Input AXI stream
    output TREADY_IN;
    input TVALID_IN;
    input [31:0] TDATA_IN;
    // Output AXI stream
    input TREADY_OUT;
    output TVALID_OUT;
    output [31:0] TDATA_OUT;

    reg [3:0] seq_counter;
    reg run_ready;

    // Demo sequencer for stalls
    always @(posedge ACLK)
    begin
        run_ready <= 1'b0;
        seq_counter <= seq_counter + 1;
        if(seq_counter == 5)
        begin
            seq_counter <= 0;
            run_ready <= 1'b1;
        end
    end

    assign TREADY_IN = run_ready;

    always @(posedge ACLK)
    begin
        if (TREADY_OUT)
            TVALID_OUT <= 1'b0;

        if (run_ready && TVALID_IN)
        begin
            TDATA_OUT <= TDATA_IN;
            TVALID_OUT <= 1'b1;
        end
    end

endmodule
