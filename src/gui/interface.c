#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkBuilder *builder;
GtkWidget *window;

int interface(int argc,char *argv[])
{
    gtk_init(&argc,&argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "PS4_inter.glade",NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_windows"));


    gtk_builder_connect_signals(builder,NULL);

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();



    return EXIT_SUCCESS;
}

void on_Close_cliked()
{
    exit(0);
}

void on_Support_cliked()
{
    printf("You cliked on Support\n");
}

void on_Compr_cliked()
{
    printf("You cliked on Compression\n");
}

void on_Decompr_cliked()
{
    printf("You cliked on Decompression\n");
}

void on_Chiffr_cliked()
{
    printf("You cliked on Chiffrement\n");
}


