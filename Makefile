CC = g++

CCOPTS = -c -Xpreprocessor -fopenmp #-c
LDOPTS = -L./ -lfreeimage -lglut -lGL

SOURCES = main.cpp read.cpp
OBJECTS = main.o
TARGET = raytracer

raytracer: $(OBJECTS) 
	$(CC) $(LDOPTS) $(OBJECTS) -o $(TARGET) -lomp && ./$(TARGET) && rm -f *.o $(TARGET)
main.o:
	$(CC) $(CCOPTS) $(SOURCES)
run:
	./$(TARGET) ${ARGS}
defualt: $(TARGET)
clean:
	rm -f *.o $(TARGET)


 
