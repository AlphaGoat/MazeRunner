#OBJS specifies which files to compile as part of the project
OBJS = main.c GameMechanics/gameGrid.c GameMechanics/survivors.c GraphicsEngine/spriteSheetOps.c GraphicsEngine/texture_app.c GraphicsEngine/camera.c GameMechanics/stateManagement.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -g -Wall

#LINKER_FLAGS specifies the libraries that we're linking against
LINKER_FLAGS = -D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lSDL2_image -lm

#OBJ_NAME specifies the name of our executable
OBJ_NAME = DBD2EB.out

#This i the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
