#TARGET specifies which file to compile
TARGET = EtchaSketch

#CC specifies compiler
CC = clang

#COMPILER_FLAGS specifies the additional compilation options
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries, which are
# SDL library, SDL library for loading images, SDL library for loading texts	
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf


#Target entry that compiles the files
all : $(TARGET).c
	$(CC)  $(TARGET).c  -o $(TARGET) $(COMPILER_FLAGS) $(LINKER_FLAGS)
clean: 
	$(RM) $(TARGET)

