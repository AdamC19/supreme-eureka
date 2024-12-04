INC_PATH=inc
SRC_PATH_ALL=src
BUILD_PATH_ALL=build

SRC_PATH_DAY1=src_day1
BUILD_PATH_DAY1=build/day1

SRC_PATH_DAY2=src_day2
BUILD_PATH_DAY2=build/day2

SRC_PATH_DAY3=src_day3
BUILD_PATH_DAY3=build/day3

SRC_PATH_DAY4=src_day4
BUILD_PATH_DAY4=build/day4

BIN_PATH=bin
CC=gcc
CCFLAGS=-Wall -lm -lrt -lpthread
CXX=g++
CXXFLAGS=-std=c++17 -Wall -lm -lrt -lpthread
CXXOBJFLAGS=$(CXXFLAGS) -c

TARGET_DAY1_NAME=day1
TARGET_DAY1:=$(BIN_PATH)/$(TARGET_DAY1_NAME)

TARGET_DAY2_NAME=day2
TARGET_DAY2:=$(BIN_PATH)/$(TARGET_DAY2_NAME)

TARGET_DAY3_NAME=day3
TARGET_DAY3:=$(BIN_PATH)/$(TARGET_DAY3_NAME)

TARGET_DAY4_NAME=day4
TARGET_DAY4:=$(BIN_PATH)/$(TARGET_DAY4_NAME)

SRC_ALL:=$(foreach x, $(SRC_PATH_ALL), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY1:=$(foreach x, $(SRC_PATH_DAY1), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY2:=$(foreach x, $(SRC_PATH_DAY2), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY3:=$(foreach x, $(SRC_PATH_DAY3), $(wildcard $(addprefix $(x)/*,.cpp)))
SRC_DAY4:=$(foreach x, $(SRC_PATH_DAY4), $(wildcard $(addprefix $(x)/*,.cpp)))

OBJ_ALL:=$(addprefix $(BUILD_PATH_ALL)/, $(addsuffix .o, $(notdir $(basename $(SRC_ALL)))))
OBJ_DAY1:=$(addprefix $(BUILD_PATH_DAY1)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY1)))))
OBJ_DAY2:=$(addprefix $(BUILD_PATH_DAY2)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY2)))))
OBJ_DAY3:=$(addprefix $(BUILD_PATH_DAY3)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY3)))))
OBJ_DAY4:=$(addprefix $(BUILD_PATH_DAY4)/, $(addsuffix .o, $(notdir $(basename $(SRC_DAY4)))))

INC:=$(foreach x, $(INC_PATH), $(wildcard $(addprefix $(x)/*,.h)))

# clean files list
DISTCLEAN_LIST=$(OBJ_ALL) $(OBJ_DAY1) $(OBJ_DAY2) $(OBJ_DAY3) $(OBJ_DAY4)
CLEAN_LIST=$(TARGET_DAY1) $(TARGET_DAY2) $(TARGET_DAY3)  $(TARGET_DAY4) $(DISTCLEAN_LIST)

#default rule
default: makedir all

# non-phony targets
$(TARGET_DAY1): $(OBJ_DAY1) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY1) $(OBJ_ALL)

$(TARGET_DAY2): $(OBJ_DAY2) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY2) $(OBJ_ALL)

$(TARGET_DAY3): $(OBJ_DAY3) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY3) $(OBJ_ALL)

$(TARGET_DAY4): $(OBJ_DAY4) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -I$(INC_PATH) -o $@ $(OBJ_DAY4) $(OBJ_ALL)

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

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(BUILD_PATH_ALL) $(BUILD_PATH_DAY1) $(BUILD_PATH_DAY2) $(BUILD_PATH_DAY3) $(BUILD_PATH_DAY4) 

.PHONY: all
all: $(TARGET_DAY1) $(TARGET_DAY2) $(TARGET_DAY3) $(TARGET_DAY4)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)