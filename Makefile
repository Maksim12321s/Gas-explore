CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
BUILD    := build-make

# Исходники библиотеки, программ и тестов.
LIB_SRC  := $(wildcard src/*.cpp)
LIB_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(LIB_SRC))
APP_OBJ  := $(BUILD)/main.o
VIS_OBJ  := $(BUILD)/visual.o
TEST_SRC := $(wildcard tests/*.cpp)
TEST_OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(TEST_SRC))

APP      := $(BUILD)/gaz
VISUAL   := $(BUILD)/gaz_visual
TEST_BIN := $(BUILD)/gaz_tests

.PHONY: all run visual test clean

all: $(APP)

$(APP): $(LIB_OBJ) $(APP_OBJ)
	$(CXX) $^ -o $@

$(VISUAL): $(LIB_OBJ) $(VIS_OBJ)
	$(CXX) $^ -o $@ $(SFML_LIBS)

$(TEST_BIN): $(LIB_OBJ) $(TEST_OBJ)
	$(CXX) $^ -o $@

# Общее правило: build-make/путь/файл.o собирается из путь/файл.cpp
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

clean:
	rm -rf $(BUILD)

# Файлы зависимостей от заголовков (создаются флагами -MMD -MP).
-include $(LIB_OBJ:.o=.d) $(APP_OBJ:.o=.d) $(VIS_OBJ:.o=.d) $(TEST_OBJ:.o=.d)