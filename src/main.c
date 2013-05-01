/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2013 Allann Jones <allanjos[at]gmail.com>
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
 * bugtracker IS PROVIDED BY Allann Jones ``AS IS'' AND ANY EXPRESS
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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "sqlite3.h"
#include "database.h"
#include "issue_new_dialog.h"
#include "issue_edit_dialog.h"
#include "project_main_dialog.h"
#include "main.h"
#include "pixmaps/bug_add_16x16.xpm"
#include "pixmaps/project_16x16.xpm"
#include "pixmaps/refresh_16x16.xpm"


GtkWidget *main_dialog_window = NULL;
GtkWidget *treeview = NULL;
GtkWidget *main_dialog_status_bar = NULL;
GtkWidget *main_dialog_toolbar_combo_projects = NULL;

static gchar *app_exec_path = NULL;
static gchar *app_db_path = NULL;

static int main_dialog_issues_count = 0;
static short main_dialog_view_fixed_issues_enabled = FALSE;


int main(int argc, char *argv[])
{
  main_dialog_view_fixed_issues_enabled = FALSE;

  // Database path

  printf("argv[0]: %s\n", argv[0]);

  app_exec_path = g_path_get_dirname(argv[0]);

  printf("Execution directory: %s\n", app_exec_path);

  // Database path

  app_db_path = g_build_path(G_DIR_SEPARATOR_S, (const gchar *) app_exec_path, (const gchar *) "issues.db", NULL);

  database_set_data_path(app_db_path);

  // UI

  GtkWidget *box_top;

  gtk_init(&argc, &argv);

  // Main window

  main_dialog_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(main_dialog_window), GTK_WIN_POS_CENTER);

  gtk_window_set_default_size(GTK_WINDOW(main_dialog_window), 950, 600);

  gtk_window_set_title(GTK_WINDOW(main_dialog_window), "Bugtracker");

  g_signal_connect(main_dialog_window, "delete-event", G_CALLBACK (main_dialog_delete_event), NULL);

  g_signal_connect(main_dialog_window, "destroy", G_CALLBACK(main_dialog_on_destroy_request), NULL);

  gtk_container_set_border_width(GTK_CONTAINER(main_dialog_window), 0);

  // Main sizer

  box_top = gtk_vbox_new(FALSE, 0);

  gtk_container_add(GTK_CONTAINER(main_dialog_window), box_top);

  // Menu bar

  GtkWidget *menu_bar;

  menu_bar = gtk_menu_bar_new();

  gtk_box_pack_start(GTK_BOX(box_top), menu_bar, FALSE, FALSE, 0);

  // File menu

  GtkWidget *menu_file = gtk_menu_new();

  GtkWidget *menu_item_file = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_file), menu_file);

  GtkWidget *menu_item_quit = gtk_menu_item_new_with_label("Quit");

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_item_quit);
  g_signal_connect(G_OBJECT(menu_item_quit), "activate", G_CALLBACK(main_app_term), NULL);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item_file);

  // View menu

  GtkWidget *menu_view = gtk_menu_new();

  GtkWidget *menu_item_view = gtk_menu_item_new_with_label("View");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_view), menu_view);

  GtkWidget *menu_shell_item_view_fixed_issues = gtk_check_menu_item_new_with_label("View fixed issues");
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_shell_item_view_fixed_issues), FALSE);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), menu_shell_item_view_fixed_issues);

  g_signal_connect(G_OBJECT(menu_shell_item_view_fixed_issues), "activate", G_CALLBACK(main_dialog_on_menu_shell_item_view_fixed_issues), NULL);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item_view);

  // Help menu

  GtkWidget *menu_help = gtk_menu_new();

  GtkWidget *menu_item_help = gtk_menu_item_new_with_label("Help");
  GtkWidget *menu_item_about = gtk_menu_item_new_with_label("About");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_help), menu_help);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_help), menu_item_about);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item_help);

  g_signal_connect(G_OBJECT(menu_item_about), "activate", G_CALLBACK(gtk_main_about_dialog_callback), NULL);


  // Toolbar

  GtkWidget *toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

  gtk_box_pack_start(GTK_BOX(box_top), toolbar, FALSE, FALSE, 0);

  // Toolbar item - New issue

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) bug_add_16x16_xpm);
  GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_new = gtk_tool_button_new(image, "New issue");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_new), "Register a new project issue");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_new, 0);

  g_signal_connect(G_OBJECT(toolbar_item_new), "clicked", G_CALLBACK(main_dialog_on_issue_new_request), NULL);

  // Separator

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), 1);

  // Toolbar item - New project

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) project_16x16_xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_projects = gtk_tool_button_new(image, "Projects");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_projects), "Projects");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_projects, 2);

  g_signal_connect(G_OBJECT(toolbar_item_projects), "clicked", G_CALLBACK(main_dialog_on_projects_main_dialog_request), NULL);

  // Separator

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), 3);

  // Toolbar item - Refresh window

  pixbuf = gdk_pixbuf_new_from_xpm_data((const char **) refresh_16x16_xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  GtkToolItem *toolbar_item_refresh = gtk_tool_button_new(image, "Refresh");

  gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(toolbar_item_refresh), "Refresh issues list");

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbar_item_refresh, 4);

  g_signal_connect(G_OBJECT(toolbar_item_refresh), "clicked", G_CALLBACK(main_dialog_on_refresh_request), NULL);

  // Separator

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), gtk_separator_tool_item_new(), 5);

  // Projects combo

  main_dialog_toolbar_combo_projects = gtk_combo_box_new();

  GtkToolItem *tool_item = gtk_tool_item_new();
  gtk_container_add(GTK_CONTAINER(tool_item), main_dialog_toolbar_combo_projects);

  // Data model

  GtkListStore *list_store_projects = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);

  gtk_combo_box_set_model(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects), GTK_TREE_MODEL(list_store_projects));

  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(main_dialog_toolbar_combo_projects), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(main_dialog_toolbar_combo_projects), renderer, "text", 0, NULL);

  g_object_unref(G_OBJECT(list_store_projects));

  g_signal_connect(G_OBJECT(main_dialog_toolbar_combo_projects), "changed", G_CALLBACK(main_dialog_on_project_change), NULL);

  // Add combo to the toolbar

  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_item, 6);


  // List

  treeview = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);

  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), treeview, TRUE, TRUE, 0);


  gtk_container_add(GTK_CONTAINER(box_top), vbox);

  // Status bar

  main_dialog_status_bar = gtk_statusbar_new();

  gtk_box_pack_start(GTK_BOX(box_top), main_dialog_status_bar, FALSE, FALSE, 0);


  gtk_widget_grab_focus(GTK_WIDGET(treeview));


  issue_new_dialog_init_list(treeview);

  main_dialog_load_projects();

  main_dialog_load_issue_list();


  // Show widgets

  gtk_widget_show_all(main_dialog_window);


  gtk_main();


  return 0;
}


