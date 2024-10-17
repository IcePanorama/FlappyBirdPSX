CC=ccpsx

# -fcaller-saves 
# -fcse-skip-blocks -fdelayed-branch -fno-peephole -fschedule-insns -fschedule-insns2 -fstrength-reduce -fthread-jumps 
CFLAGS=-O3 -ansi -Wall -Werror -pedantic-errors -Wshadow -DDEBUG_BUILD

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
  	$(CC) -Xo$80020000 *.OBJ -o$(CPE_FILE),$(SYM_FILE),$(MAP_FILE) -L $(LIBRARY_PATH) -llibpad -llibc2
	cpe2x /ca $(CPE_FILE)

obj_files:
  echo "Building object files..."
  $(CC) $(CFLAGS) -c -oMAIN.OBJ $(SRC_DIR)\MAIN.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oVIDEO.OBJ $(SRC_DIR)\VIDEO.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oSPRITES.OBJ $(SRC_DIR)\COMPNTS\SPRITES.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oPHYSICS.OBJ $(SRC_DIR)\COMPNTS\PHYSICS.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oPLAYER.OBJ $(SRC_DIR)\GAME_OBJ\PLAYER.C -I $(INCL_DIR)
  $(CC) $(CFLAGS) -c -oGAMEMNGR.OBJ $(SRC_DIR)\GAMEMNGR.C -I $(INCL_DIR)

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
