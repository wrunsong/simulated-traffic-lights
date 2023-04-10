#ifndef	__GUI__H
#define	__GUI__H

#include <types.h>

typedef unsigned char  uint8;     /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;      /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned int   uint16;    /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   int   int16;     /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned long  uint32;    /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   long  int32;     /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

typedef u32_t		   TCOLOR;
/* 定义LCM像素数宏 */
#define  GUI_LCM_XMAX	1024	//   定义液晶x轴的像素数 *
#define  GUI_LCM_YMAX	600	//   定义液晶y轴的像素数 *

/* 定义坐标数据结构 */
typedef  struct
         {  
            uint32  x;				// x坐标变量
            uint32  y;				// y坐标变量
         }
         PointXY;

/* 画圆函数 */
#define   GUI_CircleX_EN     1

void  GUI_HLine(unsigned int x0, unsigned int y0, unsigned int x1, TCOLOR color);
void  GUI_RLine(unsigned int x0, unsigned int y0, unsigned int y1, TCOLOR color);
void  GUI_Rectangle(uint16 x0, uint16 y0, uint16 x1, uint16 y1, TCOLOR color);
void  GUI_RectangleFill(uint16 x0, uint16 y0, uint16 x1, uint16 y1, TCOLOR color);
void  GUI_Square(uint32 x0, uint32 y0, uint32  with, TCOLOR  color);
void  GUI_Line(uint32 x0, uint32 y0, uint32 x1, uint32 y1, TCOLOR color);
void  GUI_LineS(uint32 const *points, uint16 no, TCOLOR color);
void  GUI_Circle(uint32 x0, uint32 y0, uint32 r, TCOLOR color);
void  GUI_CircleFill(uint32 x0, uint32 y0, uint32 r, TCOLOR color);

#endif
