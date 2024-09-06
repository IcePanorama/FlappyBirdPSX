CC=ccpsx

CFLAGS=-ansi -Wall -Werror -pedantic-errors -Wshadow

SRC_DIR=SRC
INCL_DIR=INCLUDE

# Build targets
CPE_FILE=MAIN.CPE
SYM_FILE=MAIN.SYM
MAP_FILE=MEM.MAP

# removing clean dependency from this target fixes bug where it fails 50% of the
# time.
all: obj_files
  echo "Building ..\$(CPE_FILE), ..\$(SYM_FILE), and ..\$(MAP_FILE)..."
  $(CC) -O2 -Xo$80020000 *.OBJ -o$(CPE_FILE),$(SYM_FILE),$(MAP_FILE) -L \
    $(LIBRARY_PATH) -llibpad -llibc2
	cpe2x /ca $(CPE_FILE)

obj_files: FB_objs V_objs
  echo "Building object files..."

FB_objs:
  echo "Building `Flappy Bird` object files..."
	$(CC) -O2 $(CFLAGS) -c -oFB_MAIN.OBJ $(SRC_DIR)\FB_MAIN.C -I $(INCL_DIR)

V_objs:
  echo "Building `Video` object files..."
	$(CC) -O2 $(CFLAGS) -c -oV_VIDEO.OBJ $(SRC_DIR)\V_VIDEO.C -I $(INCL_DIR)


clean:
	del *.cpe
	del *.CPE
	del *.sym
	del *.SYM
	del *.map
	del *.MAP
	del *.obj
	del *.OBJ
	del *.EXE
