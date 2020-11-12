#include "spit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

bool    flag_verbose    = false;


#if defined (__linux__) || defined (__APPLE__)

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

size_t          length_of_file;

byte *SYS_Open (const char* name)
{
        int             fd, ret;
        struct stat     sd;
        byte*           buf;
        
        fd = open (name, O_RDONLY);
        if (fd == -1)
        {
                fprintf (stderr, "ERROR: unable to open file '%s'\n", name);
                return (byte *)0;
        }
        
        ret = fstat (fd, &sd);
        if (ret == -1)
        {
                fprintf (stderr, "ERROR: unable to fstat file '%s'\n", name);
                return (byte *)0;
        }
        
        length_of_file = sd.st_size;
        
        buf = mmap (NULL, sd.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (buf == NULL)
        {
                fprintf (stderr, "ERROR: failed to map file. Exiting.\n");
                return (byte *)0;
        }
        
        return buf;
}

void SYS_Close (void* addr)
{
        if (!addr)
                return;
        
        munmap (addr, length_of_file);
}

bool SYS_FileExists (const char *fname)
{
        struct stat     s;
        
        if (stat (fname, &s) == 0)
                return true;
        
        return false;
}

bool SYS_WriteFile (const char *filename, byte *buf, int len)
{
        FILE    *fp;
        
        if (!buf)
                return false;
        
        if (!len)
                return false;
        
        fp = fopen (filename, "w");
        if (fp == NULL)
        {
                fprintf (stderr, "ERROR: unable to open write file %s.\n", filename);
                return false;
        }
        
        fwrite (buf, 1, len, fp);
        fclose (fp);
        
        return true;
}

#elif defined (_WIN32) || defined (_WIN64)
   
BREAK! WINDOWS NOT SUPPORTED

byte *SYS_Open (const char *name)
{
        return NULL;
}

void SYS_Close (void *addr)
{
        
}

bool SYS_FileExists (const char *fname)
{
        return false;
}

bool SYS_WriteFile (const char *filename, byte *buf, int len)
{
        return false;
}
#else
        BREAK! OTHER OSSSS NOT SUPPORTED
#endif

/*
 ==============
 SPT_GetHeaders
 ==============
 */
bool SPT_GetHeaders (byte *buf, bitmapfile_t *bmpfileret, bitmapinfo_t *bmpinforet)
{
        short   stamp;
        
        if (!buf)
                return false;
        
        if (!bmpfileret)
                return false;
        
        if (!bmpinforet)
                return false;
        
        stamp = *(short *)buf;
        
        if (!LittleEndian())
                stamp = ByteSwap2(stamp);
        
        
        //FILE HEADER
        bmpfileret->type = *(uint16_t *)buf;
        bmpfileret->size = *(uint32_t *)(buf + 2);
        bmpfileret->pad1 = *(uint16_t *)(buf + 6);
        bmpfileret->pad2 = *(uint16_t *)(buf + 8);
        bmpfileret->offset = *(uint32_t *)(buf + 10);
        
        //BMP CORE HEADER
        bmpinforet->size = *(uint32_t *)(buf + 14);
        bmpinforet->width = *(int32_t *)(buf + 18);
        bmpinforet->height = *(int32_t *)(buf + 22);
        bmpinforet->planes = *(uint16_t *)(buf + 26);
        bmpinforet->bpp = *(uint16_t *)(buf + 28);
        
        if (bmpinforet->size == 12)
                return true;
        
        //INFO HEADER
        bmpinforet->compression = *(uint32_t *)(buf + 30);
        bmpinforet->imgsize = *(uint32_t *)(buf + 34);
        bmpinforet->xpxlsmeter = *(int32_t *)(buf + 38);
        bmpinforet->ypxlsmeter = *(int32_t *)(buf + 42);
        bmpinforet->clrused = *(uint32_t *)(buf + 46);
        bmpinforet->clrimportant = *(uint32_t *)(buf + 50);
        
        if (bmpinforet->size == 40)
                return true;
        
        //BMPV4HEADER
        bmpinforet->redmask = *(uint32_t *)(buf + 54);
        bmpinforet->greenmask = *(uint32_t *)(buf + 58);
        bmpinforet->bluemask = *(uint32_t *)(buf + 62);
        bmpinforet->alphamask = *(uint32_t *)(buf + 66);
        bmpinforet->cstype = *(uint32_t *)(buf + 70);
        bmpinforet->endpoints.red = *(int32_t *)(buf + 74);
        bmpinforet->endpoints.green = *(int32_t *)(buf + 78);
        bmpinforet->endpoints.blue = *(int32_t *)(buf + 82);
        bmpinforet->gammared = *(uint32_t *)(buf + 86);
        bmpinforet->gammagreen = *(uint32_t *)(buf + 90);
        bmpinforet->gammablue = *(uint32_t *)(buf + 94);
        
        return true;
}
/*
 =====================
 
 =====================
 */
byte *GetData1BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);
byte *GetData4BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);
byte *GetData8BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);
byte *GetData16BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);
byte *GetData24BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);
byte *GetData32BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret);

