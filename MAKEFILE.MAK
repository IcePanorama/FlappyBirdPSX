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

obj_files: FB_objs G_objs S_objs V_objs
  echo "Building object files..."

FB_objs:
  echo "Building `Flappy Bird` object files..."
	$(CC) -O2 $(CFLAGS) -c -oFB_MAIN.OBJ $(SRC_DIR)\FB_MAIN.C -I $(INCL_DIR)

G_objs:
  echo "Building `Game` object files..."
	$(CC) -O2 $(CFLAGS) -c -oG_PLAYER.OBJ $(SRC_DIR)\G_PLAYER.C -I $(INCL_DIR)
	$(CC) -O2 $(CFLAGS) -c -oG_PIPE.OBJ $(SRC_DIR)\G_PIPE.C -I $(INCL_DIR)
	$(CC) -O2 $(CFLAGS) -c -oG_MANGER.OBJ $(SRC_DIR)\G_MANGER.C -I $(INCL_DIR)

S_objs:
  echo "Building `System` object files..."
	$(CC) -O2 $(CFLAGS) -c -oS_CONTRL.OBJ $(SRC_DIR)\S_CONTRL.C -I $(INCL_DIR)
	$(CC) -O2 $(CFLAGS) -c -oS_HBCNTR.OBJ $(SRC_DIR)\S_HBCNTR.C -I $(INCL_DIR)
	$(CC) -O2 $(CFLAGS) -c -oS_MAIN.OBJ $(SRC_DIR)\S_MAIN.C -I $(INCL_DIR)

V_objs:
  echo "Building `Video` object files..."
	$(CC) -O2 $(CFLAGS) -c -oV_MAIN.OBJ $(SRC_DIR)\V_MAIN.C -I $(INCL_DIR)

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
