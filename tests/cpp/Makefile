# Standalone build, kept for convenience; the CMake build (see the top-level
# CMakeLists.txt) is the supported one.
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


all: tests.cpp
	$(CXX) -o tests tests.cpp -I../.. -Wall -Wextra

clean:
	rm -f *.o tests
