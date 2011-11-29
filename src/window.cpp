/**********************************************************/
/*						window.cpp						  */
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


#include "window.h"

/**********************************************************/
/*						 GLOBALS						  */
/**********************************************************/

extern double decrypt_progress;

/**********************************************************/
/*						NAMEPACES						  */
/**********************************************************/

using namespace rapidxml;
using namespace std;

/**********************************************************/
/*						PUBLIC							  */
/**********************************************************/

/**********************************************************/
MainWindow::MainWindow():
/**********************************************************/
/*
 * Create the main window
 */
l_inFile("*input (.ipsw):"),
l_key("decrypt key:"),
Button_Close("Close"),
Button_Decrypt("Decrypt")
{
	//set icons
	i_icon.set("icon_40.png");
	set_icon_from_file("icon.png");

	//buttons
	l_inFile.set_size_request(120,30);
	filechooserbutton.set_size_request(120,30);
	l_key.set_size_request(120,30);
	m_key.set_size_request(490,30);
	Button_Close.set_size_request(285,40);
	Button_Decrypt.set_size_request(285,40);
	i_icon.set_size_request(40,40);

	//progressbar
	m_ProgressBar.set_text("0%");
	b_progressbar.pack_start(m_ProgressBar);//,Gtk::PACK_SHRINK);

	//connect signal handlers
	Button_Close.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_close) );
	Button_Decrypt.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_decrypt) );
	filechooserbutton.signal_current_folder_changed().connect(sigc::mem_fun(*this, &MainWindow::on_button_ipsw) );

	add(HBox);

	//borders
	b_inFile.set_border_width(3);
	b_key.set_border_width(3);
	HBox_buttons.set_border_width(3);
	b_progressbar.set_border_width(3);
	VBox.set_border_width(0);
	HBox.set_border_width(0);

	//pack widgets
	b_inFile.pack_start(l_inFile,Gtk::PACK_SHRINK);
	b_inFile.pack_start(filechooserbutton);
	b_key.pack_start(l_key,Gtk::PACK_SHRINK);
	b_key.pack_start(m_key,Gtk::PACK_SHRINK);
	HBox_buttons.pack_start(i_icon,Gtk::PACK_SHRINK);
	HBox_buttons.pack_start(Button_Decrypt,Gtk::PACK_SHRINK);
	HBox_buttons.pack_start(Button_Close,Gtk::PACK_SHRINK);
	HBox.pack_start(VBox,Gtk::PACK_SHRINK);
	VBox.pack_start(b_inFile,Gtk::PACK_SHRINK);
	VBox.pack_start(b_key,Gtk::PACK_SHRINK);
	VBox.pack_start(b_progressbar,Gtk::PACK_SHRINK);
	VBox.pack_start(HBox_buttons,Gtk::PACK_SHRINK);

	/* Windows settings: */
	set_title("iOS decryptor by @Jikax");
	set_border_width(5);
	set_default_size(400, 130);
	set_position(Gtk::WIN_POS_CENTER);
	//fullscreen();

	/* show all widgets: */
	show_all_children();

}

/**********************************************************/
MainWindow::~MainWindow(){
/**********************************************************/
/*
 * Destructor
 */

}

/**********************************************************/
/*						PRIVATE							  */
/**********************************************************/

/**********************************************************/
void MainWindow::on_button_close(){
/**********************************************************/
/*
 * Close the window
 */
	hide();
	exit(1);
}

/**********************************************************/
void MainWindow::on_button_decrypt(){
/**********************************************************/
/*
 * Whe the decrypt button is pushed it will be disabled.
 * A new thread will be started to decrypt the file system
 * And a timeout signal will be connected to update the progressbar
 */
	decrypting = 1;
	Button_Decrypt.set_sensitive(false);
	//m_ProgressBar.set_pulse_step(0.05);
	Glib::Thread::create( sigc::mem_fun(*this,&MainWindow::Decrypt),true);
	Glib::signal_timeout().connect(sigc::mem_fun(*this,&MainWindow::UpdateProgressBar), 50 );
}

/**********************************************************/
void MainWindow::Decrypt(){
/**********************************************************/
/*
 * This will be started as a new thread when the decrypt button is selected.
 * The encrypted dmg inside the ipsw will be extracted.
 */
	std::stringstream output;

	/* reset the decryption status
	 * this will be updated by vfdecrypt
	 */
	decrypt_progress = 0;

	/* console data: print the input file and the location of the decrypted dmg file */
	std::cout << filechooserbutton.get_filename() << std::endl;
	output << filechooserbutton.get_current_folder() << "\\decrypted_" << file.get_SystemRestoreImage();
	std::cout << output.str() << std::endl;

	/* extract the root file system from ipsw */
	file.get_file(file.get_SystemRestoreImage());

	/* decrypt SystemRestoreImage */
	std::stringstream outfile;
	outfile << filechooserbutton.get_current_folder()
			<< "\\" << file.get_ProductType()
			<< "_" << file.get_ProductVersion()
			<< "_" << file.get_ProductBuildVersion()
			<< "_Decrypted_"
			<< file.get_SystemRestoreImage().c_str();

	Glib::ustring keystring = GetKey(file).c_str();

	if(!keystring.size() && m_key.get_text().size())
		keystring = m_key.get_text();


	decrypt(
		(char *)file.get_SystemRestoreImage().c_str(),		/* root file system image name */
		(char *)outfile.str().c_str(),						/* output location and name */
		(char *)keystring.c_str(),						/* try to obtain the decryption key */
		NULL												/* passphrase not used*/
	);

	/* remove the encrypted file */
	if( std::remove( file.get_SystemRestoreImage().c_str() ) != 0 )
		std::cout << "Error deleting " << file.get_filename() << std::endl;

	/* stop decrypting */
	decrypting = 0;

	/* enable the decryption button */
	Button_Decrypt.set_sensitive(true);
}

