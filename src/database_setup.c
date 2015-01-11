/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * database_setup.c
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

#include <stdio.h>
#include "database_setup.h"


/**
 * List of commands to setup database.
 */
const char *database_table_setup_cmd_list[2] = {
  // Create table command for table "issue"
  "CREATE TABLE issue ("
    "issue_id integer PRIMARY KEY, "
    "summary varchar(255), "
    "project_id integer, "
    "priority smallint, "
    "status char(1), "
    "platform integer, "
    "version varchar(15), "
    "build varchar(10), "
    "fixed_in_version varchar(15), "
    "target_version varchar(15), "
    "reproducibility integer, "
    "register_timestamp timestamp DEFAULT (datetime('now','localtime')), "
    "description varchar(255), "
    "reproduction_steps varchar(255), "
    "FOREIGN KEY(project_id) REFERENCES project(project_id)"
    ");",
  // Create table command for table "project"
  "CREATE TABLE project ("
    "project_id integer PRIMARY KEY, "
    "name varchar(255), "
    "status char(1)"
  ");"
};

/** Quantity of commands to process from commands list. */
#define DATABASE_CMD_LIST_SIZE 2


/**
 * Setup the database structure.
 */
int database_setup()
{
  // Query database and fill the model

  char *error_message = NULL;

  sqlite3 *db = database_open();

  if (db == NULL) {
    g_print("Can't open database: %s\n", sqlite3_errmsg(db));

    database_close();

    return -1;
  }

  int db_cmd_index = 0;

  for (db_cmd_index = 0; db_cmd_index < DATABASE_CMD_LIST_SIZE; db_cmd_index++) {
    printf("Command %d = %s\n", db_cmd_index, database_table_setup_cmd_list[db_cmd_index]);

    int rc = sqlite3_exec(db, database_table_setup_cmd_list[db_cmd_index], NULL, 0, &error_message);

    if (rc != SQLITE_OK) {
      g_print("SQL error: %s\n", error_message);

      sqlite3_free(error_message);
    }
  }

  database_close();

  return 0;
}
