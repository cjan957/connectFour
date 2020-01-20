CXX = g++
CXXFLAGS = -Wall -g

all: c4_test

c4: main.cpp ConnectFour/*.cpp
	$(CXX) $(CXXFLAGS) -o c4 $^

c4_test: test.cpp ConnectFour/*.cpp
	$(CXX) $(CXXFLAGS) -o c4_test $^

test: c4_test
	./c4_test

run: c4
	./c4

clean:
	rm -f c4 c4_test