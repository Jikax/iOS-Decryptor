/*
 * update.cpp
 *
 *  Created on: 29 nov. 2011
 *      Author: martijn
 */

#include "update.h"


/*
 *      wget_sortof.c
 *
 *      Copyright 2007 Vyacheslav Goltser <slavikg@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* get the main page from google.com */

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

int check_for_update()
{
	CURL *curl;
	FILE *fp;
	std::string url = SERVER;
	char outfilename[FILENAME_MAX] = "keys.xml";
	curl = curl_easy_init();
	printf("copying keys.xml to backup file.\n\r");
	system("copy keys.xml keys.xml_backup");
	if(curl)
	{
		fp = fopen(outfilename,"wb");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_perform(curl);
		/* always cleanup */
		curl_easy_cleanup(curl);
		fclose(fp);
	}

	FILE *pFile = NULL;
	pFile = fopen(outfilename, "rb" );
	fseek( pFile, 0, SEEK_END );
	int Size = ftell( pFile );
	fclose( pFile );
	printf("new file size: %d\r\n", Size);
	if(Size <= 1000){
		printf("no new data found, restore backup\n\r");
		system("copy keys.xml_backup keys.xml");
		return 0;
	}
	printf("Keys.xml has been downloaded\n\r");
	return 1;
}