void main_app_close()
{
  printf("main_app_close()\n");

  gtk_main_quit();
}


void main_app_term()
{
  printf("main_app_term()\n");

  database_term();

  if (app_db_path != NULL) {
    g_free(app_db_path);

    app_db_path = NULL;
  }

  if (app_exec_path != NULL) {
    g_free(app_exec_path);

    app_exec_path = NULL;
  }
}


gboolean main_dialog_delete_event(GtkWidget *widget,
                             GdkEvent *event,
                             gpointer data)
{
  g_print("delete event occurred\n");

  main_app_close();

  // FALSE: the main window will be destroyed with a "delete-event".

  return FALSE;
}

void main_dialog_on_destroy_request(GtkWidget *widget, gpointer data)
{
  main_app_term();
}

void issue_new_dialog_init_list(GtkWidget *treeview)
{
  GtkCellRenderer *renderer;
  GtkListStore *store;

  // Tree view configuration

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
  gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(treeview), FALSE);
  gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_BOTH);

  // Render

  renderer = gtk_cell_renderer_text_new();

  GtkTreeViewColumn *column;

  column = gtk_tree_view_column_new_with_attributes("Issue", renderer, "text", MAIN_DIALOG_LIST_COLUMN_ISSUE_ID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, MAIN_DIALOG_LIST_COLUMN_ISSUE_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  column = gtk_tree_view_column_new_with_attributes("Project", renderer, "text", MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, MAIN_DIALOG_LIST_COLUMN_PROJECT_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  column = gtk_tree_view_column_new_with_attributes("Priority", renderer, "text", MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  column = gtk_tree_view_column_new_with_attributes("Status", renderer, "text", MAIN_DIALOG_LIST_COLUMN_STATUS_ID, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  gtk_tree_view_column_set_sort_column_id(column, MAIN_DIALOG_LIST_COLUMN_STATUS_ID);
  gtk_tree_view_column_set_resizable(column, TRUE);

  // List model

  store = gtk_list_store_new(MAIN_DIALOG_LIST_N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

  g_object_unref(store);


  // Setup the selection handler

  GtkTreeSelection *select;

  select = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);

  // Sort columns

  GtkTreeSortable *sortable = GTK_TREE_SORTABLE(store);

  gtk_tree_sortable_set_sort_func(sortable,
                                  MAIN_DIALOG_LIST_COLUMN_ISSUE_ID,
                                  main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(MAIN_DIALOG_LIST_COLUMN_ISSUE_ID),
                                  NULL);
  gtk_tree_sortable_set_sort_func(sortable,
                                  MAIN_DIALOG_LIST_COLUMN_PROJECT_ID,
                                  main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(MAIN_DIALOG_LIST_COLUMN_PROJECT_ID),
                                  NULL);
  gtk_tree_sortable_set_sort_func(sortable,
                                  MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID,
                                  main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID),
                                  NULL);
  gtk_tree_sortable_set_sort_func(sortable,
                                  MAIN_DIALOG_LIST_COLUMN_STATUS_ID,
                                  main_dialog_list_sort_callback,
                                  GINT_TO_POINTER(MAIN_DIALOG_LIST_COLUMN_STATUS_ID),
                                  NULL);

  // List callbacks

  g_signal_connect(G_OBJECT(treeview), "row-activated",
                   G_CALLBACK(main_dialog_on_list_row_activated),
                   NULL);
}


void main_dialog_status_bar_update()
{
  gchar *rows_count_str = g_strdup_printf("Issues: %d", main_dialog_issues_count);

  gtk_statusbar_remove_all(GTK_STATUSBAR(main_dialog_status_bar), 0);

  gtk_statusbar_push(GTK_STATUSBAR(main_dialog_status_bar), 0, rows_count_str);

  g_free(rows_count_str);
}


gint main_dialog_list_sort_callback(GtkTreeModel *model,
                                    GtkTreeIter  *a,
                                    GtkTreeIter  *b,
                                    gpointer      userdata)
{
  gint sortcol = GPOINTER_TO_INT(userdata);
  gint ret = 0;

  switch (sortcol) {
    case MAIN_DIALOG_LIST_COLUMN_ISSUE_ID:
      {
        gchar *name1, *name2;

        gtk_tree_model_get(model, a, MAIN_DIALOG_LIST_COLUMN_ISSUE_ID, &name1, -1);
        gtk_tree_model_get(model, b, MAIN_DIALOG_LIST_COLUMN_ISSUE_ID, &name2, -1);

        if (name1 == NULL || name2 == NULL) {
          if (name1 == NULL) {
            if (name2 == NULL) {
              ret = 0;
            }
            else {
              ret = -1;
            }
          }
          else {
            ret = 1;
          }
        }
        else {
          ret = g_strcmp0(name1, name2);
        }

        if (name1 != NULL)
          g_free(name1);

        if (name2 != NULL)
          g_free(name2);
      }

      break;

    case MAIN_DIALOG_LIST_COLUMN_PROJECT_ID:
      {
        gchar *name1, *name2;

        gtk_tree_model_get(model, a, MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, &name1, -1);
        gtk_tree_model_get(model, b, MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, &name2, -1);

        if (name1 == NULL || name2 == NULL) {
          if (name1 != NULL || name2 != NULL) {
            ret = (name1 == NULL) ? -1 : 1;
          }
          else {
            ret = 0;
          }
        }
        else {
          ret = g_strcmp0(name1, name2);
        }

        if (name1 != NULL)
          g_free(name1);

        if (name2 != NULL)
          g_free(name2);
      }

      break;

    case MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID:
      {
        gchar *name1, *name2;

        gtk_tree_model_get(model, a, MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID, &name1, -1);
        gtk_tree_model_get(model, b, MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID, &name2, -1);

        if (name1 == NULL || name2 == NULL) {
          if (name1 != NULL || name2 != NULL) {
            ret = (name1 == NULL) ? -1 : 1;
          }
          else {
            ret = 0;
          }
        }
        else {
          ret = g_strcmp0(name1, name2);
        }

        if (name1 != NULL)
          g_free(name1);

        if (name2 != NULL)
          g_free(name2);
      }

      break;

    case MAIN_DIALOG_LIST_COLUMN_STATUS_ID:
      {
        gchar *name1, *name2;

        gtk_tree_model_get(model, a, MAIN_DIALOG_LIST_COLUMN_STATUS_ID, &name1, -1);
        gtk_tree_model_get(model, b, MAIN_DIALOG_LIST_COLUMN_STATUS_ID, &name2, -1);

        if (name1 == NULL || name2 == NULL) {
          if (name1 != NULL || name2 != NULL) {
            ret = (name1 == NULL) ? -1 : 1;
          }
          else {
            ret = 0;
          }
        }
        else {
          ret = g_strcmp0(name1, name2);
        }

        if (name1 != NULL)
          g_free(name1);

        if (name2 != NULL)
          g_free(name2);
      }

      break;

    //default:
    //  g_return_val_if_reached(0);
  }

  return ret;
}


int main_dialog_load_issue_list()
{
  g_print("main_dialog_load_issue_list()\n");

  // Clear model

  GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

  gtk_list_store_clear(store);


  main_dialog_issues_count = 0;


  main_dialog_status_bar_update();


  // Query database and fill the model

  char *error_message = NULL;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    database_close();

    return -1;
  }

  gchar *sql_command;
  gchar *project_filter = NULL;

  // Project

  GtkTreeIter iter_project;
  int project_id = 0;

  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects), &iter_project)) {
    GtkTreeModel *project_model = gtk_combo_box_get_model(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects));

    gtk_tree_model_get(project_model, &iter_project, 1, &project_id, -1);

    if (main_dialog_view_fixed_issues_enabled) {
      project_filter = g_strdup_printf("WHERE i.project_id = %d ", project_id);
    }
    else {
      project_filter = g_strdup_printf("AND i.project_id = %d ", project_id);
    }
  }

  // SQL command

  sql_command = g_strdup_printf("SELECT i.issue_id, i.summary, i.priority, p.name AS project_name, i.status "
                                "FROM issue i "
                                "LEFT JOIN project p ON (i.project_id = p.project_id) "
                                "%s"
                                "%s"
                                "ORDER BY i.status, i.priority DESC, i.register_timestamp DESC, i.summary",
                                (main_dialog_view_fixed_issues_enabled ? "" : "WHERE i.status != 'F' "),
                                (project_id > 0 ? project_filter : ""));

  g_print("query command: %s\n", sql_command);

  g_free(project_filter);

  // SQL query execution

  int rc = sqlite3_exec(db, sql_command, main_dialog_issue_list_query_callback, 0, &error_message);

  if (rc != SQLITE_OK) {
    g_print("SQL error: %s\n", error_message);

    sqlite3_free(error_message);
  }

  g_free(sql_command);

  database_close();

  return 0;
}

