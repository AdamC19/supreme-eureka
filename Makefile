INC_PATH=inc
SRC_PATH_ALL=src
BUILD_PATH_ALL=build

##### BEGIN SRC DEFS #####
SRC_PATH_DAY1=src_day1
BUILD_PATH_DAY1=build/day1

SRC_PATH_DAY2=src_day2
BUILD_PATH_DAY2=build/day2

SRC_PATH_DAY3=src_day3
BUILD_PATH_DAY3=build/day3

SRC_PATH_DAY4=src_day4
BUILD_PATH_DAY4=build/day4

SRC_PATH_DAY5=src_day5
BUILD_PATH_DAY5=build/day5

SRC_PATH_DAY6=src_day6
BUILD_PATH_DAY6=build/day6

SRC_PATH_DAY7=src_day7
BUILD_PATH_DAY7=build/day7

SRC_PATH_DAY8=src_day8
BUILD_PATH_DAY8=build/day8

SRC_PATH_DAY9=src_day9
BUILD_PATH_DAY9=build/day9
##### END SRC PATHS #####

BIN_PATH=bin
CC=gcc
CCFLAGS=-Wall -lm -lrt -lpthread
CXX=g++
CXXFLAGS=-std=c++17 -Wall -lm -lrt -lpthread
CXXOBJFLAGS=$(CXXFLAGS) -c


##### BEGIN TARGET DEFS #####
TARGET_DAY1_NAME=day1
TARGET_DAY1:=$(BIN_PATH)/$(TARGET_DAY1_NAME)

TARGET_DAY2_NAME=day2
TARGET_DAY2:=$(BIN_PATH)/$(TARGET_DAY2_NAME)

TARGET_DAY3_NAME=day3
TARGET_DAY3:=$(BIN_PATH)/$(TARGET_DAY3_NAME)

TARGET_DAY4_NAME=day4
TARGET_DAY4:=$(BIN_PATH)/$(TARGET_DAY4_NAME)

TARGET_DAY5_NAME=day5
TARGET_DAY5:=$(BIN_PATH)/$(TARGET_DAY5_NAME)

TARGET_DAY6_NAME=day6
TARGET_DAY6:=$(BIN_PATH)/$(TARGET_DAY6_NAME)

TARGET_DAY7_NAME=day7
TARGET_DAY7:=$(BIN_PATH)/$(TARGET_DAY7_NAME)

TARGET_DAY8_NAME=day8
TARGET_DAY8:=$(BIN_PATH)/$(TARGET_DAY8_NAME)

TARGET_DAY9_NAME=day9
TARGET_DAY9:=$(BIN_PATH)/$(TARGET_DAY9_NAME)
##### END TARGET DEFS #####

