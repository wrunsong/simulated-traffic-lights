#include <main.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

#include "nanojpeg.h"
//#include "car1.cin" // skin-1.jpg


int tester_framebuffer_jpeg(int argc, char * argv[])
{
	unsigned char *img, *imga, *p;
	int w, h;
	struct surface_t * screen;
	u32_t c;
	struct surface_t * obj;
    struct rect_t dstRect, srcRect;
    u64_t t1, t2;
    int i, j;

    const unsigned char file[] = {
    0xff,0xd8,0xff,0xe0,0x00,0x10,0x4a,0x46,0x49,0x46,0x00,0x01,0x01,0x01,0x00,0x78,
    0x00,0x78,0x00,0x00,0xff,0xe1,0x00,0x22,0x45,0x78,0x69,0x66,0x00,0x00,0x4d,0x4d,
    0x00,0x2a,0x00,0x00,0x00,0x08,0x00,0x01,0x01,0x12,0x00,0x03,0x00,0x00,0x00,0x01,
    0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xdb,0x00,0x43,0x00,0x06,0x04,0x05,
    0x06,0x05,0x04,0x06,0x06,0x05,0x06,0x07,0x07,0x06,0x08,0x0a,0x10,0x0a,0x0a,0x09,
    0x09,0x0a,0x14,0x0e,0x0f,0x0c,0x10,0x17,0x14,0x18,0x18,0x17,0x14,0x16,0x16,0x1a,
    0x1d,0x25,0x1f,0x1a,0x1b,0x23,0x1c,0x16,0x16,0x20,0x2c,0x20,0x23,0x26,0x27,0x29,
    0x2a,0x29,0x19,0x1f,0x2d,0x30,0x2d,0x28,0x30,0x25,0x28,0x29,0x28,0xff,0xdb,0x00,
    0x43,0x01,0x07,0x07,0x07,0x0a,0x08,0x0a,0x13,0x0a,0x0a,0x13,0x28,0x1a,0x16,0x1a,
    0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,
    0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,
    0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,
    0x28,0x28,0xff,0xc0,0x00,0x11,0x08,0x00,0x50,0x00,0x78,0x03,0x01,0x22,0x00,0x02,
    0x11,0x01,0x03,0x11,0x01,0xff,0xc4,0x00,0x1f,0x00,0x00,0x01,0x05,0x01,0x01,0x01,
    0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,
    0x06,0x07,0x08,0x09,0x0a,0x0b,0xff,0xc4,0x00,0xb5,0x10,0x00,0x02,0x01,0x03,0x03,
    0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7d,0x01,0x02,0x03,0x00,0x04,
    0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,
    0x91,0xa1,0x08,0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,
    0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,0x29,0x2a,0x34,0x35,0x36,
    0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,
    0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,
    0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x92,0x93,0x94,0x95,
    0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,
    0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,
    0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
    0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xff,0xc4,0x00,
    0x1f,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
    0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0xff,0xc4,
    0x00,0xb5,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,
    0x01,0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,
    0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xa1,0xb1,0xc1,0x09,0x23,
    0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,
    0x19,0x1a,0x26,0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,
    0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,
    0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,
    0x85,0x86,0x87,0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,
    0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,
    0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
    0xd8,0xd9,0xda,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,
    0xf6,0xf7,0xf8,0xf9,0xfa,0xff,0xda,0x00,0x0c,0x03,0x01,0x00,0x02,0x11,0x03,0x11,
    0x00,0x3f,0x00,0xfa,0x76,0x8a,0x28,0xa0,0x02,0x8a,0x28,0xa0,0x02,0x8a,0x28,0xa0,
    0x02,0x8a,0x28,0xa0,0x02,0x8a,0x28,0xa0,0x02,0x8a,0x29,0xb1,0xb6,0xf4,0x0d,0x8c,
    0x67,0x91,0xf4,0xa0,0x07,0x51,0x45,0x14,0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,
    0x00,0x51,0x45,0x14,0x00,0x51,0x4c,0x96,0x54,0x89,0x77,0x48,0xd8,0x1d,0x07,0xa9,
    0x3e,0x83,0xd6,0xbc,0xe7,0xc7,0x5f,0x14,0x6c,0xb4,0x33,0x25,0xa6,0x9c,0xab,0x77,
    0xa8,0x0e,0x0a,0x86,0xf9,0x63,0x3f,0xed,0x11,0xdf,0xd8,0x73,0xea,0x45,0x44,0xea,
    0x46,0x1b,0x9a,0xd2,0xa3,0x3a,0xae,0xd1,0x47,0xa2,0x5d,0x5c,0xc1,0x69,0x03,0xcd,
    0x75,0x34,0x70,0xc2,0x83,0x2c,0xf2,0x30,0x55,0x03,0xdc,0x9a,0xe5,0x25,0xf8,0x85,
    0xa4,0x3c,0x8d,0x1e,0x95,0x06,0xa5,0xab,0xb2,0x9c,0x1f,0xb0,0x5a,0xb4,0x8a,0x0f,
    0xfb,0xc7,0x03,0xf2,0xac,0x7f,0x09,0x78,0x7d,0x7c,0x4d,0xa7,0xd9,0xeb,0xde,0x28,
    0xb9,0x7d,0x52,0x59,0xd7,0xcd,0x8a,0xd5,0xc6,0xdb,0x78,0x3d,0x82,0x74,0x63,0xee,
    0x7f,0xfa,0xf5,0xdf,0xc5,0x1a,0x43,0x1a,0xc7,0x12,0x2a,0x46,0xa3,0x0a,0xaa,0x30,
    0x00,0xf6,0x15,0x8b,0xa9,0x37,0xb6,0x86,0xdc,0x94,0x69,0xe8,0xfd,0xe7,0xf7,0x2f,
    0xf3,0x7f,0x81,0xca,0x8f,0x19,0xea,0x0d,0xcc,0x7e,0x11,0xd7,0x0a,0xff,0x00,0xb4,
    0xa8,0xa7,0xf2,0xdd,0x50,0xdd,0x7c,0x42,0x5b,0x07,0x81,0x75,0x3f,0x0e,0x6b,0xb6,
    0xc6,0x79,0x04,0x51,0x7e,0xe5,0x1f,0x7b,0x9e,0x8a,0x30,0xdc,0x9f,0x6a,0xec,0xeb,
    0xcf,0xbe,0x36,0x3b,0xdb,0xf8,0x62,0xc6,0xfa,0x1f,0xf5,0xb6,0x7a,0x84,0x53,0xa9,
    0xf4,0x20,0x37,0xf5,0x22,0x85,0x29,0xdf,0x70,0x73,0xa5,0x6f,0x83,0xf1,0x66,0xab,
    0x7c,0x43,0xf0,0xfe,0xe5,0x82,0xfa,0x5b,0xad,0x3a,0x47,0x38,0x2b,0x79,0x6b,0x24,
    0x7c,0x77,0xe7,0x18,0xfd,0x6b,0xa8,0xd3,0x75,0x1b,0x2d,0x4e,0x0f,0x3b,0x4e,0xbb,
    0x82,0xea,0x2e,0x9b,0xe1,0x90,0x38,0x1f,0x95,0x43,0xb6,0xdb,0x53,0xb0,0x43,0x2c,
    0x51,0xcf,0x6d,0x3a,0x06,0xdb,0x22,0x86,0x56,0x04,0x64,0x64,0x1a,0xf1,0xdf,0x1c,
    0xf8,0x72,0xfb,0xc0,0x7a,0x92,0xf8,0x8f,0xc2,0x52,0x3c,0x16,0x45,0x80,0x9a,0x11,
    0xca,0xc6,0x49,0xe8,0x47,0x74,0x3e,0xfd,0x0f,0xe1,0x52,0xeb,0x4e,0x1a,0xcb,0x54,
    0x6b,0x4b,0x0f,0x47,0x10,0xf9,0x22,0xdc,0x65,0xd2,0xfa,0xa7,0xf9,0x5b,0xf1,0x3d,
    0xca,0x8a,0xe5,0xfc,0x01,0xe3,0x0b,0x4f,0x17,0x69,0x7e,0x74,0x58,0x8a,0xf6,0x2c,
    0x0b,0x8b,0x7c,0xe4,0xa1,0xf5,0x1e,0xaa,0x7b,0x1a,0xea,0x2b,0xa6,0x32,0x52,0x5c,
    0xc8,0xe2,0xa9,0x4e,0x54,0xa4,0xe1,0x35,0x66,0x82,0x8a,0x28,0xaa,0x20,0x28,0xa2,
    0xb9,0xff,0x00,0x1d,0x5f,0xcf,0x63,0xe1,0xe9,0x52,0xc2,0x55,0x8b,0x51,0xbc,0x74,
    0xb4,0xb5,0x2c,0xdb,0x7f,0x78,0xe7,0x1c,0x1e,0xc4,0x0c,0x9f,0xc2,0x94,0xa4,0xa2,
    0xae,0xcb,0xa7,0x07,0x39,0x28,0xae,0xa5,0x2f,0x1d,0xf8,0xda,0xd7,0xc2,0x56,0x2b,
    0x34,0x96,0x93,0xdd,0x3b,0xbf,0x96,0xbb,0x3e,0x54,0x0d,0x82,0x70,0x5c,0xfd,0x0f,
    0x40,0x6b,0xc8,0x35,0x6f,0x8c,0x7e,0x22,0xbb,0x66,0x16,0x3f,0x65,0xb1,0x8f,0xb7,
    0x97,0x17,0x98,0xdf,0x89,0x6c,0x8f,0xd0,0x56,0x27,0xc4,0x9d,0x73,0x53,0xd4,0x75,
    0xb7,0xb2,0xd4,0x56,0x38,0x62,0xd3,0xc9,0x86,0x3b,0x68,0xa4,0xde,0xa8,0x47,0x04,
    0xee,0xfe,0x22,0x7d,0x4f,0xe9,0x5e,0x6f,0x77,0xe2,0x18,0xd2,0xe3,0xc8,0xb3,0x82,
    0x4b,0xa9,0xba,0x6d,0x8c,0x7f,0x2e,0xb9,0xfc,0xab,0xcc,0x9d,0x6a,0xb5,0x64,0xd5,
    0x33,0xe9,0x28,0x61,0x30,0xb8,0x5a,0x4a,0x78,0x8b,0x5d,0xf7,0xd4,0xef,0xae,0xbc,
    0x75,0xe2,0x6b,0xbf,0x33,0xcf,0xd5,0xee,0x88,0x91,0x4a,0x36,0xdc,0x29,0x23,0xd3,
    0x80,0x31,0xf8,0x57,0x36,0xcc,0x72,0x38,0x3c,0x9a,0xc2,0xb3,0xf1,0x24,0x52,0x4f,
    0xe4,0xdd,0xc4,0xf6,0xd2,0x03,0x82,0x1f,0xa0,0x3e,0xfd,0x31,0xf9,0x56,0xf6,0x72,
    0x54,0xd7,0x3c,0xfd,0xa4,0x1d,0xe5,0xb9,0xe8,0x51,0x78,0x6a,0xf0,0x6a,0x9a,0x4d,
    0x7a,0x7e,0x87,0x43,0x63,0xe3,0x5f,0x11,0x58,0x58,0xc5,0x67,0x67,0xa9,0xdc,0x43,
    0x6d,0x12,0xed,0x44,0x50,0xa3,0x68,0xfc,0xa9,0xed,0xe3,0x9f,0x13,0xb7,0x5d,0x66,
    0xff,0x00,0xf0,0x93,0x1f,0xca,0xb9,0xda,0x2b,0x3e,0x79,0x77,0x37,0x58,0x6a,0x4b,
    0x68,0xaf,0xb9,0x1b,0x8d,0xe3,0x2f,0x12,0x37,0x5d,0x6f,0x54,0xfc,0x2e,0x58,0x7f,
    0x5a,0x6d,0xc6,0xa1,0xe2,0x1d,0x5a,0xd1,0x12,0xea,0xf7,0x50,0xbb,0xb6,0x95,0xf6,
    0xaa,0x4d,0x74,0x59,0x4b,0x0f,0x62,0xd5,0x8b,0x45,0x35,0x37,0xd4,0x25,0x42,0x36,
    0xf7,0x12,0x5f,0x2f,0xf8,0x63,0x4f,0xfb,0x7b,0x5c,0xb7,0xcd,0xbf,0xf6,0xa6,0xa4,
    0x82,0x13,0xe5,0x84,0x17,0x4f,0x85,0xc7,0x18,0x18,0x38,0xc0,0xa8,0xa7,0xd6,0xf5,
    0x5b,0x88,0x9a,0x3b,0x8d,0x46,0xf6,0x58,0xd8,0x61,0x91,0xee,0x19,0x81,0x1e,0xe0,
    0x9a,0xcf,0x5e,0xad,0xf5,0xaa,0xba,0x96,0xa3,0x6f,0xa7,0x45,0xbe,0xe5,0xf1,0x9f,
    0xba,0xa3,0x92,0x68,0x5c,0xd2,0x76,0x41,0x25,0x4e,0x9c,0x79,0xa5,0x64,0x91,0xd0,
    0x78,0x63,0x5e,0xbd,0xf0,0xe6,0xb3,0x06,0xa3,0x60,0x71,0x24,0x67,0x0c,0x84,0xfc,
    0xb2,0x2f,0x75,0x3e,0xc7,0xff,0x00,0xaf,0x5f,0x56,0xf8,0x77,0x58,0xb6,0xd7,0xb4,
    0x7b,0x6d,0x46,0xc9,0xb3,0x0c,0xe9,0xbb,0x07,0xaa,0x9e,0xea,0x7d,0xc1,0xc8,0xaf,
    0x87,0xff,0x00,0xe1,0x24,0x8d,0x55,0x24,0x9e,0xce,0xea,0x28,0x1c,0xfc,0xb2,0xb2,
    0xf0,0x6b,0xdb,0xff,0x00,0x67,0x8f,0x10,0x33,0x6b,0x73,0x69,0xd1,0xcc,0x64,0xb2,
    0xba,0x85,0xa5,0x45,0xcf,0x0b,0x22,0xe3,0x9f,0x6e,0x33,0x9f,0xa0,0xae,0xdc,0x33,
    0x9d,0x19,0x28,0xcd,0x68,0xcf,0x17,0x31,0x8d,0x0c,0x65,0x37,0x56,0x8c,0xaf,0x28,
    0xfe,0x47,0xd0,0x54,0x51,0x45,0x7a,0x47,0xcd,0x85,0x79,0xaf,0xc7,0xd1,0x07,0xfc,
    0x21,0x51,0x34,0xca,0xc6,0x55,0xbb,0x4f,0x28,0x8c,0xe1,0x5b,0x0d,0x9c,0xfe,0x19,
    0xeb,0x5e,0x95,0x4d,0x96,0x34,0x96,0x36,0x49,0x51,0x5d,0x18,0x60,0xab,0x0c,0x83,
    0xf8,0x54,0x54,0x87,0x3c,0x5c,0x7b,0x9b,0x61,0xea,0xfb,0x1a,0xb1,0xa9,0xd8,0xf8,
    0x6f,0xc4,0x57,0x0d,0x16,0x97,0x33,0x46,0xdf,0x31,0x18,0xce,0x7f,0xcf,0x6c,0xd7,
    0xaa,0xfc,0x2a,0xf0,0x95,0xb3,0x69,0x6d,0xa5,0x58,0xea,0x56,0x12,0x45,0xa8,0xe9,
    0xf1,0xbd,0xe8,0x44,0x22,0xe6,0x39,0x4e,0xd7,0x07,0x71,0x5e,0x55,0x58,0xe3,0x00,
    0xe3,0x1d,0xb2,0x73,0x5e,0xcb,0xad,0x7c,0x3b,0xf0,0xae,0xaf,0x0c,0xe9,0x73,0xa3,
    0x5a,0x23,0x4b,0x1b,0xc7,0xbe,0x24,0xd8,0x57,0x72,0x95,0x2c,0x00,0xe3,0x20,0x13,
    0x83,0x8e,0x0f,0x35,0xe2,0x31,0xcb,0xae,0x7c,0x38,0xbb,0xd3,0xb4,0x9d,0x7a,0xc6,
    0xe2,0x0d,0x3e,0xce,0xe1,0x4b,0x6b,0x36,0x16,0xa6,0x45,0xbf,0xb7,0x52,0x36,0xa3,
    0xb0,0xe5,0x48,0x50,0x46,0xdf,0xcf,0xa6,0x4e,0x14,0xa8,0xba,0x51,0xb1,0xd5,0x8f,
    0xc6,0x2c,0x55,0x45,0x34,0xac,0xac,0x45,0xf1,0xa3,0xc3,0x76,0xc6,0xce,0xea,0x1b,
    0xbd,0x42,0xc1,0x26,0xd2,0xac,0x13,0xec,0x11,0x6c,0x26,0x79,0x02,0x01,0xbb,0x7b,
    0x05,0xc6,0x08,0x0f,0xc1,0x3e,0x98,0x15,0xe6,0x7e,0x1c,0x9c,0xc9,0xa6,0xa0,0x90,
    0x93,0xe5,0xbb,0x47,0x92,0x7b,0x0e,0x9f,0xa1,0x1f,0x95,0x7a,0x85,0x9d,0x8e,0xbb,
    0xf1,0x3a,0x46,0xd2,0x6d,0xec,0xae,0x3f,0xb2,0x1e,0xe4,0xbc,0xda,0xed,0xed,0xb1,
    0x89,0xe3,0xb7,0xce,0x4c,0x51,0x13,0xcb,0x13,0xc7,0x1d,0xb9,0x1d,0x0e,0x6b,0xdb,
    0x1b,0xe1,0xaf,0x84,0x0e,0xdd,0xba,0x15,0xac,0x61,0x57,0x6e,0x22,0x2d,0x18,0x3e,
    0xe4,0x29,0x00,0x9f,0x73,0xc9,0xa2,0xb5,0x17,0x56,0x16,0x42,0xc0,0x62,0xe3,0x85,
    0xaa,0xe7,0x2d,0xac,0x7c,0xb3,0xe6,0x27,0xf7,0x85,0x1e,0x62,0x7a,0xd7,0xd5,0x0b,
    0xf0,0xe3,0xc2,0x43,0xa6,0x8d,0x0f,0xe3,0x23,0x9f,0xfd,0x9a,0x9c,0x3e,0x1d,0xf8,
    0x50,0x7f,0xcc,0x16,0xdf,0xfe,0xfa,0x6f,0xf1,0xae,0x5f,0xa8,0xcf,0xba,0x3d,0xaf,
    0xed,0xda,0x3f,0xca,0xff,0x00,0x0f,0xf3,0x3e,0x55,0xf3,0x17,0xd6,0x8f,0x30,0x1f,
    0xba,0x0b,0x1a,0xfa,0xb3,0xfe,0x15,0xf7,0x85,0x07,0xfc,0xc1,0x2d,0x7f,0xf1,0xef,
    0xf1,0xa5,0xff,0x00,0x84,0x03,0xc2,0xa3,0xfe,0x60,0x96,0x9f,0x91,0xff,0x00,0x1a,
    0x3e,0xa3,0x3e,0xe8,0x5f,0xdb,0xb4,0xbf,0x95,0xfe,0x1f,0xe6,0x7c,0xa8,0xa3,0x03,
    0x9e,0xbd,0xea,0x2f,0x05,0x69,0xb6,0xda,0xbe,0xbf,0xac,0x5f,0xdf,0x5f,0xd9,0xda,
    0xcd,0xa7,0x18,0xd2,0xd5,0x6f,0x10,0xb4,0x6d,0x23,0x16,0x20,0xe3,0x04,0x70,0xa8,
    0x7a,0x8c,0x64,0xd7,0xd6,0x3f,0xf0,0x80,0xf8,0x5b,0xfe,0x80,0x96,0x7f,0xf7,0xc9,
    0xff,0x00,0x1a,0xf3,0x4f,0x88,0x1e,0x02,0x9f,0xc2,0xda,0xf2,0x78,0x9b,0xc1,0xba,
    0x14,0x1a,0x8d,0x8b,0x42,0x62,0xbd,0xd2,0xd6,0x2f,0x31,0x95,0xc6,0x4a,0x5c,0x46,
    0x87,0xef,0x30,0x27,0x95,0xff,0x00,0x12,0x46,0xf4,0x30,0xd2,0xa4,0xdc,0x99,0xe7,
    0xe6,0x39,0x94,0x31,0x50,0x50,0x82,0x6a,0xc5,0xff,0x00,0x14,0x78,0x6a,0x4b,0x6d,
    0x17,0x54,0xd4,0xe4,0xbc,0xb1,0x8a,0x5d,0x51,0x21,0x4b,0xc3,0x72,0xbb,0xe0,0x8d,
    0x46,0x03,0xed,0x1b,0x4e,0x77,0x36,0x31,0xc7,0x03,0x81,0x5c,0x1f,0xc0,0x9d,0x1d,
    0x34,0xdf,0x8d,0x9a,0xa5,0x8e,0x97,0x70,0x97,0x3a,0x65,0x9d,0xb1,0xb9,0x59,0x54,
    0x30,0x1b,0x64,0x44,0xda,0x06,0x79,0xe3,0xcc,0xc7,0x3c,0xf1,0x4b,0x1f,0x8e,0xaf,
    0xae,0xed,0xf4,0x9b,0x4b,0x38,0x35,0xad,0x6b,0x52,0x57,0x94,0x5e,0x69,0xb2,0xd8,
    0xb9,0x13,0x06,0x61,0xb5,0x1f,0x23,0x00,0x01,0x9c,0x9e,0x76,0xf5,0x19,0xc5,0x7a,
    0xb7,0xc1,0xbf,0x02,0xcf,0xe1,0x9b,0x4b,0xcd,0x5b,0x5c,0x8e,0x14,0xf1,0x06,0xa8,
    0xfb,0xe7,0x8e,0x13,0xf2,0x5b,0xc7,0xc6,0xc8,0x57,0xb1,0xda,0x00,0xe7,0x9f,0x4c,
    0x9c,0x64,0xf4,0x46,0x37,0xdc,0xf2,0x94,0xdc,0x75,0x89,0xe9,0x14,0x51,0x45,0x6a,
    0x40,0x51,0x45,0x14,0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,
    0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,0x00,0x51,0x45,0x14,
    0x01,0xff,0xd9
    };


    serial_printf(2, "Decode jpeg file\n");
	njInit();
	t1 = clock_gettime();
	if (njDecode(file, 2339)) {
		serial_printf(2, "Error decoding the input file.\n");
		goto Error;
	}
	t2 = clock_gettime();

	img = njGetImage();
	w = njGetWidth();
	h = njGetHeight();
	serial_printf(2, "Time = %ld\r\n", t2 - t1);
	serial_printf(2, "Image width = %d, height = %d, color = %d\r\n", w, h, njIsColor());

	// Convert RGB to RGBA
	imga = malloc(w * h * 4);
	if (imga == NULL) {
		serial_printf(2, "Could not alloc image buffer.\r\n");
		goto Error;
	}
	p = imga;
	for(i = 0; i < w * h; i++)
	{
		for(j = 0; j < 3; j++)
		{
			*p++ = *img++;
		}
		*p++ = 0xff;
	}

	serial_printf(2, "Draw image\r\n");
	screen = s5pv210_screen_surface();


	c = surface_map_color(screen, get_named_color("springgreen"));

	obj = surface_alloc(imga, w, h, PIXEL_FORMAT_ABGR_8888);
	if(obj == NULL) {
		serial_printf(2, "Could not alloc surface.\r\n");
		goto Error;
	}

	s5pv210_screen_swap();
	//surface_fill(screen, &screen->clip, c, BLEND_MODE_REPLACE);
    /* setup rects for drawing */
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = w;
    srcRect.h = h;
    dstRect.w = w;
    dstRect.h = h;
    dstRect.x = 0;
    dstRect.y = 0;
    surface_blit(screen, &dstRect, obj, &srcRect, BLEND_MODE_REPLACE);
	s5pv210_screen_flush();
    while(1)
    	;

Error:
	njDone();

	return 0;
}
