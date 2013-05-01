/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * issue_new_dialog.h
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

#include <gtk/gtk.h>


#ifndef _PROJECT_MAIN_DIALOG_H_
#define _PROJECT_MAIN_DIALOG_H_


// Attributes


enum {
  PROJECT_MAIN_DIALOG_LIST_COLUMN_PROJECT_ID = 0,
  PROJECT_MAIN_DIALOG_LIST_COLUMN_ISSUES_COUNT_ID,
  PROJECT_MAIN_DIALOG_LIST_COLUMN_STATUS_ID
};


// Public interface


int project_main_dialog_open(GtkWidget *parent);

void project_main_dialog_close(void);

int project_main_dialog_load_list(void);


// Private interface


void on_project_main_toolbar_close_clicked(GtkWidget *widget, gpointer data);

void project_main_dialog_init_list(GtkWidget *treeview);

void project_main_dialog_on_list_row_activated(GtkTreeView *treeview,
                                               GtkTreePath *path,
                                               GtkTreeViewColumn *col,
                                               gpointer userdata);

int project_main_dialog_issue_list_query_callback(void *NotUsed, int argc, char **argv, char **col_name);

void project_main_dialog_on_new_request(GtkWidget *widget, gpointer data);

void project_main_dialog_on_refresh_request(GtkWidget *widget, gpointer data);

/**
 * Callback for sorting list column
 */
gint project_main_dialog_list_sort_callback(GtkTreeModel *model,
                                            GtkTreeIter  *a,
                                            GtkTreeIter  *b,
                                            gpointer     userdata);


#endif /* _PROJECT_MAIN_DIALOG_H_ */