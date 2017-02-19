/*
 * @file project_main_dialog.c
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
#include "project_main_dialog.h"
#include "project_edit_dialog.h"
#include "project_new_dialog.h"
#include "pixmaps/add_16x16.xpm"
#include "pixmaps/refresh_16x16.xpm"
#include "pixmaps/close_16x16.xpm"


// Tree view list
GtkWidget *project_main_treeview;
// Window
GtkWidget *project_main_window = NULL;


int project_main_dialog_open(GtkWidget *parent) {
  project_main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_modal(GTK_WINDOW(project_main_window), TRUE);

  gtk_window_set_default_size(GTK_WINDOW(project_main_window), 750, 450);

  gtk_window_set_title(GTK_WINDOW(project_main_window), "Projects");

  gtk_container_set_border_width(GTK_CONTAINER(project_main_window), 0);

  gtk_window_set_type_hint(GTK_WINDOW(project_main_window), GDK_WINDOW_TYPE_HINT_DIALOG);

  gtk_window_set_transient_for(GTK_WINDOW(project_main_window), GTK_WINDOW(parent));

  gtk_window_set_position(GTK_WINDOW(project_main_window), GTK_WIN_POS_CENTER_ON_PARENT);


  GtkWidget *box_top;

  box_top = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(project_main_window), box_top);


  // Toolbar

  GtkWidget *toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  gtk_box_pack_start(GTK_BOX(box_top), toolbar, FALSE, FALSE, 0);

  // Toolbar item - New issue

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) add_16x16_xpm);
  GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_new = gtk_tool_button_new(image, "New project");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_new), "Register a new project");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_new, 0);

  g_signal_connect(G_OBJECT(toolbar_item_new), "clicked", G_CALLBACK(project_main_dialog_on_new_request), NULL);

  // Separator

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), 1);

  // Toolbar item - Refresh window

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) refresh_16x16_xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_refresh = gtk_tool_button_new(image, "Refresh");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_refresh), "Refresh project list");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_refresh, 2);

  g_signal_connect(G_OBJECT(toolbar_item_refresh), "clicked", G_CALLBACK(project_main_dialog_on_refresh_request), NULL);

  // Separator

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), 3);

  // Toolbar item - Close window

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) close_16x16_xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_close = gtk_tool_button_new(image, "Close");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_close), "Close window");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_close, 4);

  g_signal_connect(G_OBJECT(toolbar_item_close), "clicked", G_CALLBACK(on_project_main_toolbar_close_clicked), NULL);

  // List

  project_main_treeview = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(project_main_treeview), FALSE);

  gtk_box_pack_start(GTK_BOX(box_top), project_main_treeview, TRUE, TRUE, 0);

  project_main_dialog_init_list(project_main_treeview);

  //

  gtk_widget_show_all(project_main_window);


  project_main_dialog_load_list();


	return 0;
}


void project_main_dialog_close()
{
  gtk_widget_destroy(GTK_WIDGET(project_main_window));

  main_dialog_load_projects();
}


void on_project_main_toolbar_close_clicked(GtkWidget *widget, gpointer data)
{
  project_main_dialog_close();
}


void project_main_dialog_init_list(GtkWidget *treeview)
{
  GtkCellRenderer *renderer;
  GtkListStore *store;

  // Tree view configuration

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(treeview), FALSE);
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

  // Render

  renderer = gtk_cell_renderer_text_new();

  GtkTreeViewColumn *column;

  column = gtk_tree_view_column_new_with_attributes("Project", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  column = gtk_tree_view_column_new_with_attributes("Issues", renderer, "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  column = gtk_tree_view_column_new_with_attributes("Status", renderer, "text", 2, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);


  // List model

  store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);

  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

  g_object_unref(store);


  // Setup the selection handler

  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

  // Sort columns

  GtkTreeSortable *sortable = GTK_TREE_SORTABLE(store);

  gtk_tree_sortable_set_sort_func(sortable,
                                  PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID,
                                  project_main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID),
                                  NULL);
  gtk_tree_sortable_set_sort_func(sortable,
                                  PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID,
                                  project_main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID),
                                  NULL);
  gtk_tree_sortable_set_sort_func(sortable,
                                  PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID,
                                  project_main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID),
                                  NULL);

  // List callbacks

  g_signal_connect(G_OBJECT(treeview), "row-activated",
                   G_CALLBACK(project_main_dialog_on_list_row_activated),
                   NULL);
}


gint project_main_dialog_list_sort_callback(GtkTreeModel *model,
                                            GtkTreeIter  *a,
                                            GtkTreeIter  *b,
                                            gpointer     userdata)
{
  gint sortcol = GPOINTER_TO_INT(userdata);
  gint ret = 0;

  switch (sortcol) {
    case PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID:
      {
        gchar *string1, *string2;

        gtk_tree_model_get(model, a, PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, &string1, -1);
        gtk_tree_model_get(model, b, PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, &string2, -1);

        if (string1 == NULL || string2 == NULL) {
          if (string1 != NULL || string2 != NULL) {
            ret = (string1 == NULL) ? -1 : 1;
          }

          // both equal => ret = 0
        }
        else {
          ret = g_strcmp0(string1, string2);
        }

        if (string1 != NULL)
          g_free(string1);

        if (string2 != NULL)
          g_free(string2);
      }

      break;

    case PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID:
      {
        guint count1, count2;

        gtk_tree_model_get(model, a, PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID, &count1, -1);
        gtk_tree_model_get(model, b, PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID, &count2, -1);

        if (count1 != count2) {
          ret = (count1 > count2) ? 1 : -1;
        }
        else {
          ret = 0;
        }
      }

      break;

    case PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID:
      {
        gchar *string1, *string2;

        gtk_tree_model_get(model, a, PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID, &string1, -1);
        gtk_tree_model_get(model, b, PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID, &string2, -1);

        if (string1 == NULL || string2 == NULL) {
          if (string1 != NULL || string2 != NULL) {
            ret = (string1 == NULL) ? -1 : 1;
          }

          // both equal => ret = 0
        }
        else {
          ret = g_strcmp0(string1, string2);
        }

        if (string1 != NULL)
          g_free(string1);

        if (string2 != NULL)
          g_free(string2);
      }

      break;

    //default:
    //  g_return_val_if_reached(0);
  }

  return ret;
}


void project_main_dialog_on_list_row_activated(GtkTreeView *treeview,
                                               GtkTreePath *path,
                                               GtkTreeViewColumn *col,
                                               gpointer userdata)
{
  GtkTreeModel *model;
  GtkTreeIter iter;

  g_print("main_dialog_on_list_row_activated()\n");

  model = gtk_tree_view_get_model(treeview);

  if (gtk_tree_model_get_iter(model, &iter, path)) {
    int project_id;

    gtk_tree_model_get(model, &iter, 3, &project_id, -1);

    int rc = project_edit_dialog_open(project_main_window, project_id);

    if (rc != 0) {
      // @todo
    }
  }
}


int project_main_dialog_issue_list_query_callback(void *NotUsed, int argc, char **argv, char **col_name)
{
  GtkListStore *store;
  GtkTreeIter iter;
  gchar *status;

  g_print("project_main_dialog_issue_list_query_callback()\n");

  if (argv[2]) {
    if (g_strcmp0(argv[2], "A") == 0) {
      status = g_strdup("Active");
    }
    else if (g_strcmp0(argv[2], "I") == 0) {
      status = g_strdup("Inactive");
    }
  }
  else {
    status = g_strdup("Unspecified");
  }

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(project_main_treeview)));

  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store,
                     &iter,
                     0, argv[1],
                     1, atoi(argv[3]),
                     2, status,
                     3, atoi(argv[0]),
                     -1);

  g_free(status);

  return 0;
}


int project_main_dialog_load_list()
{
  // Clear model

  GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(project_main_treeview)));

  gtk_list_store_clear(store);

  // Query database and fill the model

  char *error_message = NULL;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    database_close();

    return -1;
  }

  const gchar sql_command[] = "SELECT p.project_id, p.name, p.status, "
                                    "(SELECT COUNT(*) FROM issue i WHERE i.project_id = p.project_id) "
                              "FROM project p "
                              "ORDER BY p.name, p.status";

  g_print("query command: %s\n", sql_command);

  int rc = sqlite3_exec(db, sql_command, project_main_dialog_issue_list_query_callback, 0, &error_message);

  if (rc != SQLITE_OK) {
    g_print("SQL error: %s\n", error_message);

    sqlite3_free(error_message);
  }

  database_close();

  return 0;
}


void project_main_dialog_on_new_request(GtkWidget *widget, gpointer data)
{
  int rc = project_new_dialog_open(project_main_window);

  if (rc != 0) {
    // @todo
  }
}


void project_main_dialog_on_refresh_request(GtkWidget *widget, gpointer data)
{
  project_main_dialog_load_list();
}
