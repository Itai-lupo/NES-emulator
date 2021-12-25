CC = clang

GTEST_DIR=/usr/src/googletest/

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
rwildcardDir=$(wildcard $1*/) $(foreach d,$(wildcard $1*),$(call rwildcardDir,$d/))


TARGET_EXEC ?= theMaze.out

BUILD_DIR ?= ./build
OUTPUT_DIR ?= ./output
INCLUDE_DIR ?= ./include ./vendor
SRC_DIRS ?= ./src/ ./vendor
TEST_DIR ?= ./tests
DLLS_DIR ?= ./DLLS

SRCS += $(foreach  dir,$(SRC_DIRS),$(call rwildcard,$(dir),*.c*))
TESTS := $(call rwildcard,$(TEST_DIR),*.c*)
OBJS :=  $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS  := $(TESTS:%=$(BUILD_DIR)/%.o) $(filter-out ./build/./src/main.cpp.o, $(subst ./build/./src/core.h.o,./build/./src/app.cpp.o,$(OBJS)))
DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_OBJS:.o=.d)

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

INC_DIRS := $(foreach dir,$(INCLUDE_DIR),$(call rwildcardDir,$(dir)))
LIB_DIR := $(call rwildcardDir,$(DLLS_DIR))
TEST_INC_DIRS := $(foreach dir,$(INCLUDE_DIR),$(call rwildcardDir,$(dir)))
INC_FLAGS := $(addprefix -I,$(INC_DIRS) $(LIB_DIR))
LIB_FLAGS := $(addprefix -L,$(LIB_DIR))

CPPFLAGS ?=   -std=c++20
TEST_CPP_FLAGE = -lgtest -lgtest_main -lgmock  
# CFLAGS :=

CXXFLAGS += $(INC_FLAGS)  -MMD -MP  -g -Wall -Wc++17-extensions -g -Wall -Wextra -pthread  $(LIB_FLAGS) -lstdc++ -lgflags -lglog -lGL -lglfw   -lrt -lm -ldl -lasound -O0

LDFLAGS =  $(LIB_FLAGS)


$(OUTPUT_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p output
	$(CC) $(CPPFLAGS   $(CXXFLAGS) $(OBJS)  -o $@ $(LDFLAGS)

print:
	@echo ./include/ $(wildcard ./include/*/) $(wildcard ./include/*/*/) $(wildcard ./include/*/*/*/) $(wildcard ./include/*/*/*/*/)
	@echo $(call rwildcardDir,./include)

	

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC)  $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC)  $(CFLAGS) $(CXXFLAGS) -c $< -o $@


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
