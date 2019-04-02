#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *window;

int interface(int argc,char *argv[])
{
    
    GtkBuilder *builder;

    gtk_init(&argc,&argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "PS4_inter.glade",NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    gtk_builder_connect_signals(builder,NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();



    return EXIT_SUCCESS;
}

void on_Close_clicked()
{	
     gtk_main_quit();
}

void on_huffman_clicked()
{
    printf("You clicked on Huffman\n");
}

void on_filesystem_clicked()
{
    printf("You clicked on Filesystem\n");
}

void on_backup_clicked()
{
    printf("You clicked on Backup\n");
}

void on_rotn_clicked()
{
    printf("You clicked on Rotn\n");
}

void on_vigenere_clicked()
{
    printf("You clicked on Vigenere\n");
}

void on_rsa_clicked()
{
    printf("You clicked on RSA\n");
}

void on_aes_clicked()
{
    printf("You clicked on AES\n");
}

void on_aes_on_file_clicked()
{
    printf("You clicked on AES on file\n");
}


