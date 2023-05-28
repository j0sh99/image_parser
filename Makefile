CC = gcc
CFLAGS = -Wall -Werror

# Add any additional libraries or flags if needed
LIBS = -ljpeg
LDFLAGS =

# Specify the target executable name
TARGET = parser

# List the source files
SRCS = parser.c

# Derive the object file names from the source file names
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule for compiling each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
