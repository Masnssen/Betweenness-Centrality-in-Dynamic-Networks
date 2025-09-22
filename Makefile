# Compiler and flags
CXX      := clang++
CXXFLAGS := -O3 -std=gnu++17 -march=native -flto -fstrict-aliasing -DNDEBUG \
            -Wno-vla-cxx-extension -Wno-deprecated-declarations

# Project files
SRC      := stat_reel_bc_v1.cpp
TARGET   := bc_calc

# Default dataset configuration (can be overridden from command line)
DATA_DIR ?= ../../Experiment/Paris/dataset_reel
DATASET  ?= network_temporal_day

# Compiler definitions to pass dataset info into the program
DEFINES  := -DDATA_DIR=\"$(DATA_DIR)\" -DDATASET_NAME=\"$(DATASET)\"

# Default rule: build the executable
all: $(TARGET)

$(TARGET): $(SRC) fonction_v1.h fonction_bb_v1.h
	$(CXX) $(CXXFLAGS) $(DEFINES) $(SRC) -o $(TARGET)

# Run the program after building
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o
