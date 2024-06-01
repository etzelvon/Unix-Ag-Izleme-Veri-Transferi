# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Iinclude -pthread

# Lib
LIBS = -lncurses -lpcap -lssl -lcrypto

# Dir
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
LOG_DIR = log

# Src dosyaları
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Obj dosyları
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable name
TARGET = $(BIN_DIR)/main

# Gerekli klasörlein oluşturulması
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR))

# Default target
all: install_deps $(TARGET)

# Bağımlıkların Yüklenmesi
install_deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libpcap-dev libncurses5-dev libssl-dev net-tools

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)


.PHONY: all clean install_deps
