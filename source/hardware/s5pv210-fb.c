/*
 * driver/s5pv210-fb.c
 *
 * s5pv210 framebuffer drivers.
 *
 * Copyright (c) 2007-2012  jianjun jiang <jerryjianjun@gmail.com>
 * official site: http://xboot.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <types.h>
#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <io.h>
#include <graphic/surface.h>
#include <s5pv210/reg-gpio.h>
#include <s5pv210/reg-others.h>
#include <s5pv210/reg-lcd.h>
#include <s5pv210-clk.h>
#include <s5pv210-tick-delay.h>
#include <s5pv210-fb.h>

enum s5pv210fb_output
{
	S5PV210FB_OUTPUT_RGB,
	S5PV210FB_OUTPUT_ITU,
	S5PV210FB_OUTPUT_I80LDI0,
	S5PV210FB_OUTPUT_I80LDI1,
	S5PV210FB_OUTPUT_WB_RGB,
	S5PV210FB_OUTPUT_WB_I80LDI0,
	S5PV210FB_OUTPUT_WB_I80LDI1,
};

enum s5pv210fb_rgb_mode
{
	S5PV210FB_MODE_RGB_P 			= 0,
	S5PV210FB_MODE_BGR_P 			= 1,
	S5PV210FB_MODE_RGB_S 			= 2,
	S5PV210FB_MODE_BGR_S 			= 3,
};

enum s5pv210_bpp_mode
{
	S5PV210FB_BPP_MODE_1BPP			= 0x0,
	S5PV210FB_BPP_MODE_2BPP			= 0x1,
	S5PV210FB_BPP_MODE_4BPP			= 0x2,
	S5PV210FB_BPP_MODE_8BPP_PAL		= 0x3,
	S5PV210FB_BPP_MODE_8BPP			= 0x4,
	S5PV210FB_BPP_MODE_16BPP_565	= 0x5,
	S5PV210FB_BPP_MODE_16BPP_A555	= 0x6,
	S5PV210FB_BPP_MODE_18BPP_666	= 0x8,
	S5PV210FB_BPP_MODE_18BPP_A665	= 0x9,
	S5PV210FB_BPP_MODE_24BPP_888	= 0xb,
	S5PV210FB_BPP_MODE_24BPP_A887	= 0xc,
	S5PV210FB_BPP_MODE_32BPP		= 0xd,
	S5PV210FB_BPP_MODE_16BPP_A444	= 0xe,
	S5PV210FB_BPP_MODE_15BPP_555	= 0xf,
};

enum {
	S5PV210FB_SWAP_WORD				= (0x1 << 0),
	S5PV210FB_SWAP_HWORD			= (0x1 << 1),
	S5PV210FB_SWAP_BYTE				= (0x1 << 2),
	S5PV210FB_SWAP_BIT				= (0x1 << 3),
};

struct s5pv210fb_lcd
{
	/* horizontal resolution */
	s32_t width;

	/* vertical resolution */
	s32_t height;

	/* bits per pixel */
	s32_t bits_per_pixel;

	/* bytes per pixel */
	s32_t bytes_per_pixel;

	/* vframe frequency */
	s32_t freq;

	/* output path */
	enum s5pv210fb_output output;

	/* rgb mode */
	enum s5pv210fb_rgb_mode rgb_mode;

	/* bpp mode */
	enum s5pv210_bpp_mode bpp_mode;

	/* swap flag */
	u32_t swap;

	struct {
		/* red color */
		s32_t r_mask;
		s32_t r_field;

		/* green color */
		s32_t g_mask;
		s32_t g_field;

		/* blue color */
		s32_t b_mask;
		s32_t b_field;

		/* alpha color */
		s32_t a_mask;
		s32_t a_field;
	} rgba;

	struct {
		/* horizontal front porch */
		s32_t h_fp;

		/* horizontal back porch */
		s32_t h_bp;

		/* horizontal sync width */
		s32_t h_sw;

		/* vertical front porch */
		s32_t v_fp;

		/* vertical front porch for even field */
		s32_t v_fpe;

		/* vertical back porch */
		s32_t v_bp;

		/* vertical back porch for even field */
		s32_t v_bpe;

		/* vertical sync width */
		s32_t v_sw;
	} timing;

	struct {
		/* if 1, video data is fetched at rising edge */
		s32_t rise_vclk;

		/* if HSYNC polarity is inversed */
		s32_t inv_hsync;

		/* if VSYNC polarity is inversed */
		s32_t inv_vsync;

		/* if VDEN polarity is inversed */
		s32_t inv_vden;
	} polarity;

	/* video ram front buffer */
	void * vram_front;

	/* video ram back buffer */
	void * vram_back;

	/* lcd init */
	void (*init)(void);

	/* lcd backlight */
	void (*backlight)(u8_t brightness);
};

