CXX:=em++
PROJDIR:=./emscripten_project
OUTFORMAT:=lib.mjs
SOURCES:=$(PROJDIR)/src/main.cpp $(PROJDIR)/src/shader.cpp
OUTDIR:=bin
INCLUDES:=-I$(PROJDIR)/src/vendor

FLAGS=-s WASM=1 -s EXIT_RUNTIME=1 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1
LIBS=-lGL

all: $(OUTDIR)/$(OUTFORMAT)
	
$(OUTDIR): 
	mkdir $(OUTDIR)


$(OUTDIR)/$(OUTFORMAT): $(SOURCES) | $(OUTDIR)
	$(CXX) -o $@ $(SOURCES) $(FLAGS) $(LIBS) $(INCLUDES) --preload-file ./emscripten_project/src/resources

.PHONY: clean all

clean:
	del $(OUTDIR)/$(OUTFORMAT)

