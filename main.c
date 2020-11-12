//
//  main.c
//  spit-spitter
//
//  Created by Nicholas Kemp on 2020-10-08.
//  Copyright © 2020 Nicholas Kemp. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spit.h"

int main (int argc, char *argv[])
{
        char                    desfile[100], srcfile[100];
        byte                    *inbuf, *outbuf;
        bitmapfile_t            bmpfileheader;
        bitmapinfo_t            bmpinfoheader;
        spritefileheader_t      sptfileh;
        
        memset(desfile, 0, 100);
        memset(srcfile, 0, 100);
        
        SPT_ParseCommandLine(argc, argv, desfile, srcfile);
        
        if (*srcfile == '\0')
        {
                fprintf (stderr, "ERROR: did not supply source filename.\n");
                return -1;
        }
        
        if (*desfile == '\0')
        {
                if (strcmp (srcfile + strlen (srcfile) - 3, "bmp") == 0)
                {
                        strncpy(desfile, srcfile, strlen (srcfile) - 3);
                        strcat(desfile, "spt");
                } else
                        strcat(desfile, "spt");
        }
        
        if (!SYS_FileExists (srcfile))
        {
                fprintf (stderr, "ERROR: %s doesn't exist\n", srcfile);
                return -1;
        }

        if (SYS_FileExists(desfile))
        {
                char    c;
                
                printf ("File already exists. Overwrite file (Y/N):     ");
                
                while (1) {
                        
                       // while ((c = getchar () != '\n') && c != EOF); //flush input buffer
                        
                        c = getchar ();
                        
                        if (c == 'Y' || c == 'y')
                        {
                                remove(desfile);
                                break;
                        }
                        else if (c == 'N' || c == 'n')
                        {
                                printf ("OK. Exiting\n");
                                return 0;
                        }
                        else
                                fprintf (stderr, "ERROR: invalid input. Please enter either Y or N:     ");
                }
        }
        
        inbuf = SYS_Open (srcfile);
        if (!inbuf)
                return -1;
        
        SPT_GetHeaders(inbuf, &bmpfileheader, &bmpinfoheader);
        
        outbuf = SPT_GetData(&bmpfileheader, &bmpinfoheader, inbuf, &sptfileh);
        if (!outbuf)
        {
                SYS_Close(inbuf);
                return -1;
        }
        
        SYS_WriteFile(desfile, outbuf, sptfileh.size);
        SYS_Close(inbuf);
        free (outbuf);
        return 0;
}
