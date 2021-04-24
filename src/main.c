
#include <libintl.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h> /* g_fopen, etc */
#include <gtk/gtk.h>
#include "support.h"
#include "callbacks.h"
#include "interface.h"
#include "mttfiles.h"
#include "misc.h"
#include "undo.h"



/*********************************

  prepare textview

*********************************/
GtkWidget *prepare_view ()
{
  GtkWidget *view;

  view = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(view), GTK_WRAP_WORD);
  gtk_widget_set_name (view, "view");
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW(view), 8);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW(view), 8);
  return view;
}
/*********************************
  startup for a standard
  g_application

**********************************/

static void redac_startup (APP_data *data)
{

  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps/" PACKAGE); /* New location for all pixmaps */
  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps"); /* Gnome users /usr/share/pixmaps folder */  

}

/*********************************
  activation for a standard
  g_application
**********************************/
static void
redac_activate (GApplication *app, APP_data *data)
{
  GList *list;
  GtkWidget *window, *scrolledwindow1;

  list = gtk_application_get_windows (app);
  /* test for uniqueness */
  if (list)
    {
      gtk_window_present (GTK_WINDOW (list->data));
    }
  else
    {
      redac_prepare_GUI (app, data);
      gtk_window_set_application (GTK_WINDOW (data->appWindow), app);
      gtk_widget_show (data->appWindow);
      gtk_widget_show (data->view);
      /* move to something like end of text */
      scrolledwindow1 = lookup_widget (GTK_WIDGET(data->appWindow), "scrolledwindow1");
      // misc_jump_to_end_view (scrolledwindow1, data->buffer, data->view);
    }
}

/**********************

  MAIN

***********************/

int main(int argc, char *argv[]) {
  GtkApplication *app;
  APP_data app_data;
  gint status;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  app = gtk_application_new ("org.gtk.redac", 0);
  app_data.app=app;
  g_signal_connect (app, "startup", G_CALLBACK (redac_startup), &app_data);
  g_signal_connect (app, "activate", G_CALLBACK (redac_activate), &app_data);
  /* main loop */
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