int main_dialog_issue_list_query_callback(void *NotUsed, int argc, char **argv, char **col_name)
{
  GtkListStore *store;
  GtkTreeIter iter;
  gchar *status = NULL;
  gchar *priority = NULL;

  g_print("main_dialog_issue_list_query_callback()\n");

  ++main_dialog_issues_count;
  
  if (argv[2]) {
    switch (atoi(argv[2])) {
      case 1:
        priority = g_strdup("Low");
        break;
      case 2:
        priority = g_strdup("Medium");
        break;
      case 3:
        priority = g_strdup("High");
        break;
      case 4:
        priority = g_strdup("Critical");
        break;
      default:
        priority = g_strdup("Unspecified");
    }
  }

  if (argv[4]) {
    if (g_strcmp0(argv[4], "S") == 0) {
      status = g_strdup("Submited");
    }
    else if (g_strcmp0(argv[4], "O") == 0) {
      status = g_strdup("Open");
    }
    else if (g_strcmp0(argv[4], "F") == 0) {
      status = g_strdup("Fixed");
    }
    else if (g_strcmp0(argv[4], "N") == 0) {
      status = g_strdup("No Plan to Fix");
    }
    else if (g_strcmp0(argv[4], "C") == 0) {
      status = g_strdup("Closed");
    }
  }
  else {
    status = g_strdup("Unspecified");
  }

  g_print("issue ID = %s\n", argv[0] ? argv[0] : "NULL");
  //g_print("Project Name = %s\n", argv[1] ? argv[1] : "NULL");

  //gtk_combo_box_append_text(GTK_COMBO_BOX(combo_project), argv[1]);

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store,
                     &iter,
                     MAIN_DIALOG_LIST_COLUMN_ISSUE_ID, argv[1],
                     MAIN_DIALOG_LIST_COLUMN_PROJECT_ID, argv[3],
                     MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID, priority,
                     MAIN_DIALOG_LIST_COLUMN_STATUS_ID, status,
                     MAIN_DIALOG_LIST_COLUMN_ID, atoi(argv[0]),
                     -1);

  g_free(status);

  g_free(priority);


  main_dialog_status_bar_update();


  return 0;
}

