/* Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_himax.h"

static struct msm_panel_info pinfo;

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	/* DSI_BIT_CLK at 500MHz, 2 lane, RGB888 */
	{0x03, 0x01, 0x01, 0x00},   /* regulator */
	/* timing   */
	{0xB4, 0x8D, 0x1D, 0x00, 0x20, 0x94, 0x20,  //steve
	 0x8F, 0x20, 0x03, 0x04},  //steve
	{0x7f, 0x00, 0x00, 0x00},   /* phy ctrl */
	{0xee, 0x02, 0x86, 0x00},   /* strength */
	/* pll control */
	{0x40, 0xf9, 0xb0, 0xda, 0x00, 0x50, 0x48, 0x63,
	0x30, 0x07, 0x03,
	0x05, 0x14, 0x03, 0x0, 0x0, 0x54, 0x06, 0x10, 0x04, 0x0},
};

static int __init mipi_video_himax_wvga_pt_init(void)
{
	int ret;

	if (msm_fb_detect_client("mipi_video_himax_wvga")) {
		return 0;		
	}

	pinfo.xres = 480;
	pinfo.yres = 800;
	pinfo.type = MIPI_VIDEO_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.lcdc.h_back_porch = 15;
	pinfo.lcdc.h_front_porch = 5;
	pinfo.lcdc.h_pulse_width = 5;
	pinfo.lcdc.v_back_porch = 16;
	pinfo.lcdc.v_front_porch = 8;
	pinfo.lcdc.v_pulse_width = 8;
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	/* number of dot_clk cycles HSYNC active edge
	   is delayed from VSYNC active edge */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.clk_rate = 499000000;
	pinfo.bl_max = 31;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;

	pinfo.mipi.mode = DSI_VIDEO_MODE;
	pinfo.mipi.pulse_mode_hsa_he = TRUE; /* send HSA and HE following
						VS/VE packet */
	pinfo.mipi.hfp_power_stop = FALSE; /* LP-11 during the HFP period */
	pinfo.mipi.hbp_power_stop = FALSE; /* LP-11 during the HBP period */
	pinfo.mipi.hsa_power_stop = FALSE; /* LP-11 during the HSA period */
	/* LP-11 or let Command Mode Engine send packets in
	HS or LP mode for the BLLP of the last line of a frame */
	pinfo.mipi.eof_bllp_power_stop = TRUE;
	/* LP-11 or let Command Mode Engine send packets in
	HS or LP mode for packets sent during BLLP period */
	pinfo.mipi.bllp_power_stop = TRUE;

	pinfo.mipi.traffic_mode = DSI_BURST_MODE;//When use MDP303,we use DSI_BURST_MODE
	pinfo.mipi.dst_format =  DSI_VIDEO_DST_FORMAT_RGB888;
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB; /* RGB */
	pinfo.mipi.data_lane0 = TRUE;
	pinfo.mipi.data_lane1 = TRUE;

	pinfo.mipi.t_clk_post = 0x04;
	pinfo.mipi.t_clk_pre = 0x1c;

	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.frame_rate = 60; /* FIXME */

	pinfo.mipi.dsi_phy_db = &dsi_video_mode_phy_db;
	pinfo.mipi.dlane_swap = 0x01;
	pinfo.mipi.tx_eot_append = 0x01; /* append EOT at the end
					    of data burst */

	pinfo.lcd.refx100 = 6000;
	ret = mipi_himax_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_WVGA_PT);

	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_video_himax_wvga_pt_init);