/**********************************************************/
bool  MainWindow::UpdateProgressBar(){
/**********************************************************/
/*
 * Update the progressbar. This will be called on timeout.
 * the decryption progress is being updated by vfdecrypted.
 */
	std::stringstream txt;
	double current_value;

	/* as long as the decryption progress is 0 the file system is being extracted */
	if (decrypt_progress == 0 ){
		current_value = (((float)Get_Size(file.get_SystemRestoreImage()) / ((float)Get_Size(file.get_filename())*0.93))/2);
		current_value += (decrypt_progress/2);
	}
	else
		current_value = 0.5 + (decrypt_progress/2);

	/* only update the progressbar when the value is valid */
	if(current_value <= 1 && current_value >= 0){
		txt.str("");
		txt << (int)(current_value * 100) << "% - iOS decryptor by @Jikax" ;
		set_title(txt.str());

		txt.str("");
		if(decrypt_progress == 0)
			txt << "unpacking root file system: " << (int)(current_value * 100) << "%";
		else
			txt << "decrypting image: " << (int)(current_value * 100) << "%";

		m_ProgressBar.set_text(txt.str());
		m_ProgressBar.set_fraction(current_value);
	}
	if(current_value >= 1){
		/* done */
		return false;
	}

	return true;
}

/**********************************************************/
Glib::ustring MainWindow::GetKey(ipsw file){
/**********************************************************/
/*
 * Get the decryption key.
 */
	xml_document<> xml;
	Glib::ustring key;
	std::stringstream buffer;
	std::string 	line;

	/* open XML_KEYS */
	cout << "opening " << XML_KEYS << endl;
	ifstream xmlkeys(XML_KEYS);
	if(xmlkeys.fail())
		cout << "error opening " << XML_KEYS << endl;
	if (xmlkeys.is_open())
	{
		while ( xmlkeys.good() )
		{
		  getline (xmlkeys,line);
		  buffer << line << "\n\r";
		}
		xmlkeys.close();
	}

	/*testing only*/
	//cout << "key data:" << endl << buffer.str() << endl;

	/*parse XML_KEYS*/
	xml.parse<0>((char *)buffer.str().c_str());

	bool found = false;

	/* search for the right key */
	xml_node<>* current_node = xml.first_node("decryptor");
	for (xml_node<>* k = current_node->first_node("keyinfo"); k ; k = k->next_sibling("keyinfo")){
		if(  ! file.get_ProductType().compare(k->first_node("device")->value())  ){
			//found right device
			if(  ! file.get_ProductVersion().compare(k->first_node("version")->value())  ){
				//found right version
				if(  ! file.get_ProductBuildVersion().compare(k->first_node("build")->value())  ){
					//found key
					key = k->first_node("key")->value();
					found = true;
					break;
				}
			}
		}
	}

	/* check whether the key has been found*/
	if(!found)
		key = "";

	cout << "key found: " << key << endl;

	return key;
}

/**********************************************************/
void MainWindow::on_button_ipsw(){
/**********************************************************/
/*
 * Called when a new file has been selected
 */
	/* open file */
	cout 	<< endl<<"New file selected: " << filechooserbutton.get_filename() << endl;
	file.set_file(filechooserbutton.get_filename()); //open ipsw

	/* display the firmware information on the progressbar */
	std::stringstream text;
	text << file.get_ProductType() << " " << file.get_ProductVersion() << " (" << file.get_ProductBuildVersion() << ")" ;
	m_ProgressBar.set_text(text.str());

	/* retrieve the key */
	cout << "retreiving key." << endl;
	Glib::ustring k = GetKey(file);
	if(!k.size()){
		/* key not found */
		m_key.set_text("");
		text << " - No key available, please enter key manually";
		m_ProgressBar.set_text(text.str());
	}
	else{
		/* key found */
		m_key.set_text(k);
	}

	/* reset progressbar */
	m_ProgressBar.set_fraction(0);
}

/**********************************************************/
int MainWindow::Get_Size( string path ){
/**********************************************************/
/*
 * Read the size of a file in bytes
 */
	FILE *pFile = NULL;
	pFile = fopen(path.c_str(), "rb" );
	fseek( pFile, 0, SEEK_END );
	int Size = ftell( pFile );
	fclose( pFile );
	return Size;
}

/**********************************************************/
/*						END OF FILE						  */
/**********************************************************/


