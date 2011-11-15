/**********************************************************/
/*						window.h						  */
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

#ifndef WINDOW_H_
#define WINDOW_H_

/**********************************************************/
/*						INCLUDES						  */
/**********************************************************/

#include <iostream>
#include <stdio.h>
#include <gtkmm.h>
#include <zlib.h>
#include <rapidxml.hpp>
#include "ipsw.h"

/**********************************************************/
/*						DEFINES							  */
/**********************************************************/

#define XML_KEYS "keys.xml"

/**********************************************************/
/*					STRUCTS / CLASSES					  */
/**********************************************************/

typedef struct{
	Glib::ustring device;
	Glib::ustring build;
	Glib::ustring version;
	Glib::ustring key;
}KeyInfo;

/**********************************************************/
class MainWindow : public Gtk::Window{
/**********************************************************/
/*
 * The main window of the application.
 */
public:
	MainWindow();
	~MainWindow();
private:
	void on_button_close();				/* close the application */
	void on_button_decrypt();			/* extract and decrypt the root file system */
	void on_button_ipsw();				/* read the information from a selected ipsw file */
	void Decrypt();						/* decrypt the encrypted file system image */
	virtual bool UpdateProgressBar();	/* update the progressbar */
	Glib::ustring GetKey(ipsw file);	/* get the decryption key */
	int Get_Size(std::string);			/* get the size of a file */

	Gtk::Label l_inFile;
	Gtk::Label l_key;
	Gtk::Entry m_key;
	Gtk::Button Button_Close;
	Gtk::Button Button_Decrypt;
	Gtk::ProgressBar m_ProgressBar;
	Gtk::VBox VBox;
	Gtk::HBox HBox;
	Gtk::HBox HBox_buttons;
	Gtk::HBox b_inFile;
	Gtk::FileChooserButton filechooserbutton ;
	Gtk::HBox b_key;
	Gtk::HBox b_progressbar;
	Gtk::Image i_icon;

	ipsw file;

	int decrypting;
};

/**********************************************************/
/*						END 							  */
/**********************************************************/

#endif /* WINDOW_H_ */

/**********************************************************/
/*					END OF FILE							  */
/**********************************************************/
