#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "../encryption/encryption.h"

#include "../compression/wrap.h"
// #include "../compression/lz78/lz78.h"

#include "../filesystem/restore_save.h"
#include "../filesystem/create_save.h"



GtkWidget *window;
GtkBuilder *builder;

GtkWidget *main_window;
GtkWidget *make_backup;
GtkWidget *ask_file;
GtkWidget *ask_file_restore;
GtkWidget *ask_file_load;
GtkWidget *ask_file_save;
GtkWidget *Encryption;
GtkWidget *Decryption;
GtkWidget *Compr;
GtkWidget *Decompr;
GtkWidget *Rotn_window;
GtkWidget *Vigenere_window;
GtkWidget *AES_window;
GtkWidget *ask_path_key;
GtkWidget *ask_path_create_key;
GtkWidget *generate_key;
GtkWidget *ask_path_create_private;
GtkWidget *create_key;

GtkToggleButton *compr_toggle;
GtkToggleButton *crypt_toggle;
GtkToggleButton *compr_load_toggle;
GtkToggleButton *crypt_load_toggle;
GtkToggleButton *decompr_toggle;
GtkToggleButton *decrypt_toggle;
GtkToggleButton *create_private;

GtkEntry *rotn_entry;
GtkEntry *vigenere_entry;
GtkEntry *aes_entry;
GtkEntry *key_entry;

gboolean crypt_state;
gboolean compr_state;
gboolean decrypt_state;
gboolean decompr_state;

int new_save = 0;
int do_rsa;
int restore_rotn=0;
int restore_vigenere=0;
int restore_aes = 0;

char *path;
char *old_path;
char *save_path;
char *key_path;
char *key_private_path;

char *path_tmp;
char *path_tmp_2;
char *path_in;
char *path_out;

FILE *in;
FILE *out;

int interface(int argc,char *argv[])
{

	GtkBuilder *builder;

	gtk_init(&argc,&argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "PS4_inter.glade",NULL);

	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	make_backup = GTK_WIDGET(gtk_builder_get_object(builder, "make_backup"));
	ask_file_load = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_load"));
	ask_file = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file"));
	ask_file_save = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_save"));
	ask_file_restore = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_restore"));
	Encryption = GTK_WIDGET(gtk_builder_get_object(builder, "Encryption"));
	Decryption = GTK_WIDGET(gtk_builder_get_object(builder, "Decryption"));
	Compr = GTK_WIDGET(gtk_builder_get_object(builder, "Compr"));
	Decompr = GTK_WIDGET(gtk_builder_get_object(builder, "Decompr"));
	Rotn_window = GTK_WIDGET(gtk_builder_get_object(builder, "Rotn_window"));
	Vigenere_window = GTK_WIDGET(gtk_builder_get_object(builder, "Vigenere_window"));
	AES_window = GTK_WIDGET(gtk_builder_get_object(builder, "AES_window"));
	ask_path_key = GTK_WIDGET(gtk_builder_get_object(builder, "ask_path_key"));
	ask_path_create_key = GTK_WIDGET(gtk_builder_get_object(builder, "ask_path_create_key"));
	generate_key = GTK_WIDGET(gtk_builder_get_object(builder, "generate_key"));
	ask_path_create_private = GTK_WIDGET(gtk_builder_get_object(builder, "ask_path_create_private"));
	create_key = GTK_WIDGET(gtk_builder_get_object(builder, "create_key"));

	crypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt"));
	compr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr"));
	crypt_load_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt_load"));
	compr_load_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr_load"));
	decrypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"decrypt"));
	decompr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"decompr"));
	create_private= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"create_private"));

	rotn_entry=GTK_ENTRY(gtk_builder_get_object(builder, "rotn_entry"));
	vigenere_entry=GTK_ENTRY(gtk_builder_get_object(builder, "vigenere_entry"));
	aes_entry=GTK_ENTRY(gtk_builder_get_object(builder, "aes_entry"));
	key_entry=GTK_ENTRY(gtk_builder_get_object(builder, "key_entry"));

	gtk_builder_connect_signals(builder,NULL);

	g_object_unref(builder);

	gtk_widget_show(main_window);
	gtk_main();



	return EXIT_SUCCESS;
}

void on_Close_clicked()
{	
	gtk_main_quit();
}

void on_backup_clicked()
{
	printf("You clicked on Backup\n");
	gtk_widget_show(make_backup);
	gtk_widget_hide(main_window);
}

void on_restore_clicked()
{
	printf("You clicked on Restore\n");
	gtk_widget_show(ask_file_restore);
	gtk_widget_hide(main_window);
}