SRC_ALL:=$(foreach x, $(SRC_PATH_ALL), $(wildcard $(addprefix $(x)/*,.cpp)))

##### BEGIN SRC LISTS #####
SRC_DAY1:=$(foreach x, $(SRC_PATH_DAY1), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY2:=$(foreach x, $(SRC_PATH_DAY2), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY3:=$(foreach x, $(SRC_PATH_DAY3), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY4:=$(foreach x, $(SRC_PATH_DAY4), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY5:=$(foreach x, $(SRC_PATH_DAY5), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY6:=$(foreach x, $(SRC_PATH_DAY6), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY7:=$(foreach x, $(SRC_PATH_DAY7), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY8:=$(foreach x, $(SRC_PATH_DAY8), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY9:=$(foreach x, $(SRC_PATH_DAY9), $(wildcard $(addprefix $(x)/*,.cpp)))
##### END SRC LISTS #####

OBJ_ALL:=$(addprefix $(BUILD_PATH_ALL)/, $(addsuffix .o, $(notdir $(basename $(SRC_ALL)))))
##### BEGIN OBJ LISTS #####
OBJ_DAY1:=$(addprefix $(BUILD_PATH_DAY1)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY1)))))
OBJ_DAY2:=$(addprefix $(BUILD_PATH_DAY2)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY2)))))
OBJ_DAY3:=$(addprefix $(BUILD_PATH_DAY3)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY3)))))
OBJ_DAY4:=$(addprefix $(BUILD_PATH_DAY4)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY4)))))
OBJ_DAY5:=$(addprefix $(BUILD_PATH_DAY5)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY5)))))
OBJ_DAY6:=$(addprefix $(BUILD_PATH_DAY6)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY6)))))
OBJ_DAY7:=$(addprefix $(BUILD_PATH_DAY7)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY7)))))
OBJ_DAY8:=$(addprefix $(BUILD_PATH_DAY8)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY8)))))
OBJ_DAY9:=$(addprefix $(BUILD_PATH_DAY9)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY9)))))
##### END OBJ LISTS #####

INC:=$(foreach x, $(INC_PATH), $(wildcard $(addprefix $(x)/*,.h)))

# clean files list
DISTCLEAN_LIST= $(OBJ_ALL) \
$(OBJ_DAY1) $(OBJ_DAY2) $(OBJ_DAY3) $(OBJ_DAY4) $(OBJ_DAY5) \
$(OBJ_DAY6) $(OBJ_DAY7) $(OBJ_DAY8) $(OBJ_DAY9)

CLEAN_LIST= $(DISTCLEAN_LIST) \
$(TARGET_DAY1) $(TARGET_DAY2) $(TARGET_DAY3) $(TARGET_DAY4) $(TARGET_DAY5) \
$(TARGET_DAY6) $(TARGET_DAY7) $(TARGET_DAY8) $(TARGET_DAY9)

#default rule
default: makedir all

# non-phony targets
##### BEGIN TARGET RULES #####
$(TARGET_DAY1): $(OBJ_DAY1) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY1) $(OBJ_ALL)

$(TARGET_DAY2): $(OBJ_DAY2) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY2) $(OBJ_ALL)

$(TARGET_DAY3): $(OBJ_DAY3) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY3) $(OBJ_ALL)

$(TARGET_DAY4): $(OBJ_DAY4) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY4) $(OBJ_ALL)

$(TARGET_DAY5): $(OBJ_DAY5) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY5) $(OBJ_ALL)

$(TARGET_DAY6): $(OBJ_DAY6) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY6) $(OBJ_ALL)

$(TARGET_DAY7): $(OBJ_DAY7) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY7) $(OBJ_ALL)

$(TARGET_DAY8): $(OBJ_DAY8) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY8) $(OBJ_ALL)

$(TARGET_DAY9): $(OBJ_DAY9) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY9) $(OBJ_ALL)
##### END TARGET RULES #####

##### BEGIN BUILD OBJECT RULES #####
$(BUILD_PATH_ALL)/%.o: $(SRC_PATH_ALL)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY1)/%.o: $(SRC_PATH_DAY1)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY2)/%.o: $(SRC_PATH_DAY2)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY3)/%.o: $(SRC_PATH_DAY3)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<
	
$(BUILD_PATH_DAY4)/%.o: $(SRC_PATH_DAY4)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY5)/%.o: $(SRC_PATH_DAY5)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY6)/%.o: $(SRC_PATH_DAY6)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY7)/%.o: $(SRC_PATH_DAY7)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY8)/%.o: $(SRC_PATH_DAY8)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<

$(BUILD_PATH_DAY9)/%.o: $(SRC_PATH_DAY9)/%.cpp
	$(CXX) $(CXXOBJFLAGS) -I$(INC_PATH) -o $@ $<
##### END BUILD OBJECT RULES #####

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(BUILD_PATH_ALL) $(BUILD_PATH_DAY1) $(BUILD_PATH_DAY2) $(BUILD_PATH_DAY3) $(BUILD_PATH_DAY4) $(BUILD_PATH_DAY5) \
	$(BUILD_PATH_DAY6) $(BUILD_PATH_DAY7) $(BUILD_PATH_DAY8) $(BUILD_PATH_DAY9) 

.PHONY: all
all: $(TARGET_DAY1) $(TARGET_DAY2) $(TARGET_DAY3) $(TARGET_DAY4) $(TARGET_DAY5) \
 $(TARGET_DAY6) $(TARGET_DAY7) $(TARGET_DAY8) $(TARGET_DAY9)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)