/*
 * defined the structure of framebuffer.
 */
struct fb_t
{
	/* the framebuffer's surface */
	struct surface_t surface;

	/* framebuffer's lcd data */
	struct s5pv210fb_lcd * lcd;
};

static bool_t s5pv210fb_set_output(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	cfg = readl(S5PV210_VIDCON0);
	cfg &= ~S5PV210_VIDCON0_VIDOUT_MASK;

	if(lcd->output == S5PV210FB_OUTPUT_RGB)
		cfg |= S5PV210_VIDCON0_VIDOUT_RGB;
	else if(lcd->output == S5PV210FB_OUTPUT_ITU)
		cfg |= S5PV210_VIDCON0_VIDOUT_ITU;
	else if(lcd->output == S5PV210FB_OUTPUT_I80LDI0)
		cfg |= S5PV210_VIDCON0_VIDOUT_I80LDI0;
	else if(lcd->output == S5PV210FB_OUTPUT_I80LDI1)
		cfg |= S5PV210_VIDCON0_VIDOUT_I80LDI1;
	else if(lcd->output == S5PV210FB_OUTPUT_WB_RGB)
		cfg |= S5PV210_VIDCON0_VIDOUT_WB_RGB;
	else if(lcd->output == S5PV210FB_OUTPUT_WB_I80LDI0)
		cfg |= S5PV210_VIDCON0_VIDOUT_WB_I80LDI0;
	else if(lcd->output == S5PV210FB_OUTPUT_WB_I80LDI1)
		cfg |= S5PV210_VIDCON0_VIDOUT_WB_I80LDI1;
	else
		return FALSE;
	writel(S5PV210_VIDCON0, cfg);


	cfg = readl(S5PV210_VIDCON2);
	cfg &= ~(S5PV210_VIDCON2_WB_MASK | S5PV210_VIDCON2_TVFORMATSEL_MASK | S5PV210_VIDCON2_TVFORMATSEL_YUV_MASK);

	if(lcd->output == S5PV210FB_OUTPUT_RGB)
		cfg |= S5PV210_VIDCON2_WB_DISABLE;
	else if(lcd->output == S5PV210FB_OUTPUT_ITU)
		cfg |= S5PV210_VIDCON2_WB_DISABLE;
	else if(lcd->output == S5PV210FB_OUTPUT_I80LDI0)
		cfg |= S5PV210_VIDCON2_WB_DISABLE;
	else if(lcd->output == S5PV210FB_OUTPUT_I80LDI1)
		cfg |= S5PV210_VIDCON2_WB_DISABLE;
	else if(lcd->output == S5PV210FB_OUTPUT_WB_RGB)
		cfg |= (S5PV210_VIDCON2_WB_ENABLE | S5PV210_VIDCON2_TVFORMATSEL_SW | S5PV210_VIDCON2_TVFORMATSEL_YUV444);
	else if(lcd->output == S5PV210FB_OUTPUT_WB_I80LDI0)
		cfg |= (S5PV210_VIDCON2_WB_ENABLE | S5PV210_VIDCON2_TVFORMATSEL_SW | S5PV210_VIDCON2_TVFORMATSEL_YUV444);
	else if(lcd->output == S5PV210FB_OUTPUT_WB_I80LDI1)
		cfg |= (S5PV210_VIDCON2_WB_ENABLE | S5PV210_VIDCON2_TVFORMATSEL_SW | S5PV210_VIDCON2_TVFORMATSEL_YUV444);
	else
		return FALSE;
	writel(S5PV210_VIDCON2, cfg);

	return TRUE;
}

