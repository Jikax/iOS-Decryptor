/*
 * update.h
 *
 *  Created on: 29 nov. 2011
 *      Author: martijn
 */

#ifndef UPDATE_H_
#define UPDATE_H_


	#include <stdio.h>
	#include <curl/curl.h>
	//#include <curl/types.h>
	#include <curl/easy.h>
	#include <string>

#define SERVER "http://192.168.1.99/Jikax/iOS_Decryptor/keys/keys.xml"
//#define SERVER "http://80.56.207.117/Jikax/iOS_Decryptor/keys/keys.xml"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
int check_for_update();

#endif /* UPDATE_H_ */
