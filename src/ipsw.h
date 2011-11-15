/**********************************************************/
/*						ipsw.h						  	  */
/**********************************************************/
/* Copyright (c) 2011									  */
/* Jikax Developent										  */
/* 														  */
/* This material is provided "as is", with absolutely no  */
/* warranty expressed * or implied. 					  */
/* Any use is at your own risk.							  */
/* 														  */
/* Permission to use or copy this software for any purpose*/
/* is hereby granted without fee, provided the above 	  */
/* notices are retained on all copies. Permission to 	  */
/* modify the code and to distribute modified code is 	  */
/* granted, provided the above notices are retained, and  */
/* a notice that the code was modified is included with   */
/* the above copyright notice.						  	  */
/**********************************************************/

#ifndef IPSW_H_
#define IPSW_H_

/**********************************************************/
/*						INCLUDES						  */
/**********************************************************/

#include <gtkmm.h>
#include <rapidxml.hpp>
#include <fstream>
#include <iostream>
#include <cstring>

extern "C"{
	#include "miniunz.h"
	#include "minizip.h"
	#include "vfdecrypt.h"
}

/**********************************************************/
/*						DEFINES							  */
/**********************************************************/

#define RESTORE_PLIST	"Restore.plist"

/**********************************************************/
/*					STRUCTS / CLASSES					  */
/**********************************************************/

/**********************************************************/
class ipsw{
/**********************************************************/
/*
 * Get the information from an ipsw file and be able to
 * extract files from it.
 */
public:
	ipsw();
	ipsw(Glib::ustring filename);
	~ipsw();

	int set_file(Glib::ustring filename); 		/*open ipsw file*/
	int get_file(Glib::ustring filename);		/*extract single file*/
	int get_all();								/*extract complete ipsw*/
	Glib::ustring 	get_ProductVersion();		/*extract product version*/
	Glib::ustring 	get_ProductBuildVersion();	/*extract product build version*/
	Glib::ustring 	get_ProductType();			/*extract product type*/
	Glib::ustring 	get_filename();				/*get the ipsw file name*/
	Glib::ustring 	get_SystemRestoreImage();	/*get the name of the root file system image*/
	int get_size();								/*get the size of the ipsw file*/

private:
	int parse_plist();							/*extract and parse the restore.plist file inside the ipsw*/

	Glib::ustring ipswFileName;
	Glib::ustring ProductVersion;
	Glib::ustring ProductBuildVersion;
	Glib::ustring ProductType;
	Glib::ustring SystemRestoreImage;
	int size;
};


/**********************************************************/
/*						END 							  */
/**********************************************************/

#endif /* IPSW_H_ */

/**********************************************************/
/*					END OF FILE							  */
/**********************************************************/
