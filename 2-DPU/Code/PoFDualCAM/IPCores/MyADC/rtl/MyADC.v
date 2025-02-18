
/*******************************************************************************
    Verilog netlist generated by IPGEN Lattice Radiant Software (64-bit)
    2023.1.1.200.1
    Soft IP Version: 1.3.1
    2024 11 11 10:36:22
*******************************************************************************/
/*******************************************************************************
    Wrapper Module generated per user settings.
*******************************************************************************/
module MyADC (adc_en_i, 
        adc_resetn_i, 
        adc_clk_i, 
        fab_clk_i, 
        adc_cal_i, 
        adc_soc_i, 
        adc_calrdy_o, 
        adc0_ch_sel_i, 
        adc1_ch_sel_i, 
        adc_cog_o, 
        adc_eoc_o, 
        adc_convstop_i, 
        adc0_o, 
        adc1_o, 
        ADC_CP0, 
        ADC_CN0, 
        ADC_CP1, 
        ADC_CN1, 
        ADC_CP8, 
        ADC_CN8, 
        ADC_CP9, 
        ADC_CN9) ;
    input adc_en_i ; 
    input adc_resetn_i ; 
    input adc_clk_i ; 
    input fab_clk_i ; 
    input adc_cal_i ; 
    input adc_soc_i ; 
    output adc_calrdy_o ; 
    input [3:0] adc0_ch_sel_i ; 
    input [3:0] adc1_ch_sel_i ; 
    output adc_cog_o ; 
    output adc_eoc_o ; 
    input adc_convstop_i ; 
    output [11:0] adc0_o ; 
    output [11:0] adc1_o ; 
    input ADC_CP0 ; 
    input ADC_CN0 ; 
    input ADC_CP1 ; 
    input ADC_CN1 ; 
    input ADC_CP8 ; 
    input ADC_CN8 ; 
    input ADC_CP9 ; 
    input ADC_CN9 ; 
    MyADC_ipgen_lscc_adc_core #(.FAMILY("LIFCL"),
            .DEVICE_TYPE("LIFCL-40"),
            .PACKAGE_TYPE("CSFBGA121"),
            .SETUP_ADC_EN("ENABLED"),
            .SETUP_CLK_DIV("2"),
            .SETUP_ADC_DATA_FORMAT_SEL("STRAIGHT_BINARY"),
            .SETUP_ADC0_VREF_SEL("INTERNAL"),
            .SETUP_ADC0_VREF_VOLTAGE("1.2"),
            .SETUP_ADC0_VREF_CFG("1P2_TO_1P4"),
            .SETUP_ADC0_CONV_MODE("UNIPOLAR"),
            .SETUP_ADC1_VREF_SEL("INTERNAL"),
            .SETUP_ADC1_VREF_VOLTAGE("1.2"),
            .SETUP_ADC1_VREF_CFG("1P2_TO_1P4"),
            .SETUP_ADC1_CONV_MODE("UNIPOLAR"),
            .SETUP_COMP1_EN("DISABLED"),
            .SETUP_COMP1_SW_CTRL("ENABLED"),
            .SETUP_COMP2_EN("DISABLED"),
            .SETUP_COMP2_SW_CTRL("ENABLED"),
            .SETUP_COMP3_EN("DISABLED"),
            .SETUP_COMP3_SW_CTRL("ENABLED"),
            .ADC0_CHANNEL_1_EN("ENABLED"),
            .ADC0_CHANNEL_2_EN("ENABLED"),
            .ADC0_CHANNEL_3_EN("DISABLED"),
            .ADC0_CHANNEL_4_EN("DISABLED"),
            .ADC0_CHANNEL_5_EN("DISABLED"),
            .ADC0_CHANNEL_6_EN("DISABLED"),
            .ADC0_CHANNEL_7_EN("DISABLED"),
            .ADC0_CHANNEL_8_EN("DISABLED"),
            .ADC1_CHANNEL_9_EN("ENABLED"),
            .ADC1_CHANNEL_10_EN("ENABLED"),
            .ADC1_CHANNEL_11_EN("DISABLED"),
            .ADC1_CHANNEL_12_EN("DISABLED"),
            .ADC1_CHANNEL_13_EN("DISABLED"),
            .ADC1_CHANNEL_14_EN("DISABLED"),
            .ADC1_CHANNEL_15_EN("DISABLED"),
            .ADC1_CHANNEL_16_EN("DISABLED")) lscc_adc_core_inst (.adc_en_i(adc_en_i), 
                .resetn_i(adc_resetn_i), 
                .clk_dclk_i(adc_clk_i), 
                .clk_fabric_i(fab_clk_i), 
                .cal_i(adc_cal_i), 
                .soc_i(adc_soc_i), 
                .calrdy_o(adc_calrdy_o), 
                .chasel_i(adc0_ch_sel_i[3:0]), 
                .chbsel_i(adc1_ch_sel_i[3:0]), 
                .cog_o(adc_cog_o), 
                .eoc_o(adc_eoc_o), 
                .convstop_i(adc_convstop_i), 
                .da_o(adc0_o[11:0]), 
                .db_o(adc1_o[11:0]), 
                .dn0_i(), 
                .dp0_i(), 
                .dn1_i(), 
                .dp1_i(), 
                .chan1p_i(ADC_CP0), 
                .chan1n_i(ADC_CN0), 
                .chan2p_i(ADC_CP1), 
                .chan2n_i(ADC_CN1), 
                .chan3p_i(), 
                .chan3n_i(), 
                .chan4p_i(), 
                .chan4n_i(), 
                .chan5p_i(), 
                .chan5n_i(), 
                .chan6p_i(), 
                .chan6n_i(), 
                .chan7p_i(), 
                .chan7n_i(), 
                .chan8p_i(), 
                .chan8n_i(), 
                .chan9p_i(ADC_CP8), 
                .chan9n_i(ADC_CN8), 
                .chan10p_i(ADC_CP9), 
                .chan10n_i(ADC_CN9), 
                .chan11p_i(), 
                .chan11n_i(), 
                .chan12p_i(), 
                .chan12n_i(), 
                .chan13p_i(), 
                .chan13n_i(), 
                .chan14p_i(), 
                .chan14n_i(), 
                .chan15p_i(), 
                .chan15n_i(), 
                .chan16p_i(), 
                .chan16n_i(), 
                .comp1in_io(), 
                .comp1ip_io(), 
                .comp1ol_o(), 
                .comp1o_o(), 
                .comp2in_io(), 
                .comp2ip_io(), 
                .comp2ol_o(), 
                .comp2o_o(), 
                .comp3in_io(), 
                .comp3ip_io(), 
                .comp3ol_o(), 
                .comp3o_o()) ; 
