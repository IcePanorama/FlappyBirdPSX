CC=ccpsx

CFLAGS=-ansi -Wall -Werror -pedantic-errors -Wshadow -DDEBUG_BUILD

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
  	$(CC) -O2 -Xo$80020000 *.OBJ -o$(CPE_FILE),$(SYM_FILE),$(MAP_FILE) -L $(LIBRARY_PATH) -llibpad -llibc2
	cpe2x /ca $(CPE_FILE)

obj_files:
  echo "Building object files..."
  $(CC) -O2 $(CFLAGS) -c -oMAIN.OBJ $(SRC_DIR)\MAIN.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oVIDEO.OBJ $(SRC_DIR)\VIDEO.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oCONTROL.OBJ $(SRC_DIR)\CONTROL.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oCOMMAND.OBJ $(SRC_DIR)\COMMAND.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oSPRITES.OBJ $(SRC_DIR)\SPRITES.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oPLAYER.OBJ $(SRC_DIR)\PLAYER.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oGAMEMNGR.OBJ $(SRC_DIR)\GAMEMNGR.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oPIPES.OBJ $(SRC_DIR)\PIPES.C -I $(INCL_DIR)
  $(CC) -O2 $(CFLAGS) -c -oCOLSHAPE.OBJ $(SRC_DIR)\COLSHAPE.C -I $(INCL_DIR)
  #$(CC) -O2 $(CFLAGS) -c -oPOSCSHAP.OBJ $(SRC_DIR)\POSCSHAP.C -I $(INCL_DIR)

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
