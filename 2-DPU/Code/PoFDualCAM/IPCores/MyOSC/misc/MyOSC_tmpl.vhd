component MyOSC is
    port(
        hf_out_en_i: in std_logic;
        hf_clk_out_o: out std_logic;
        lf_clk_out_o: out std_logic
    );
end component;

__: MyOSC port map(
    hf_out_en_i=>,
    hf_clk_out_o=>,
    lf_clk_out_o=>
);