void on_create_clicked()
{
	printf("You clicked on Create\n");
	gtk_widget_show(ask_file);
	gtk_widget_hide(make_backup);

	new_save = 1;
}

void on_load_clicked()
{
	printf("You clicked on Load\n");
	gtk_widget_show(ask_file);
	gtk_widget_hide(make_backup);
	new_save = 0;
}

void on_save_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file);
	path = gtk_file_chooser_get_filename(chooser);

	printf("File to save : PATH: %s\n", path);

	gtk_widget_hide(ask_file);
	gtk_widget_show(ask_file_save);
}

void on_save_valid_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file_save);
	save_path = gtk_file_chooser_get_filename(chooser);
	compr_state = gtk_toggle_button_get_active(compr_toggle);
	crypt_state = gtk_toggle_button_get_active(crypt_toggle);


	if(new_save)
	{

		if(compr_state)
		{
			// Do Save -> Go to Compression 
/*			path_tmp = tmpnam(NULL);
			path_tmp_2 = tmpnam(NULL);
			FILESYSTEM_create_save(path, path_tmp);
			path_in = path_tmp;
			path_out = path_tmp_2;
*/			gtk_widget_hide(ask_file_save);
			gtk_widget_show(Compr);
		}
		else
		{
			if(crypt_state)
			{
				//Do Save -> Got to Encryption
/*				path_tmp = tmpnam(NULL);
				FILESYSTEM_create_save(path, path_tmp);
				in = fopen(path_tmp, "r");
				out = fopen(save_path, "w+");
*/				gtk_widget_hide(ask_file_save);
				gtk_widget_show(Encryption);
			}
			else
			{
				//Do Save
//				FILESYSTEM_create_save(path,save_path);

				gtk_widget_hide(ask_file_save);
				gtk_widget_show(main_window);
			}
		}
	}
	else
	{
		gtk_widget_hide(ask_file_save);
		gtk_widget_show(ask_file_load);
	} 


	printf("File to put the save : PATH: %s\n", save_path);
}

void on_save_old_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file_load);
	old_path = gtk_file_chooser_get_filename(chooser);

	compr_state = gtk_toggle_button_get_active(compr_toggle);
	crypt_state = gtk_toggle_button_get_active(crypt_toggle);

	// Caling Save fonction, using an olt save.
//	FILESYSTEM_create_new_save(path,save_path,old_path);

	if(compr_state)
	{
		//Do Save(with old) -> Do Compression
/*		path_tmp = tmpnam(NULL);
		path_tmp_2 = tmpnam(NULL);
		FILESYSTEM_create_new_save(path, path_tmp, old_path);
		path_in = path_tmp;
		path_out = path_tmp_2; 
*/		gtk_widget_hide(ask_file_load);
		gtk_widget_show(Compr);

	}
	else
	{
		if(crypt_state)
		{
			//Do Save(with old) -> Do Encryption
/*			path_tmp = tmpnam(NULL);
			FILESYSTEM_create_new_save(path, path_tmp, old_path);
			in = fopen(path_tmp, "r");
			out = fopen(save_path, "w+");
*/			gtk_widget_hide(ask_file_load);
			gtk_widget_show(Encryption);
		}
		else
		{
			//Do Save(with old)   
//			FILESYSTEM_create_new_save(path,save_path,old_path);

			gtk_widget_hide(ask_file_load);
			gtk_widget_show(main_window);
		}
	}

	printf("File of the old  save : PATH: %s\n", old_path);

}

void on_restore_button_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file_restore);
	path = gtk_file_chooser_get_filename(chooser);

	decompr_state = gtk_toggle_button_get_active(decompr_toggle);
	decrypt_state = gtk_toggle_button_get_active(decrypt_toggle);

	printf("TODO : Launch restore\n");

	if(decrypt_state)
	{
		gtk_widget_hide(ask_file_restore);
		gtk_widget_show(Decryption);

	}
	else
	{
		if(decompr_state)
		{
			gtk_widget_hide(ask_file_restore);
			gtk_widget_show(Decompr);

		}
		else
		{
			//Do Restore
//			FILESYSTEM_restore_save(path);
			gtk_widget_hide(ask_file_restore);
			gtk_widget_show(main_window);
		}
	}

	printf("PATH: %s\n", path);
}

void on_Rotn_clicked()
{
	gtk_widget_hide(Encryption);
	gtk_widget_show(Rotn_window);
}

void on_Vigenere_clicked()
{
	gtk_widget_hide(Encryption);
	gtk_widget_show(Vigenere_window);
}


void on_AES_clicked()
{
	gtk_widget_hide(Encryption);
	gtk_widget_show(AES_window);
}


