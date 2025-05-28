module ZOV5640_Capture(
    input iClk,
    input iRst_N,
    input iEn,

    input iDVP_PCLK,
    input iDVP_HSYNC,
    input iDVP_VSYNC,
    input [7:0] iDVP_D,

    output reg oDVP_SCL,
    output reg oDVP_SDA,

    output reg oDVP_RST,
    output reg oDVP_PWDN
);

reg [7:0] step_i;
always @(posedge iClk or negedge iRst_N) 
if(!iRst_N) begin
    step_i<=0;
    oDVP_SCL<=0; 
    oDVP_SDA<=0;
    oDVP_RST<=0;
    oDVP_PWDN<=0;
end
else begin
    if(iEn) begin
        if(iDVP_D==8'h33 && iDVP_PCLK && iDVP_HSYNC && iDVP_VSYNC) begin
            oDVP_SCL<=~oDVP_SCL; 
            oDVP_SDA<=~oDVP_SDA;
            oDVP_RST<=~oDVP_RST;
            oDVP_PWDN<=~oDVP_PWDN;
        end
    end
    else begin 
        oDVP_SCL<=0; 
        oDVP_SDA<=0;
        oDVP_RST<=0;
        oDVP_PWDN<=0;
    end
end
endmodule