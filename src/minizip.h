/*
 * minizip.h
 *
 *  Created on: 10 nov. 2011
 *      Author: Martijn
 */

#ifndef MINIZIP_H_
#define MINIZIP_H_

#ifndef _WIN32
        #ifndef __USE_FILE_OFFSET64
                #define __USE_FILE_OFFSET64
        #endif
        #ifndef __USE_LARGEFILE64
                #define __USE_LARGEFILE64
        #endif
        #ifndef _LARGEFILE64_SOURCE
                #define _LARGEFILE64_SOURCE
        #endif
        #ifndef _FILE_OFFSET_BIT
                #define _FILE_OFFSET_BIT 64
        #endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef unix
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
# include <direct.h>
# include <io.h>
#endif

#include "zip.h"

#ifdef _WIN32
        #define USEWIN32IOAPI
        #include "iowin32.h"
#endif



#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

uLong filetime(char *f, tm_zip *tmzip,uLong * dt);
int check_exist_file(const char *filename);
void do_banner();
void do_help();
int getFileCrc(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc);
int isLargeFile(const char* filename);
int Inject(char * zipfile,char * file);

#endif /* MINIZIP_H_ */
