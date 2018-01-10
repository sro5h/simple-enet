CXX = g++
RM = rm
CXXFLAGS = -I.
LDFLAGS = -lenet -lsfml-system -lsfml-network

PROJECT = app
PROJECT_HEADERS = Event.hpp Host.hpp Peer.hpp
PROJECT_SOURCES = main.cpp Event.cpp Host.cpp Peer.cpp
PROJECT_OBJECTS = $(subst .cpp,.o,$(PROJECT_SOURCES))

all: $(PROJECT)

$(PROJECT): $(PROJECT_OBJECTS)
	$(CXX) -o $(PROJECT) $(PROJECT_OBJECTS) $(CXXFLAGS) $(LDFLAGS)

$(PROJECT_OBJECTS): $(PROJECT_SOURCES) $(PROJECT_HEADERS)
	$(CXX) -c $(PROJECT_SOURCES) $(CXXFLAGS) $(LDFLAGS)

clean:
	$(RM) $(PROJECT_OBJECTS)
	$(RM) $(PROJECT)

.PHONY: clean all
