CC = clang

GTEST_DIR=/usr/src/googletest/


TARGET_EXEC ?= raftel.out

BUILD_DIR ?= ./build
OUTPUT_DIR ?= ./output
SRC_DIRS ?= ./src ./headers ./vendor
TEST_DIRS ?= ./tests

SRCS := $(shell find $(SRC_DIRS) -name *.c*)
TESTS := $(shell find $(TEST_DIRS) -name *.c*)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS  := $(TESTS:%=$(BUILD_DIR)/%.o) $(subst ./build/./src/core.h.o,./build/./src/app.cpp.o,$(OBJS))
DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
TEST_INC_DIRS := $(shell find $(TEST_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?=  $(INC_FLAGS)  -MMD -MP  -g -Wall
TEST_CPP_FLAGE = -lgtest -lgtest_main -lgmock  
CFLAGS := -lstdc++ -lglog -lGL -lglfw  -lc -lrt -lm -ldl  -lasound

CXXFLAGS += -g -Wall -Wextra -pthread

$(OUTPUT_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p output
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS)  -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC)  $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC)  $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

test: $(OUTPUT_DIR)/test.out

$(OUTPUT_DIR)/test.out: $(TEST_OBJS)
	mkdir -p output
	$(CC)  $(CFLAGS) $(CPPFLAGS)  $(TEST_CPP_FLAGE) $(CXXFLAGS) $(TEST_OBJS) -o output/test.out $(LDFLAGS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) -f gtest.a gtest_main.a *.o
	$(RM) -r $(BUILD_DIR) 
	$(RM) -r $(OUTPUT_DIR)

-include $(DEPS)
-include $(TEST_DEPS)

MKDIR_P ?= mkdir -p
