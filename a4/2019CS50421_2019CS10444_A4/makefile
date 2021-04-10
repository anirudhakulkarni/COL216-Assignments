CC = g++
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CPPFLAGS  = -g -Wall
TARGETD1 = dram
TARGETN1 = nba
TARGETN2=nba-v2
TARGETD2=dram-v2
BUILD_DIR = ./build
OUT_DIR = ./outputs
LIB = pkg-config --cflags --libs
TESTFILE= ../input.txt
clean:
	@echo Removing old build files :
	$(RM) -r $(BUILD_DIR)

dram-v1: build1 run1
build1:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGETD1) $(TARGETD1).cpp
	@echo Generated the executable without errors ...
	
run1:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGETD1) $(TESTFILE)

dram-v2: build2 run2
build2:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGETD2) $(TARGETD2).cpp
	@echo Generated the executable without errors ...
	
run2:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGETD2) $(TESTFILE)


nba-v1: build3 run3
build3:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGETN1) $(TARGETN1).cpp
	@echo Generated the executable without errors ...
run3:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGETN1) $(TESTFILE)


nba-v2: build4 run4
build4:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGETN2) $(TARGETN2).cpp
	@echo Generated the executable without errors ...
run4:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGETN2) $(TESTFILE)



back: build5 run5
build5:
	@echo Building project :
	$(CC) $(CPPFLAGS) -o $(BUILD_DIR)/$(TARGET2) ../temp/$(TARGET2).cpp
	@echo Generated the executable without errors ...
run5:
	@echo Running the executable ...
	cd $(BUILD_DIR); ./$(TARGET2) $(TESTFILE)
