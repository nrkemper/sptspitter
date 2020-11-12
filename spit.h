#ifndef __SPIT_H__
#define __SPIT_H__

#include <stdint.h>

#define VERSION                 0.1

#define BI_RGB                  0x0000
#define BI_RLE8                 0x0001
#define BI_RLE4                 0x0002
#define BI_BITFIELDS            0x0003
#define BI_JPEG                 0x0004
#define BI_PNG                  0x0005
#define BI_CMYK                 0x000B
#define CMYKRLE8                0x000C
#define CMYKRLE4                0x000D

#define SPRITEHEADERSIZE        23
#define MAXFILELENGTH           100

#define         false           0
#define         true            1

typedef         int8_t          bool;
typedef         int8_t          byte;
typedef         int8_t          boolean_t;

typedef struct bitmapfile_s
{
	uint16_t	type;
	uint32_t	size;
	uint16_t	pad1;
	uint16_t	pad2;
	uint32_t	offset;
} bitmapfile_t;

typedef struct ciexyztriple_s
{
	float		red;
	float		green;
	float		blue;
} ciexyztriple_t;

typedef struct bitmapinfo_s
{
        uint32_t	        size;
	int32_t		        width;
	int32_t		        height;
	uint16_t	        planes;
	uint16_t	        bpp;
	uint32_t	        compression;
	uint32_t	        imgsize;
	int32_t		        xpxlsmeter;
	int32_t		        ypxlsmeter;
	uint32_t	        clrused;
	uint32_t	        clrimportant;
	uint32_t	        redmask;
	uint32_t	        greenmask;
	uint32_t	        bluemask;
	uint32_t	        alphamask;
	uint32_t	        cstype;
	ciexyztriple_t	        endpoints;
	uint32_t	        gammared;
	uint32_t	        gammagreen;
	uint32_t	        gammablue;
} bitmapinfo_t;

typedef struct spritefileheader_s
{
        char    filetype[3];// must be SPT
        int     width, height;
        int     nocolours;
        int     offset;
        int     size;
} spritefileheader_t;

typedef struct palette_entry_s
{
        byte    r;
        byte    g;
        byte    b;
} palette_entry_t;

typedef struct colour_s
{
        byte    r;
        byte    g;
        byte    b;
} colour_t;

typedef struct palette_s
{
        int                     nocolours;
        int                     maxcolours;
        palette_entry_t         *data;
} palette_t;

//System Dependent Functions
byte *SYS_Open (const char *name);
void SYS_Close (void *addr);
bool SYS_FileExists (const char *fname);
bool SYS_WriteFile (const char *filename, byte *buf, int len);


//Sprite File Functions
bool SPT_GetHeaders (byte *buffer, bitmapfile_t *bmpfileret, bitmapinfo_t *bmpinforet);
byte* SPT_GetData (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buffer, spritefileheader_t *sptfilehret);
bool SPT_ParseCommandLine (int argc, char *argv[], char *desfileret, char *srcfileret);

//Palette Functions
void P_InitPalette (palette_t *palette, int nocolours);
bool P_ColourExists (palette_t *palette, colour_t *colour);
int P_GetIndex (palette_t *palette, colour_t *colour);
bool P_CopyPalette (const palette_t *src, palette_t *des);
void P_AddColour (palette_t *palette, colour_t *colour);
void P_RemoveColour (palette_t *palette, colour_t *colour);
void P_RemoveColourAtIndex (palette_t *pallete, int index);
void P_PrintPalette (palette_t *palette);
void P_DestroyPalette (palette_t *palette);

//Miscellaneous Functions
bool LittleEndian (void);
short ByteSwap2 (short n);
int ByteSwap4 (int n);
long ByteSwap8 (long n);
#endif
