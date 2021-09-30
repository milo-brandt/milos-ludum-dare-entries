index:
	emcc -g  Source/FontRenderer.cpp Source/Game/CannedLevel.cpp Source/Transitions.cpp Source/Game/Random.cpp Source/KeyboardHandler.cpp Source/Game/GameState.cpp Source/Game/Physics.cpp Source/frame.cpp Source/Templates/Dynamic.cpp Source/Templates/RawBuffer.cpp Source/main.cpp Source/Common/Window.cpp Source/Common/Audio.cpp -std=c++17 -s WASM=1 -s USE_SDL=2 -O3 -s ALLOW_MEMORY_GROWTH=1 -o index.js -DCOMPILE_FOR_EMSCRIPTEN

.PHONY: clean
clean:
	rm index.html
	rm index.js
	rm index.wasm
