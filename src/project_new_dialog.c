/*
 * @file project_new_dialog.c
 * @author Allann Jones <allanjos[at]gmail.com>
 * @since 2013
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Allann Jones'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * issuedb IS PROVIDED BY Allann Jones ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Allann Jones OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "main_dialog.h"
#include "project_new_dialog.h"
#include "project_main_dialog.h"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


GtkWidget *project_new_dialog_entry_name = NULL;
GtkWidget *project_new_dialog_window = NULL;
GtkWidget *project_new_dialog_combo_priority = NULL;
GtkWidget *project_new_dialog_combo_os = NULL;
GtkWidget *project_new_dialog_combo_status = NULL;
GtkWidget *project_new_dialog_entry_version = NULL;
GtkWidget *project_new_dialog_text_view = NULL;
GtkWidget *project_new_dialog_description_scroll;


int project_new_dialog_open(GtkWidget *parent) {
  project_new_dialog_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_modal(GTK_WINDOW(project_new_dialog_window), TRUE);

  gtk_window_set_default_size(GTK_WINDOW(project_new_dialog_window), 600, 200);

  gtk_window_set_title(GTK_WINDOW(project_new_dialog_window), "New Project");

  gtk_container_set_border_width(GTK_CONTAINER(project_new_dialog_window), 5);

  gtk_window_set_type_hint(GTK_WINDOW(project_new_dialog_window), GDK_WINDOW_TYPE_HINT_DIALOG);

  gtk_window_set_transient_for(GTK_WINDOW(project_new_dialog_window), GTK_WINDOW(parent));

  gtk_window_set_position(GTK_WINDOW(project_new_dialog_window), GTK_WIN_POS_CENTER_ON_PARENT);


  GtkWidget *box_top;

  box_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(project_new_dialog_window), box_top);

  // Grid sizer

  GtkWidget *grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(box_top), grid, TRUE, TRUE, 5);

  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);

  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  // Issue name

  GtkWidget *label = gtk_label_new("Project name: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  project_new_dialog_entry_name = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), project_new_dialog_entry_name, 1, 0, 1, 1);

  gtk_widget_set_hexpand(project_new_dialog_entry_name, TRUE);

  gtk_entry_set_width_chars(GTK_ENTRY(project_new_dialog_entry_name), 30);

  // Status

  label = gtk_label_new("Status: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

  project_new_dialog_combo_status = gtk_combo_box_new();
  gtk_grid_attach(GTK_GRID(grid), project_new_dialog_combo_status, 1, 1, 1, 1);

  // Data model

  GtkListStore *list_store_status = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  GtkTreeIter iter_status;

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Active", 1, "A", -1);

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Inactive", 1, "I", -1);

  gtk_combo_box_set_model(GTK_COMBO_BOX(project_new_dialog_combo_status), GTK_TREE_MODEL(list_store_status));

  GtkCellRenderer *status_name_renderer;

  status_name_renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(project_new_dialog_combo_status), status_name_renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(project_new_dialog_combo_status), status_name_renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_status));

  gtk_combo_box_set_active(GTK_COMBO_BOX(project_new_dialog_combo_status), 0);

  // Buttons

  GtkWidget *sizer_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_end(GTK_BOX(box_top), sizer_buttons, FALSE, FALSE, 3);

  // Close button

  GtkWidget *button_close = gtk_button_new_from_icon_name("window-close", GTK_ICON_SIZE_BUTTON);

  gtk_button_set_label(GTK_BUTTON(button_close), "Close");

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) close_16x16_xpm);
  GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);

  gtk_button_set_image(GTK_BUTTON(button_close), image);
  gtk_button_set_always_show_image(GTK_BUTTON(button_close), TRUE);

  gtk_box_pack_end(GTK_BOX(sizer_buttons), button_close, FALSE, FALSE, 3);

  // Save button

  GtkWidget *button_save = gtk_button_new_from_icon_name("edit-save", GTK_ICON_SIZE_BUTTON);

  gtk_button_set_label(GTK_BUTTON(button_save), "Save");

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) save_16x16_xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  gtk_button_set_image(GTK_BUTTON(button_save), image);
  gtk_button_set_always_show_image(GTK_BUTTON(button_save), TRUE);

  gtk_box_pack_end(GTK_BOX(sizer_buttons), button_save, FALSE, FALSE, 3);

  // Close window using button

  g_signal_connect(G_OBJECT(button_close), "clicked",
                   G_CALLBACK(on_project_new_button_close_clicked), G_OBJECT(project_new_dialog_window));

  // Save issue information

  g_signal_connect(G_OBJECT(button_save), "clicked",
                   G_CALLBACK(on_project_new_button_save_clicked), G_OBJECT(project_new_dialog_window));


  gtk_widget_show_all(project_new_dialog_window);


  //gtk_widget_grab_focus(GTK_WIDGET(project_new_dialog_combo_project));

  return 0;
}


void project_new_dialog_close()
{
  gtk_widget_destroy(GTK_WIDGET(project_new_dialog_window));
}


void on_project_new_button_close_clicked(GtkWidget *widget, gpointer data)
{
  project_new_dialog_close();
}


void on_project_new_button_save_clicked(GtkWidget *widget, gpointer data)
{
  project_new_dialog_save_data();
}


int project_new_dialog_save_data()
{
  g_print("project_new_dialog_save_data()\n");

  GtkTreeIter iter_status;

  // Check values

  if (g_utf8_strlen(gtk_entry_get_text(GTK_ENTRY(project_new_dialog_entry_name)), -1) <= 0) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(project_new_dialog_window),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The project name is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(project_new_dialog_combo_status), &iter_status)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(project_new_dialog_window),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The status is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  // Status

  gchar *status_id;

  GtkTreeModel *status_model = gtk_combo_box_get_model(GTK_COMBO_BOX(project_new_dialog_combo_status));

  gtk_tree_model_get(status_model, &iter_status, 1, &status_id, -1);

  g_print("Selected status: %s\n", status_id);


  // Mount SQL command

  char *error_message = 0;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    g_free(status_id);

    sqlite3_close(db);

    return -1;
  }

  gchar *sql_command = g_strdup_printf("INSERT INTO project "
                                       "(name, status) "
                                       "VALUES (\"%s\", \"%s\")",
                                       gtk_entry_get_text(GTK_ENTRY(project_new_dialog_entry_name)),
                                       status_id);

  g_print("SQL INSERT command: %s\n", sql_command);

  int rc = sqlite3_exec(db, sql_command, NULL, 0, &error_message);

  if (rc != SQLITE_OK) {
    gchar *user_message = g_strdup_printf("Database error: %s\n", error_message);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(project_new_dialog_window),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             user_message);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    sqlite3_free(error_message);

    sqlite3_free(user_message);
  }

  g_free(status_id);

  g_free(sql_command);

  sqlite3_close(db);

  // Reload projects list on project main dialog

  project_main_dialog_load_list();

  // Reload projects list on main dialog

  main_dialog_load_projects();

  // Close this dialog

  project_new_dialog_close();


  return 0;
}
