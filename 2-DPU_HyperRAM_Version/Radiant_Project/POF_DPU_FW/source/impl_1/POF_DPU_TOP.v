`include "ZPortableDefine.v"
//Naming Rules:
//XXXX_i:  internal registers.
//iXXXXX:  external input signals.
//oXXXX:   output to external signals.
//ioXXX:   bi-directional signals.
module POF_DPU_TOP(
    input iSYS_CLK,

    //HyperRAM-1#.
    output oH1_RSTN,
    output oH1_CSN,
    output oH1_CKP,
    //output oH1_CKN,
    output oH1_RWDS0,
    output oH1_RWDS1,
    inout [15:0] ioH1_D,

    //HyperRAM-2#.
    output oH2_RSTN,
    output oH2_CSN,
    output oH2_CKP,
    //output oH2_CKN,
    output oH2_RWDS0,
    output oH2_RWDS1,
    inout [15:0] ioH2_D,

    //IR Image Sensor.
    input iIR_PCLK,
    input iIR_VSYNC,
    input iIR_HSYNC,
    input [13:0] iIR_Data,
    output oIR_PWR_EN,
    input iIR_UART_RX,
    output oIR_UART_TX,

    //UV Image Sensor.
    input iBT1120_CLK,
    input iSYNC_O,
    input [7:0] iBT1120_D,

    input iUV_UART_RX,
    output oUV_UART_TX,

    output oUV_PWR_EN,

    //OV5640 Image Sensor.
    input iDVP_PCLK,
    input iDVP_HSYNC,
    input iDVP_VSYNC,
    input [7:0] iDVP_D,

    output oDVP_SCL,
    output oDVP_SDA,

    output oDVP_RST,
    output oDVP_PWDN,

    //VersionID*2.
    input [1:0] iVersion_ID,
    //Debug LED*3.
    output reg oLED1,
    output reg oLED2,
    output reg oLED3,

    //UART UPLOAD IF.
    input iDPU_RX,
    output reg oDPU_TX,

    output reg oLD_PWR_EN,
    output reg oIAM_ALIVE,
    output reg oIO_P5V0_SHDN,

    //Laser Diode.
    output reg oTX_EN,
    output reg oUART_T_LD,
    output reg oUART_T,

    //Aux.
    output reg [7:0] oIO

);


wire SYS_CLK_i;
wire rst_n_i;
ZPLL __(.clki_i( iSYS_CLK),
        .clkop_o(SYS_CLK_i ),
        .lock_o(rst_n_i));

always @(posedge SYS_CLK_i or negedge rst_n_i)
if(!rst_n_i) begin 
    oDPU_TX<=0; oLD_PWR_EN<=0; oIAM_ALIVE<=0; oIO_P5V0_SHDN<=0; 
    oTX_EN<=0; oUART_T_LD<=0; oUART_T<=0; oIO<=0;
end
else begin
    if(iDPU_RX) begin
        oDPU_TX<=~oDPU_TX; 
        oLD_PWR_EN<=~oLD_PWR_EN; 
        oIAM_ALIVE<=~oIAM_ALIVE; 
        oIO_P5V0_SHDN<=~oIO_P5V0_SHDN;
        oTX_EN<=~oTX_EN; oUART_T_LD<=~oUART_T_LD; oUART_T<=~oUART_T; oIO<=~oIO;
    end
end
//////////////////////////////////////////////////////////////////
//HyperRAM-1# DDR.
reg H1_output_en_i; //tristate control.
wire [15:0] H1_D_In;
assign H1_D_In=ioH1_D;
wire [15:0] H1_D_Out;
assign ioH1_D=(H1_output_en_i)?(H1_D_Out):(16'bz);
//16-bits DDR IN.
wire [15:0] H1_D_In_Rising;
wire [15:0] H1_D_In_Falling;
genvar H1_D_In_i;
generate
    for(H1_D_In_i=0; H1_D_In_i<16; H1_D_In_i=H1_D_In_i+1) begin
        IDDRX1
        #(
        .GSR ("ENABLED")
        )H1_D_IN_DDR(
        .D    (H1_D_In[H1_D_In_i]),  // I
        .SCLK (SYS_CLK_i),  // I
        .RST  (rst_n_i),  // I
        .Q0   (H1_D_In_Rising[H1_D_In_i]),  // O
        .Q1   (H1_D_In_Falling[H1_D_In_i])   // O
        );
    end 
endgenerate

//16-bits DDR OUT.
reg [15:0] H1_D_Out_Rising;
reg [15:0] H1_D_Out_Falling;
genvar H1_D_Out_i;
generate 
    for(H1_D_Out_i=0; H1_D_Out_i<16; H1_D_Out_i=H1_D_Out_i+1) begin
        ODDRX1
        #(
        .GSR ("ENABLED")
        )H1_D_OUT_DDR (
        .D0   (H1_D_Out_Rising[H1_D_Out_i]),  // I
        .D1   (H1_D_Out_Falling[H1_D_Out_i]),  // I
        .SCLK (SYS_CLK_i),  // I
        .RST  (rst_n_i),  // I
        .Q    (H1_D_Out[H1_D_Out_i])   // O
        );
    end
endgenerate



///////////////////////////////////////////////////////////
//HyperRAM-2# DDR.
reg H2_output_en_i; //tristate control.
wire [15:0] H2_D_In;
assign H2_D_In=ioH2_D;
wire [15:0] H2_D_Out;
assign ioH2_D=(H2_output_en_i)?(H2_D_Out):(16'bz);
//16-bits DDR IN.
wire [15:0] H2_D_In_Rising;
wire [15:0] H2_D_In_Falling;
genvar H2_D_In_i;
generate
    for(H2_D_In_i=0; H2_D_In_i<16; H2_D_In_i=H2_D_In_i+1) begin
        IDDRX1
        #(
        .GSR ("ENABLED")
        )H2_D_IN_DDR(
        .D    (H2_D_In[H2_D_In_i]),  // I
        .SCLK (SYS_CLK_i),  // I
        .RST  (rst_n_i),  // I
        .Q0   (H2_D_In_Rising[H2_D_In_i]),  // O
        .Q1   (H2_D_In_Falling[H2_D_In_i])   // O
        );
    end 
endgenerate
//16-bits DDR OUT.
reg [15:0] H2_D_Out_Rising;
reg [15:0] H2_D_Out_Falling;
genvar H2_D_Out_i;
generate 
    for(H2_D_Out_i=0; H2_D_Out_i<16; H2_D_Out_i=H2_D_Out_i+1) begin
        ODDRX1
        #(
        .GSR ("ENABLED")
        )H2_D_OUT_DDR (
        .D0   (H2_D_Out_Rising[H2_D_Out_i]),  // I
        .D1   (H2_D_Out_Falling[H2_D_Out_i]),  // I
        .SCLK (SYS_CLK_i),  // I
        .RST  (rst_n_i),  // I
        .Q    (H2_D_Out[H2_D_Out_i])   // O
        );
    end
endgenerate
/////////////////////////////////////////////////////
reg IR_En_i;
reg HRAM_Init_Done;
wire Cap_Frame_Done;
ZCDS3_Capture   IRCapture_U1(
    .iClk(SYS_CLK_i),
    .iRst_N(rst_n_i),
    .iEn(IR_En_i),

    //input signals.
    .iIR_PCLK(iIR_PCLK),
    .iIR_Data(iIR_Data[7:0]),

    //Start to capture a new frame.
    .oCap_Frame_Start(),
    //End to capture a new frame.
    .oCap_Frame_Done(Cap_Frame_Done),

    //Capture one frame done?
    .oCap_Line_Done(),

	//Write Single-Port RAM Interfaces.
	.oWr_Which(), //O, Write which SPRAM:0/1.
    .oWr_Addr(), //O, Write Address.
    .oWr_Data(), //O, Write Data.
    .oWr_En(), //O, Write Enable. 1:Write, 0:Read.

    //Notify me that DDR-Writer has done initilization.
    .iRAM_Init_Done(HRAM_Init_Done)
);
///////////////////////////////////////////////////////
ZUV_Capture UV_Capture_U1(
    .iClk(SYS_CLK_i),
    .iRst_N(rst_n_i),
    .iEn(IR_En_i),

    .iBT1120_CLK(iBT1120_CLK),
    .iSYNC_O(iSYNC_O),
    .iBT1120_D(iBT1120_D),

    .iUV_UART_RX(iUV_UART_RX),
    .oUV_UART_TX(oUV_UART_TX),

    .oUV_PWR_EN(oUV_PWR_EN)
);
/////////////////////////////////////////////////////
ZOV5640_Capture OV5640_Capture(
    .iClk(SYS_CLK_i),
    .iRst_N(rst_n_i),
    .iEn(1'b1),

    .iDVP_PCLK(iDVP_PCLK),
    .iDVP_HSYNC(iDVP_HSYNC),
    .iDVP_VSYNC(iDVP_VSYNC),
    .iDVP_D(iDVP_D),

    .oDVP_SCL(oDVP_SCL),
    .oDVP_SDA(oDVP_SDA),

    .oDVP_RST(oDVP_RST),
    .oDVP_PWDN(oDVP_PWDN)
);

//////////////////////////////////////////
reg [31:0] Temp_DDR_In_DR1;
///////////////////////////////////////
reg [15:0] cnt1_i;
reg [15:0] step_i;
always @(posedge SYS_CLK_i or negedge rst_n_i)
if(!rst_n_i) begin
    step_i<=0; cnt1_i<=0;
    oLED1<=0; oLED2<=0; oLED3<=0;
end
else begin
    case(step_i)
    `STEP_00: //Fireware ID check.
        if(iVersion_ID==2'b11) begin
            oLED1<=1; oLED2<=1; oLED3<=1; step_i<=step_i+1; 
        end
        else begin
            if(cnt1_i==32768-1) begin cnt1_i<=0; oLED1<=~oLED1; oLED2<=~oLED2; oLED3<=~oLED3; end
            else begin cnt1_i<=cnt1_i+1; end
        end
    `STEP_01: //H1 DDR Shift Out.
        if(cnt1_i==16-1) begin cnt1_i<=0; oLED1<=1; step_i<=step_i+1; end
        else begin 
            H1_output_en_i<=1; //DDR Output.
            H1_D_Out_Rising<=step_i; H1_D_Out_Falling<=step_i+1;
        end
    `STEP_02: //H1 DDR Shift In.
        if(cnt1_i==16-1) begin cnt1_i<=0; oLED1<=0; step_i<=step_i+1; end
        else begin 
            H1_output_en_i<=0; //DDR Input.
            Temp_DDR_In_DR1<={H1_D_In_Rising,H1_D_In_Falling};
        end
    `STEP_03:
        if(Temp_DDR_In_DR1==32'h2025) begin
            oLED2<=1; step_i<=step_i+1; 
        end
        else begin 
            oLED2<=0; step_i<=step_i+1; 
        end
    `STEP_04:
        if(cnt1_i==16-1) begin cnt1_i<=0; oLED1<=1; step_i<=step_i+1; end
        else begin 
            H2_output_en_i<=1; //DDR Output.
            H2_D_Out_Rising<=step_i+2; H2_D_Out_Falling<=step_i+5;
        end
    `STEP_05: //H2 DDR Shift In.
        if(cnt1_i==16-1) begin cnt1_i<=0; oLED3<=0; step_i<=step_i+1; end
        else begin 
            H2_output_en_i<=0; //DDR Input.
            Temp_DDR_In_DR1<={H2_D_In_Rising,H2_D_In_Falling};
        end
    `STEP_06:
        if(Temp_DDR_In_DR1==32'h1999) begin
            oLED2<=1; step_i<=step_i+1; 
        end
        else begin 
            oLED2<=0; step_i<=step_i+1;
        end
    `STEP_07:
        begin HRAM_Init_Done<=1; step_i<=step_i+1; end
    `STEP_08:
        if(Cap_Frame_Done) begin IR_En_i<=0; step_i<=step_i+1; end
        else begin IR_En_i<=1; end
    `STEP_09: //disable optimization.
        if(iIR_Data[13:8]==6'b110111 && iIR_HSYNC && iIR_VSYNC && iIR_UART_RX) begin step_i<=0; end
        else begin step_i<=step_i-1; end
    default:
        begin step_i<=0; end
    endcase
end



// always @(posedge SYS_CLK_i or negedge rst_n_i) 
// if(!rst_n_i)
// begin 
//     oLED1<=0;
//     oLED2<=0;
//     oLED3<=0;
// end
// else begin
//     oLED1<=~oLED1;
//     oLED2<=~oLED2;
//     oLED3<=~oLED3;
// end

endmodule