byte *GetData1BitPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        
        return NULL;
}

byte *GetData4BitsPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        
        return NULL;
}

byte *GetData8BitsPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        
        return NULL;
}

byte *GetData16BitsPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        
        //BI_RGB
        return NULL;
}

byte *GetData24BitsPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        byte            *ret, *bmpdata;
        palette_t       palette;
        colour_t        colour;
        int             offset, nocolours, size, rowsize, i, x;
        
        if (!bmpfile || !bmpinfo || !buf)
                return (byte *)0;
   
        size = bmpinfo->width * bmpinfo->height * 3;
        
        bmpdata = (byte *) malloc(size);
        if (!bmpdata)
        {
                fprintf (stderr, "ERROR: unable to allocate output buffer\n");
                return (byte *)0;
        }
        
        P_InitPalette(&palette, 0);
        
        nocolours = bmpinfo->clrused;
        if (nocolours)
        { //extract colour table
                offset = 14 + bmpinfo->size;
                for (i=0; i<nocolours * 3; i+=3)
                {
                        colour.b = *(buf + offset + i);
                        colour.g = *(buf + offset + i + 1);
                        colour.r = *(buf + offset + i + 2);
                        
                        P_AddColour (&palette, &colour);
                }
        }
        
        offset = bmpfile->offset;
        rowsize = ceil (((bmpinfo->bpp * bmpinfo->width) / 32) * 4); //size of bmp row with padding
        
        switch (bmpinfo->compression)
        {
                case BI_RGB:
                        if (bmpinfo->height > 0)
                        {       //bottom-up bitmap
                                int     x, y, a, b;
                                
                                for (y=(bmpinfo->height - 1) * rowsize, a=0; y>=0; y-= rowsize, a+=(bmpinfo->width * 3))
                                { // get data
                                        for (x=0, b=0; x<bmpinfo->width * 3; x+=3, b+=3)
                                        {
                                                *(bmpdata + a + b + 2) = colour.b = *(buf + offset + y + x);
                                                *(bmpdata + a + b + 1) = colour.g = *(buf + offset + y + x + 1);
                                                *(bmpdata + a + b) = colour.r = *(buf + offset + y + x + 2);

                                                if (!bmpinfo->clrused)
                                                { //if colour table doesn't already exist
                                                        if (!P_ColourExists (&palette, &colour))
                                                        {//and if the colour doesn't exist
                                                                P_AddColour (&palette, &colour); //add colour
                                                                nocolours++;
                                                        }
                                                }
                                        }
                                }
                        } else { //top-down bitmap
                                int x, y, a, b;
                                
                                for (y=0, a=0; y<bmpinfo->height; y+=rowsize, a+=(bmpinfo->width * 3))
                                {
                                        for (x=0, b=0; x<bmpinfo->width * 3; x+=3, b+=3)
                                        {
                                                *(bmpdata + a + b + 2) = colour.b = *(buf + offset + y + x);
                                                *(bmpdata + a + b + 1) = colour.g = *(buf + offset + y + x + 1);
                                                *(bmpdata + a + b) = colour.r = *(buf + offset + y + x + 2);
                                
                                                if (bmpinfo->clrused)
                                                { //if colour table doesn't already exist
                                                        if (!P_ColourExists (&palette, &colour))
                                                        { //and if the colour isn't in the table
                                                                P_AddColour (&palette, &colour); // add the colour
                                                                nocolours++;
                                                        }
                                                }
                                        }
                                }
                        }
                        break;
                        
                case BI_BITFIELDS:
                        fprintf (stderr, "ERROR: BI_BITFIELDS only valid for 16 or 32 bpp\n");
                        return NULL;
                        break;
                        
                case BI_RLE8:
                case BI_RLE4:
                case BI_JPEG:
                case BI_PNG:
                case BI_CMYK:
                case CMYKRLE8:
                case CMYKRLE4:
                        fprintf (stderr, "ERROR: unsupported compression type\n");//FIXME: implement decompression
                        return NULL;
                        break;
                        
                default:
                        fprintf (stderr, "ERROR: invalid compression type\n");
                        return NULL;
                        break;
        }
        
        //put it all together
        //allocate memory
        //add header info
        //add colour table
        //add bitmap info (r, g, b)
        size += SPRITEHEADERSIZE + (nocolours * 3);
        ret = (byte *)malloc (size);
        if (!ret)
        {
                fprintf (stderr, "ERROR: unable to allocate memory for output file data\n");
                free (bmpdata);
                return (byte *)0;
        }
        
        //add header info
        *ret                    = 'S';
        *(ret + 1)              = 'P';
        *(ret + 2)              = 'T';
        *(int *)(ret + 3)       = LittleEndian() ? bmpinfo->width : ByteSwap4(bmpinfo->width);
        *(int *)(ret + 7)       = LittleEndian() ? bmpinfo->height : ByteSwap4(bmpinfo->height);
        *(int *)(ret + 11)      = LittleEndian() ? nocolours : ByteSwap4(nocolours);
        *(int *)(ret + 15)      = LittleEndian() ? SPRITEHEADERSIZE + (nocolours * 3) :
                                                        ByteSwap4 (SPRITEHEADERSIZE + (nocolours * 3));
        
        //add colour table
        offset = SPRITEHEADERSIZE;
        
        for (i=0, x=0; x<nocolours; i+=3, x++)
        {
                *(ret + offset + i)     = palette.data[x].r;
                *(ret + offset + i + 1) = palette.data[x].g;
                *(ret + offset + i + 2) = palette.data[x].b;
        }
        
        //add bitmap info
        offset += nocolours * 3;
        
        for (i=0; i<bmpinfo->width * bmpinfo->height * 3; i+=3)
        {
                *(ret + offset + i) = *(bmpdata + i);           //r
                *(ret + offset + i + 1) = *(bmpdata + i + 1);   //g
                *(ret + offset + i + 2) = *(bmpdata + i + 2);   //b
        }
        
        *(int *)(ret + 19) = LittleEndian() ? size : ByteSwap4(size);
        
        //fill the sprite file header
        sptfilehret->filetype[0] = 'S';
        sptfilehret->filetype[1] = 'P';
        sptfilehret->filetype[2] = 'T';
        sptfilehret->width = bmpinfo->width;
        sptfilehret->height = bmpinfo->height;
        sptfilehret->nocolours = nocolours;
        sptfilehret->offset = SPRITEHEADERSIZE + (nocolours * 3);
        sptfilehret->size = size;
        
        free (bmpdata);
        
        return ret;
}

