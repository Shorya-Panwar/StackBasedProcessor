module processor_tb;
    reg clk, reset;
    output reg signed [31:0] top;

    processor uut(
        .clk(clk),
        .reset(reset),
        .top(top)
    );

    always #5 clk = ~clk;

    initial begin
        $dumpfile("processor_tb.vcd");
        $dumpvars(0, processor_tb);

        clk = 0;
        reset = 1; #10;
        reset = 0;

        #2000;
        $finish;
    end

    // initial begin
    //     $monitor("Time: %0t | Top of Stack %d", $time, top);
    // end

    initial begin
    forever begin
        #10; // Print every 10 time units
        $display("Time=%0t | Top of Stack=%d", $time, top);
    end
end

endmodule