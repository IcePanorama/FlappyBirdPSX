CC=ccpsx

# -fcaller-saves 
# -fcse-skip-blocks -fdelayed-branch -fno-peephole -fschedule-insns -fschedule-insns2 -fstrength-reduce -fthread-jumps 
CFLAGS=-O3 -ansi -Wall -Werror -pedantic-errors -Wshadow -DDEBUG_BUILD

SRC_DIR=SRC
INCL_DIR=INCLUDE

# Build targets
CPE_FILE=FLPPYBIR.CPE
SYM_FILE=FLPPYBIR.SYM
MAP_FILE=MEM.MAP

# removing clean dependency from this target fixes bug where it fails 50% of the
# time.
all: obj_files
  echo "Building ..\$(CPE_FILE), ..\$(SYM_FILE), and ..\$(MAP_FILE)..."
  $(CC) -Xo$80020000 BIN\*.OBJ -o$(CPE_FILE),$(SYM_FILE),$(MAP_FILE) -L $(LIBRARY_PATH) -llibpad -llibc2 -llibcd
  echo "Building \$(CPE_FILE)."
  cpe2x /ca $(CPE_FILE)

full: all
  makecd

obj_files:
  echo "Creating BIN subdir..."
  mkdir BIN
  echo "Building object files..."
  $(CC) $(CFLAGS) -c -oBIN\MAIN.OBJ $(SRC_DIR)\MAIN.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\UTILS.OBJ $(SRC_DIR)\UTILS.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\COLSHAPE.OBJ $(SRC_DIR)\COMPNTS\COLSHAPE.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\SPRITES.OBJ $(SRC_DIR)\COMPNTS\SPRITES.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\PHYSICS.OBJ $(SRC_DIR)\COMPNTS\PHYSICS.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\WIFRAME.OBJ $(SRC_DIR)\COMPNTS\WIFRAME.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\COLMNGR.OBJ $(SRC_DIR)\GAME\COLMNGR.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\GAMEMNGR.OBJ $(SRC_DIR)\GAME\GAMEMNGR.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\GMSTATE.OBJ $(SRC_DIR)\GAME\GMSTATE.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\PIPEMNGR.OBJ $(SRC_DIR)\GAME\PIPEMNGR.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\SCORE.OBJ $(SRC_DIR)\GAME\SCORE.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\SIGNALS.OBJ $(SRC_DIR)\GAME\SIGNALS.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\PLAYER.OBJ $(SRC_DIR)\GAME_OBJ\PLAYER.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\PIPES.OBJ $(SRC_DIR)\GAME_OBJ\PIPES.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\CMDLIST.OBJ $(SRC_DIR)\INPUT\CMDLIST.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\CONTROLR.OBJ $(SRC_DIR)\INPUT\CONTROLR.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\SCRBUFF.OBJ $(SRC_DIR)\VIDEO\SCRBUFF.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oBIN\VIDEO.OBJ $(SRC_DIR)\VIDEO\VIDEO.C -I $(INCL_DIR)

clean:
  del *.exe
  del *.cpe
  del *.sym
  del *.map
  del bin\*.obj
  rmdir bin
  del cdrom\*.img
  del cdrom\*.iso
  rmdir cdrom
