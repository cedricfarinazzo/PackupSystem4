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

int encrypt;
int compr;
int decrypt;
int decompr;

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

void on_save_clicked()
{
    //GtkToggleButton *decompr_toggle;
    //GtkToggleButton *decrypt_toggle;

    //decompr_toggle = GTK_Widget(gtk_builder_get_object(builder, "decompr"));
    //decrypt_toggle = GTK_Widget(gtk_builder_get_object(builder, "decrypt"));


    printf("You clicked on save\n");
    //gboolean decrypt_state;
    //gboolean decompr_state;

    //decompr_state = gtk_toggle_button_get_active(decompr_toggle);
    //decrypt_state = gtk_toggle_button_get_active(decrypt_toggle);

    //if(decompr_state){decompr=1;}
    //else{decompr=0;}

    //if(decrypt_state){decrypt=1;}
    //else{decrypt=0;}
}

void on_save_create_clicked()
{
    printf("You clicked on save1\n");
}

void on_restore_button_clicked()
{
    printf("You clicked on Restore\n");
}
