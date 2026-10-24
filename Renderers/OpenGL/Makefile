# Cross build of the renderer DLLs for the 32-bit Windows game (MinGW-w64), and native tests of the portable core.
OUT ?= build
CXX = i686-w64-mingw32-g++
CXXFLAGS = -O2 -g -Wall -Wno-unused-parameter -std=c++11 -DGTA2GL_BUILD_DLL
# --kill-at: undecorated stdcall export names, as the game resolves them with GetProcAddress.
LDFLAGS = -shared -static -static-libgcc -static-libstdc++ -Wl,--kill-at

CORE = core/gl_funcs.cpp core/gl_renderer.cpp core/gl_enhanced.cpp core/vid_system.cpp core/capture.cpp core/options.cpp
VIDEO_SRC = dll/glvideo.cpp dll/dinput_hook.cpp platform/wgl_context.cpp $(CORE)
RENDER_SRC = dll/glrender.cpp core/gbh_api.cpp core/cmdlog.cpp core/capture.cpp
CAPVIDEO_SRC = tools/capvideo.cpp tools/d3dlog.cpp dll/dinput_hook.cpp core/capture.cpp core/cmdlog.cpp
HEADERS = $(wildcard core/*.hpp platform/*.hpp dll/*.hpp tools/*.hpp)

all: $(OUT)/glvideo.dll $(OUT)/glrender.dll $(OUT)/capvideo.dll

$(OUT):
	mkdir -p $@

$(OUT)/glvideo.dll: $(VIDEO_SRC) $(HEADERS) | $(OUT)
	$(CXX) $(CXXFLAGS) -o $@ $(VIDEO_SRC) $(LDFLAGS) -lopengl32 -lgdi32 -ldinput -ldxguid

$(OUT)/glrender.dll: $(RENDER_SRC) $(HEADERS) | $(OUT)
	$(CXX) $(CXXFLAGS) -o $@ $(RENDER_SRC) $(LDFLAGS)

# Development tool: proxy of DMA's Dmavideo.dll that captures reference frames and logs the D3D calls.
$(OUT)/capvideo.dll: $(CAPVIDEO_SRC) $(HEADERS) | $(OUT)
	$(CXX) $(CXXFLAGS) -o $@ $(CAPVIDEO_SRC) $(LDFLAGS) -lddraw -ldxguid -luuid

# Native tests: gbh layer (texture updates), ini options, and a compile check of the GL core and SDL platform.
HOSTCXX = g++ -O1 -g -Wall -Wno-unused-parameter -std=c++11
test: | $(OUT)
	$(HOSTCXX) -o $(OUT)/gbh_test tests/gbh_test.cpp core/gbh_api.cpp core/cmdlog.cpp core/capture.cpp && $(OUT)/gbh_test
	$(HOSTCXX) -o $(OUT)/options_test tests/options_test.cpp core/options.cpp && $(OUT)/options_test
	for f in core/gl_renderer.cpp core/gl_enhanced.cpp core/vid_system.cpp core/gl_funcs.cpp; do $(HOSTCXX) -c -o /dev/null $$f || exit 1; done
	$(HOSTCXX) $$(sdl2-config --cflags) -c -o /dev/null platform/sdl_context.cpp

clean:
	rm -rf $(OUT)

.PHONY: all test clean
