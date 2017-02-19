/*
 * @file issue_new_dialog.c
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
#include "issue_new_dialog.h"
#include "main_dialog.h"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


GtkWidget *entry_summary = NULL;
GtkWidget *window_issue_new = NULL;
GtkWidget *combo_project = NULL;
GtkWidget *combo_priority = NULL;
GtkWidget *combo_os = NULL;
GtkWidget *combo_status = NULL;
GtkWidget *entry_version = NULL;
GtkWidget *text_view = NULL;
GtkWidget *scroll;


int issue_new_dialog_open(GtkWidget *parent) {
  window_issue_new = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_modal(GTK_WINDOW(window_issue_new), TRUE);

  gtk_window_set_default_size(GTK_WINDOW(window_issue_new), 750, 450);

  gtk_window_set_title(GTK_WINDOW(window_issue_new), "New Issue");

  gtk_container_set_border_width(GTK_CONTAINER(window_issue_new), 5);

  gtk_window_set_type_hint(GTK_WINDOW(window_issue_new), GDK_WINDOW_TYPE_HINT_DIALOG);

  gtk_window_set_transient_for(GTK_WINDOW(window_issue_new), GTK_WINDOW(parent));

  gtk_window_set_position(GTK_WINDOW(window_issue_new), GTK_WIN_POS_CENTER_ON_PARENT);


  GtkWidget *box_top;

  box_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window_issue_new), box_top);

  // Grid

  GtkWidget *grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(box_top), grid, TRUE, TRUE, 5);

  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);

  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  // Project

  GtkWidget *label = gtk_label_new("Project: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  // Create combo box with store as data source
  combo_project = gtk_combo_box_new();
  gtk_grid_attach(GTK_GRID(grid), combo_project, 1, 0, 1, 1);

  // Data model
  GtkListStore *list_store_projects = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);

  gtk_combo_box_set_model(GTK_COMBO_BOX(combo_project), GTK_TREE_MODEL(list_store_projects));

  GtkCellRenderer *renderer;

  renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_project), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_project), renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_projects));


  // Issue name

  label = gtk_label_new("Summary: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

  entry_summary = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), entry_summary, 1, 1, 1, 1);

  gtk_widget_set_hexpand(entry_summary, TRUE);

  gtk_entry_set_width_chars(GTK_ENTRY(entry_summary), 30);

  // Version

  label = gtk_label_new("Version: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

  entry_version = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid), entry_version, 1, 2, 1, 1);

  gtk_entry_set_width_chars(GTK_ENTRY(entry_version), 15);

  // Priority

  label = gtk_label_new("Priority: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);

  combo_priority = gtk_combo_box_new();
  gtk_grid_attach(GTK_GRID(grid), combo_priority, 1, 3, 1, 1);

  // Data model
  GtkListStore *list_store_priorities = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
  GtkTreeIter iter_priorities;

  gtk_list_store_append(list_store_priorities, &iter_priorities);
  gtk_list_store_set(list_store_priorities, &iter_priorities, 0, "Low", 1, 1, -1);

  gtk_list_store_append(list_store_priorities, &iter_priorities);
  gtk_list_store_set(list_store_priorities, &iter_priorities, 0, "Medium", 1, 2, -1);

  gtk_list_store_append(list_store_priorities, &iter_priorities);
  gtk_list_store_set(list_store_priorities, &iter_priorities, 0, "High", 1, 3, -1);

  gtk_list_store_append(list_store_priorities, &iter_priorities);
  gtk_list_store_set(list_store_priorities, &iter_priorities, 0, "Critical", 1, 4, -1);

  gtk_combo_box_set_model(GTK_COMBO_BOX(combo_priority), GTK_TREE_MODEL(list_store_priorities));

  GtkCellRenderer *priority_name_renderer;

  priority_name_renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_priority), priority_name_renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_priority), priority_name_renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_priorities));

  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_priority), 0);

  // Operating system

  label = gtk_label_new("OS: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);

  combo_os = gtk_combo_box_new();
  gtk_grid_attach(GTK_GRID(grid), combo_os, 1, 4, 1, 1);

  // Data model
  GtkListStore *list_store_os = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
  GtkTreeIter iter_os;

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "Linux", 1, 1, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "Mac OS X", 1, 2, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "Microsoft Windows", 1, 3, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "FreeBSD", 1, 4, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "OpenBSD", 1, 5, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "Android", 1, 6, -1);


  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "iOS", 1, 7, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "Other operating system", 1, 999, -1);

  gtk_list_store_append(list_store_os, &iter_os);
  gtk_list_store_set(list_store_os, &iter_os, 0, "All operating systems", 1, 1000, -1);

  gtk_combo_box_set_model(GTK_COMBO_BOX(combo_os), GTK_TREE_MODEL(list_store_os));

  GtkCellRenderer *os_name_renderer;

  os_name_renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_os), os_name_renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_os), os_name_renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_os));

  // Status

  label = gtk_label_new("Status: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 5, 1, 1);

  combo_status = gtk_combo_box_new();
  gtk_grid_attach(GTK_GRID(grid), combo_status, 1, 5, 1, 1);

  // Data model
  GtkListStore *list_store_status = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  GtkTreeIter iter_status;

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Submited", 1, "S", -1);

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Open", 1, "O", -1);

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Fixed", 1, "F", -1);

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "No Plan to Fix", 1, "N", -1);

  gtk_list_store_append(list_store_status, &iter_status);
  gtk_list_store_set(list_store_status, &iter_status, 0, "Closed", 1, "C", -1);

  gtk_combo_box_set_model(GTK_COMBO_BOX(combo_status), GTK_TREE_MODEL(list_store_status));

  GtkCellRenderer *status_name_renderer;

  status_name_renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_status), status_name_renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_status), status_name_renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_status));

  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_status), 0);

  // Description

  label = gtk_label_new("Description: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 6, 1, 1);

  // scrolled window

  scroll = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  gtk_grid_attach(GTK_GRID(grid), scroll, 1, 6, 1, 1);

  // text view

  GtkTextBuffer *text_buffer;

  text_view = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(scroll), text_view);

  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);

  gtk_widget_set_size_request(GTK_WIDGET(text_view), -1, 30);

  text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  gtk_text_buffer_set_text(text_buffer, "", -1);


  // Buttons

  GtkWidget *sizer_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start(GTK_BOX(box_top), sizer_buttons, FALSE, FALSE, 5);

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
                   G_CALLBACK(on_issue_new_button_close_clicked), G_OBJECT(window_issue_new));

  // Save issue information

  g_signal_connect(G_OBJECT(button_save), "clicked",
                   G_CALLBACK(on_issue_new_button_save_clicked), G_OBJECT(window_issue_new));


  gtk_widget_show_all(window_issue_new);


  issue_new_dialog_load_projects();


  gtk_widget_grab_focus(GTK_WIDGET(combo_project));


  return 0;
}


void issue_new_dialog_close()
{
  gtk_widget_destroy(GTK_WIDGET(window_issue_new));
}


int issue_new_dialog_load_projects()
{
  g_print("issue_new_dialog_load_projects()\n");

  char *error_message = 0;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);

    return -1;
  }

  const gchar sql_command[] = "SELECT project_id, name FROM project WHERE status = \"A\"";

  int rc = sqlite3_exec(db, sql_command, project_query_callback, 0, &error_message);

  if (rc != SQLITE_OK) {
    g_print("SQL error: %s\n", error_message);

    sqlite3_free(error_message);
  }

  sqlite3_close(db);

  return 0;
}


int project_query_callback(void *NotUsed, int argc, char **argv, char **col_name)
{
  //g_print("Project ID = %s\n", argv[0] ? argv[0] : "NULL");
  //g_print("Project Name = %s\n", argv[1] ? argv[1] : "NULL");

  GtkListStore *store;
  GtkTreeIter iter;

  //g_print("Project ID = %s\n", argv[0] ? argv[0] : "NULL");
  //g_print("Project Name = %s\n", argv[1] ? argv[1] : "NULL");

  store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(combo_project)));

  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store, &iter, 0, argv[1], 1, atoi(argv[0]), -1);

  return 0;
}


void on_issue_new_button_close_clicked(GtkWidget *widget, gpointer data)
{
  issue_new_dialog_close();
}


void on_issue_new_button_save_clicked(GtkWidget *widget, gpointer data)
{
  issue_new_dialog_save_data();
}


int issue_new_dialog_save_data()
{
  g_print("issue_new_dialog_save_data()\n");

  GtkTreeIter iter_project;
  GtkTreeIter iter_status;
  GtkTreeIter iter_os;
  GtkTreeIter iter_priority;

  //GtkWidget *text_view = NULL;

  // Check values

  if (g_utf8_strlen(gtk_entry_get_text(GTK_ENTRY(entry_summary)), -1) <= 0) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The summary is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (g_utf8_strlen(gtk_entry_get_text(GTK_ENTRY(entry_version)), -1) <= 0) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The version is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_project), &iter_project)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The project is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_priority), &iter_priority)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The priority is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_os), &iter_os)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The operating system is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo_status), &iter_status)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "The status is needed.");

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    return -1;
  }

  // Project value

  int project_id;

  GtkTreeModel *project_model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_project));

  gtk_tree_model_get(project_model, &iter_project, 1, &project_id, -1);

  g_print("Selected Project: %d\n", project_id);

  // Priority value

  int priority_id;

  GtkTreeModel *priority_model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_priority));

  gtk_tree_model_get(priority_model, &iter_priority, 1, &priority_id, -1);

  g_print("Selected priority: %d\n", priority_id);

  // OS value

  int os_id;

  GtkTreeModel *os_model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_os));

  gtk_tree_model_get(os_model, &iter_os, 1, &os_id, -1);

  g_print("Selected os: %d\n", os_id);

  // Project value

  gchar *status_id;

  GtkTreeModel *status_model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_status));

  gtk_tree_model_get(status_model, &iter_status, 1, &status_id, -1);

  g_print("Selected status: %s\n", status_id);

  // Description value

  GtkTextIter iter_start;
  GtkTextIter iter_end;

  gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &iter_start);

  gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)), &iter_end);

  g_print("Descrição: %s\n",
          gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)),
                                   &iter_start,
                                   &iter_end,
                                   FALSE));

  // Mount SQL command

  char *error_message = 0;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);

    return -1;
  }

  gchar *sql_command = g_strdup_printf("INSERT INTO issue "
                                       "(summary, project_id, priority, status, platform, version, description) "
                                       "VALUES (\"%s\", \"%d\", \"%d\", \"%s\", \"%d\", \"%s\", \"%s\")",
                                       gtk_entry_get_text(GTK_ENTRY(entry_summary)),
                                       project_id,
                                       priority_id,
                                       status_id,
                                       os_id,
                                       gtk_entry_get_text(GTK_ENTRY(entry_version)),
                                       gtk_text_buffer_get_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view)),
                                                                &iter_start,
                                                                &iter_end,
                                                                FALSE));

  g_print("SQL INSERT command: %s\n", sql_command);

  int rc = sqlite3_exec(db, sql_command, NULL, 0, &error_message);

  if (rc != SQLITE_OK) {
    gchar *user_message = g_strdup_printf("Database error: %s\n", error_message);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_issue_new),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             user_message);

    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

    sqlite3_free(error_message);

    sqlite3_free(user_message);
  }

  g_free(sql_command);

  sqlite3_close(db);

  // Reload the list

  main_dialog_load_issue_list();

  // Close this dialog

  issue_new_dialog_close();


  return 0;
}
