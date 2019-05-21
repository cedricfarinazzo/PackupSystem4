#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "../encryption/rotn.h"
#include "../encryption/vigenere.h"

#include "../encryption/rsa/rsa.h"
#include "../encryption/rsa/genkey.h"
#include "../encryption/rsa/tools.h"
#include "../encryption/rsa/rsa_file.h"

#include "../encryption/aes/aes.h"
#include "../encryption/aes/aes_file.h"
#include "../encryption/aes/aes_matrix.h"
#include "../encryption/aes/aes_addroundkey.h"
#include "../encryption/aes/aes_shiftrows.h"
#include "../encryption/aes/aes_subbytes.h"
#include "../encryption/aes/aes_mixcolumns.h"
#include "../encryption/aes/hashpass.h"

#include "../compression/huffman/huffman.h"
#include "../compression/struct.h"
#include "../compression/file.h"

#include "../filesystem/build_metatree.h"
#include "../filesystem/save_metatree.h"
#include "../filesystem/build_restore_tree.h"
#include "../filesystem/save_file_content.h"
#include "../filesystem/restore_save.h"
#include "../filesystem/create_save.h"



GtkWidget *window;
GtkBuilder *builder;

GtkWidget *main_window;
GtkWidget *make_backup;
GtkWidget *ask_file_load;
GtkWidget *ask_file_restore;
GtkWidget *ask_file_create;
GtkWidget *Encryption;
GtkWidget *Decryption;
GtkWidget *Compr;
GtkWidget *Decompr;
GtkWidget *Rotn_window;
GtkWidget *Vigenere_window;
GtkWidget *AES_window;
GtkWidget *ask_path_key;
GtkWidget *ask_path_key_create;
GtkWidget *generate_key;

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
gboolean is_private;

int create_key = 0;
int do_rsa;

char *path;
char *key_path;

int interface(int argc,char *argv[])
{
    
    GtkBuilder *builder;

    gtk_init(&argc,&argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "PS4_inter.glade",NULL);

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    make_backup = GTK_WIDGET(gtk_builder_get_object(builder, "make_backup"));
    ask_file_load = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_load"));
    ask_file_create = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_create"));
    ask_file_restore = GTK_WIDGET(gtk_builder_get_object(builder, "ask_file_restore"));
    Encryption = GTK_WIDGET(gtk_builder_get_object(builder, "Encryption"));
    Decryption = GTK_WIDGET(gtk_builder_get_object(builder, "Decryption"));
    Compr = GTK_WIDGET(gtk_builder_get_object(builder, "Compr"));
    Decompr = GTK_WIDGET(gtk_builder_get_object(builder, "Decompr"));
    Rotn_window = GTK_WIDGET(gtk_builder_get_object(builder, "Rotn_window"));
    Vigenere_window = GTK_WIDGET(gtk_builder_get_object(builder, "Vigenere_window"));
    AES_window = GTK_WIDGET(gtk_builder_get_object(builder, "AES_window"));
    ask_path_key = GTK_WIDGET(gtk_builder_get_object(builder, "ask_path_key"));
    ask_path_key_create = GTK_WIDGET(gtk_builder_get_object(builder, "ask_path_key_create"));
    generate_key = GTK_WIDGET(gtk_builder_get_object(builder, "generate_key"));

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
    gtk_widget_show(ask_file_create);
    gtk_widget_hide(make_backup);

    create_key = 1;
}

void on_load_clicked()
{
    printf("You clicked on Load\n");
    gtk_widget_show(ask_file_load);
    gtk_widget_hide(make_backup);
}



void on_save_load_clicked()
{
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file_load);
    path = gtk_file_chooser_get_filename(chooser);

    compr_state = gtk_toggle_button_get_active(compr_load_toggle);
    crypt_state = gtk_toggle_button_get_active(crypt_load_toggle);

    printf("TODO : Launch Load andSave\n");

    if(crypt_state)
    {
	gtk_widget_hide(ask_file_load);
        gtk_widget_show(Encryption);

    }
    else{
       if(compr_state)
       {
           gtk_widget_hide(ask_file_load);
           gtk_widget_show(Compr);
       }
    }


    printf("PATH: %s\n", path);
}

void on_save_create_clicked()
{
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_file_create);
    path = gtk_file_chooser_get_filename(chooser);
        
    compr_state = gtk_toggle_button_get_active(compr_toggle);
    crypt_state = gtk_toggle_button_get_active(crypt_toggle);

    printf("TODO : Launch Create and Save\n");

    if(crypt_state)
    {
        gtk_widget_hide(ask_file_create);
        gtk_widget_show(Encryption);

    }
    else
    {
       if(compr_state)
       {
           gtk_widget_hide(ask_file_create);
           gtk_widget_show(Compr);
       }
    }

    printf("PATH: %s\n", path);    
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
	if(compr_state)
        {
	   printf("TO DO: Encryption Rotn with the key %d\n", key);
           gtk_widget_hide(Rotn_window);
           gtk_widget_show(Compr);
        }
     }
     else
     {
        if(decompr_state)
        {
            printf("TO DO: Decryption Rotn with the key %i\n",key);
            gtk_widget_hide(Rotn_window);
            gtk_widget_show(Decompr);
         }       
     }
}

