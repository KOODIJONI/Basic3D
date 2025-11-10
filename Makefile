# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lopengl32 -lgdi32 -lglu32

CFLAGS = -Wall -g -Ilibs/cJSON-master -Ilibs/stb_image


# Output binary
TARGET = build/hello14.exe

# Source files
SRCS = src/MAIN.c \
       src/renderer.c \
       src/projection.c \
       src/matrix_utils.c \
       src/gl_loader.c \
       src/shader.c \
       src/user_input.c\
       src/camera_control.c\
       src/obj_file_loader.c \
       src/shader_manager.c \
       src/gl_setup.c \
       src/object_manager.c \
       src/scene_loader.c \
       libs/cJSON-master/cJSON.c \
       src/texture_loader.c \
       src/texture_utils.c

# Default rule
all: $(TARGET)

# Linking step
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Clean rule
clean:
	del /Q build\*.exe 2>nul || exit 0
