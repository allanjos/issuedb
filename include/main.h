/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.h
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
 * 3. Neither the name ``Sindhu S'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * bugtracker IS PROVIDED BY Sindhu S ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Sindhu S OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MAIN_H_
#define _MAIN_H_


// Public interface

int main_dialog_load_issue_list(void);

gboolean main_dialog_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

void main_dialog_on_destroy_request(GtkWidget *widget, gpointer data);

void main_dialog_on_issue_new_request(GtkWidget *widget, gpointer data);

void main_dialog_on_projects_main_dialog_request(GtkWidget *widget, gpointer data);

void main_dialog_add_to_list(GtkWidget *treeview, const gchar *issue_name, const gchar *project_name, const gchar *severity);

void main_dialog_on_list_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata);


// Private interface

enum {
  MAIN_DIALOG_LIST_COLUMN_ISSUE_ID = 0,
  MAIN_DIALOG_LIST_COLUMN_PROJECT_ID,
  MAIN_DIALOG_LIST_COLUMN_PRIORITY_ID,
  MAIN_DIALOG_LIST_COLUMN_STATUS_ID,
  MAIN_DIALOG_LIST_COLUMN_ID,
  MAIN_DIALOG_LIST_N_COLUMNS
};

int main_dialog_issue_list_query_callback(void *NotUsed, int argc, char **argv, char **col_name);

void gtk_main_about_dialog_callback(GtkWidget *widget, gpointer data);

void main_dialog_on_refresh_request(GtkWidget *widget, gpointer data);

void main_dialog_on_menu_shell_item_view_fixed_issues(GtkWidget *widget, gpointer data);

int main_dialog_load_projects(void);

int main_dialog_project_query_callback(void *NotUsed, int argc, char **argv, char **col_name);

void main_dialog_on_project_change(GtkWidget *widget, gpointer data);

void main_app_term(void);

void issue_new_dialog_init_list(GtkWidget *treeview);

/**
 * List column sort callback.
 */
gint main_dialog_list_sort_callback (GtkTreeModel *model,
                                     GtkTreeIter  *a,
                                     GtkTreeIter  *b,
                                     gpointer      userdata);


#endif /* _MAIN_H_ */