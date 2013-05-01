# Path for GTK binaries for Windows
GTK_PATH="third/gtk+"

# Build directory
BUILD_DIR="build/win32"

# Output file name
EXECUTABLE_FILE_NAME="bugtracker.exe"

# Compiler flags
CFLAGS="-Wall -mwindows -Iinclude -I${GTK_PATH}/include/atk-1.0 -I${GTK_PATH}/include/cairo -I${GTK_PATH}/include/gdk-pixbuf-2.0 -I${GTK_PATH}/include/pango-1.0 -I${GTK_PATH}/include/glib-2.0 -I${GTK_PATH}/lib/glib-2.0/include -mms-bitfields -I${GTK_PATH}/include/pixman-1 -I${GTK_PATH}/include -I${GTK_PATH}/include/freetype2 -I${GTK_PATH}/include/libpng14 -I${GTK_PATH}/include/gtk-2.0 -I${GTK_PATH}/lib/gtk-2.0/include"

# Linker flags
LDFLAGS="-L${GTK_PATH}/lib -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgio-2.0 -lpangowin32-1.0 -lgdi32 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lcairo -lgobject-2.0 -lglib-2.0 -lintl"

echo "CFLAGS = ${CFLAGS}"
echo "LDFLAGS = ${LDFLAGS}"

echo "Compiling sqlite library"

gcc -c ${CFLAGS} src/sqlite3.c -o ${BUILD_DIR}/sqlite3.o

echo "Compiling database library"

gcc -c ${CFLAGS} src/database.c -o ${BUILD_DIR}/database.o

echo "Compiling issue new dialog"

gcc -c ${CFLAGS} src/issue_new_dialog.c -o ${BUILD_DIR}/issue_new_dialog.o

echo "Compiling issue edit dialog"

gcc -c ${CFLAGS} src/issue_edit_dialog.c -o ${BUILD_DIR}/issue_edit_dialog.o

echo "Compiling projects main dialog"

gcc -c ${CFLAGS} src/project_main_dialog.c -o ${BUILD_DIR}/project_main_dialog.o

echo "Compiling projects new dialog"

gcc -c ${CFLAGS} src/project_new_dialog.c -o ${BUILD_DIR}/project_new_dialog.o

echo "Compiling projects edit dialog"

gcc -c ${CFLAGS} src/project_edit_dialog.c -o ${BUILD_DIR}/project_edit_dialog.o

echo "Compiling main dialog"

gcc -c ${CFLAGS} src/main.c -o ${BUILD_DIR}/main.o

echo "Creating RC file"

windres.exe -Isrc -isrc/app.rc -o ${BUILD_DIR}/app_rc.o

echo "Generating executable ${EXECUTABLE_FILE_NAME}"

gcc ${CFLAGS} ${BUILD_DIR}/sqlite3.o \
            ${BUILD_DIR}/database.o \
            ${BUILD_DIR}/issue_new_dialog.o \
            ${BUILD_DIR}/issue_edit_dialog.o \
            ${BUILD_DIR}/project_main_dialog.o \
            ${BUILD_DIR}/project_new_dialog.o \
            ${BUILD_DIR}/project_edit_dialog.o \
            ${BUILD_DIR}/main.o \
            ${BUILD_DIR}/app_rc.o ${LDFLAGS} \
            -o ${BUILD_DIR}/${EXECUTABLE_FILE_NAME}