endmodule



// =============================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------
// Copyright (c) 2021 by Lattice Semiconductor Corporation
// ALL RIGHTS RESERVED
// --------------------------------------------------------------------
//
// Permission:
//
// Lattice SG Pte. Ltd. grants permission to use this code
// pursuant to the terms of the Lattice Reference Design License Agreement.
//
//
// Disclaimer:
//
// This VHDL or Verilog source code is intended as a design reference
// which illustrates how these types of functions can be implemented.
// It is the user's responsibility to verify their design for
// consistency and functionality through the use of formal
// verification methods. Lattice provides no warranty
// regarding the use or functionality of this code.
//
// -----------------------------------------------------------------------------
//
//                     Lattice SG Pte. Ltd.
//                     101 Thomson Road, United Square #07-02
//                     Singapore 307591
//
//
//                     TEL: 1-800-Lattice (USA and Canada)
//                     +65-6631-2000 (Singapore)
//                     +1-503-268-8001 (other locations)
//
//                     web: http://www.latticesemi.com/
//                     email: techsupport@latticesemi.com
//
// -----------------------------------------------------------------------------
//
// =============================================================================
// FILE DETAILS
// Project : <>
// File : lscc_adc_core.v
// Title :
// Dependencies : 1.
//              : 2.
// Description :
// =============================================================================
// REVISION HISTORY
// Version : 1.0
// Author(s) : Lattice Semiconductor
// Mod. Date : 10-AUG-2021
// Changes Made : Initial version of RTL
// -----------------------------------------------------------------------------
// Version      : 1.1
// Author(s)    : Lattice Semiconductor
// Mod. Date    : 2-Dec-2021
// Changes Made : Updated SETUP_CLK_DIV decoding tree
// =============================================================================
module MyADC_ipgen_lscc_adc_core #(parameter FAMILY = "common", 
        parameter DEVICE_TYPE = "common", 
        parameter PACKAGE_TYPE = "CABGA400", 
        parameter SETUP_ADC_EN = "DISABLED", 
        parameter SETUP_ADC_DATA_FORMAT_SEL = "STRAIGHT_BINARY", 
        parameter SETUP_ADC0_VREF_SEL = "INTERNAL", 
        parameter SETUP_ADC0_VREF_CFG = "1P6_TO_1P8", 
        parameter SETUP_ADC0_VREF_VOLTAGE = "1.8", 
        parameter SETUP_ADC0_CONV_MODE = "BIPOLAR", 
        parameter SETUP_ADC1_VREF_SEL = "INTERNAL", 
        parameter SETUP_ADC1_VREF_CFG = "1P6_TO_1P8", 
        parameter SETUP_ADC1_VREF_VOLTAGE = "1.8", 
        parameter SETUP_ADC1_CONV_MODE = "BIPOLAR", 
        parameter SETUP_COMP1_EN = "DISABLED", 
        parameter SETUP_COMP2_EN = "DISABLED", 
        parameter SETUP_COMP3_EN = "DISABLED", 
        parameter SETUP_COMP1_SW_CTRL = "DISABLED", 
        parameter SETUP_COMP2_SW_CTRL = "DISABLED", 
        parameter SETUP_COMP3_SW_CTRL = "DISABLED", 
        parameter SETUP_CLK_DIV = "2", 
        parameter ADC0_CHANNEL_1_EN = "DISABLED", 
        parameter ADC0_CHANNEL_2_EN = "DISABLED", 
        parameter ADC0_CHANNEL_3_EN = "DISABLED", 
        parameter ADC0_CHANNEL_4_EN = "DISABLED", 
        parameter ADC0_CHANNEL_5_EN = "DISABLED", 
        parameter ADC0_CHANNEL_6_EN = "DISABLED", 
        parameter ADC0_CHANNEL_7_EN = "DISABLED", 
        parameter ADC0_CHANNEL_8_EN = "DISABLED", 
        parameter ADC1_CHANNEL_9_EN = "DISABLED", 
        parameter ADC1_CHANNEL_10_EN = "DISABLED", 
        parameter ADC1_CHANNEL_11_EN = "DISABLED", 
        parameter ADC1_CHANNEL_12_EN = "DISABLED", 
        parameter ADC1_CHANNEL_13_EN = "DISABLED", 
        parameter ADC1_CHANNEL_14_EN = "DISABLED", 
        parameter ADC1_CHANNEL_15_EN = "DISABLED", 
        parameter ADC1_CHANNEL_16_EN = "DISABLED") (
    // -----------------------------------------------------------------------------
    // Module Parameters
    // -----------------------------------------------------------------------------
    //Setup Tab
    // parameters
    // -----------------------------------------------------------------------------
    // Input/Output Ports
    // -----------------------------------------------------------------------------
    //Native interface
    input adc_en_i, 
    input cal_i, 
    input soc_i, 
    output calrdy_o, 
    input [3:0] chasel_i, 
    input [3:0] chbsel_i, 
    input clk_dclk_i, 
    input clk_fabric_i, 
    input resetn_i, 
    output cog_o, 
    inout comp1in_io, 
    inout comp1ip_io, 
    output comp1ol_o, 
    output comp1o_o, 
    inout comp2in_io, 
    inout comp2ip_io, 
    output comp2ol_o, 
    output comp2o_o, 
    inout comp3in_io, 
    inout comp3ip_io, 
    output comp3ol_o, 
    output comp3o_o, 
    input convstop_i, 
    output [11:0] da_o, 
    output [11:0] db_o, 
    input dn0_i, 
    input dp0_i, 
    input dn1_i, 
    input dp1_i, 
    output eoc_o, 
    input chan1p_i, 
    input chan1n_i, 
    input chan2p_i, 
    input chan2n_i, 
    input chan3p_i, 
    input chan3n_i, 
    input chan4p_i, 
    input chan4n_i, 
    input chan5p_i, 
    input chan5n_i, 
    input chan6p_i, 
    input chan6n_i, 
    input chan7p_i, 
    input chan7n_i, 
    input chan8p_i, 
    input chan8n_i, 
    input chan9p_i, 
    input chan9n_i, 
    input chan10p_i, 
    input chan10n_i, 
    input chan11p_i, 
    input chan11n_i, 
    input chan12p_i, 
    input chan12n_i, 
    input chan13p_i, 
    input chan13n_i, 
    input chan14p_i, 
    input chan14n_i, 
    input chan15p_i, 
    input chan15n_i, 
    input chan16p_i, 
    input chan16n_i) ;
    localparam REFBUFAEN = ((SETUP_ADC0_VREF_SEL == "INTERNAL") ? "ENABLED" : "DISABLED") ; 
    localparam REFBUFBEN = ((SETUP_ADC1_VREF_SEL == "INTERNAL") ? "ENABLED" : "DISABLED") ; 
    localparam CLK_DIV = ((SETUP_CLK_DIV == "2") ? 2 : ((SETUP_CLK_DIV == "4") ? 4 : ((SETUP_CLK_DIV == "6") ? 6 : ((SETUP_CLK_DIV == "8") ? 8 : ((SETUP_CLK_DIV == "10") ? 10 : ((SETUP_CLK_DIV == "12") ? 12 : ((SETUP_CLK_DIV == "14") ? 14 : ((SETUP_CLK_DIV == "16") ? 16 : ((SETUP_CLK_DIV == "18") ? 18 : ((SETUP_CLK_DIV == "20") ? 20 : ((SETUP_CLK_DIV == "22") ? 22 : ((SETUP_CLK_DIV == "24") ? 24 : ((SETUP_CLK_DIV == "26") ? 26 : ((SETUP_CLK_DIV == "28") ? 28 : ((SETUP_CLK_DIV == "30") ? 30 : ((SETUP_CLK_DIV == "32") ? 32 : ((SETUP_CLK_DIV == "34") ? 34 : ((SETUP_CLK_DIV == "36") ? 36 : ((SETUP_CLK_DIV == "38") ? 38 : ((SETUP_CLK_DIV == "40") ? 40 : 2)))))))))))))))))))) ; // default is 2
    //Comparator wires
    wire comp1in_w ; 
    wire comp1ip_w ; 
    wire comp2in_w ; 
    wire comp2ip_w ; 
    wire comp3in_w ; 
    wire comp3ip_w ; 
    // ADC MUX wires
    wire [15:0] adcn_w ; 
    wire [15:0] adcp_w ; 
    generate
        if (((ADC0_CHANNEL_1_EN == "ENABLED") || ((SETUP_COMP1_EN == "ENABLED") && (SETUP_COMP1_SW_CTRL == "DISABLED")))) 
            begin : genblk_adc_ch1_enabled
                (* syn_black_box=1, black_box_pad_pin="chan1n_i,adcn_w[0]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan1n_i), 
                            .INADC(adcn_w[0])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan1p_i,adcp_w[0]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan1p_i), 
                            .INADC(adcp_w[0])) ; 
            end
        if (((ADC0_CHANNEL_2_EN == "ENABLED") || ((SETUP_COMP2_EN == "ENABLED") && (SETUP_COMP2_SW_CTRL == "DISABLED")))) 
            begin : genblk_adc_ch2_enabled
                (* syn_black_box=1, black_box_pad_pin="chan2n_i,adcn_w[1]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan2n_i), 
                            .INADC(adcn_w[1])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan2p_i,adcp_w[1]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan2p_i), 
                            .INADC(adcp_w[1])) ; 
            end
        if (((ADC0_CHANNEL_3_EN == "ENABLED") || ((SETUP_COMP3_EN == "ENABLED") && (SETUP_COMP3_SW_CTRL == "DISABLED")))) 
            begin : genblk_adc_ch3_enabled
                (* syn_black_box=1, black_box_pad_pin="chan3n_i,adcn_w[2]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan3n_i), 
                            .INADC(adcn_w[2])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan3p_i,adcp_w[2]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan3p_i), 
                            .INADC(adcp_w[2])) ; 
            end
        if ((ADC0_CHANNEL_4_EN == "ENABLED")) 
            begin : genblk_adc_ch4_enabled
                (* syn_black_box=1, black_box_pad_pin="chan4n_i,adcn_w[3]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan4n_i), 
                            .INADC(adcn_w[3])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan4p_i,adcp_w[3]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan4p_i), 
                            .INADC(adcp_w[3])) ; 
            end
        if ((ADC0_CHANNEL_5_EN == "ENABLED")) 
            begin : genblk_adc_ch5_enabled
                (* syn_black_box=1, black_box_pad_pin="chan5n_i,adcn_w[4]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan5n_i), 
                            .INADC(adcn_w[4])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan5p_i,adcp_w[4]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan5p_i), 
                            .INADC(adcp_w[4])) ; 
            end
        if ((ADC0_CHANNEL_6_EN == "ENABLED")) 
            begin : genblk_adc_ch6_enabled
                (* syn_black_box=1, black_box_pad_pin="chan6n_i,adcn_w[5]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan6n_i), 
                            .INADC(adcn_w[5])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan6p_i,adcp_w[5]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan6p_i), 
                            .INADC(adcp_w[5])) ; 
            end
        if ((ADC0_CHANNEL_7_EN == "ENABLED")) 
            begin : genblk_adc_ch7_enabled
                (* syn_black_box=1, black_box_pad_pin="chan7n_i,adcn_w[6]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan7n_i), 
                            .INADC(adcn_w[6])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan7p_i,adcp_w[6]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan7p_i), 
                            .INADC(adcp_w[6])) ; 
            end
        if ((ADC0_CHANNEL_8_EN == "ENABLED")) 
            begin : genblk_adc_ch8_enabled
                (* syn_black_box=1, black_box_pad_pin="chan8n_i,adcn_w[7]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan8n_i), 
                            .INADC(adcn_w[7])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan8p_i,adcp_w[7]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan8p_i), 
                            .INADC(adcp_w[7])) ; 
            end
        if ((ADC1_CHANNEL_9_EN == "ENABLED")) 
            begin : genblk_adc_ch9_enabled
                (* syn_black_box=1, black_box_pad_pin="chan9n_i,adcn_w[8]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan9n_i), 
                            .INADC(adcn_w[8])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan9p_i,adcp_w[8]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan9p_i), 
                            .INADC(adcp_w[8])) ; 
            end
        if ((ADC1_CHANNEL_10_EN == "ENABLED")) 
            begin : genblk_adc_ch10_enabled
                (* syn_black_box=1, black_box_pad_pin="chan10n_i,adcn_w[9]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan10n_i), 
                            .INADC(adcn_w[9])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan10p_i,adcp_w[9]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan10p_i), 
                            .INADC(adcp_w[9])) ; 
            end
        if ((ADC1_CHANNEL_11_EN == "ENABLED")) 
            begin : genblk_adc_ch11_enabled
                (* syn_black_box=1, black_box_pad_pin="chan11n_i,adcn_w[10]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan11n_i), 
                            .INADC(adcn_w[10])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan11p_i,adcp_w[10]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan11p_i), 
                            .INADC(adcp_w[10])) ; 
            end
        if ((ADC1_CHANNEL_12_EN == "ENABLED")) 
            begin : genblk_adc_ch12_enabled
                (* syn_black_box=1, black_box_pad_pin="chan12n_i,adcn_w[11]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan12n_i), 
                            .INADC(adcn_w[11])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan12p_i,adcp_w[11]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan12p_i), 
                            .INADC(adcp_w[11])) ; 
            end
        if ((ADC1_CHANNEL_13_EN == "ENABLED")) 
            begin : genblk_adc_ch13_enabled
                (* syn_black_box=1, black_box_pad_pin="chan13n_i,adcn_w[12]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan13n_i), 
                            .INADC(adcn_w[12])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan13p_i,adcp_w[12]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan13p_i), 
                            .INADC(adcp_w[12])) ; 
            end
        if ((ADC1_CHANNEL_14_EN == "ENABLED")) 
            begin : genblk_adc_ch14_enabled
                (* syn_black_box=1, black_box_pad_pin="chan14n_i,adcn_w[13]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan14n_i), 
                            .INADC(adcn_w[13])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan14p_i,adcp_w[13]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan14p_i), 
                            .INADC(adcp_w[13])) ; 
            end
        if ((ADC1_CHANNEL_15_EN == "ENABLED")) 
            begin : genblk_adc_ch15_enabled
                (* syn_black_box=1, black_box_pad_pin="chan15n_i,adcn_w[14]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan15n_i), 
                            .INADC(adcn_w[14])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan15p_i,adcp_w[14]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan15p_i), 
                            .INADC(adcp_w[14])) ; 
            end
        if ((ADC1_CHANNEL_16_EN == "ENABLED")) 
            begin : genblk_adc_ch16_enabled
                (* syn_black_box=1, black_box_pad_pin="chan16n_i,adcn_w[15]" *) BB_ADC BB_ADC_GPION (.IOPAD(chan16n_i), 
                            .INADC(adcn_w[15])) ; 
                (* syn_black_box=1, black_box_pad_pin="chan16p_i,adcp_w[15]" *) BB_ADC BB_ADC_GPIOP (.IOPAD(chan16p_i), 
                            .INADC(adcp_w[15])) ; 
            end
    endgenerate
    generate
        if ((SETUP_COMP1_EN == "ENABLED")) 
            begin : genblk17
                (* syn_black_box=1, black_box_pad_pin="comp1in_io,comp1in_w" *) BB_ADC BB_ADC_COMP1IN (.IOPAD(comp1in_io), 
                            .INADC(comp1in_w)) ; 
                (* syn_black_box=1, black_box_pad_pin="comp1ip_io,comp1ip_w" *) BB_ADC BB_ADC_COMP1IP (.IOPAD(comp1ip_io), 
                            .INADC(comp1ip_w)) ; 
            end
    endgenerate
    generate
        if ((SETUP_COMP2_EN == "ENABLED")) 
            begin : genblk18
                (* syn_black_box=1, black_box_pad_pin="comp2in_io,comp2in_w" *) BB_ADC BB_ADC_COMP2IN (.IOPAD(comp2in_io), 
                            .INADC(comp2in_w)) ; 
                (* syn_black_box=1, black_box_pad_pin="comp2ip_io,comp2ip_w" *) BB_ADC BB_ADC_COMP2IP (.IOPAD(comp2ip_io), 
                            .INADC(comp2ip_w)) ; 
            end
    endgenerate
    generate
        if ((SETUP_COMP3_EN == "ENABLED")) 
            begin : genblk19
                (* syn_black_box=1, black_box_pad_pin="comp3in_io,comp3in_w" *) BB_ADC BB_ADC_COMP3IN (.IOPAD(comp3in_io), 
                            .INADC(comp3in_w)) ; 
                (* syn_black_box=1, black_box_pad_pin="comp3ip_io,comp3ip_w" *) BB_ADC BB_ADC_COMP3IP (.IOPAD(comp3ip_io), 
                            .INADC(comp3ip_w)) ; 
            end
    endgenerate
    ADC #(.ADC_ENP("ENABLED"),
            .CLK_DIV(SETUP_CLK_DIV),
            .CTLCOMPSW1(SETUP_COMP1_SW_CTRL),
            .CTLCOMPSW2(SETUP_COMP2_SW_CTRL),
            .CTLCOMPSW3(SETUP_COMP3_SW_CTRL),
            .DF(SETUP_ADC_DATA_FORMAT_SEL),
            .EN_COMP1(SETUP_COMP1_EN),
            .EN_COMP2(SETUP_COMP2_EN),
            .EN_COMP3(SETUP_COMP3_EN),
            .OMA(SETUP_ADC0_CONV_MODE),
            .OMB(SETUP_ADC1_CONV_MODE),
            .REFBUFAEN(REFBUFAEN),
            .REFBUFBEN(REFBUFBEN),
            .SLEEP("DISABLED"),
            .VREFACFG(SETUP_ADC0_VREF_CFG),
            .VREFASEL(SETUP_ADC0_VREF_SEL),
            .VREFBCFG(SETUP_ADC1_VREF_CFG),
            .VREFBSEL(SETUP_ADC1_VREF_SEL)) ADC_0 (.ADCEN(adc_en_i), 
                .CAL(cal_i), 
                .CALRDY(calrdy_o), 
                .CHAEN(1'b1), 
                .CHASEL(chasel_i), 
                .CHBEN(1'b1), 
                .CHBSEL(chbsel_i), 
                .CLKDCLK(clk_dclk_i), 
                .CLKFAB(clk_fabric_i), 
                .COG(cog_o), 
                .COMP1IN(comp1in_w), 
                .COMP1IP(comp1ip_w), 
                .COMP1OL(comp1ol_o), 
                .COMP1O(comp1o_o), 
                .COMP2IN(comp2in_w), 
                .COMP2IP(comp2ip_w), 
                .COMP2OL(comp2ol_o), 
                .COMP2O(comp2o_o), 
                .COMP3IN(comp3in_w), 
                .COMP3IP(comp3ip_w), 
                .COMP3OL(comp3ol_o), 
                .COMP3O(comp3o_o), 
                .CONVSTOP(convstop_i), 
                .DA(da_o), 
                .DB(db_o), 
                .EOC(eoc_o), 
                .DN0(dn0_i), 
                .DN1(dn1_i), 
                .DP0(dp0_i), 
                .DP1(dp1_i), 
                .GPION(adcn_w), 
                .GPIOP(adcp_w), 
                .RESETN(resetn_i), 
                .RSTN(resetn_i), 
                .SOC(soc_i)) ; 
endmodule