byte *GetData32BitsPerPixel (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        
        return NULL;
}

byte *SPT_GetData (bitmapfile_t *bmpfile, bitmapinfo_t *bmpinfo, byte *buf, spritefileheader_t *sptfilehret)
{
        byte                    *ret=(byte *)0;
        
        if (!bmpfile)
        {
                fprintf (stderr, "ERROR: null bitmap file header\n");
                return NULL;
        }
        
        if (!bmpinfo)
        {
                fprintf (stderr, "ERROR: null bitmap info header\n");
                return NULL;
        }
        
        if (!buf)
        {
                fprintf (stderr, "ERROR: null buffer\n");
                return NULL;
        }
        
        switch (bmpinfo->bpp)
        {
                case 32:
                        ret = GetData32BitsPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
                        
                case 24:
                        ret = GetData24BitsPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
                        
                case 16:
                        ret = GetData16BitsPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
                        
                case 8:
                        ret = GetData8BitsPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
                        
                case 4:
                        ret = GetData4BitsPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
                        
                case 1:
                        ret = GetData1BitPerPixel(bmpfile, bmpinfo, buf, sptfilehret);
                        break;
        }
        
        return ret;
}

bool SPT_ParseCommandLine (int argc, char *argv[], char *desfileret, char *srcfileret)
{
        int     i;
        char    *ptr;
        
        if (!desfileret)
                return false;
        
        if (!srcfileret)
                return false;
        
        for (i=1; i<argc; )
        {
                ptr = argv[i];

                switch (*ptr)
                {
                        case '-':
                                for (ptr++, i++; *ptr; ptr++)
                                {
                                        switch (*ptr)
                                        {
                                                case 'f':
                                                case 'F':
                                                        if (*srcfileret == '\0')
                                                                strncpy (srcfileret, argv[i], MAXFILELENGTH);
                                                        else
                                                                fprintf (stderr, "ERROR: Source filename already supplied. %s will be ignored.\n", argv[i]);
                                                        
                                                        i++;
                                                        break;
                                                        
                                                
                                                case 'o':
                                                case 'O':
                                                        
                                                        if (*desfileret == '\0')
                                                                strncpy (desfileret, argv[i], MAXFILELENGTH);
                                                        else
                                                                fprintf (stderr, "ERROR: destination filename already supplied\n");
                                                        
                                                        i++;
                                                        break;
                                                        
                                                case 'v':
                                                case 'V':
                                                        flag_verbose = true;
                                                        break;
                                        }
                                }
                                break;
                                
                        default:
                                if (*srcfileret == '\0')
                                {
                                        strncpy(srcfileret, argv[i], MAXFILELENGTH);
                                        i++;
                                }
                                else if (*srcfileret != '\0' && *desfileret == '\0')
                                {
                                        strncpy(desfileret, argv[i], MAXFILELENGTH);
                                        i++;
                                }
                                else if (*srcfileret != '\0' && *desfileret != '\0')
                                {
                                        fprintf (stderr, "ERROR: already supplied file name. '%s' will be ignored\n", ptr);
                                        i++;
                                }
                                break;
                }
        }
        
        return 1;
}

