/**********************************************************/
/*						ipsw.cpp						  */
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

/**********************************************************/
/*						INCLUDES						  */
/**********************************************************/

#include "ipsw.h"

/**********************************************************/
/*						NAMEPACES						  */
/**********************************************************/

using namespace Glib;
using namespace Gtk;
using namespace rapidxml;
using namespace std;

/**********************************************************/
/*						PUBLIC							  */
/**********************************************************/

/**********************************************************/
ipsw::ipsw(){
/**********************************************************/
/*
 * constructor
 */
	ipswFileName = "";
}

/**********************************************************/
ipsw::ipsw(Glib::ustring filename){
/**********************************************************/
/*
 * set the file.
 */
	set_file(filename);
}

/**********************************************************/
ipsw::~ipsw(){
/**********************************************************/
/*
 * destructor
 */

}

/**********************************************************/
int ipsw::set_file(ustring filename){
/**********************************************************/
/*
 * set the file name and get file size
 */
	ipswFileName = filename;
	struct stat filestatus;
	stat( ipswFileName.c_str(), &filestatus );
	size = filestatus.st_size;
	parse_plist();

	return 0;
}

/**********************************************************/
int ipsw::get_size(){
/**********************************************************/
/*
 * Return the file size
 */
	return size;
}

/**********************************************************/
ustring ipsw::get_ProductBuildVersion(){
/**********************************************************/
/*
 * return the product build version
 */
	return ProductBuildVersion;
}

/**********************************************************/
ustring ipsw::get_filename(){
/**********************************************************/
/*
 * return the file name
 */
	return ipswFileName;
}

/**********************************************************/
ustring ipsw::get_ProductType(){
/**********************************************************/
/*
 * return the device type
 */
	return ProductType;
}

/**********************************************************/
ustring ipsw::get_ProductVersion(){
/**********************************************************/
/*
 * return the firmare version
 */
	return ProductVersion;
}

/**********************************************************/
ustring ipsw::get_SystemRestoreImage(){
/**********************************************************/
/*
 * return the root file system file name
 */
	return SystemRestoreImage;
}

/**********************************************************/
int ipsw::get_file(ustring filename){
/**********************************************************/
/*
 * exract a file from the ipsw
 */
	unzFile uf=NULL;

	if(!ipswFileName.length()){
		cout << "no ipsw file selected" << endl;
		return 1;
	}

#   ifdef USEWIN32IOAPI
	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64A(&ffunc);
	uf = unzOpen2_64(ipswFileName.c_str(),&ffunc);
#   else
	uf = unzOpen64(ipswFileName.c_str());
#   endif

	cout << "extracting: " << filename << endl;
	int ret_val = do_extract_onefile(uf,filename.c_str(),0,1,NULL);

	unzClose(uf);

	return ret_val;
}

/**********************************************************/
/*						PRIVATE							  */
/**********************************************************/

/**********************************************************/
int ipsw::parse_plist(){
/**********************************************************/
/*
 * retreive information about the selected ipsw file by
 * extracting the restore.plist file and reading the
 * required information
 */
	xml_document<> xml;
	ustring key;

	if(get_file(RESTORE_PLIST)){	/*extract restore.plist from ipsw*/
		cout << "could not retrieve " << RESTORE_PLIST << endl;
		return 1;
	}

	/*open RESTORE_PLIST*/
	cout << "opening " << RESTORE_PLIST << endl;
	ifstream RestorePlist(RESTORE_PLIST);
	if(RestorePlist.fail())
		cout << "error opening " << RESTORE_PLIST << endl;

	/*place data from RESTORE_PLIST inside buffer*/
	RestorePlist.seekg(0,ios::end);
	size_t lenght = RestorePlist.tellg();
	RestorePlist.seekg(0,ios::beg);
	char *buffer = new char [lenght +1];
	RestorePlist.read(buffer,lenght);
	buffer[lenght] = '\0';
	RestorePlist.close();

	/*testing only*/
	//cout << "plist data:" << endl << buffer << endl;

	/*parse RESTORE_PLIST*/
	xml.parse<0>(buffer);

	/* read the information */
	xml_node<>* current_node = xml.first_node("plist")->first_node("dict");
	for (xml_node<>* n = current_node->first_node("key"); n ; n = n->next_sibling("key")){
		key = n->value();
		if(key == "ProductType")
			ProductType = n->next_sibling()->value();
		if(key == "ProductBuildVersion")
			ProductBuildVersion = n->next_sibling()->value();
		if(key == "ProductVersion")
			ProductVersion = n->next_sibling()->value();
		if(key == "SystemRestoreImages")
			SystemRestoreImage = n->next_sibling()->first_node("string")->value();
	}

	/* print the resulting data in a console window */
	cout 	<<endl<<"read ipsw information:"<<endl
			<<"\ttype:\t\t\t"<<ProductType << endl
			<<"\tbuild:\t\t\t"<<ProductBuildVersion <<endl
			<<"\tversion:\t\t"<<ProductVersion<<endl
			<<"\tfile system image:\t"<<SystemRestoreImage <<endl<<endl;

	if( remove( RESTORE_PLIST ) != 0 )
	    cout << "Error deleting RESTORE_PLIST" << endl;

	delete [] buffer;

	return 0;
}

/**********************************************************/
/*						END OF FILE						  */
/**********************************************************/
