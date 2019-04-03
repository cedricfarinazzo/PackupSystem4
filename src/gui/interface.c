#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkBuilder *builder;

GtkWidget *file_window;
GtkWidget *input_window;

int interface(int argc,char *argv[])
{
    
    GtkBuilder *builder;

    gtk_init(&argc,&argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "PS4_inter.glade",NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    file_window = GTK_WIDGET(gtk_builder_get_object(builder, "file_main"));
    input_window = GTK_WIDGET(gtk_builder_get_object(builder, "input_Huffman"));

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
     gtk_widget_show(input_window);
}

void on_filesystem_clicked()
{
    printf("You clicked on File System\n");
    gtk_widget_show(file_window);
}

void on_Rotn_clicked()
{
    printf("You clicked on Rotn\n");
}

void on_Vigenere_clicked()
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

void on_aes_file_clicked()
{
    printf("You clicked on AES on file\n");
}

void on_file_Validate_clicked()
{
    printf("You clicked on Validate\n");
}

void on_file_Close_clicked()
{
    printf("You clicked on Close\n");
    gtk_widget_hide(file_window);
}

void on_input_huffman_close_clicked()
{
    printf("You clicked on Close\n");
    gtk_widget_hide(input_window);
}