void main_dialog_on_list_row_activated(GtkTreeView *treeview,
                                       GtkTreePath *path,
                                       GtkTreeViewColumn *col,
                                       gpointer userdata)
{
  GtkTreeModel *model;
  GtkTreeIter iter;

  g_print("main_dialog_on_list_row_activated()\n");

  model = gtk_tree_view_get_model(treeview);

  if (gtk_tree_model_get_iter(model, &iter, path)) {
    gchar *summary;
    int issue_id;

    gtk_tree_model_get(model, &iter, MAIN_DIALOG_LIST_COLUMN_ISSUE_ID, &summary, -1);
    gtk_tree_model_get(model, &iter, MAIN_DIALOG_LIST_COLUMN_ID, &issue_id, -1);

    g_print("Double-clicked row\n");
    g_print("Issue ID: %d\n", issue_id);
    g_print("Issue summary: %s\n", summary);


    int rc = issue_edit_dialog_open(main_dialog_window, issue_id);

    if (rc != 0) {
      // @todo
    }

    g_free(summary);
  }
}


void main_dialog_on_issue_new_request(GtkWidget *widget, gpointer data)
{
  int rc = issue_new_dialog_open(main_dialog_window);

  if (rc != 0) {
    // @todo
  }
}


void main_dialog_on_projects_main_dialog_request(GtkWidget *widget, gpointer data)
{
  int rc = project_main_dialog_open(main_dialog_window);

  if (rc != 0) {
    // @todo
  }
}


