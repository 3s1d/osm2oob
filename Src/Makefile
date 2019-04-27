PROG=osm2oob
CPPFLAGS=-g -Wall
LDFLAGS=-lboost_system -lboost_filesystem -lexpat -lboost_iostreams
OBJS=oob.o obstacle.o nodeDb.o elevation.o coord3d.o bazlCsv.o osm2oob.o

all: $(OBJS)
	mkdir -p Build
	$(CXX) -o Build/$(PROG) $(OBJS) $(LDFLAGS)

clean:
	        rm *.o osm2oob
