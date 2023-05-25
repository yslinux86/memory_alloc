CC      = gcc
INCLUDE =
LIB     =
CFLAGS  = -g -O0 -Wall -fno-strict-aliasing

OBJECTS = $(patsubst %.c, %.o,$(wildcard *.c))
TARGET  = memory-alloc

all: $(TARGET)

$(TARGET):${OBJECTS}
	$(CC) -o $@ $^ $(INCLUDE) $(LIB) $(CFLAGS)
	@echo ----------------------------------------
	@echo         $(TARGET) complete
	@echo ----------------------------------------

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)
