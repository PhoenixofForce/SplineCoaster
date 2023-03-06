default: debug	
	Game.exe

debug:
	g++ ../src/*.cpp ../src/math/*.cpp -o Game.exe -O2 -Wall -Wno-missing-braces -I ../include/win/ -L ../lib/win/ -lraylib -lopengl32 -lgdi32 -lwinmm -Wno-unused-function -Wno-sign-compare

desktop:
	g++ ../src/*.cpp ../src/math/*.cpp -o Game.exe -O2 -Wall -Wno-missing-braces -I ../include/win/ -L ../lib/win/ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

web: 
	emcc ../src/main.cpp ../src/math/matrix.cpp ../src/math/vector.cpp \
	-o index.html -Os -Wall -Wno-narrowing -Wno-missing-braces ../lib/web/libraylib.a -I ../include/web/ -L ../lib/web/ -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB \
	-s EXPORTED_FUNCTIONS="['_main', '_malloc']" \
	-s EXPORTED_RUNTIME_METHODS=["ccall"] \
	-s TOTAL_MEMORY=512mb \
	-s INITIAL_MEMORY=256mb \
	-sALLOW_MEMORY_GROWTH \
	-sASSERTIONS --profiling-funcs -g \
	--shell-file ./shell.html
#--preload-file res/ \

build: desktop web