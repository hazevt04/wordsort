TARGET_EXEC = ws
CC = gcc
CFLAGS = -g -I. $(DEBUG)

.PHONY: default all clean

default: $(TARGET_EXEC)
all: default	

debug: DEBUG = -DHDEBUG
debug: $(TARGET_EXEC)

# Reminder:
# $@ file name of the target of the rule
# $% target member name
# $< name of first prerequisite

# patsubst and wildcard allow this makefile to 
# automagically include all .c and *.h files 
# in current directory
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# If make is killed or interrupted the targets,
# $(TARGET) and $(OBJECTS) are not deleted.
# Also makes sure to keep .o files around
.PRECIOUS: $(TARGET_EXEC) $(OBJECTS)

$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET_EXEC)

check:
	echo "OBJECTS are $(OBJECTS)"
	echo "HEADERS are $(HEADERS)"



