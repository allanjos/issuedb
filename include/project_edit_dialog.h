/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * project_edit_dialog.h
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
 * issuedb IS PROVIDED BY Sindhu S ``AS IS'' AND ANY EXPRESS
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


#ifndef _PROJECT_EDIT_DIALOG_H_
#define _PROJECT_EDIT_DIALOG_H_


// Public interface


int project_edit_dialog_open(GtkWidget *parent, int issue_id);

void project_edit_dialog_close(void);

int project_edit_dialog_load_data(void);


// Private interface


void on_project_edit_button_close_clicked(GtkWidget *widget, gpointer data);

void on_project_edit_button_save_clicked(GtkWidget *widget, gpointer data);

void on_project_edit_button_delete_clicked(GtkWidget *widget, gpointer data);

int project_edit_dialog_delete(void);

int project_edit_dialog_save_data(void);

int project_edit_project_query_callback(void *NotUsed, int argc, char **argv, char **col_name);

int project_edit_data_query_callback(void *NotUsed, int argc, char **argv, char **col_name);


#endif /* _PROJECT_EDIT_DIALOG_H_ */
