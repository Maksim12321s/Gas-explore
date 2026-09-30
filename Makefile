CXX        := g++
CXXFLAGS   := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Isimulation/include -MMD -MP
SFML_LIBS  := -lsfml-graphics -lsfml-window -lsfml-system
GTEST_LIBS := -lgtest_main -lgtest -pthread
BUILD      := build-make

# ---------- Исходники по модулям ----------
SIM_SRC  := $(wildcard simulation/src/*.cpp)
VIS_SRC  := $(wildcard visual/src/*.cpp)
APP_SRC  := console/main.cpp
TEST_SRC := $(wildcard tests/*.cpp)

SIM_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(SIM_SRC))
VIS_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(VIS_SRC))
APP_OBJ  := $(patsubst %.cpp,$(BUILD)/%.o,$(APP_SRC))
TEST_OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(TEST_SRC))

APP      := $(BUILD)/gaz
VISUAL   := $(BUILD)/gaz_visual
TEST_BIN := $(BUILD)/gaz_tests

.PHONY: all run visual test clean

all: $(APP)

# Консольная программа: физика + console/main.cpp
$(APP): $(SIM_OBJ) $(APP_OBJ)
	$(CXX) $^ -o $@

# Визуализация: физика + всё из visual/src, плюс SFML
$(VISUAL): $(SIM_OBJ) $(VIS_OBJ)
	$(CXX) $^ -o $@ $(SFML_LIBS)

# Тесты: физика + tests, плюс GoogleTest (библиотеки — после объектных файлов)
$(TEST_BIN): $(SIM_OBJ) $(TEST_OBJ)
	$(CXX) $^ -o $@ $(GTEST_LIBS)

# Заголовки визуализации видны только файлам визуализации.
$(VIS_OBJ): CXXFLAGS += -Ivisual/include

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
-include $(SIM_OBJ:.o=.d) $(VIS_OBJ:.o=.d) $(APP_OBJ:.o=.d) $(TEST_OBJ:.o=.d)