static bool_t s5pv210fb_set_display_mode(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	cfg = readl(S5PV210_VIDCON0);
	cfg &= ~S5PV210_VIDCON0_PNRMODE_MASK;
	cfg |= (lcd->rgb_mode << S5PV210_VIDCON0_PNRMODE_SHIFT);
	writel(S5PV210_VIDCON0, cfg);

	return TRUE;
}

static bool_t s5pv210fb_display_on(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	cfg = readl(S5PV210_VIDCON0);
	cfg |= (S5PV210_VIDCON0_ENVID_ENABLE | S5PV210_VIDCON0_ENVID_F_ENABLE);
	writel(S5PV210_VIDCON0, cfg);

	return TRUE;
}

static bool_t s5pv210fb_display_off(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	cfg = readl(S5PV210_VIDCON0);
	cfg &= ~S5PV210_VIDCON0_ENVID_ENABLE;
	writel(S5PV210_VIDCON0, cfg);

	cfg &= ~S5PV210_VIDCON0_ENVID_F_ENABLE;
	writel(S5PV210_VIDCON0, cfg);

	return TRUE;
}

static bool_t s5pv210fb_set_clock(struct s5pv210fb_lcd * lcd)
{
	u64_t hclk, pixel_clock;
	u32_t div;
	u32_t cfg;

	/*
	 * get hclk for lcd
	 */
	if(! clk_get_rate("dsys-hclk", &hclk))
		return FALSE;

	pixel_clock = ( lcd->freq * (lcd->timing.h_fp + lcd->timing.h_bp + lcd->timing.h_sw + lcd->width) *
			(lcd->timing.v_fp + lcd->timing.v_bp + lcd->timing.v_sw + lcd->height) );

	div = (u32_t)(hclk / pixel_clock);
	if((hclk % pixel_clock) > 0)
		div++;

	/*
	 * fixed clock source: hclk
	 */
	cfg = readl(S5PV210_VIDCON0);
	cfg &= ~(S5PV210_VIDCON0_CLKSEL_MASK | S5PV210_VIDCON0_CLKVALUP_MASK | S5PV210_VIDCON0_VCLKEN_MASK | S5PV210_VIDCON0_CLKDIR_MASK);
	cfg |= (S5PV210_VIDCON0_CLKSEL_HCLK | S5PV210_VIDCON0_CLKVALUP_ALWAYS | S5PV210_VIDCON0_VCLKEN_NORMAL | S5PV210_VIDCON0_CLKDIR_DIVIDED);
	cfg |= S5PV210_VIDCON0_CLKVAL_F(div - 1);

	writel(S5PV210_VIDCON0, cfg);
	return TRUE;
}

static bool_t s5pv210fb_set_polarity(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg = 0;

	if(lcd->polarity.rise_vclk)
		cfg |= S5PV210_VIDCON1_IVCLK_RISING_EDGE;

	if(lcd->polarity.inv_hsync)
		cfg |= S5PV210_VIDCON1_IHSYNC_INVERT;

	if(lcd->polarity.inv_vsync)
		cfg |= S5PV210_VIDCON1_IVSYNC_INVERT;

	if(lcd->polarity.inv_vden)
		cfg |= S5PV210_VIDCON1_IVDEN_INVERT;

	writel(S5PV210_VIDCON1, cfg);

	return TRUE;
}

