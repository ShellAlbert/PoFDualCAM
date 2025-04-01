module PSU_TMR_FM_TOP(

    //Dynamic Power Management Auxiliary. 
    output reg oIAM_ALIVE,

    //Debug LED*3.
	output reg oLED1,
    output reg oLED2,
    output reg oLED3
)/* synthesis RGB_TO_GPIO = "oLED1, oLED2, oLED3" */;

//HSOSC
//High-frequency oscillator.
//Generates 48-MHz nominal clock, +/- 10 percent, with user-programmable divider. 
//Can drive global clock network or fabric routing.
//Input Ports
//CLKHFPU :Power up the oscillator. After power up, output will be stable after 100 �s. Active high.
//CLKHFEN :Enable the clock output. Enable should be low for the 100-�s power-up period. Active high.
//Output Ports
//CLKHF :Oscillator output
//Parameters
//CLKHF_DIV
//Clock divider selection:
//0'b00 = 48 MHz
//0'b01 = 24 MHz
//0'b10 = 12 MHz
//0'b11 = 6 MHz
wire clk_48MHz;
//By default, the outputs are routed to global clock network. 
//To route to local fabric, see the examples in the Appendix: Design Entry section.
HSOSC #(.CLKHF_DIV("0b00")) //48 MHz
my_HSOSC(
    .CLKHFPU(1'b1), 
    .CLKHFEN(1'b1), 
    .CLKHF(clk_48MHz)
)/* synthesis ROUTE_THROUGH_FABRIC= 0 */; //the value can be either 0 or 1

///////////////////////////////////////////////////////////////////
//We are not allowed to use PLL output, because it's exclusive with Pin-35. ADQ[7].
//if PLL use internal clock source, Pin-35 can be used as output.
//PLL: 48MHz->66MHz.
//WARNING!!!!!
//If I configured PLL outputs 70MHz, it doesn't work correctly.
//Then I slowed down to 66MHz, it starts to work.
//66MHz is not reliable, down to 48MHz.

//ERROR <67201318> - 
//When PLL.OUTCORE or PLL.OUTGLOBAL is used, the input IO at site 'PR13B' can only drive PLL.REFERENCECLK due to architecture constraint. 
//When PLL is utilized in the design, the I/O site 'PR13B' can only be used exclusively as a PLL clock input. 
//If PLL uses an internal clock, the I/O site 'PR13B' can be used as an output.
wire rst_n;
wire clk_48MHz_Global;
wire clk_48MHz_Fabric;
ZPLL ic_pll(
	.ref_clk_i(clk_48MHz), 
	.rst_n_i(1'b1), 
	.lock_o(rst_n), 
	.outcore_o(clk_48MHz_Fabric), 
	.outglobal_o(clk_48MHz_Global)
);

reg [31:0] CNT1;
always @(posedge clk_48MHz_Global or negedge rst_n)
if(!rst_n) begin
    oLED1<=0;
    oLED2<=0;
    oLED3<=0;
    oIAM_ALIVE<=1;
    CNT1<=0;
end
else begin
    if(CNT1==32'd12000000) begin
        CNT1<=0;
        oLED1<=~oLED1;
        oLED2<=~oLED2;
        oLED3<=~oLED3;
    end
    else begin
        CNT1<=CNT1+1;
    end
end
endmodule