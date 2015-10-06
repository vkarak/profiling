.PHONY: all clean
.DEFAULT: all

DEBUG  ?= 1
VTUNE  ?= 0
LIKWID ?= 0
OPENMP ?= 1
CXX = g++
MKDIR_P = mkdir -p
RM = /bin/rm

MAKE_CPPFLAGS = -D_GNU_SOURCE -Iinclude -Iinclude/profilers
MAKE_CXXFLAGS = -Wall -std=c++11
MAKE_LDFLAGS = -L.

ifeq ($(DEBUG), 1)
	MAKE_CPPFLAGS += -D_DEBUG_
	MAKE_CXXFLAGS += -g -O0
else
	MAKE_CPPFLAGS += -DNDEBUG
	MAKE_CXXFLAGS += -g -O3
endif

ifeq ($(OPENMP), 1)
	MAKE_CXXFLAGS += -fopenmp
	MAKE_LDFLAGS  += -fopenmp
endif

ifeq ($(VTUNE), 1)
	MAKE_CPPFLAGS += -I$(VTUNE_INCDIR) -DHAVE_VTUNE
	MAKE_LDFLAGS  += -L$(VTUNE_LIBDIR) -littnotify -ldl
endif

ifeq ($(LIKWID), 1)
	MAKE_CPPFLAGS += -I$(LIKWID_INCDIR) -DHAVE_LIKWID
	MAKE_LDFLAGS  += -L$(LIKWID_LIBDIR) -llikwid
endif

CPU_CPPFLAGS = $(MAKE_CPPFLAGS) $(CPPFLAGS)
CPU_CXXFLAGS = $(MAKE_CXXFLAGS) $(CXXFLAGS)
CPU_LDFLAGS  = $(LDFLAGS) $(MAKE_LDFLAGS)

CPU_COMPILE = $(CXX) -c $(CPU_CXXFLAGS) $(CPU_CPPFLAGS)

LIBRARIES = libprof.a
libprof_SOURCES = \
	src/chrono_profiler.cpp \
	src/profiling.cpp

libprof_OBJECTS = $(libprof_SOURCES:%.cpp=%.o)


PROGRAMS = profiling_example
profiling_example_SOURCES = \
	src/profiling_example.cpp
profiling_example_OBJECTS = $(profiling_example_SOURCES:%.cpp=%.o) $(LIBRARIES)
profiling_example_LIBS = -lprof

all: $(PROGRAMS) $(LIBRARIES)

DEPDIR = .deps
df = $(DEPDIR)/$(*D)/$(*F)
%.o: %.cpp
	@$(MKDIR_P) $(DEPDIR)/$(*D)
	$(CPU_COMPILE) -MD -o $@ $<
	@cp $*.d $(df).P; \
    sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P; \
	rm -f $*.d

-include $(profiling_example_SOURCES:%.cpp=$(DEPDIR)/%.P)

profiling_example: $(profiling_example_OBJECTS) $(profiling_example_LIBS)
	$(CXX) -o $@ $(profiling_example_OBJECTS) $(profiling_example_LIBS) $(CPU_LDFLAGS)

libprof.a: $(libprof_OBJECTS)
	$(AR) cru $@ $^

clean:
	$(RM) -rf $(DEPDIR)
	$(RM) -f $(PROGRAMS) $(profiling_example_OBJECTS) $(libprof_OBJECTS)
