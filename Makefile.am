AUTOMAKE_OPTIONS = foreign

CFLAGS=-Wall $(GTK_CFLAGS) -I$(srcdir)/include
LDFLAGS=-L. $(GTK_LIBS)

bin_PROGRAMS = issuedb
issuedb_SOURCES = src/sqlite3.c \
									 src/database.c \
                   src/database_setup.c \
									 src/issue_new_dialog.c \
									 src/issue_edit_dialog.c \
									 src/project_new_dialog.c \
									 src/project_edit_dialog.c \
									 src/project_main_dialog.c \
									 src/main_dialog.c