static bool_t s5pv210fb_set_timing(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	cfg = 0;
	cfg |= S5PV210_VIDTCON0_VBPDE(lcd->timing.v_bpe - 1);
	cfg |= S5PV210_VIDTCON0_VBPD(lcd->timing.v_bp - 1);
	cfg |= S5PV210_VIDTCON0_VFPD(lcd->timing.v_fp - 1);
	cfg |= S5PV210_VIDTCON0_VSPW(lcd->timing.v_sw - 1);
	writel(S5PV210_VIDTCON0, cfg);

	cfg = 0;
	cfg |= S5PV210_VIDTCON1_VFPDE(lcd->timing.v_fpe - 1);
	cfg |= S5PV210_VIDTCON1_HBPD(lcd->timing.h_bp - 1);
	cfg |= S5PV210_VIDTCON1_HFPD(lcd->timing.h_fp - 1);
	cfg |= S5PV210_VIDTCON1_HSPW(lcd->timing.h_sw - 1);
	writel(S5PV210_VIDTCON1, cfg);

	return TRUE;
}

static bool_t s5pv210fb_set_lcd_size(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg = 0;

	cfg |= S5PV210_VIDTCON2_HOZVAL(lcd->width - 1);
	cfg |= S5PV210_VIDTCON2_LINEVAL(lcd->height - 1);
	writel(S5PV210_VIDTCON2, cfg);

	return TRUE;
}

static bool_t s5pv210fb_set_buffer_address(struct s5pv210fb_lcd * lcd, s32_t id)
{
	u32_t start, end;
	u32_t shw;

	start = (u32_t)(lcd->vram_front);
	end = (u32_t)((start + lcd->width * (lcd->height * lcd->bytes_per_pixel)) & 0x00ffffff);

	shw = readl(S5PV210_SHADOWCON);
	shw |= S5PV210_SHADOWCON_PROTECT(id);
	writel(S5PV210_SHADOWCON, shw);

	switch(id)
	{
	case 0:
		writel(S5PV210_VIDW00ADD0B0, start);
		writel(S5PV210_VIDW00ADD1B0, end);
		break;

	case 1:
		writel(S5PV210_VIDW01ADD0B0, start);
		writel(S5PV210_VIDW01ADD1B0, end);
		break;

	case 2:
		writel(S5PV210_VIDW02ADD0B0, start);
		writel(S5PV210_VIDW02ADD1B0, end);
		break;

	case 3:
		writel(S5PV210_VIDW03ADD0B0, start);
		writel(S5PV210_VIDW03ADD1B0, end);
		break;

	case 4:
		writel(S5PV210_VIDW04ADD0B0, start);
		writel(S5PV210_VIDW04ADD1B0, end);
		break;

	default:
		break;
	}

	shw = readl(S5PV210_SHADOWCON);
	shw &= ~(S5PV210_SHADOWCON_PROTECT(id));
	writel(S5PV210_SHADOWCON, shw);

	return TRUE;
}

