CC = g++

INCLUDE = ./inc
SOURCE = ./src
BUILD = ./build
BINARY = ./bin

API = init.o interface.o io.o

all: $(API)
	$(CC) $(addprefix $(BUILD)/, $^) -o $(BINARY)/con -lm -lpthread -lglfw -lGL -ldl -lX11 -lXxf86vm -lXrandr -lXi

$(API):
	$(CC) -c $(SOURCE)/$(basename $@)/$(basename $@).cpp -o $(BUILD)/$@ -I $(INCLUDE) -I $(SOURCE)