component MyADC is
    port(
        adc_en_i: in std_logic;
        adc_resetn_i: in std_logic;
        adc_clk_i: in std_logic;
        fab_clk_i: in std_logic;
        adc_cal_i: in std_logic;
        adc_soc_i: in std_logic;
        adc_calrdy_o: out std_logic;
        adc0_ch_sel_i: in std_logic_vector(3 downto 0);
        adc1_ch_sel_i: in std_logic_vector(3 downto 0);
        adc_cog_o: out std_logic;
        adc_eoc_o: out std_logic;
        adc_convstop_i: in std_logic;
        adc0_o: out std_logic_vector(11 downto 0);
        adc1_o: out std_logic_vector(11 downto 0);
        ADC_CP0: in std_logic;
        ADC_CN0: in std_logic;
        ADC_CP1: in std_logic;
        ADC_CN1: in std_logic;
        ADC_CP8: in std_logic;
        ADC_CN8: in std_logic;
        ADC_CP9: in std_logic;
        ADC_CN9: in std_logic
    );
end component;

__: MyADC port map(
    adc_en_i=>,
    adc_resetn_i=>,
    adc_clk_i=>,
    fab_clk_i=>,
    adc_cal_i=>,
    adc_soc_i=>,
    adc_calrdy_o=>,
    adc0_ch_sel_i=>,
    adc1_ch_sel_i=>,
    adc_cog_o=>,
    adc_eoc_o=>,
    adc_convstop_i=>,
    adc0_o=>,
    adc1_o=>,
    ADC_CP0=>,
    ADC_CN0=>,
    ADC_CP1=>,
    ADC_CN1=>,
    ADC_CP8=>,
    ADC_CN8=>,
    ADC_CP9=>,
    ADC_CN9=>
);