static bool_t s5pv210fb_set_buffer_size(struct s5pv210fb_lcd * lcd, s32_t id)
{
	u32_t cfg = 0;

	cfg = S5PV210_VIDADDR_PAGEWIDTH(lcd->width * lcd->bytes_per_pixel);
	cfg |= S5PV210_VIDADDR_OFFSIZE(0);

	switch(id)
	{
	case 0:
		writel(S5PV210_VIDW00ADD2, cfg);
		break;

	case 1:
		writel(S5PV210_VIDW01ADD2, cfg);
		break;

	case 2:
		writel(S5PV210_VIDW02ADD2, cfg);
		break;

	case 3:
		writel(S5PV210_VIDW03ADD2, cfg);
		break;

	case 4:
		writel(S5PV210_VIDW04ADD2, cfg);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

static bool_t s5pv210fb_set_window_position(struct s5pv210fb_lcd * lcd, s32_t id)
{
	u32_t cfg, shw;

	shw = readl(S5PV210_SHADOWCON);
	shw |= S5PV210_SHADOWCON_PROTECT(id);
	writel(S5PV210_SHADOWCON, shw);

	switch(id)
	{
	case 0:
		cfg = S5PV210_VIDOSD_LEFT_X(0) | S5PV210_VIDOSD_TOP_Y(0);
		writel(S5PV210_VIDOSD0A, cfg);
		cfg = S5PV210_VIDOSD_RIGHT_X(lcd->width - 1) | S5PV210_VIDOSD_BOTTOM_Y(lcd->height - 1);
		writel(S5PV210_VIDOSD0B, cfg);
		break;

	case 1:
		cfg = S5PV210_VIDOSD_LEFT_X(0) | S5PV210_VIDOSD_TOP_Y(0);
		writel(S5PV210_VIDOSD1A, cfg);
		cfg = S5PV210_VIDOSD_RIGHT_X(lcd->width - 1) | S5PV210_VIDOSD_BOTTOM_Y(lcd->height - 1);
		writel(S5PV210_VIDOSD1B, cfg);
		break;

	case 2:
		cfg = S5PV210_VIDOSD_LEFT_X(0) | S5PV210_VIDOSD_TOP_Y(0);
		writel(S5PV210_VIDOSD2A, cfg);
		cfg = S5PV210_VIDOSD_RIGHT_X(lcd->width - 1) | S5PV210_VIDOSD_BOTTOM_Y(lcd->height - 1);
		writel(S5PV210_VIDOSD2B, cfg);
		break;

	case 3:
		cfg = S5PV210_VIDOSD_LEFT_X(0) | S5PV210_VIDOSD_TOP_Y(0);
		writel(S5PV210_VIDOSD3A, cfg);
		cfg = S5PV210_VIDOSD_RIGHT_X(lcd->width - 1) | S5PV210_VIDOSD_BOTTOM_Y(lcd->height - 1);
		writel(S5PV210_VIDOSD3B, cfg);
		break;

	case 4:
		cfg = S5PV210_VIDOSD_LEFT_X(0) | S5PV210_VIDOSD_TOP_Y(0);
		writel(S5PV210_VIDOSD4A, cfg);
		cfg = S5PV210_VIDOSD_RIGHT_X(lcd->width - 1) | S5PV210_VIDOSD_BOTTOM_Y(lcd->height - 1);
		writel(S5PV210_VIDOSD4B, cfg);
		break;

	default:
		break;
	}

	shw = readl(S5PV210_SHADOWCON);
	shw &= ~(S5PV210_SHADOWCON_PROTECT(id));
	writel(S5PV210_SHADOWCON, shw);

	return TRUE;
}

static bool_t s5pv210fb_set_window_size(struct s5pv210fb_lcd * lcd, s32_t id)
{
	u32_t cfg;

	if(id > 2)
		return FALSE;

	cfg = S5PV210_VIDOSD_SIZE(lcd->width * lcd->height);

	switch(id)
	{
	case 0:
		writel(S5PV210_VIDOSD0C, cfg);
		break;

	case 1:
		writel(S5PV210_VIDOSD1D, cfg);
		break;

	case 2:
		writel(S5PV210_VIDOSD2D, cfg);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

static bool_t s5pv210fb_window2_enable(struct s5pv210fb_lcd * lcd)
{
	u32_t cfg;

	/*
	 * window control
	 */
	cfg = readl(S5PV210_WINCON2);

	cfg &= ~(S5PV210_WINCON_BITSWP_ENABLE | S5PV210_WINCON_BYTESWP_ENABLE |
				S5PV210_WINCON_HAWSWP_ENABLE | S5PV210_WINCON_WSWP_ENABLE |
				S5PV210_WINCON_BURSTLEN_MASK | S5PV210_WINCON_BPPMODE_MASK |
				S5PV210_WINCON_INRGB_MASK | S5PV210_WINCON_DATAPATH_MASK |
				S5PV210_WINCON_ALPHA_SEL_MASK);

	cfg |= S5PV210_WINCON_ALPHA0_SEL;
	cfg |= S5PV210_WINCON_INRGB_RGB;
	cfg |= S5PV210_WINCON_DATAPATH_DMA;
	cfg |= S5PV210_WINCON_ENWIN_ENABLE;

	if(lcd->swap & S5PV210FB_SWAP_WORD)
		cfg |= S5PV210_WINCON_WSWP_ENABLE;
	else
		cfg |= S5PV210_WINCON_WSWP_DISABLE;

	if(lcd->swap & S5PV210FB_SWAP_HWORD)
		cfg |= S5PV210_WINCON_HAWSWP_ENABLE;
	else
		cfg |= S5PV210_WINCON_HAWSWP_DISABLE;

	if(lcd->swap & S5PV210FB_SWAP_BYTE)
		cfg |= S5PV210_WINCON_BYTESWP_ENABLE;
	else
		cfg |= S5PV210_WINCON_BYTESWP_DISABLE;

	if(lcd->swap & S5PV210FB_SWAP_BIT)
		cfg |= S5PV210_WINCON_BITSWP_ENABLE;
	else
		cfg |= S5PV210_WINCON_BITSWP_DISABLE;

	cfg |= (lcd->bpp_mode << S5PV210_WINCON_BPPMODE_SHIFT);

	writel(S5PV210_WINCON2, cfg);

	/*
	 * hardware version = 0x62
	 */
	cfg = readl(S5PV210_SHADOWCON);
	cfg |= 1 << 2;
	writel(S5PV210_SHADOWCON, cfg);

	return TRUE;
}

static void lcd_init(void)
{
	/*
	 * set gpd0_0 (backlight pwm pin) output and pull up and high level for disabled
	 */
	writel(S5PV210_GPD0CON, (readl(S5PV210_GPD0CON) & ~(0xf<<0)) | (0x1<<0));
	writel(S5PV210_GPD0PUD, (readl(S5PV210_GPD0PUD) & ~(0x3<<0)) | (0x2<<0));
	writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<0)) | (0x1<<0));

	/*
	 * set gpf3_5 (backlight enable pin) output and pull up and low level for disabled
	 */
	writel(S5PV210_GPF3CON, (readl(S5PV210_GPF3CON) & ~(0xf<<20)) | (0x1<<20));
	writel(S5PV210_GPF3PUD, (readl(S5PV210_GPF3PUD) & ~(0x3<<10)) | (0x2<<10));
	writel(S5PV210_GPF3DAT, (readl(S5PV210_GPF3DAT) & ~(0x1<<5)) | (0x0<<5));

	/*
	 * wait a moment
	 */
	mdelay(10);
}

static void lcd_backlight(u8_t brightness)
{
	if(brightness)
	{
		writel(S5PV210_GPF3DAT, (readl(S5PV210_GPF3DAT) & ~(0x1<<5)) | (0x1<<5));
		writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<0)) | (0x0<<0));
	}
	else
	{
		writel(S5PV210_GPF3DAT, (readl(S5PV210_GPF3DAT) & ~(0x1<<5)) | (0x0<<5));
		writel(S5PV210_GPD0DAT, (readl(S5PV210_GPD0DAT) & ~(0x1<<0)) | (0x1<<0));
	}
}

