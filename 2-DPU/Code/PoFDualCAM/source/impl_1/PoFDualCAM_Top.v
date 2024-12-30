module PoFDualCAM(
	output oLED1,
	input[1:0] adc_c0,
	input [1:0] adc_c1,
	input [1:0] adc_c8,
	input [1:0] adc_c9,
	
	output clk2ram
);


assign oLED1=1;
assign clk2ram=1;
//on-chip oscillator.
wire clk_hf;
wire clk_lf;
MyOSC osc1(
		.hf_out_en_i(1'b1), 
        .hf_clk_out_o(clk_hf), 
        .lf_clk_out_o(clk_lf)) ;
//on-chip ADC.
reg [3:0] adc0_ch_sel;
reg [3:0] adc1_ch_sel;
wire [11:0] adc0_data;
wire [11:0] adc1_data;

reg adc_convstop;
wire adc_eoc;
wire adc_cog;
MyADC adc_module(
		.adc_en_i(1'b1), //input adc_en_i ; 
        .adc_resetn_i(1'b0), //input adc_resetn_i ; 
        .adc_clk_i(clk_hf), //input adc_clk_i ; 
        .fab_clk_i(clk_hf), //input fab_clk_i ; 
        .adc_cal_i(1'b1), //input adc_cal_i ; 
        .adc_soc_i(1'b0), //input adc_soc_i ;
        .adc_calrdy_o(), //output adc_calrdy_o ; 
        .adc0_ch_sel_i(adc0_ch_sel), //input [3:0] adc0_ch_sel_i ; 
        .adc1_ch_sel_i(adc1_ch_sel),  //input [3:0] adc1_ch_sel_i ; 
        .adc_cog_o(adc_cog), //output adc_cog_o ; 
        .adc_eoc_o(adc_eoc),  //output adc_eoc_o ; 
        .adc_convstop_i(adc_convstop), //input adc_convstop_i ; 
        .adc0_o(adc0_data), //output [11:0] adc0_o ; 
        .adc1_o(adc1_data), //output [11:0] adc1_o ; 
        .ADC_CP0(adc_c0[0]), //input ADC_CP0 ; 
        .ADC_CN0(adc_c0[1]), //input ADC_CN0 ; 
        .ADC_CP1(adc_c1[0]), //input ADC_CP1 ; 
        .ADC_CN1(adc_c1[1]), //input ADC_CN1 ; 
        .ADC_CP8(adc_c8[0]), //input ADC_CP8 ; 
        .ADC_CN8(adc_c8[1]), //input ADC_CN8 ; 
        .ADC_CP9(adc_c9[0]), //input ADC_CP9 ; 
        .ADC_CN9(adc_c9[1])); //input ADC_CN9 ; 

endmodule