void on_valid_vigenere_clicked()
{
    const char* entry_text;

    entry_text = gtk_entry_get_text(vigenere_entry);

    if(crypt_state)
    {
       if(compr_state)
       {
	  printf("TO DO: Encryption Vigenere with the key \"%s\"\n", entry_text);
          gtk_widget_hide(Vigenere_window);
          gtk_widget_show(Compr);
       }
    }
    else
    {
      if(decompr_state)
      {
	printf("TO DO: Decryption Vigenere with the key \"%s\"\n",entry_text);
        gtk_widget_hide(Vigenere_window);
        gtk_widget_show(Decompr);
      }
    }
}

void on_valid_aes_clicked()
{
    const char* entry_text;

    entry_text = gtk_entry_get_text(aes_entry);
    if(crypt_state)
    {
       printf("TO DO: Encryption AES \"%s\"\n", entry_text);
       if(compr_state)
      {
         gtk_widget_hide(AES_window);
         gtk_widget_show(Compr);
      }
    }
    else
    {
       if(decompr_state)
        {
	   printf("TO DO: Decryption AES with the key \"%s\"\n", entry_text);
           gtk_widget_hide(AES_window);
           gtk_widget_show(Decompr);
        }
    }
}

void on_RSA_clicked()
{
    do_rsa = 1;
    gtk_widget_hide(Encryption);
    if(create_key)
    {
        gtk_widget_show(ask_path_key_create);
    }
    else
    {
        gtk_widget_show(ask_path_key);
    }
}

void on_ELGAMAL_clicked()
{
    do_rsa = 0;
    gtk_widget_hide(Encryption);
    if(create_key)
    {
        gtk_widget_show(ask_path_key_create);
    }
    else
    {
        gtk_widget_show(ask_path_key);
    }
}

void on_Rotn_decrypt_clicked()
{
    gtk_widget_hide(Decryption);
    gtk_widget_show(Rotn_window);
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

void on_valid_create_key_clicked()
{
   GtkFileChooser *chooser = GTK_FILE_CHOOSER(ask_path_key_create);
   key_path = gtk_file_chooser_get_filename(chooser);

   is_private = gtk_toggle_button_get_active(create_private);

   gtk_widget_hide(ask_path_key_create);
   gtk_widget_show(generate_key);
}

void on_valid_key_entry_clicked()
{
    const char* entry_text;

    entry_text = gtk_entry_get_text(key_entry);
    unsigned long key;

    key = strtoul(entry_text, NULL, 10);
    
    if (is_private){
       if (do_rsa)
       { 
           printf("generate private key from the long %lu for RSA Encryption in the path : %s\n", key, key_path);
	   if(compr_state)
	   {
	      gtk_widget_hide(generate_key);
	      gtk_widget_show(Compr);
	   }
       }
       else
       {
          printf("generate private key from the long %lu for ELGAMAL Encryption in the path : %s\n", key, key_path);
	  if(compr_state)
           {
              gtk_widget_hide(generate_key);
              gtk_widget_show(Compr);
           }
       }
    }
    else
    {
       if (do_rsa)
       {
          printf("generate public key from the long %lu for RSA Encryption in the path : %s\n", key,key_path);
	  if(compr_state)
           {
              gtk_widget_hide(generate_key);
              gtk_widget_show(Compr);
           }
       }
       else
       {
          printf("generate public key from the long %lu for ELGAMAL Encryption in the path: %s\n", key, key_path);
          if(compr_state)
           {
              gtk_widget_hide(generate_key);
              gtk_widget_show(Compr);
           }
       }
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
	 if(compr_state)
           {
              gtk_widget_hide(ask_path_key);
              gtk_widget_show(Compr);
           }
      }
      else
      {
         printf("Encoding using public key for ELGAMAL Encryption in the path : %s\n", key_path);
	 if(compr_state)
           {
              gtk_widget_hide(ask_path_key);
              gtk_widget_show(Compr);
           }
      }
   }
   else
   {
      if(do_rsa)
      {
         printf("Decoding using public key for RSA Encryption in the path : %s\n", key_path);
	 if(decompr_state)
           {
              gtk_widget_hide(ask_path_key);
              gtk_widget_show(Decompr);
           }
      }
      else
      {
         printf("Decoding using public key for RSA Encryption in the path : %s\n", key_path);
	 if(decompr_state)
           {
              gtk_widget_hide(ask_path_key);
              gtk_widget_show(Decompr);
           }
      }
   }
}

void on_Huffman_clicked()
{
    printf("TO DO: Compression Huffman\n");
}

void on_lz78_clicked()
{
    printf("TO DO: Compression lz78\n");
}

void on_Huffman_decompr_clicked()
{
    printf("TO DO: Decompression Huffman\n");
}

void on_lz78_decompr_clicked()
{
    printf("TO DO: Decompression lz78\n");
}
