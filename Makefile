TARGET := rubiks-cube-opengl

BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC := g++
CFLAGS := -std=c++11 -pedantic -Werror -Wall -Wextra -g
LDFLAGS := -fsanitize=address

CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++11 -m64 -march=native -fopt-info-vec-optimized #-fopt-info-vec-missed -ftree-vectorize
LDXX_FLAGS = -lGL  -lGLEW -lglut -lpthread

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CXX_FLAGS) $(LDXX_FLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CXX_FLAGS) $(LDXX_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
	rm $(TARGET)
