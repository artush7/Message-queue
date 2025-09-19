#Clang
CXX = clang++ -std=c++23
CXXFLAGS = -O2 -Wall -Wextra -g -Wunreachable-code -Wunreachable-code-loop-increment -Wunreachable-code-return

#Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests

#Source Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

#Library files
LIB_OS_STATIC = $(BUILD_DIR)/libos.a
LIB_OS_DYNAMIC = $(BUILD_DIR)/libos.so

#Test files
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_FILES))

#Executable files
TEST_EXEC = $(BUILD_DIR)/runTests
TEST_EXEC_STATIC = $(BUILD_DIR)/runTestsStatic
TEST_EXEC_DYNAMIC = $(BUILD_DIR)/runTestsDynamic

#includes
INCLUDES = -I$(SRC_DIR)

#Library flags
LIBS = -lgtest -lgtest_main -pthread
LIBS_STATIC = -L$(BUILD_DIR) -los


all:$(TEST_EXEC)


#Create directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


#Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -fPIC $< -o $@


#Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#Create executable file
$(TEST_EXEC):$(OBJ_FILES) $(TEST_OBJ_FILES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

#Run 
test:$(TEST_EXEC)
	./$(TEST_EXEC)

#Create static library
$(LIB_OS_STATIC):$(OBJ_FILES)
	ar rcs $@ $^

#Create executable file with static library
$(TEST_EXEC_STATIC):$(LIB_OS_STATIC) $(TEST_OBJ_FILES) | $(BUILD_DIR)
		$(CXX) $(CXXFLAGS) $(TEST_OBJ_FILES) -o $@ $(LIBS) $(LIBS_STATIC)

#Run 
test_static:$(TEST_EXEC_STATIC)
	./$(TEST_EXEC_STATIC)

#Create dynamic library
$(LIB_OS_DYNAMIC):$(OBJ_FILES)
	$(CXX) -shared -o $@ $^

#Create executable file with dynamic library
$(TEST_EXEC_DYNAMIC):$(LIB_OS_DYNAMIC) $(TEST_OBJ_FILES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ_FILES) -o $@ $(LIBS) $(LIBS_STATIC)

test_dynamic:$(TEST_EXEC_DYNAMIC)
	LD_LIBRARY_PATH=build ./$(TEST_EXEC_DYNAMIC)


#remove all files in build
clean:
	rm -rf $(BUILD_DIR)


.PHONY:all clean test