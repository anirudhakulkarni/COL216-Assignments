CC = g++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CPPFLAGS  = -g -Wall
TARGET = dram
TARGET2 = nba
BUILD_DIR = ./build
OUT_DIR = ./outputs
LIB = pkg-config --cflags --libs
TESTFILE= ../input.txt

dram: clean build run
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
	cd $(BUILD_DIR); ./$(TARGET) $(TESTFILE)
nba: clean2 build2 run2
clean2:
	@echo Removing old build files :
	$(RM) -r $(BUILD_DIR)
build2:
	@echo Building project :
	mkdir $(BUILD_DIR)
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGET2) $(TARGET2).cpp
	@echo Generated the executable without errors ...
run2:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGET2) $(TESTFILE)

