CXX        := g++
MODE ?= release

ifeq ($(MODE),debug)
	OPT 	:= -O0 -g
	BUILD 	:= build-make/debug
else ifeq ($(MODE), asan)
	OPT 	:= -O1 -g  -fno-omit-frame-pointer -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
  	BUILD   := build-make/asan
else
	OPT     := -O2
  	BUILD   := build-make/release
endif

CXXFLAGS   := -std=c++17 $(OPT) -Wall -Wextra -Wpedantic \
              -Isimulation/include -Ianalysis/include -MMD -MP
SFML_LIBS  := -lsfml-graphics -lsfml-window -lsfml-system
GTEST_LIBS := -lgtest_main -lgtest -pthread

SIM_SRC  := $(wildcard simulation/src/*.cpp)
VIS_SRC  := $(wildcard visual/src/*.cpp)
APP_SRC  := console/main.cpp
TEST_SRC := $(wildcard tests/*.cpp)
AN_SRC   := $(wildcard analysis/src/*.cpp)


SIM_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(SIM_SRC))
VIS_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(VIS_SRC))
APP_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(APP_SRC))
TEST_OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(TEST_SRC))
AN_OBJ   := $(patsubst %.cpp,$(BUILD)/%.o,$(AN_SRC))


APP      := $(BUILD)/gaz
VISUAL   := $(BUILD)/gaz_visual
TEST_BIN := $(BUILD)/gaz_tests

.PHONY: all run visual test clean plot gdb gdb-visual gdb-test 

all: $(APP)

$(APP): $(SIM_OBJ) $(APP_OBJ) $(AN_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(VISUAL): $(SIM_OBJ) $(VIS_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(SFML_LIBS) 

$(TEST_BIN): $(SIM_OBJ) $(TEST_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS) $(GTEST_LIBS)

$(VIS_OBJ): CXXFLAGS += -Ivisual/include

$(AN_OBJ) $(APP_OBJ): CXXFLAGS += -Ianalysis/include 

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(APP)
	./$(APP)

visual: $(VISUAL)
	./$(VISUAL)

ifeq ($(TEST_SRC),)
test:
	@echo "В папке tests/ пока нет тестов"
else
test: $(TEST_BIN)
	./$(TEST_BIN)
endif

plot: $(APP)
	  ./$(APP)
	#   python3 scripts/plot_adiabat.py

gdb: $(APP)
	gdb ./$(APP)

gdb-visual: $(VISUAL)
	gdb ./$(VISUAL)

gdb-test: $(TEST_BIN)
	gdb ./$(TEST_BIN)

clean:
	rm -rf build-make

-include $(SIM_OBJ:.o=.d) $(VIS_OBJ:.o=.d) $(APP_OBJ:.o=.d) $(TEST_OBJ:.o=.d) $(AN_OBJ:.o=.d)