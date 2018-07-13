/************************************************************************
  Project   : SoC Robot WAR Support
  Title     : FPGA HDL Source for Image Processing(SRAM Interface)
  File name : FPGA_Processing.v
  Author(s) : Advanced Digital Chips Inc. 
  History
        + v0.0   2002/ 9/18 : First version released
        + v0.1   2003/ 7/08 : Update
		  + v0.2   2004/ 6/12 : Update(Conversion for FPGA Chip(XC2S100))
	     + v0.3   2006/ 6/30 : Update(Conversion for FPGA Chip(XC3S400))
	Modify by KAIST SDIA
	     + v0.5	 2008/7/23: Update(Conversion for FPGA Chip (Cyclone3 - EP3C16U256C7N)
	Modify
	     + v0.6  2011/9/07: Update(Conversion for FPGA Chip (Cyclone4 - EP4CE75U19I7))
		                      - change to use Internal FPGA RAM
		                     Video Decoder Input YCbCr422 to RGB565
									Add Interrupt Request 1 channel
	Modify
	     + v0.7  2014/6/19: Update(Conversion for AMAZON2 Chip)								
									
*************************************************************************/
module RAM_CLEAN_2 ( clock, wren, addr, data, q );

input clock;
input wren;
input [14:0] addr;
input [ 5:0] data;

output [ 5:0] q;

reg [ 5:0] ram [0:21599];

reg [14:0] top, bot, left, right;
reg [ 5:0] result;

always @(posedge clock)
begin
	if (wren) ram[addr] <= data;

	top <= addr / 180 < 1 ? addr : addr - 180;
	bot <= addr / 180 < 119 ? addr + 180 : addr;
	left <= addr % 180 < 1 ? addr : addr - 1;
	right <= addr % 180 < 179 ? addr + 1 : addr;
	
//	result <= (ram[top] | ram[bot] | ram[left] | ram[right]) & ram[addr_rd];  //  clean
//	result <= ram[top] & ram[bot] & ram[left] & ram[right] & ram[addr_rd];  //  erode
	result <= ((ram[top] & ram[bot]) | (ram[top] & ram[left]) | (ram[top] & ram[right]) | (ram[bot] & ram[left]) | (ram[bot] & ram[right]) | (ram[left] & ram[right])) & ram[addr];
end

assign q = result;

endmodule

module RAM_BRIDGE ( clock, wren, addr, data, q );

input clock;
input wren;
input [14:0] addr;
input [ 5:0] data;

output [ 5:0] q;

reg [ 5:0] ram [0:21599];

reg [14:0] top, bot, left, right;
reg [ 5:0] result;

always @(posedge clock)
begin
	if (wren) ram[addr] <= data;

	top <= addr / 180 < 1 ? addr : addr - 180;
	bot <= addr / 180 < 119 ? addr + 180 : addr;
	left <= addr % 180 < 1 ? addr : addr - 1;
	right <= addr % 180 < 179 ? addr + 1 : addr;
	
	result <= (ram[top] & ram[bot]) | (ram[left] & ram[right]) | ram[addr];	
end

assign q = result;

endmodule

module RAM_BUFF ( clock, wren, addr, data, q );

input clock,
		wren;
input [14:0] addr;
input [ 5:0] data;
output [ 5:0] q;

reg [ 5:0] ram [0:21599];

always @(posedge clock)
begin
	if (wren) ram[addr] <= data;
end

assign q = ram[addr];

endmodule

module FPGA_Processing ( resetx,
					clk_llc2, clk_llc, vref, href, odd, vpo,                  	   // mem_ctrl <- SAA7111A
					AMAmem_adr, AMAmem_data, AMAmem_csx, 			            // Amazon2 Interface
					AMAmem_wrx, AMAmem_rdx, AMAmem_waitx, AMAmem_irq0, AMAmem_irq1,	   // Amazon2 Interface
					led_test       										                  // FPGA test(LED On/Off)
					);

input         resetx;

/* mem_ctrl <- SAA7111A */
input         clk_llc2;      // 13.5 MHz
input         clk_llc;       // 27 MHz
input         vref;          // vertical sync.
input         href;          // horizontal sync.
input         odd;           // odd field (RTS0) 
input  [15:0] vpo;           // RGB(565) input vidoe data

