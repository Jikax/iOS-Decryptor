/*
 * miniunz.h
 *
 *  Created on: 10 nov. 2011
 *      Author: Martijn
 */

#ifndef MINIUNZ_H_
#define MINIUNZ_H_

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
#else
# include <direct.h>
# include <io.h>
#endif

#include "unzip.h"

#define CASESENSITIVITY (0)
#define _WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifdef _WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif

void change_file_date(const char *filename,uLong dosdate,tm_unz tmu_date);
int mymkdir(const char* dirname);
int makedir ( char *newdir);
void Display64BitsSize(ZPOS64_T n, int size_char);
int do_list(unzFile uf);
int do_extract_currentfile(unzFile uf,const int* popt_extract_without_path,int* popt_overwrite,const char* password);
int do_extract(unzFile uf,int opt_extract_without_path,int opt_overwrite,const char* password);
int do_extract_onefile(unzFile uf,const char* filename,int opt_extract_without_path,int opt_overwrite,const char* password);
int extract(char *zipfilename);








#endif /* MINIUNZ_H_ */