static void fb_init(struct fb_t * fb)
{
	struct s5pv210fb_lcd * lcd = (struct s5pv210fb_lcd *)(fb->lcd);

	/*
	 * initial lcd port
	 */
	writel(S5PV210_GPF0CON, 0x22222222);
	writel(S5PV210_GPF0DRV, 0xffffffff);
	writel(S5PV210_GPF0PUD, 0x0);
	writel(S5PV210_GPF1CON, 0x22222222);
	writel(S5PV210_GPF1DRV, 0xffffffff);
	writel(S5PV210_GPF1PUD, 0x0);
	writel(S5PV210_GPF2CON, 0x22222222);
	writel(S5PV210_GPF2DRV, 0xffffffff);
	writel(S5PV210_GPF2PUD, 0x0);
	writel(S5PV210_GPF3CON, (readl(S5PV210_GPF3CON) & ~(0xffff<<0)) | (0x2222<<0));
	writel(S5PV210_GPF3DRV, (readl(S5PV210_GPF3DRV) & ~(0xff<<0)) | (0xff<<0));
	writel(S5PV210_GPF3PUD, (readl(S5PV210_GPF3PUD) & ~(0xff<<0)) | (0x00<<0));

	/*
	 * lcd init function
	 */
	if(lcd->init)
		lcd->init();

	/*
	 * display path selection
	 */
	writel(S5PV210_DISPLAY_CONTROL, (readl(S5PV210_DISPLAY_CONTROL) & ~(0x3<<0)) | (0x2<<0));

	/*
	 * turn all windows off
	 */
	writel(S5PV210_WINCON0, (readl(S5PV210_WINCON0) & ~0x1));
	writel(S5PV210_WINCON1, (readl(S5PV210_WINCON1) & ~0x1));
	writel(S5PV210_WINCON2, (readl(S5PV210_WINCON2) & ~0x1));
	writel(S5PV210_WINCON3, (readl(S5PV210_WINCON3) & ~0x1));
	writel(S5PV210_WINCON4, (readl(S5PV210_WINCON4) & ~0x1));

	/*
	 * turn all windows color map off
	 */
	writel(S5PV210_WIN0MAP, (readl(S5PV210_WIN0MAP) & ~(1<<24)));
	writel(S5PV210_WIN1MAP, (readl(S5PV210_WIN1MAP) & ~(1<<24)));
	writel(S5PV210_WIN2MAP, (readl(S5PV210_WIN2MAP) & ~(1<<24)));
	writel(S5PV210_WIN3MAP, (readl(S5PV210_WIN3MAP) & ~(1<<24)));
	writel(S5PV210_WIN4MAP, (readl(S5PV210_WIN4MAP) & ~(1<<24)));

	/*
	 * turn all windows color key off
	 */
	writel(S5PV210_W1KEYCON0, (readl(S5PV210_W1KEYCON0) & ~(3<<25)));
	writel(S5PV210_W2KEYCON0, (readl(S5PV210_W2KEYCON0) & ~(3<<25)));
	writel(S5PV210_W3KEYCON0, (readl(S5PV210_W3KEYCON0) & ~(3<<25)));
	writel(S5PV210_W4KEYCON0, (readl(S5PV210_W4KEYCON0) & ~(3<<25)));

	/*
	 * initial lcd controller
	 */
	s5pv210fb_set_output(lcd);
	s5pv210fb_set_display_mode(lcd);
	s5pv210fb_display_off(lcd);
	s5pv210fb_set_polarity(lcd);
	s5pv210fb_set_timing(lcd);
	s5pv210fb_set_lcd_size(lcd);
	s5pv210fb_set_clock(lcd);

	/*
	 * set lcd video buffer
	 */
	s5pv210fb_set_buffer_address(lcd, 2);
	s5pv210fb_set_buffer_size(lcd, 2);
	s5pv210fb_set_window_position(lcd, 2);
	s5pv210fb_set_window_size(lcd, 2);

	/*
	 * enable window 2 for main display area
	 */
	s5pv210fb_window2_enable(lcd);

	/*
	 * display on
	 */
	s5pv210fb_display_on(lcd);

	/*
	 * wait a moment
	 */
	mdelay(100);
}