/* Amazon2 SRAM Interface */
input  [14:0] AMAmem_adr;     // Amazon2 Address[15:1] 
inout  [15:0] AMAmem_data;    // Amazon2 Data[15:0] 
input         AMAmem_csx;     // FPGA Chip Select, Amazon2 nCS3
input         AMAmem_wrx;     // write strobe, Amazon2 nWR
input         AMAmem_rdx;     // read strobe, Amazon2 nRD
output        AMAmem_waitx;   // Amazon2 read wait, Amazon2 nWAIT 
output        AMAmem_irq0;    // external read interrupt(FPGA -> Amazon2), Amazon2 IRQ6
output        AMAmem_irq1;    // external read interrupt(FPGA -> Amazon2), Amazon2 IRQ7

/* FPGA test */
output       led_test;

//-----------------------------------------------------------------
// SRAM WRITE & Interrupt	
// SAA7111A Video Decoder => SRAM, V/H sync. input 
// 720x480 -> 180x120 compression
//-----------------------------------------------------------------

reg [ 1:0] clk_div;     

always @(negedge resetx or posedge clk_llc2)
   if      (~resetx)         clk_div <= 2'b0;
   else                      clk_div <= clk_div + 1'b1;

// clk_llc8 : 180(720/4) clock generation
wire clk_llc4  = clk_div[0];
wire clk_llc8  = clk_div[1];

// href2 : (480/2) clock generation
reg  href2;
always @(negedge resetx or posedge href)
   if      (~resetx)          href2 <= 1'b0;
   else                       href2 <= ~href2;

// select only odd frame
wire oddframe   = odd & vref;

wire evenframe  = ~odd & vref;

// 120(480/4) clock generation
wire href2_wr   = href2 & href & oddframe;// & oddframe2; 

wire href2_wr_E = href2 & href & evenframe;

wire href2_wr_A = href2 & href & vref;

/////////////////////////////////////////////////////////////////////////////
// YCbCr422 to RGB565
reg [ 1:0] CodeCnt;

reg [ 7:0] Y_Data1, Cb_Data1, Cr_Data1, Y_Data2, Cb_Data2, Cr_Data2;
reg [20:0] R_int,G_int,B_int,X_int,A_int,B1_int,B2_int,C_int; 
reg [ 9:0] const1,const2,const3,const4,const5;


always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)					CodeCnt <= 2'b0;
	else if (href2_wr)				CodeCnt <= CodeCnt + 1'b1;
	else if (~href2_wr)				CodeCnt <= 2'b0;
	
	