/*
 
 Palette Functions
 
 */

#define PALETTE_INCREASE_SIZE           20

void P_InitPalette (palette_t *palette, int nocolours)
{
        palette->nocolours = 0;
        palette->maxcolours = nocolours;
        palette->data = malloc (sizeof (palette_t) * nocolours);
}

bool P_ColourExists (palette_t *pallete, colour_t *colour)
{
        int     i;
        
        for (i=0; i<pallete->nocolours; i++)
        {
                if (colour->r == pallete->data[i].r &&
                    colour->g == pallete->data[i].g &&
                    colour->b == pallete->data[i].b)
                        return true;
        }
        
        return false;
}

int P_GetIndex (palette_t *palette, colour_t *colour)
{
        int     i;
        
        if (!palette || !colour)
                return -1;
        
        for (i=0; i<palette->nocolours; i++)
        {
                if (palette->data[i].r == colour->r &&
                    palette->data[i].g == colour->g &&
                    palette->data[i].b == colour->b)
                        return i;
        }
        
        return -1;
}

bool P_CopyPalette (const palette_t *src, palette_t *des)
{
        int     i;
        
        if (!src || !des)
                return false;
        
        if (!src->maxcolours) //uninitialized src palette
                return false;
        
        if (!des->maxcolours) // uninitialized des palette
                P_InitPalette (des, src->maxcolours);
        
        if (src->nocolours > des->maxcolours)
        {
                P_DestroyPalette (des);
                P_InitPalette(des, src->maxcolours);
        }
        
        for (i=0; i<src->nocolours; i++)
        {
                des->data[i].r = src->data[i].r;
                des->data[i].g = src->data[i].g;
                des->data[i].b = src->data[i].b;
        }
        
        des->nocolours = src->nocolours;
        des->maxcolours = src->maxcolours;
        
        return true;
}

