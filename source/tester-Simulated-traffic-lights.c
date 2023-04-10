#include <main.h>
#include <gui.h>
#include <graphic/color.h>
#include <font.h>
#include <stdlib.h>
#include <s5pv210/reg-wdg.h>
#include "nanojpeg.h"
#include "data.h"

/*left - 绿/红灯加一秒
 * down - 绿/红灯减一秒
 * up - 黄/红灯加一秒
 * right - 黄/红灯减一秒
 * back - 左/红转灯加一秒
 * menu - 左/红转灯减一秒
 */

//index_y/r/g/l 代表初始黄灯、红灯、绿灯以及左转灯的持续时长，分别为5s 18s 8s 5s 将他们设为全局变量
int index_y = 5;
int index_r = 18;
int index_g = 8;
int index_l = 5;
int index[2] = {8, 18};	//每个灯持续时间的中间变量，此处为初始化，代表一个从绿灯开始，一个从红灯开始
char status[2] = {'g', 'r'};//初始状态

void watchdog_born()	//看门狗诞生，设定倒计时参数对应的实际时间为1秒
{
	writel(S5PV210_WTCNT, 0x3f9c);		//看门狗计数寄存器，代表看门狗倒计时的次数
	writel(S5PV210_WTCON, 0xff20);		//写入1111_1111_0010_0000
}

void watchdog_dead()	//看门狗死亡，当计数器计到最后一位时，再次调用watchdog_born函数
{
	while(1)
	{
		if (readl(S5PV210_WTCNT) <= 0x0001)
		{
			break;
		}
	}
	watchdog_born();
}

void watchdog_dead_500()//看门狗中间态，标志看门狗计时器计时到0.5s的时刻
{
	while(1)
	{
		if (readl(S5PV210_WTCNT) <= 0x1fce)
		{
			break;
		}
	}
}

int key_control_seconds(u32_t keyup, u32_t keydown)	//按键控制交通灯持续时长的函数，红灯的持续时长必须等于绿灯+黄灯+左转灯
{
	enum {
		KEY_NAME_POWER	= 0x1 << 0,
		KEY_NAME_LEFT	= 0x1 << 1,
		KEY_NAME_DOWN	= 0x1 << 2,
		KEY_NAME_UP		= 0x1 << 3,
		KEY_NAME_RIGHT	= 0x1 << 4,
		KEY_NAME_BACK	= 0x1 << 5,
		KEY_NAME_MENU	= 0x1 << 6,
		};
if (get_key_event(&keyup, &keydown))
	{if (keydown != 0)		//如果按下按键
	{
		if (keydown & KEY_NAME_LEFT)	//按下LEFT
		{
			if (index_g < 99 && index_r < 99)	//未达到99s就绿灯和红灯同时加1
			{
				index_g = index_g + 1;
				index_r = index_r + 1;
				return 0;
			}
		}
		if (keydown & KEY_NAME_DOWN)	//按下DOWN
		{
			if (index_g > 5 && index_r > 5)	//未达到5s就绿灯和红灯同时减1
			{
				index_g = index_g - 1;
				index_r = index_r - 1;
				return 0;
			}
		}
		if (keydown & KEY_NAME_UP)		//按下UP
		{
			if (index_y < 99 && index_r < 99)	//未达到99s就黄灯和红灯同时加1
			{
				index_y = index_y + 1;
				index_r = index_r + 1;
				return 1;
			}
		}
		if (keydown & KEY_NAME_RIGHT)		//按下RIGHT
		{
			if (index_y > 5 && index_r > 5)		//未达到5s就黄灯和红灯同时减1
			{
				index_y = index_y - 1;
				index_r = index_r - 1;
				return 1;
			}
		}
		if (keydown & KEY_NAME_BACK)		//按下BACK
		{
			if (index_r < 99 && index_l < 99)	//未达到99s就左转灯和红灯同时加1
			{
				index_l = index_l + 1;
				index_r = index_r + 1;
				return -1;
			}
		}
		if (keydown & KEY_NAME_MENU)		//按下MENU
		{
			if ((index_l > 5) && (index_r > 5))		//未达到5s就左转和红灯同时减1
			{
				index_l = index_l - 1;
				index_r = index_r - 1;
				return -1;
			}
		}
	}
	}
    }