always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Cb_Data1 <= 8'b0;
	else if (CodeCnt==2'b00)	Cb_Data1 <= vpo[15:8];
	
always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Y_Data1 <= 8'b0;
	else if (CodeCnt==2'b01)	Y_Data1 <= vpo[15:8];
	
always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Cr_Data1 <= 8'b0;
	else if (CodeCnt==2'b10)	Cr_Data1 <= vpo[15:8];
	
always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Y_Data2 <= 8'b0;
	else if (CodeCnt==2'b11)	Y_Data2 <= vpo[15:8];
	
always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Cb_Data2 <= 8'b0;
	else if (CodeCnt==2'b00)	Cb_Data2 <= vpo[15:8];

always @ (posedge clk_llc or negedge resetx)
	if      (~resetx)				Cr_Data2 <= 8'b0;
	else if (CodeCnt==2'b10)	Cr_Data2 <= vpo[15:8];

	
//registering constants
always @ (posedge clk_llc)
begin
 const1 = 10'b 0100101010; //1.164 = 01.00101010
 const2 = 10'b 0110011000; //1.596 = 01.10011000
 const3 = 10'b 0011010000; //0.813 = 00.11010000
 const4 = 10'b 0001100100; //0.392 = 00.01100100
 const5 = 10'b 1000000100; //2.017 = 10.00000100
end

wire [9:0] YData1 = {Y_Data1, 2'b00};
wire [9:0] CbData1 = {Cb_Data1, 2'b00};
wire [9:0] CrData1 = {Cr_Data1, 2'b00};
wire [9:0] YData2 = {Y_Data2, 2'b00};
wire [9:0] CbData2 = {Cb_Data2, 2'b00};
wire [9:0] CrData2 = {Cr_Data2, 2'b00};

always @ (posedge clk_llc or negedge resetx)
   if (~resetx)
      begin
       A_int <= 0; B1_int <= 0; B2_int <= 0; C_int <= 0; X_int <= 0;
      end
   else if (CodeCnt==2'b10)
     begin
     X_int <= (const1 * (YData1 - 'd64)) ;
     A_int <= (const2 * (CrData1 - 'd512));
     B1_int <= (const3 * (CrData1 - 'd512));
     B2_int <= (const4 * (CbData1 - 'd512));
     C_int <= (const5 * (CbData1 - 'd512));
     end
	else if (CodeCnt==2'b11)
     begin
     X_int <= (const1 * (YData2 - 'd64)) ;
     A_int <= (const2 * (CrData2 - 'd512));
     B1_int <= (const3 * (CrData2 - 'd512));
     B2_int <= (const4 * (CbData2 - 'd512));
     C_int <= (const5 * (CbData2 - 'd512));
     end
	  
always @ (posedge clk_llc or negedge resetx)
   if (~resetx)
      begin
       R_int <= 0; G_int <= 0; B_int <= 0;
      end
   else if ((CodeCnt==2'b10) | (CodeCnt==2'b11))
     begin
     R_int <= X_int + A_int;  
     G_int <= X_int - B1_int - B2_int; 
     B_int <= X_int + C_int; 
     end


//wire [ 4:0] R = (R_int[20]) ? 5'b0 : (R_int[19:18] == 2'b0) ? R_int[17:13] : 5'b11111;
//wire [ 5:0] G = (G_int[20]) ? 6'b0 : (G_int[19:18] == 2'b0) ? G_int[17:12] : 6'b111111;
//wire [ 4:0] B = (B_int[20]) ? 5'b0 : (B_int[19:18] == 2'b0) ? B_int[17:13] : 5'b11111;	  

wire [ 7:0] R8 = (R_int[20]) ? 8'b0 : (R_int[19:18] == 2'b0) ? R_int[17:10] : 8'b11111111;
wire [ 7:0] G8 = (G_int[20]) ? 8'b0 : (G_int[19:18] == 2'b0) ? G_int[17:10] : 8'b11111111;
wire [ 7:0] B8 = (B_int[20]) ? 8'b0 : (B_int[19:18] == 2'b0) ? B_int[17:10] : 8'b11111111;	

//wire [15:0] DecVData = {R,G,B};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//	RGB888 to HSV
integer C_MAX, C_MIN, DELTA, H_DIFF, S_DATA, H_DATA;
reg  [ 1:0] MAX;

always @(negedge resetx or posedge clk_llc4)
	if (~resetx)
		begin
		C_MAX <= 0;
		C_MIN <= 0;
		MAX <= 2'b00;
		end
	else
		begin
		if (R8 > G8 & R8 > B8) 
			begin
			C_MAX <= R8;
			MAX <= 2'b00;
			end
		if (R8 < G8 & R8 < B8) C_MIN <= R8;
		if (G8 > R8 & G8 > B8) 
			begin
			C_MAX <= G8;
			MAX <= 2'b01;
			end
		if (G8 < R8 & G8 < B8) C_MIN <= G8;
		if (B8 > R8 & B8 > G8) 
			begin
			C_MAX <= B8;
			MAX <= 2'b10;
			end
		if (B8 < R8 & B8 < G8) C_MIN <= B8;
		end
		
wire MAX_R = ~MAX[1] & ~MAX[0];
wire MAX_G = ~MAX[1] & MAX[0];
wire MAX_B = MAX[1] & ~MAX[0];

always @(negedge resetx or posedge clk_llc4)
	if (~resetx)		DELTA <= 0;
	else					DELTA <= C_MAX - C_MIN;

always @(negedge resetx or posedge clk_llc4)
	if (~resetx)
		begin
		H_DIFF <= 0;
		S_DATA <= 0;
		end
	else
		begin
		if (MAX_R) H_DIFF <= G8 - B8;
		if (MAX_G) H_DIFF <= B8 - R8;
		if (MAX_B) H_DIFF <= R8 - G8;
		
		S_DATA <= C_MAX > 0 ? DELTA * 255 / C_MAX : 0;
		end

always @(negedge resetx or posedge clk_llc4)
	if (~resetx)		H_DATA <= 0;
	else
		if (DELTA == 0) H_DATA <= 0;
		else
			begin
			if (MAX_R) H_DATA <= (H_DIFF * 40 / DELTA + 240) % 240;
			if (MAX_G) H_DATA <= H_DIFF * 40 / DELTA + 80;
			if (MAX_B) H_DATA <= H_DIFF * 40 / DELTA + 160;
			end
	
wire [ 7:0] H = H_DATA[7 :0];
wire [ 7:0] S = S_DATA[7 :0];
wire [ 7:0] V = C_MAX[ 7 :0];

wire [15:0] DecVData = {S[7:3], H[7:2], V[7:3]};
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//	Binarization

reg [ 7:0] H_THRES, S_THRES, V_THRES, 
				R_H, G_H, B_H, Y_H, O_H, 
				R_MIN, R_MAX, G_MIN, G_MAX, B_MIN, B_MAX, Y_MIN, Y_MAX, O_MIN, O_MAX;
reg R_B, G_B, B_B, Y_B, O_B, BK_B, C;

always @ (posedge clk_llc4)
begin
	H_THRES = 8'd10;
	S_THRES = 8'd96;
	V_THRES = 8'd96;

	R_H	= 8'd230;
	G_H	= 8'd90;
	B_H	= 8'd144;
	Y_H	= 8'd40;
	O_H	= 8'd15;
	
	R_MIN = R_H - H_THRES;
	R_MAX = R_H + H_THRES;
	
	G_MIN = G_H - H_THRES;
	G_MAX = G_H + H_THRES;
	
	B_MIN = B_H - H_THRES;
	B_MAX = B_H + H_THRES;
	
	Y_MIN = Y_H - H_THRES;
	Y_MAX = Y_H + H_THRES;
	
	O_MIN = O_H - H_THRES;
	O_MAX = O_H + H_THRES;
end


always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	BK_B <= 1'b0;
	else
	begin
		BK_B <= V < V_THRES;
	end


always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	C <= 1'b0;
	else
	begin
		C <= ~BK_B & (S > S_THRES);
	end


always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	R_B <= 1'b0;
	else
	begin
		R_B <= C & ((R_MIN < H) & (H < R_MAX));
	end
	
always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	G_B <= 1'b0;
	else
	begin
		G_B <= C & ((G_MIN < H) & (H < G_MAX));
	end
	
always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	B_B <= 1'b0;
	else
	begin
		B_B <= C & ((B_MIN < H) & (H < B_MAX));
	end
	
always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	Y_B <= 1'b0;
	else
	begin
		Y_B <= C & ((Y_MIN < H) & (H < Y_MAX));
	end
	
always @ (negedge resetx or posedge clk_llc4)
	if		(~resetx)	O_B <= 1'b0;
	else
	begin
		O_B <= C & ((O_MIN < H) & (H < O_MAX));
	end

//wire ROY = R_B | O_B | Y_B;
//wire ROYBK = R_B | O_B | Y_B | BK_B;
//wire GY	= G_B | Y_B;
//wire GYO = G_B | Y_B | O_B;
//wire GYOBK = G_B | Y_B | O_B | BK_B;
//wire BBK = B_B | BK_B;
//wire [15:0] DecVData = {ROY, ROYBK, R_B, O_B, Y_B,		GY, GYOBK, GYO, G_B, G_B, G_B,	B_B, BBK, B_B, B_B, BK_B};
wire [ 5:0] DecVData_C = {R_B, O_B, Y_B, G_B, B_B, BK_B};

wire R_M = vmem_B_q[5];
wire O_M = vmem_B_q[4];
wire Y_M = vmem_B_q[3];
wire G_M = vmem_B_q[2];
wire B_M = vmem_B_q[1];
wire BK_M = vmem_B_q[0];

wire ROY_M = R_M | O_M | Y_M;
wire ROYBK_M = R_M | O_M | Y_M | BK_M;
wire GY_M = G_M | Y_M;
wire GYO_M = G_M | Y_M | O_M;
wire GYOBK_M = G_M | Y_M | O_M | BK_M;
wire BBK_M = B_M | BK_M;

//wire [15:0] DecVData = {ROY_M, ROYBK_M, R_M, O_M, Y_M,	GY_M, GYOBK_M, GYO_M, G_M, G_M, G_M,	B_M, BBK_M, B_M, B_M, BK_M};
/////////////////////////////////////////////////////////////////////////////

// 180x120 write clock generation 
wire vpo_wrx    = ~(vref & href2_wr & clk_llc8);

reg vpo_wrxd1;
reg vpo_wrxd2;
reg vpo_wrxd3;

always @(negedge resetx or posedge clk_llc)
   if      (~resetx)           vpo_wrxd1 <= 1'b1;
   else                        vpo_wrxd1 <= vpo_wrx;

always @(negedge resetx or posedge clk_llc)
   if      (~resetx)           vpo_wrxd2 <= 1'b1;
   else                        vpo_wrxd2 <= vpo_wrxd1;

always @(negedge resetx or posedge clk_llc)
   if      (~resetx)           vpo_wrxd3 <= 1'b1;
   else                        vpo_wrxd3 <= vpo_wrxd2;



// delayed write clock for no grich
wire   vd_wrx    = ~(~vpo_wrxd1 & vpo_wrxd3);

//------------------------------------------------------
// 16bit SRAM address generation (64KB)
// 180 x 120
//   __________ 
//  |          |  0x00000  
//  | 180x120  |  
//  |          |  RGB565 1
//  |          |  
//  |----------|  0x05460(word)
//  | reserved |  
//  |----------|  0x08000(word)
//  |          |  
//  | 180x120  |  RGB565 2
//  |          |  
//  |          |  
//  |----------|  0x0D460(word)  
//  | reserved |  
//  |__________|  0x0FFFF
//------------------------------------------------------

reg [15:0] vdata;
reg [15:0] vadr;
reg A_addr;
always @(negedge resetx or posedge clk_llc8)
   if      (~resetx)           vdata <= 16'b0;
	else if (href2_wr)          vdata <= DecVData;

always @(negedge resetx or posedge clk_llc8)
   if      (~resetx)           vadr[14:0] <= 15'b0;
   else if (~oddframe)         vadr[14:0] <= 15'b0;
   else if (href2_wr)          vadr[14:0] <= vadr[14:0] + 1'b1;

always @(negedge resetx or posedge odd)
   if      (~resetx)       vadr[15] <= 1'b0;
   else                    vadr[15] <= ~vadr[15];
	
always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)       A_addr <= 1'b0;
   else                    A_addr <= AMAmem_irq1;

reg [ 5:0] vdata_C;
reg [14:0] vadr_M;
always @(negedge resetx or posedge clk_llc8)
	if		  (~resetx)			vdata_C <= 6'b0;
	else if (~vref)			vdata_C <= 6'b0;
	else if (href2_wr_A)		vdata_C <= DecVData_C;
	
always @(negedge resetx or posedge clk_llc8)
	if		  (~resetx)			vadr_M <= 15'b0;
	else if (~vref)			vadr_M <= 15'b0;
	else if (href2_wr_A)		vadr_M <= vadr_M + 1'b1;
	

reg [ 5:0] vdata_B;
reg [14:0] vadr_B;
always @(negedge resetx or posedge clk_llc8)
	if		  (~resetx)			vdata_B <= 6'b0;
	else if (~vref)			vdata_B <= 6'b0;
	else if (href2_wr_A)		vdata_B <= vmem_C_q;
	
always @(negedge resetx or posedge clk_llc8)
	if		  (~resetx)			vadr_B <= 15'b0;
	else if (~vref)			vadr_B <= 15'b0;
	else if (href2_wr_A)		vadr_B <= vadr_B + 1'b1;


//----------------------------------------------------------------------------------
// External Interrupt Generation
// 1 interrupter per 1 frame(interrupt length = Sys_clk 2cycle)
//----------------------------------------------------------------------------------

reg  oddframe_d1;
reg  oddframe_d2;
reg  oddframe_d3;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)             oddframe_d1 <= 1'b0;
   else                          oddframe_d1 <= oddframe;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)             oddframe_d2 <= 1'b0;
   else                          oddframe_d2 <= oddframe_d1;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)             oddframe_d3 <= 1'b0;
   else                          oddframe_d3 <= oddframe_d2;

assign AMAmem_irq0 = ~oddframe_d1 & oddframe_d3 & (vadr[15] == 1);
assign AMAmem_irq1 = ~oddframe_d1 & oddframe_d3 & (vadr[15] == 0);





wire	[15:0]  	vmem_addr;
wire	[15:0]  	vmem_data;
wire	        	vmem_rden;
wire	        	vmem_wren;
wire  [15:0]  	vmem_q;

wire  [14:0]  	vmem_C_addr;
wire	[ 5:0]  	vmem_C_data;
wire				vmem_C_wren;
wire	[ 5:0]	vmem_C_q;

wire	[14:0]  	vmem_B_addr;
wire	[ 5:0]  	vmem_B_data;
wire	        	vmem_B_rden;
wire	        	vmem_B_wren;
wire  [ 5:0]  	vmem_B_q;

//////////////////////////// MEGA Wizard //////////////////////////////////
// FPGA PLL
wire  Sys_clk;
// clk_llc PLL
pll	pll_inst (
		.inclk0 ( clk_llc ),
		.c0 ( Sys_clk )
		);
// Binarized Image Block RAM Instance
RAM	RAM_inst (
	.address ( vmem_addr ),
	.clock ( Sys_clk ),
	.data ( vmem_data ),
	.rden ( vmem_rden ),
	.wren ( vmem_wren ),
	.q ( vmem_q )
	);
	
// Original Image Block RAM Instance
RAM_CLEAN_2 RAM_inst_M (
	.clock( Sys_clk ),
	.wren( vmem_C_wren ), 
	.addr( vmem_C_addr ), 
	.data( vmem_C_data ),
	.q( vmem_C_q )
);


// Blurred Image Block RAM Instance
RAM_BUFF	RAM_inst_B	(
	.addr ( vmem_B_addr ),
	.clock ( Sys_clk ),
	.data ( vmem_B_data ),
	.wren ( vmem_B_wren ),
	.q ( vmem_B_q )
);

////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------
// SRAM Controller State Machine
// SRAM (2cycle command & wait enable)
//-----------------------------------------------------------------
supply1   vdd;
reg [6:0] cs, ns;

parameter s0  = 7'b0000001;
parameter s1  = 7'b0000010;
parameter s2  = 7'b0000100;
parameter s3  = 7'b0001000;
parameter s4  = 7'b0010000;
parameter s5  = 7'b0100000;
parameter s6  = 7'b1000000;

wire mcs0 = cs[0];    // idle state
wire mcs1 = cs[1];    // sa7111 video data write state 
wire mcs2 = cs[2];    // sa7111 video data write last state 
wire mcs3 = cs[3];    // Eagle data write state(for test)
wire mcs4 = cs[4];    // Eagle data write last state
wire mcs5 = cs[5];    // Eagle data read state 
wire mcs6 = cs[6];    // Eagle data read last state

always @(negedge resetx or posedge Sys_clk)
  if (~resetx) cs <= s0;
  else         cs <= ns;

always @(mcs0 or mcs1 or mcs2 or mcs3 or mcs4 or mcs5 or mcs6 or AMAmem_csx or vd_wrx or AMAmem_wrx or AMAmem_rdx) begin
  ns = s0;
  case (vdd)	// synopsys parallel_case full_case
    mcs0    : if      ( ~vd_wrx )                            ns = s1;
              else if (  vd_wrx & ~AMAmem_csx & ~AMAmem_wrx )  ns = s3;
              else if (  vd_wrx & ~AMAmem_csx & ~AMAmem_rdx )  ns = s5;
              else							ns = s0;
    mcs1    : if      (vd_wrx)			ns = s2;
              else             			ns = s1;
              
    mcs2    :                        	ns = s0;

    mcs3    : if      (AMAmem_wrx )   	ns = s4;
              else                   	ns = s3;
              
    mcs4    : 									ns = s0;
    
	 mcs5    : if      (AMAmem_rdx)    	ns = s6;
              else                    	ns = s5;
              
    mcs6    :                     		ns = s0;

    default :                 			ns = s0;          
  endcase
end  


//-----------------------------------------------------------------
// SRAM Controller Output
//-----------------------------------------------------------------
//assign mem_csx     =  mcs0;		// SRAM Chip select

assign vmem_wren     = mcs1; 	// SRAM Write // ~( mcs1 );

assign vmem_rden     = mcs5; 		// SRAM Read  //~mcs5;
 
//assign mem_bex[1]  = ~(mcs1 | mcs3 | mcs5) ;	// 16bit MSB Byte enable
//assign mem_bex[0]  = ~(mcs1 | mcs3 | mcs5) ;	// 16bit LSB Byte enable


assign AMAmem_data  = ( ~AMAmem_csx ) ? vmem_q : 16'bZ;

assign vmem_data    = ( mcs1 | mcs2 ) ? vdata : 16'bZ ;
//assign vmem_data    = ( (~mcs0 & mcs1) | (~mcs0 & mcs2) ) ? vdata : 16'bZ ;

assign vmem_addr     = ( mcs1 | mcs2 ) ? vadr : {A_addr, AMAmem_adr};	// 16bit SRAM address
//assign vmem_addr     = ( (~mcs0 & mcs1) | (~mcs0 & mcs2) ) ? vadr : AMAmem_adr;


assign vmem_C_wren   = href2_wr & clk_llc8;

assign vmem_C_data   = vdata_C;

assign vmem_C_addr   = vadr_M;


assign vmem_B_wren   = href2_wr_E & clk_llc8;

assign vmem_B_data   = vdata_B;

assign vmem_B_addr   = vadr_B;

//-----------------------------------------------------------------
// FPGA waitx signal generation
// if Eagle is interfaced to low speed device, waitx has to delayed  
//-----------------------------------------------------------------
wire    waitx = AMAmem_csx  | ~( mcs1 | mcs2 ) ;

reg  waitx_d1;
reg  waitx_d2;
reg  waitx_d3;
reg  waitx_d4;
reg  waitx_d5;

reg  waitx_d6;
reg  waitx_d7;
reg  waitx_d8;
reg  waitx_d9;
reg  waitx_d10;


always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)     	waitx_d1 <= 1'b0;
   else                      	waitx_d1 <= waitx;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d2 <= 1'b0;
   else                     	waitx_d2 <= waitx_d1;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d3 <= 1'b0;
   else                     	waitx_d3 <= waitx_d2;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d4 <= 1'b0;
   else                     	waitx_d4 <= waitx_d3;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d5 <= 1'b0;
   else                     	waitx_d5 <= waitx_d4;

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d6 <= 1'b0;
   else                     	waitx_d6 <= waitx_d5;
   
always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d7 <= 1'b0;
   else                     	waitx_d7 <= waitx_d6;
   
always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d8 <= 1'b0;
   else                     	waitx_d8 <= waitx_d7;
   
always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d9 <= 1'b0;
   else                     	waitx_d9 <= waitx_d8; 

always @(negedge resetx or posedge Sys_clk)
   if      (~resetx)         	waitx_d10 <= 1'b0;
   else                     	waitx_d10 <= waitx_d9;   


assign AMAmem_waitx = waitx & waitx_d1 & waitx_d2 & waitx_d3 & waitx_d4 & waitx_d5 & waitx_d6 & waitx_d7 & waitx_d8 & waitx_d9 & waitx_d10;



//-----------------------------------------------------------------
// FPGA Test
// led has to on/off after FPGA download
//-----------------------------------------------------------------
reg [ 5 : 0 ]  led_blink;
wire  vadrclk = vadr[14];

always @(negedge resetx or posedge vadrclk )
   if      (~resetx)           led_blink   <= 6'b0;
   else                        led_blink   <= led_blink + 1'b1;

assign led_test = led_blink[5];

endmodule
