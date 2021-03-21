CC = g++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CPPFLAGS  = -g -Wall
TARGET = main
TARGET2 = unoptimised

BUILD_DIR = ./build
OUT_DIR = ./outputs
LIB = pkg-config --cflags --libs
TESTFILE= ../input.txt
TESTFILE2= ../input2.txt
all: clean build run
clean:
	@echo Removing old build files :
	$(RM) -r $(BUILD_DIR)
build:
	@echo Building project :
	mkdir $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGET) $(TARGET).cpp
	@echo Generated the executable without errors ...
	
run:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGET) $(TESTFILE) 10 2
run2:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGET) $(TESTFILE2) 10 2
test:
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/test test.cpp
	cd $(BUILD_DIR); ./test $(TESTFILE) 10 2
unopt:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGET2) $(TARGET2).cpp
	@echo Generated the executable without errors ...
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGET2) $(TESTFILE) 10 2
	