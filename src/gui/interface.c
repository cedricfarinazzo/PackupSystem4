#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *window;
GtkBuilder *builder;

GtkWidget *main_window;
GtkWidget *make_backup;
GtkWidget *ask_file_load;
GtkWidget *ask_file_restore;
GtkWidget *ask_file_create;

GtkToggleButton *compr_toggle;
GtkToggleButton *crypt_toggle;
GtkToggleButton *compr_load_toggle;
GtkToggleButton *crypt_load_toggle;

GtkToggleButton *decompr_toggle;
GtkToggleButton *decrypt_toggle;

gboolean crypt_state;
gboolean compr_state;
gboolean decrypt_state;
gboolean decompr_state;

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

    crypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt"));
    compr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr"));
    crypt_load_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt_load"));
    compr_load_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr_load"));
    decrypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"decrypt"));
    decompr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"decompr"));

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
}

void on_load_clicked()
{
    printf("You clicked on Load\n");
    gtk_widget_show(ask_file_load);
    gtk_widget_hide(make_backup);
}



void on_save_load_clicked()
{
    //GtkToggleButton *compr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt"));
    //GtkToggleButton *crypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr"));

    compr_state = gtk_toggle_button_get_active(compr_load_toggle);
    crypt_state = gtk_toggle_button_get_active(crypt_load_toggle);

    printf("TODO : Lauch Save\n");

    if(compr_state)
    {
        printf("TODO : Lauch Compression\n");
    }
    if(crypt_state)
    {
        printf("TODO : Lauch Encryption\n");
    }
}

void on_save_create_clicked()
{
    //GtkToggleButton *compr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"encrypt"));
    //GtkToggleButton *crypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"compr"));

        
    compr_state = gtk_toggle_button_get_active(compr_toggle);
    crypt_state = gtk_toggle_button_get_active(crypt_toggle);

    printf("TODO : Lauch Save\n");

    if(compr_state)
    {
        printf("TODO : Lauch Compression\n");
    }
    if(crypt_state)
    { 
        printf("TODO : Lauch Encryption\n");
    }

    
}

void on_restore_button_clicked()
{
    //GtkToggleButton *decompr_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"dencrypt"));
    //GtkToggleButton *decrypt_toggle= GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"decompr"));
        
    decompr_state = gtk_toggle_button_get_active(decompr_toggle);
    decrypt_state = gtk_toggle_button_get_active(decrypt_toggle);

    printf("TODO : Lauch restore\n");

    if(decompr_state)
    {
        printf("TODO : Lauch Compression\n");
    }
    if(decrypt_state)
    {
            printf("TODO : Lauch Encryption\n");
    }
    
}
