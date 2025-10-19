# C++ Trading Application - Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = trading_app

# Directories
SRCDIR = .
MODELDIR = models
SERVICEDIR = services
UTILDIR = utils
OBJDIR = obj

# Source files
MODEL_SOURCES = $(wildcard $(MODELDIR)/*.cpp)
SERVICE_SOURCES = $(wildcard $(SERVICEDIR)/*.cpp)
UTIL_SOURCES = $(wildcard $(UTILDIR)/*.cpp)
MAIN_SOURCE = main.cpp

# Object files
MODEL_OBJECTS = $(MODEL_SOURCES:$(MODELDIR)/%.cpp=$(OBJDIR)/%.o)
SERVICE_OBJECTS = $(SERVICE_SOURCES:$(SERVICEDIR)/%.cpp=$(OBJDIR)/%.o)
UTIL_OBJECTS = $(UTIL_SOURCES:$(UTILDIR)/%.cpp=$(OBJDIR)/%.o)
MAIN_OBJECT = $(OBJDIR)/main.o

ALL_OBJECTS = $(MODEL_OBJECTS) $(SERVICE_OBJECTS) $(UTIL_OBJECTS) $(MAIN_OBJECT)

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p data

# Link
$(TARGET): $(ALL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile main
$(OBJDIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile models
$(OBJDIR)/%.o: $(MODELDIR)/%.cpp $(MODELDIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile services
$(OBJDIR)/%.o: $(SERVICEDIR)/%.cpp $(SERVICEDIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile utils
$(OBJDIR)/%.o: $(UTILDIR)/%.cpp $(UTILDIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	@echo "Clean complete!"

# Clean all including data
cleanall: clean
	rm -rf data
	@echo "All files cleaned!"

# Run
run: $(TARGET)
	./$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  make         - Build the application"
	@echo "  make clean   - Remove object files and executable"
	@echo "  make cleanall- Remove all generated files including data"
	@echo "  make run     - Build and run the application"
	@echo "  make help    - Display this help message"

.PHONY: all clean cleanall run help directories