void main_dialog_on_refresh_request(GtkWidget *widget, gpointer data)
{
  main_dialog_load_issue_list();
}


void main_dialog_on_menu_shell_item_view_fixed_issues(GtkWidget *widget, gpointer data)
{
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    main_dialog_view_fixed_issues_enabled = TRUE;
  }
  else {
    main_dialog_view_fixed_issues_enabled = FALSE;
  }

  main_dialog_load_issue_list();
}


void gtk_main_about_dialog_callback(GtkWidget *widget, gpointer data)
{
  //GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("battery.png", NULL);

  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Bugtracker");

  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "0.1.0"); 

  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "(c) Allann Jones");

  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
                                "Bugtracker is a simple issue tracker with a standalone database.");

  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),  "http://www.olivum.com.br");

  //gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);

  //g_object_unref(pixbuf), pixbuf = NULL;

  gtk_dialog_run(GTK_DIALOG (dialog));

  gtk_widget_destroy(dialog);
}


int main_dialog_load_projects()
{
  // Add the first element on projects combo

  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects)));

  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store, &iter, 0, "All projects", 1, 0, -1);

  //gtk_combo_box_set_active(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects), 0);

  // Query projects

  char *error_message = 0;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);

    return -1;
  }

  const gchar sql_command[] = "SELECT project_id, name FROM project WHERE status = \"A\"";

  int rc = sqlite3_exec(db, sql_command, main_dialog_project_query_callback, 0, &error_message);

  if (rc != SQLITE_OK) {
    g_print("SQL error: %s\n", error_message);

    sqlite3_free(error_message);
  }

  sqlite3_close(db);

  return 0;
}


int main_dialog_project_query_callback(void *NotUsed, int argc, char **argv, char **col_name)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(main_dialog_toolbar_combo_projects)));

  gtk_list_store_append(store, &iter);

  gtk_list_store_set(store, &iter, 0, argv[1], 1, atoi(argv[0]), -1);

  return 0;
}


void main_dialog_on_project_change(GtkWidget *widget, gpointer data)
{
  main_dialog_load_issue_list();
}