void renew_text(u32_t c1, u32_t c2, u32_t c3)	//更新LCD屏幕的文字
{	u32_t bc_w = 0x000000;	//text的背景是黑色
	if (index_l > 1)
	{
		lcd_print(10, 516, c1, bc_w, "The duration of the left-turn light is");
		lcd_print(10, 532, c1, bc_w, "%02d seconds", index_l);
	}
	else
	{
		lcd_print(10, 516, c1, bc_w, "The duration of the left-turn light is");
		lcd_print(10, 532, c1, bc_w, "%02d second ", index_l);
	}	//打印出现在的左转灯的持续时长


	if (index_g > 1)
	{
		lcd_print(10, 548, c1, bc_w, "The duration of the green light is %02d seconds", index_g);
	}
	else
	{
		lcd_print(10, 548, c1, bc_w, "The duration of the green light is %02d second ", index_g);
	}	//打印出现在的绿灯的持续时长

	if (index_y > 1)
	{
		lcd_print(10, 564, c2, bc_w, "The duration of the yellow light is %02d seconds", index_y);
	}
	else
	{
		lcd_print(10, 564, c2, bc_w, "The duration of the yellow light is %02d second ", index_y);
	}	//打印出现在的黄灯的持续时长

	if (index_r > 1)
	{
		lcd_print(10, 580, c3, bc_w, "The duration of the red light is %02d seconds", index_r);
	}
	else
	{
		lcd_print(10, 580, c3, bc_w, "The duration of the red light is %02d second ", index_r);
	}	//打印出现在的红灯的持续时长

	}

void traffic_light_initial(u32_t *x, u32_t *y, u32_t r, u32_t *x_b, u32_t *y_b, u32_t c_b) //x y r是交通灯的坐标 x_b y_b是交通灯背景的坐标 c_b是背景的颜色
{   //initial
    GUI_RectangleFill(x_b[0], y_b[0], x_b[1], y_b[1], c_b);		//background
	GUI_CircleFill(x[0], y[0], r, 0x000000);
	GUI_CircleFill(x[1], y[1], r, 0x000000);
	GUI_CircleFill(x[2], y[2], r, 0x000000);
	GUI_CircleFill(x[3], y[3], r, 0x000000);	//灯的位置在不亮的时候为黑色
}