static void fb_swap(struct fb_t * fb)
{
	struct s5pv210fb_lcd * lcd = (struct s5pv210fb_lcd *)(fb->lcd);
	void * vram;

	vram = lcd->vram_front;
	lcd->vram_front = lcd->vram_back;
	lcd->vram_back = vram;

	fb->surface.pixels = lcd->vram_front;
}

static void fb_flush(struct fb_t * fb)
{
	struct s5pv210fb_lcd * lcd = (struct s5pv210fb_lcd *)(fb->lcd);

	s5pv210fb_set_buffer_address(lcd, 2);
}

static void fb_backlight(struct fb_t * fb, u8_t brightness)
{
	struct s5pv210fb_lcd * lcd = (struct s5pv210fb_lcd *)(fb->lcd);

	if(lcd->backlight)
		lcd->backlight(brightness);
}

/*
 * lcd module - EK070TN93
 */
static u8_t vram[2][1024 * 600 * 32 / 8] __attribute__((aligned(4)));

static struct s5pv210fb_lcd vs070cxn_lcd = {
	.width				= 1024,
	.height				= 600,
	.bits_per_pixel		= 32,
	.bytes_per_pixel	= 4,
	.freq				= 60,

	.output				= S5PV210FB_OUTPUT_RGB,
	.rgb_mode			= S5PV210FB_MODE_BGR_P,
	.bpp_mode			= S5PV210FB_BPP_MODE_32BPP,
	.swap				= S5PV210FB_SWAP_WORD,

