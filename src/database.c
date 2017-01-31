/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * database.c
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
#include "sqlite3.h"
#include "database.h"


static sqlite3 *db = NULL;
static gchar *database_data_path = NULL;
static short database_opened = FALSE;


gchar *database_set_data_path(gchar *path)
{
  if (path) {
    database_data_path = path;
  }

  return database_data_path;
}


gchar *database_get_data_path()
{
  return database_data_path;
}


sqlite3 *database_open()
{
  int rc;

  g_print("Database path: %s\n", database_data_path);

  rc = sqlite3_open_v2(database_data_path,
                       &db,
                       SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                       NULL);

  if (rc) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    sqlite3_close(db);

    return NULL;
  }

  return db;
}

int database_close()
{
  if (!database_opened) {
    return 1;
  }

  sqlite3_close(db);

  database_opened = FALSE;

  return 0;
}

int database_term()
{
  database_close();

  return 0;
}