void on_valid_rotn_clicked()
{
	const char* entry_text;

	entry_text = gtk_entry_get_text(rotn_entry);
	int key = atoi(entry_text);

	if(crypt_state)
	{
		printf("TO DO: Encryption Rotn with the key %d\n", key);
		//CALL ENCRYPTION ROTN

//		PACKUP_encryption_stream(ROTN,in, out, key);

		gtk_widget_hide(Rotn_window);
		gtk_widget_show(main_window);
	}
	else
	{
		printf("TO DO: Decryption Rotn with the key %i\n",key);
		if(decompr_state)
		{
			gtk_widget_hide(Rotn_window);
			gtk_widget_show(Decompr);
		}
		else
		{
			//CALL DECRYPTION ROTN + RESTORE
/*			path_tmp = tmpnam(NULL);
			in = fopen(path,"r");
			out = fopen(path_tmp,"w+");
			PACKUP_decryption_stream(ROTN, in, out, key);
			FILESYSTEM_restore_save(path_tmp);
*/			gtk_widget_hide(Rotn_window);
			gtk_widget_show(main_window);
		}

	}
}

void on_valid_vigenere_clicked()
{
	const char* entry_text;

	entry_text = gtk_entry_get_text(vigenere_entry);

	if(crypt_state)
	{
		//CALL ENCRYPTION VIGENERE

//		PACKUP_encryption_stream(VIGENERE,in, out, entry_text);
		printf("TO DO: Encryption Vigenere \"%s\"\n", entry_text);

		gtk_widget_hide(Vigenere_window);
		gtk_widget_show(main_window);
	}
	else
	{
		printf("TO DO: Decryption Vigenere with the key \"%s\"\n",entry_text);
		if(compr_state)
		{

			gtk_widget_hide(Vigenere_window);
			gtk_widget_show(Compr);

		}
		else
		{
			//CALL DECRYPTION VIGENERE + RESTORE
/*			path_tmp = tmpnam(NULL);
			in = fopen(path,"r");
			out = fopen(path_tmp,"w+");
			PACKUP_decryption_stream(VIGENERE, in, out, entry_text);
			FILESYSTEM_restore_save(path_tmp);
			remove(path_tmp);
*/			gtk_widget_hide(Vigenere_window);
			gtk_widget_show(main_window);

		}

	}
}

void on_valid_aes_clicked()
{
	const char* entry_text;

	entry_text = gtk_entry_get_text(aes_entry);
	if(crypt_state)
	{
		//CALL ECRYPTION AES

//		PACKUP_encryption_stream(AES,in, out, entry_text);
		printf("TO DO: Encryption AES \"%s\"\n", entry_text);

		gtk_widget_hide(AES_window);
		gtk_widget_show(main_window);
	}
	else
	{
		printf("TO DO: Decryption AES with the key \"%s\"\n", entry_text);
		if(decompr_state)
		{
			gtk_widget_hide(AES_window);
			gtk_widget_show(Decompr);
		}
		else
		{
			//CALL DECRYPTION AES + RESTORE
/*			path_tmp = tmpnam(NULL);
			in = fopen(path,"r");
			out = fopen(path_tmp,"w+");
			PACKUP_decryption_stream(AES, in, out, entry_text);
			FILESYSTEM_restore_save(path_tmp);
			remove(path_tmp);
*/			gtk_widget_hide(AES_window);
			gtk_widget_show(main_window);

		}

	}
}

void on_RSA_clicked()
{
	do_rsa = 1;
	gtk_widget_hide(Encryption);
	gtk_widget_show(create_key);
}

void on_use_clicked()
{
	gtk_widget_hide(create_key);
	gtk_widget_show(ask_path_key);
}

void on_ELGAMAL_clicked()
{
	do_rsa = 0;
	gtk_widget_hide(Encryption);
	gtk_widget_show(create_key);
}

void on_Rotn_decrypt_clicked()
{
	gtk_widget_hide(Decryption);
	gtk_widget_show(Rotn_window);
}

void on_create_new_key_clicked()
{
	gtk_widget_hide(create_key);
	gtk_widget_show(ask_path_create_key);
}

void on_create_public_key_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_path_create_key);
	key_path = gtk_file_chooser_get_filename(chooser);

	printf("Create public key in PATH : %s\n", key_path);

	gtk_widget_hide(ask_path_create_key);
	gtk_widget_show(ask_path_create_private);
}

void on_create_private_key_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_path_create_private);
	key_private_path = gtk_file_chooser_get_filename(chooser);

	printf("Create private key in PATH : %s\n", key_private_path);

	gtk_widget_hide(ask_path_create_private);
	gtk_widget_show(generate_key);
}