void traffic_light(u32_t **px, u32_t **py, u32_t *c, u32_t r, u32_t bc_w, int order, u32_t keydown, u32_t keyup)
{	//该函数的含义：根据输入参数选定一组交通灯一直保持红灯，然后令另一组交通灯按照绿灯-黄灯-左转的顺序变化。

	//以下两行的含义：输入order为0时，用2*order和2*order+1 代表下、上灯的序号；同时用order_sub_2和order_sub_2+1 代表左、右灯的序号
	//总之，order_sub_2和order_sub_2+1代表的是在本轮中一直红灯的两个交通灯，2*order和2*order+1代表在本轮中“从绿灯变为黄灯变为左转”的交通灯
	int order_sub_1 = abs(1 - order);
    int order_sub_2 = 2 * order_sub_1;

    //倒计时的位置
    int position[4][2] = {0};
    //本轮中交通灯的持续时长
    int index_for_this_time[4];
    index_for_this_time[0] = index_g; index_for_this_time[1] = index_y; index_for_this_time[2] = index_r; index_for_this_time[3] = index_l;

    index[order] = index_for_this_time[0];	//先令order的index值等于绿灯的持续时长
    status[order] = 'g';	//令order的status值为g
    //倒计时的显示位置
    if (order == 0)
    {
        position[0][0] = 504; position[0][1] = 409;
        position[1][0] = 367; position[1][1] = 292;
        position[2][0] = 504; position[2][1] = 180;
        position[3][0] = 643; position[3][1] = 292;

    }
    else
    {
        position[0][0] = 367; position[0][1] = 292;
        position[1][0] = 504; position[1][1] = 409;
        position[2][0] = 643; position[2][1] = 292;
        position[3][0] = 504; position[3][1] = 180;

    }

    while(index[order_sub_1] > 0)	//一直持续红灯的交通灯构成大循环
    {	watchdog_born();
        GUI_CircleFill(px[order_sub_2][0], py[order_sub_2][0], r, c[0]);
        GUI_CircleFill(px[order_sub_2+1][0], py[order_sub_2+1][0], r, c[0]);    //非order序号红灯亮
        //倒计时读秒
        lcd_print(position[1][0], position[1][1], c[0], bc_w, "%d", index[order_sub_1]/10);		//十位
        lcd_print(position[1][0]+8, position[1][1], c[0], bc_w, "%d", index[order_sub_1]-(index[order_sub_1]/10)*10);		//个位
        lcd_print(position[3][0], position[3][1], c[0], bc_w, "%d", index[order_sub_1]/10);		//十位
        lcd_print(position[3][0]+8, position[3][1], c[0], bc_w, "%d", index[order_sub_1]-(index[order_sub_1]/10)*10);		//个位


        if (index[order] > 0 && status[order] == 'g') //如果order对应的灯应该亮绿色
        {
            GUI_CircleFill(px[order*2][2], py[order*2][2], r, c[2]);
            GUI_CircleFill(px[order*2+1][2], py[order*2+1][2], r, c[2]);    //order序号绿灯亮
            lcd_print(position[0][0], position[0][1], c[2], bc_w, "%d", index[order]/10);		//十位
            lcd_print(position[0][0]+8, position[0][1], c[2], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
            lcd_print(position[2][0], position[2][1], c[2], bc_w, "%d", index[order]/10);		//十位
            lcd_print(position[2][0]+8, position[2][1], c[2], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
            key_control_seconds(keyup, keydown);//键控持续时长，新时长会在下一轮中生效
            renew_text(c[2], c[1], c[0]);	//更新持续时长的文本
            //mdelay(1000);	//延时
            watchdog_dead();
            index[order]--;	//自减
            move_straight(order, index[order], index_for_this_time[0]);
        }
        else
        {
            if (status[order] == 'g')	//当绿色计时结束
                {
                    GUI_CircleFill(px[order*2][2], py[order*2][2], r, 0x000000);	//绿灯时间结束后，将绿灯的位置用黑色覆盖
                    GUI_CircleFill(px[order*2+1][2], py[order*2+1][2], r, 0x000000);	//绿灯时间结束后，将绿灯的位置用黑色覆盖
                    index[order] = index_for_this_time[1];	//改变index为黄色的持续时间
                    status[order] = 'y';	//将order状态改为yellow
                }
            if (index[order] > 0 && status[order] == 'y')//如果order对应的灯应该亮黄色
            {
                GUI_CircleFill(px[order*2][1], py[order*2][1], r, c[1]);
                GUI_CircleFill(px[order*2+1][1], py[order*2+1][1], r, c[1]);    //order对应的黄灯亮
                lcd_print(position[0][0], position[0][1], c[1], bc_w, "%d", index[order]/10);		//十位
                lcd_print(position[0][0]+8, position[0][1], c[1], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
                lcd_print(position[2][0], position[2][1], c[1], bc_w, "%d", index[order]/10);		//十位
                lcd_print(position[2][0]+8, position[2][1], c[1], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
                key_control_seconds(keyup, keydown);//键控持续时长
                renew_text(c[2], c[1], c[0]);//更新持续时长的文本
                watchdog_dead_500();
                //mdelay(500);//延时
                GUI_CircleFill(px[order*2][1], py[order*2][1], r, 0x000000);	//黄灯闪烁
                GUI_CircleFill(px[order*2+1][1], py[order*2+1][1], r, 0x000000);	//黄灯闪烁
                key_control_seconds(keyup, keydown);//键控持续时长
                renew_text(c[2], c[1], c[0]);//更新持续时长的文本
                watchdog_dead();
                //mdelay(500);//延时
                index[order]--;//自减
            }
            else
            {
                if (status[order] == 'y')	//当黄色计时结束
                {
                    index[order] = index_for_this_time[3];	//改变index为左转灯的持续时间
                    status[order] = 'l';					//将order状态改为left
                }
                if (index[order] > 0 && status[order] == 'l')//如果order对应的灯应该亮左转灯（需要注意的是，该方向左转灯亮时，直行灯为红灯）
                {
                GUI_CircleFill(px[order*2][0], py[order*2][0], r, c[0]);
                GUI_CircleFill(px[order*2+1][0], py[order*2+1][0], r, c[0]);    //直行红灯亮
                left_light(px[order*2][3], py[order*2][3], c[2]);
                left_light(px[order*2+1][3], py[order*2+1][3], c[2]);	//左转亮
                //显示左转灯读秒
                lcd_print(position[0][0], position[0][1], c[2], bc_w, "%d", index[order]/10);		//十位
                lcd_print(position[0][0]+8, position[0][1], c[2], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
                lcd_print(position[2][0], position[2][1], c[2], bc_w, "%d", index[order]/10);		//十位
                lcd_print(position[2][0]+8, position[2][1], c[2], bc_w, "%d", index[order]-(index[order]/10)*10);		//个位
                key_control_seconds(keyup, keydown);//键控持续时长
                renew_text(c[2], c[1], c[0]);//更新持续时长的文本
                watchdog_dead();
                //mdelay(1000);//延时
                index[order]--;//自减
                turn_left(order, index[order], index_for_this_time[3]);
            	if (status[order] == 'l' && index[order] == 0)//当左转灯计时结束
            	{
            		index[order] = index_r;		//改变index为红灯的持续时间，注意，这里用的是index_r而非index_for_this_time，这是在为下一轮循环做准备
            		status[order] = 'r';					//将order状态改为red
            	    left_light(px[order*2][3], py[order*2][3], c[0]);
            	    left_light(px[order*2+1][3], py[order*2+1][3], c[0]);	//左转灭
            	}
                }
            }
        }
        index[order_sub_1]--;	//持续红灯的交通灯秒数自减
      }
    GUI_CircleFill(px[order_sub_2][0], py[order_sub_2][0], r, 0x000000);	//绿灯时间结束后，将红灯的位置用黑色覆盖
    GUI_CircleFill(px[order_sub_2+1][0], py[order_sub_2+1][0], r, 0x000000);	//绿灯时间结束后，将红灯的位置用黑色覆盖

}

void left_light(u32_t x, u32_t y, u32_t c)	//该函数用于绘制左转灯符号
{
    GUI_RectangleFill(x+6, y-5, x+5+8, y+13, c);//最右边的竖矩形
    GUI_RectangleFill(x+6, y-5, x-5, y+3, c);	//横矩形
    //箭头
    u32_t x_temp = x-5-9;
    u32_t y1_temp = y+1;
    u32_t y2_temp = y-1;
    while(x_temp <= x-5)
    {
    	GUI_RLine(x_temp, y2_temp, y1_temp, c);
    	x_temp ++;
    	y1_temp ++;
    	y2_temp --;
    }
    //以上参数均通过多次实验得出
}

void display_car(const void* file, const int size, int x, int y)
{
	unsigned char *img, *imga, *p;
	int w, h;
	struct surface_t * screen;
	struct surface_t * obj;
    struct rect_t dstRect, srcRect;
    int i,j;

	njInit();
	njDecode(file, 2339);

	img = njGetImage();
	w = njGetWidth();
	h = njGetHeight();

	// Convert RGB to RGBA
	imga = malloc(w * h * 4);

	p = imga;
	for(i = 0; i < w * h; i++)
	{
		for(j = 0; j < 3; j++)
		{
			*p++ = *img++;
		}
		*p++ = 0xff;
	}

	screen = s5pv210_screen_surface();

	obj = surface_alloc(imga, w, h, PIXEL_FORMAT_ABGR_8888);

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = w;
    srcRect.h = h;
    dstRect.w = w;
    dstRect.h = h;
    dstRect.x = x;
    dstRect.y = y;
    surface_blit(screen, &dstRect, obj, &srcRect, BLEND_MODE_REPLACE);

}

void turn_left(int order, int index, int index_this_time)
{
	if (order == 0)
	{
		if (index == index_this_time/2)
		{
			GUI_RectangleFill(520, 467, 617, 600, 0xD3D3D3);
			GUI_RectangleFill(407, 0, 504, 132, 0xD3D3D3);
			display_car(file1, 1587, 530, 220);
			display_car(file2, 1683, 420, 330);
		}
		if (index == 1)
		{
			GUI_RectangleFill(410, 200, 610, 400, 0xD3D3D3);
			display_car(file1, 1587, 230, 230);
			display_car(file2, 1683, 710, 330);
		}
		if (index == 0)
		{
			GUI_RectangleFill(0, 195, 345-25, 292, 0xD3D3D3);
			GUI_RectangleFill(680+25, 308, 1024, 405, 0xD3D3D3);
		    display_car(file1, 1587, 530, 470);
		    display_car(file2, 1683, 420, 90);
		}

	}
	else
	{
		if (index == index_this_time/2)
		{
			GUI_RectangleFill(0, 405, 345-25, 308, 0xD3D3D3);
			GUI_RectangleFill(680+25, 195, 1024, 292, 0xD3D3D3);
			display_car(file3, 2113, 520, 330);
			display_car(file4, 2166, 384, 230);
		}
		if (index == 1)
		{
			GUI_RectangleFill(353+30, 200, 672-30, 400, 0xD3D3D3);
			display_car(file3, 2113, 520, 90);
			display_car(file4, 2166, 384, 470);
		}
		if (index == 0)
		{
			GUI_RectangleFill(520, 0, 672-30, 132, 0xD3D3D3);
			GUI_RectangleFill(353+30, 467, 504, 600, 0xD3D3D3);
		    display_car(file3, 2113, 190, 330);
		    display_car(file4, 2166, 710, 230);

		}
	}
}

void move_straight(int order, int index, int index_this_time)
{
	if (order == 0)
	{
		if (index == index_this_time/2)
		{
			GUI_RectangleFill(520, 467, 617, 600, 0xD3D3D3);
			GUI_RectangleFill(407, 0, 504, 132, 0xD3D3D3);
			display_car(file1, 1587, 530, 280);
			display_car(file2, 1683, 420, 280);
		}
		if (index == 2)
		{
			GUI_RectangleFill(410, 200, 610, 400, 0xD3D3D3);
			display_car(file1, 1587, 530, 90);
			display_car(file2, 1683, 420, 470);
		}
		if (index == 0)
		{
			GUI_RectangleFill(520, 0, 617, 132, 0xD3D3D3);
			GUI_RectangleFill(407, 467, 504, 600, 0xD3D3D3);
		    display_car(file1, 1587, 530, 470);
		    display_car(file2, 1683, 420, 90);
		}
	}
	else
	{
		if (index == index_this_time/2)
		{
			GUI_RectangleFill(0, 405, 345-25, 308, 0xD3D3D3);
			GUI_RectangleFill(680+25, 195, 1024, 292, 0xD3D3D3);
			display_car(file3, 2113, 450, 330);
			display_car(file4, 2166, 450, 230);
		}
		if (index == 2)
		{
			GUI_RectangleFill(410, 200, 610, 400, 0xD3D3D3);
			display_car(file3, 2113, 710, 330);
			display_car(file4, 2166, 190, 230);
		}
		if (index == 0)
		{
			GUI_RectangleFill(0, 195, 345-25, 292, 0xD3D3D3);
			GUI_RectangleFill(680+25, 308, 1024, 405, 0xD3D3D3);
		    display_car(file3, 2113, 190, 330);
		    display_car(file4, 2166, 710, 230);
		}
	}
}

int tester_Simulated_traffic_lights(int argc, char * argv[])	//主函数
{	s5pv210_screen_swap();
	s5pv210_screen_flush();
	surface_fill(s5pv210_screen_surface(), 0, 0, BLEND_MODE_REPLACE);

	//分别代表红、黄、绿、左转4个圆形的坐标、颜色、半径
	u32_t x[4][4], y[4][4], c[3], r;  //4个交通灯，每个有4个灯

	u32_t keyup, keydown;	//键控变量

	//初始化道路
	GUI_RectangleFill(0, 405, 1024, 195, 0xD3D3D3);//宽210 长1024
	GUI_RectangleFill(407-25, 0, 617+25, 600, 0xD3D3D3);//宽210 长600
	int count;
	for(count=0;count<7;count++)
	{
		GUI_LineWith(count*50, 300, count*50+40, 300, 10, 0x4DFFFF);//横黄虚线
		GUI_LineWith(1024-count*50, 300, 1024-count*50-40, 300, 10, 0x4DFFFF);//横黄虚线
	}
	for (count=0;count<6;count++)
	{
		GUI_LineWith(512, count*30, 512, count*30+25, 10, 0x4DFFFF);//竖黄虚线
		GUI_LineWith(512, 600-count*30, 512, 600-count*30-25, 10, 0x4DFFFF);//竖黄虚线
	}

	//初始化
	lcd_print(0, 0, 0xFFFFFF, 0x000000, "Attention : the duration of the red light must");
	lcd_print(0, 16, 0xFFFFFF, 0x000000, "be equal to the sum of the green, yellow");
	lcd_print(0, 32, 0xFFFFFF, 0x000000, "and left turn duration.");

    display_car(file1, 1587, 530, 470);
    display_car(file2, 1683, 420, 90);
    display_car(file3, 2113, 190, 330);
    display_car(file4, 2166, 710, 230);

	//交通灯的矩形背景的坐标及颜色
	u32_t x_b[4][2], y_b[4][2], c_b;
	//字符的坐标及字符的背景颜色
	u32_t bc_w;

	//定义两个指针，便于传递交通灯坐标构成的二维数组
    u32_t *px[4]; u32_t *py[4];

    r = 20;
    c[0] = 0x0000FF;//红
    c[1] = 0x00FFFF;//黄
    c[2] = 0x00FF00;//绿
    x[0][0] = 437; y[0][0] = 435;
    x[0][1] = 487; y[0][1] = 435;
    x[0][2] = 537; y[0][2] = 435;
    x[0][3] = 587; y[0][3] = 435;	//下侧交通灯坐标

    x[1][0] = 437; y[1][0] = 165;
    x[1][1] = 487; y[1][1] = 165;
    x[1][2] = 537; y[1][2] = 165;
    x[1][3] = 587; y[1][3] = 165;	//上侧交通灯坐标

    x[2][0] = 377-25; y[2][0] = 225;
    x[2][1] = 377-25; y[2][1] = 275;
    x[2][2] = 377-25; y[2][2] = 325;
    x[2][3] = 377-25; y[2][3] = 375;   //左侧交通灯坐标

    x[3][0] = 647+25; y[3][0] = 225;
    x[3][1] = 647+25; y[3][1] = 275;
    x[3][2] = 647+25; y[3][2] = 325;
    x[3][3] = 647+25; y[3][3] = 375;   //右侧交通灯坐标

    x_b[0][0] = 412; x_b[0][1] = 612; y_b[0][0] = 405; y_b[0][1] = 465;     //下侧交通灯背景
    x_b[1][0] = 412; x_b[1][1] = 612; y_b[1][0] = 135; y_b[1][1] = 195;     //上侧交通灯背景
    x_b[2][0] = 347-25; x_b[2][1] = 407-25; y_b[2][0] = 200; y_b[2][1] = 400;     //左侧交通灯背景
    x_b[3][0] = 617+25; x_b[3][1] = 677+25; y_b[3][0] = 200; y_b[3][1] = 400;     //右侧交通灯背景
    c_b = 0x080808;//背景颜色
	bc_w = 0x080808;//倒计时的字符背景

    int i;
    for(i=0;i<4;i++)	//对4个交通灯初始化
    {
        traffic_light_initial(x[i], y[i], r, x_b[i], y_b[i], c_b);
        left_light(x[i][3], y[i][3], c[0]);
        px[i] = x[i];
        py[i] = y[i];
    }
//index_r = index_g + index_y + index_l

    int order= 0;
    while (1)
	{
        order = 0;//第一种情况
        traffic_light(px, py, c, r, bc_w, order, keydown, keyup);    //order为0代表是左右从绿灯开始，order为1代表上下从绿灯开始
        order = 1;//第二种情况
        traffic_light(px, py, c, r, bc_w, order, keydown, keyup);
	}
	return 0;
}