	.rgba = {
		.r_mask			= 8,
		.r_field		= 0,
		.g_mask			= 8,
		.g_field		= 8,
		.b_mask			= 8,
		.b_field		= 16,
		.a_mask			= 8,
		.a_field		= 24,
	},

	.timing = {
		.h_fp			= 160,
		.h_bp			= 140,
		.h_sw			= 20,
		.v_fp			= 12,
		.v_fpe			= 1,
		.v_bp			= 20,
		.v_bpe			= 1,
		.v_sw			= 3,
	},

	.polarity = {
		.rise_vclk		= 0,
		.inv_hsync		= 1,
		.inv_vsync		= 1,
		.inv_vden		= 0,
	},

	.vram_front			= &vram[0][0],
	.vram_back			= &vram[1][0],

	.init				= lcd_init,
	.backlight			= lcd_backlight,
};

static struct fb_t s5pv210_fb;

void s5pv210_fb_initial(void)
{
	struct s5pv210fb_lcd * lcd = &vs070cxn_lcd;

	if(! clk_get_rate("dsys-hclk", 0))
		return;

	if( (lcd->bits_per_pixel != 16) && (lcd->bits_per_pixel != 24) && (lcd->bits_per_pixel != 32) )
		return;

	s5pv210_fb.lcd = lcd;
	s5pv210_fb.surface.info.bits_per_pixel = lcd->bits_per_pixel;
	s5pv210_fb.surface.info.bytes_per_pixel = lcd->bytes_per_pixel;
	s5pv210_fb.surface.info.red_mask_size = lcd->rgba.r_mask;
	s5pv210_fb.surface.info.red_field_pos = lcd->rgba.r_field;
	s5pv210_fb.surface.info.green_mask_size = lcd->rgba.g_mask;
	s5pv210_fb.surface.info.green_field_pos = lcd->rgba.g_field;
	s5pv210_fb.surface.info.blue_mask_size = lcd->rgba.b_mask;
	s5pv210_fb.surface.info.blue_field_pos = lcd->rgba.b_field;
	s5pv210_fb.surface.info.alpha_mask_size = lcd->rgba.a_mask;
	s5pv210_fb.surface.info.alpha_field_pos = lcd->rgba.a_field;
	s5pv210_fb.surface.info.fmt = get_pixel_format(&(s5pv210_fb.surface.info));

	s5pv210_fb.surface.w = lcd->width;
	s5pv210_fb.surface.h = lcd->height;
	s5pv210_fb.surface.pitch = lcd->width * lcd->bytes_per_pixel;
	s5pv210_fb.surface.flag = SURFACE_PIXELS_DONTFREE;
	s5pv210_fb.surface.pixels = lcd->vram_front;

	s5pv210_fb.surface.clip.x = 0;
	s5pv210_fb.surface.clip.y = 0;
	s5pv210_fb.surface.clip.w = lcd->width;
	s5pv210_fb.surface.clip.h = lcd->height;

	memset(&s5pv210_fb.surface.maps, 0, sizeof(struct surface_maps));
	surface_set_maps(&s5pv210_fb.surface.maps);

	fb_init(&s5pv210_fb);

	if(lcd->backlight)
		lcd->backlight(255);
}

struct surface_t * s5pv210_screen_surface(void)
{
	return &s5pv210_fb.surface;
}

void s5pv210_screen_swap(void)
{
	fb_swap(&s5pv210_fb);
}

void s5pv210_screen_flush(void)
{
	fb_flush(&s5pv210_fb);
}

void s5pv210_screen_backlight(u8_t brightness)
{
	fb_backlight(&s5pv210_fb, brightness);
}