void P_AddColour (palette_t *palette, colour_t *colour)
{
        palette_t       tmp;
        int             index;
        
        if (!palette || !colour)
                return;
        
        if (palette->nocolours >= palette->maxcolours)
        {
                int     newmax = palette->maxcolours + PALETTE_INCREASE_SIZE;
                
                P_InitPalette (&tmp, newmax);
                P_CopyPalette (palette, &tmp);
                P_DestroyPalette (palette);
                P_InitPalette (palette, newmax);
                P_CopyPalette (&tmp, palette);
                P_DestroyPalette (&tmp);
        }
        
        index = palette->nocolours;
        palette->data[index].r = colour->r;
        palette->data[index].g = colour->g;
        palette->data[index].b = colour->b;
        palette->nocolours++;
}

void P_RemoveColour (palette_t *palette, colour_t *colour)
{
        if (P_ColourExists(palette, colour))
        {
                int     index;
                
                index = P_GetIndex (palette, colour);
                P_RemoveColourAtIndex (palette, index);
        }
}

void P_RemoveColourAtIndex (palette_t *palette, int index)
{
        int     i;
        
        if (!palette)
                return;
        
        if (index > palette->nocolours)
                return;
        
        for (i=index; i<palette->nocolours - 1; i++)
        {
                palette->data[i].r = palette->data[i + 1].r;
                palette->data[i].g = palette->data[i + 1].g;
                palette->data[i].b = palette->data[i + 1].b;
        }
        
        palette->nocolours--;
}

void P_DestroyPalette (palette_t *palette)
{
        if (!palette)
                return;
        
        if (palette->data)
                free (palette->data);
        
        memset (palette, 0, sizeof (palette_t));
}

void P_PrintPalette (palette_t *palette)
{
        int     i;
        
        for (i=0; i<palette->nocolours; i++)
        {
                printf ("---------------------------\n");
                printf ("Index:                 %d\n", i);
                printf ("       Red:            %d\n", palette->data[i].r);
                printf ("       Green:          %d\n", palette->data[i].g);
                printf ("       Blue:           %d\n", palette->data[i].b);
                printf ("---------------------------\n\n");
        }
}

/*
        Miscellaneous Functions
 */
bool LittleEndian (void)
{
        long            x=1;
        char            *y = (char *)&x;
        
        if (*y)
                return true;
        
        return false;
}

short ByteSwap2 (short n)
{
        return (n & 0xff00) >> 8 || (n & 0x00ff) << 8;
}

int ByteSwap4 (int n)
{
        return (n & 0xff000000 >> 24) || (n & 0x00ff0000) >> 8 || (n & 0x0000ff00) << 8 || (n & 0x000000ff) << 24;
}

long ByteSwap8 (long n)
{
        long            data;
        
        ((byte *)&data)[0] = ((byte *)&n)[7];
        ((byte *)&data)[1] = ((byte *)&n)[6];
        ((byte *)&data)[2] = ((byte *)&n)[5];
        ((byte *)&data)[3] = ((byte *)&n)[4];
        ((byte *)&data)[4] = ((byte *)&n)[3];
        ((byte *)&data)[5] = ((byte *)&n)[2];
        ((byte *)&data)[6] = ((byte *)&n)[1];
        ((byte *)&data)[7] = ((byte *)&n)[0];
        
        return data;
}