void on_Vigenere_decrypt_clicked()
{
	gtk_widget_hide(Decryption);
	gtk_widget_show(Vigenere_window);
}

void on_AES_decrypt_clicked()
{
	gtk_widget_hide(Decryption);
	gtk_widget_show(AES_window);
}

void on_RSA_decrypt_clicked()
{
	do_rsa = 1;
	gtk_widget_hide(Decryption);
	gtk_widget_show(ask_path_key);
}

void on_ELGAMAL_decrypt_clicked()
{
	do_rsa = 0;
	gtk_widget_hide(Decryption);
	gtk_widget_show(ask_path_key);
}

void on_create_private_key()
{
	gtk_widget_hide(ask_path_create_private);
	gtk_widget_show(generate_key);

}

void on_valid_key_entry_clicked()
{
	const char* entry_text;

	entry_text = gtk_entry_get_text(key_entry);
	unsigned long key;

	key = strtoul(entry_text, NULL, 10);

	if (do_rsa)
	{
		printf("generate public key from the long %lu for RSA Encryption in the path : %s\n", key,key_path);
		//ENCRYPTION RSA CREATE KEY
//		PACKUP_encryption_stream(RSA, in, out, key_path, key_private_path, key);

		gtk_widget_hide(generate_key);
		gtk_widget_show(main_window);
	}
	else
	{
		printf("generate public key from the long %lu for ELGAMAL Encryption in the path: %s\n", key, key_path);
		//ENCRYPTION ELGAMAL CREATE KEY
//		PACKUP_encryption_stream(ELGAMAL, in, out, key_path, key_private_path, key);

		gtk_widget_hide(generate_key);
		gtk_widget_show(main_window);

	}

}

void on_use_key_clicked()
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_path_key);
	key_path = gtk_file_chooser_get_filename(chooser);

	if(crypt_state)
	{
		if(do_rsa)
		{
			printf("Encoding using public key for RSA Encryption in the path : %s\n", key_path);

			//ENCRYPTION RSA USING EXISTING KEY

//			PACKUP_encryption_stream(RSA,in,out,key_path);

			gtk_widget_hide(ask_path_key);
			gtk_widget_show(main_window);

		}
		else
		{
			printf("Encoding using public key for ELGAMAL Encryption in the path : %s\n", key_path);

			//ENCRYPTION ELGAMAL USING EXISTING KEY

//			PACKUP_encryption_stream(ELGAMAL,in,out,key_path);

			gtk_widget_hide(ask_path_key);
			gtk_widget_show(main_window);

		}
	}
	else
	{
		if(do_rsa)
		{
			printf("Decoding using private key for RSA Encryption in the path : %s\n", key_path);
			if(decompr_state)
			{
				gtk_widget_hide(ask_path_key);
				gtk_widget_show(Decompr);
			}
			else
			{
				// DECRYPTION RSA + RESTORE
/*				path_tmp = tmpnam(NULL);
				in = fopen(path,"r");
				out = fopen(path_tmp, "w+");
				PACKUP_decryption_stream(RSA, in, out, key_path);
				FILESYSTEM_restore_save(path_tmp);
				remove(path_tmp);
*/				gtk_widget_hide(ask_path_key);
				gtk_widget_show(main_window);


			}
		}
		else
		{
			printf("Decoding using private key for ELGAMAL Encryption in the path : %s\n", key_path);
			if(decompr_state)
			{
				gtk_widget_hide(ask_path_key);
				gtk_widget_show(Decompr);
			}
			else
			{
				//DECRYPT ELGAMAL+RESTORE
/*				path_tmp = tmpnam(NULL);
				in = fopen(path,"r");
				out = fopen(path_tmp,"w+");
				PACKUP_decryption_stream(ELGAMAL, in, out, key_path);
				FILESYSTEM_restore_save(path_tmp);
				remove(path_tmp);
*/
				gtk_widget_hide(ask_path_key);
				gtk_widget_show(main_window);

			}
		}
	}
}

void on_Huffman_clicked()
{
	printf("TO DO: Compression Huffman\n");
//	test_simple_huffman_compress(path_in,path_out);
	in = fopen(path_out, "r");
	out = fopen(save_path, "w+");

	if(crypt_state)
	{
		gtk_widget_hide(Compr);
		gtk_widget_show(Encryption);
	}
}

void on_lz78_clicked()
{
	printf("TO DO: Compression lz78\n");
	if(crypt_state)
	{
		gtk_widget_hide(Compr);
		gtk_widget_show(Encryption);
	}
}

void on_Huffman_decompr_clicked()
{
	printf("TO DO: Decompression Huffman\n");
}

void on_lz78_decompr_clicked()
{
	printf("TO DO: Decompression lz78\n");
}
