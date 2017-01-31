all: $(EXECUTABLE_FILE_NAME)

$(EXECUTABLE_FILE_NAME): sqlite3.o database.o project_new_dialog.o project_edit_dialog.o project_main_dialog.o issue_edit_dialog.o issue_new_dialog.o main_dialog.o app_rc.o
	$(CC) $(CFLAGS) main_dialog.o issue_new_dialog.o issue_edit_dialog.o project_main_dialog.o project_edit_dialog.o project_new_dialog.o database.o sqlite3.o app_rc.o $(LDFLAGS) -o issuedb.exe

app_rc.o:
	windres.exe -I. -iapp.rc -o app_rc.o

main_dialog.o: main_dialog.c
	$(CC) $(CFLAGS) -c main_dialog.c -o main_dialog.o

issue_edit_dialog.o: issue_edit_dialog.c
	$(CC) $(CFLAGS) -c issue_edit_dialog.c -o issue_edit_dialog.o

issue_new_dialog.o: issue_new_dialog.c
	$(CC) $(CFLAGS) -c issue_new_dialog.c -o issue_new_dialog.o

project_main_dialog.o: project_main_dialog.c
	$(CC) $(CFLAGS) -c project_main_dialog.c -o project_main_dialog.o

project_edit_dialog.o: project_edit_dialog.c
	$(CC) $(CFLAGS) -c project_edit_dialog.c -o project_edit_dialog.o

project_new_dialog.o: project_new_dialog.c
	$(CC) $(CFLAGS) -c project_new_dialog.c -o project_new_dialog.o

database.o: database.c
	$(CC) $(CFLAGS) -c database.c -o database.o

sqlite3.o: sqlite3.c
	$(CC) $(CFLAGS) -c sqlite3.c -o sqlite3.o

clean:
	rm -f *.o
	rm -f *.exe
