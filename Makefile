EXE = meshflow
IMGUI_DIR = ./external/imgui
NODE_EDITOR_DIR = ./external/imgui-node-editor
SOURCES = main.cpp $(wildcard src/*.cpp) $(wildcard src/nodes/*.cpp)
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(NODE_EDITOR_DIR)/imgui_node_editor.cpp $(NODE_EDITOR_DIR)/imgui_node_editor_api.cpp $(NODE_EDITOR_DIR)/imgui_canvas.cpp $(NODE_EDITOR_DIR)/crude_json.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addprefix build/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CXXFLAGS = -std=c++17 -Isrc -Isrc/nodes -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(NODE_EDITOR_DIR) `pkg-config --cflags glfw3`
CXXFLAGS += -g -Wall -Wformat -pthread
LIBS = -lGL `pkg-config --static --libs glfw3`
CFLAGS = $(CXXFLAGS)

build/%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o:src/nodes/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/%.o:$(NODE_EDITOR_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: build/$(EXE)
	@echo Build complete for Linux

build/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f build/$(EXE) build/$(OBJS)
