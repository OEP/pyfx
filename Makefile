CC = g++
AR = ar
SWIGEXEC = swig
SWIGLD = $(CC)
CTAGS = ctags

SRC = src
INC = inc
OBJ = obj
BIN = bin
LIB = lib
PYLIB = pylib/vr
TAGFILE = tags
DEVTOOLS = dev-tools
SWIG = swig
TOOLS = tools
SCRIPTS = scripts
WAVESLIB = $(LIB)/libvrend.a

MKSWIGI = $(DEVTOOLS)/mkswigi
MKCLASS = $(DEVTOOLS)/mkclass
SWIGMODULE = vrend

ARFLAGS=rvs
override CFLAGS+=-g -O2 -Wall -I$(INC) -std=c++0x -c -fPIC -fopenmp
TOOL_CFLAGS += -g -Wall -I$(INC) -std=c++0x -L$(LIB)

LDFLAGS += 
TOOL_LDFLAGS += -lvrend -lpng -lOpenImageIO -fopenmp

SWIGCFLAGS += -g -O2 -c -I$(INC) -I/usr/include/python2.7
SWIGLDFLAGS += -L$(LIB) -lpython2.7 -lvrend -lOpenImageIO -lfftw3 -fopenmp
SWIGFLAGS+=-c++ -python -shadow -I$(INC) -outdir $(PYLIB)

SOURCES=$(wildcard $(SRC)/*.cpp)
HEADERS=$(wildcard $(INC)/*.h)
TOOL_SOURCES=$(wildcard $(TOOLS)/*.cpp)
OBJECTS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))
SWIGROOTS = $(SWIG)/$(SWIGMODULE).i
TOOL_EXECUTABLES = $(patsubst $(TOOLS)/%.cpp, $(BIN)/%, $(TOOL_SOURCES))
SWIGIS = $(patsubst $(INC)/%.h, $(SWIG)/%.i, $(HEADERS))

SWIGWRAPS = $(patsubst $(SWIG)/%.i, $(SWIG)/%_wrap.cxx, $(SWIGROOTS))
SWIGSOS = $(patsubst $(SWIG)/%.i, $(PYLIB)/_%.so, $(SWIGROOTS))
SWIGPYS = $(patsubst $(SWIG)/%.i, $(PYLIB)/%.py, $(SWIGROOTS))
#SWIGPYLINKS = $(patsubst $(LIB)/%.py, $(SCRIPTS)/%.py, $(SWIGPYS))
#SWIGSOLINKS = $(patsubst $(LIB)/%.so, $(SCRIPTS)/%.so, $(SWIGSOS))

-include custom.mk

all: debug

## 'make release' to not print debug info.
release: setreleaseflag everything

setreleaseflag:
	$(eval CFLAGS += -DRELEASE)

debug: everything

everything: directories $(SOURCES) $(TAGFILE) $(WAVESLIB) $(TOOL_EXECUTABLES) genswig

swigis: $(SWIGIS)

## generate swig interfaces and softlinks in scripts/ directory.
genswig: $(WAVESLIB) $(SWIGSOS)

## clean up base code, swig, and pyc's
clean: cleanbase cleanswig cleanpyc

cleanbase:
	rm -f $(OBJ)/*.o
	rm -f $(TOOL_EXECUTABLES)
	rm -f $(WAVESLIB)
	
cleanswig:
	rm -f $(SWIGSOS) $(SWIGPYS) $(SWIGWRAPS)

cleanpyc:
	rm -f $(PYLIB)/*.pyc
	rm -f $(SCRIPTS)/*.pyc
  
directories: $(LIB) $(OBJ) $(BIN)

## generates ctag
$(TAGFILE): $(SOURCES) $(HEADERS)
	$(CTAGS) $(SOURCES) $(HEADERS)

## generates static library
$(WAVESLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(WAVESLIB) $(OBJECTS)

## generates tool executables
bin/%: $(TOOLS)/%.cpp $(WAVESLIB)
	$(CC) $(TOOL_CFLAGS) $< -o $@ $(TOOL_LDFLAGS)

## generates object files from library
$(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h
	$(CC) $(CFLAGS) $< -o $@

## generates lib directory
$(LIB):
	mkdir -p $(LIB)

## generates obj directory
$(OBJ):
	mkdir -p $(OBJ)

## generates bin directory
$(BIN):
	mkdir -p $(BIN)

## generates swig wrapper code
$(SWIG)/%_wrap.cxx: $(SWIG)/%.i
	$(SWIGEXEC) $(SWIGFLAGS) $<

$(PYLIB)/%.py: $(SWIG)/%_wrap.o

## generate basic classes
$(INC)/%.h:
	$(MKCLASS) vr $@ > $@

## generate swig interfaces
$(SWIG)/%.i:
	$(eval HEADER=$(patsubst $(SWIG)/%.i, $(INC)/%.h, $@))
	$(eval BASENAME=$(patsubst $(SWIG)/%.i, %.i, $@))
	$(MKSWIGI) $(SWIGMODULE) $(HEADER) > $@
	echo "%include \"$(BASENAME)\"" >> $(SWIGROOTS)

## generates swig wrapper object
$(SWIG)/%_wrap.o: $(SWIG)/%_wrap.cxx
	$(CC) -fPIC $(SWIGCFLAGS) $< -o $@

## generates swig shared library
$(PYLIB)/_%.so: $(SWIG)/%_wrap.o $(WAVESLIB) $(HEADERS)
	$(SWIGLD) -fPIC -g -shared  $< -o $@ $(SWIGLDFLAGS)
