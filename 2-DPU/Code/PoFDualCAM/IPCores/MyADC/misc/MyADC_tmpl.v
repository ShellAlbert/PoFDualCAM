    MyADC __(.adc_en_i( ),
        .adc_resetn_i( ),
        .adc_clk_i( ),
        .fab_clk_i( ),
        .adc_cal_i( ),
        .adc_soc_i( ),
        .adc_calrdy_o( ),
        .adc0_ch_sel_i( ),
        .adc1_ch_sel_i( ),
        .adc_cog_o( ),
        .adc_eoc_o( ),
        .adc_convstop_i( ),
        .adc0_o( ),
        .adc1_o( ),
        .ADC_CP0( ),
        .ADC_CN0( ),
        .ADC_CP1( ),
        .ADC_CN1( ),
        .ADC_CP8( ),
        .ADC_CN8( ),
        .ADC_CP9( ),
        .ADC_CN9( ));
