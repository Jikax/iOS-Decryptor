/**********************************************************/
/*						main.cpp						  */
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

#include <gtkmm.h>
#include "window.h"

/**********************************************************/
/*						MAIN ROUTINE					  */
/**********************************************************/

/**********************************************************/
int main(int argc, char * argv[]){
/**********************************************************/
/*
 * Open the main window
 */
	if(!Glib::thread_supported())
		Glib::thread_init();

	Gtk::Main kit(argc, argv);
	MainWindow window;
	Gtk::Main::run(window);
}

/**********************************************************/
/*						END OF FILE						  */
/**********************************************************/
