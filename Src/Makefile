PROG=osm2oob
CPPFLAGS=-g -Wall
LDFLAGS=-lboost_system -lboost_filesystem -lexpat -lboost_iostreams
OBJS=oob.o obstacle.o nodeDb.o elevation.o coord3d.o bazlCsv.o osm2oob.o

all: $(OBJS)
	mkdir -p build
	$(CXX) -o build/$(PROG) $(OBJS) $(LDFLAGS)

clean:
	        rm *.o osm2oob

install:
	cp $< build/osm2oob /usr/local/bin

uninstall:
	rm -f /usr/local/bin/osm2oob

