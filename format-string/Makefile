# Makefile for format string vulnerabilities practice

CC = gcc
# macOS compatible flags
CFLAGS = -fno-stack-protector -Wno-format-security -Wno-deprecated-declarations
ARCH = -m64  # Change to -m32 for 32-bit (may need multilib on macOS)

# Targets
TARGETS = vuln1 vuln2 vuln3 vuln4

all: $(TARGETS)
	@echo "All binaries compiled successfully!"
	@echo "Protections disabled for practice purposes."

vuln1: vuln1.c
	$(CC) $(CFLAGS) $(ARCH) -o vuln1 vuln1.c

vuln2: vuln2.c
	$(CC) $(CFLAGS) $(ARCH) -o vuln2 vuln2.c

vuln3: vuln3.c
	$(CC) $(CFLAGS) $(ARCH) -o vuln3 vuln3.c

vuln4: vuln4.c
	$(CC) $(CFLAGS) $(ARCH) -o vuln4 vuln4.c

32bit: ARCH = -m32
32bit: clean all

clean:
	rm -f $(TARGETS)

test:
	@echo "=== Testing vuln1 ==="
	./vuln1 "%x %x %x %x"
	@echo "\n=== Testing vuln2 ==="
	./vuln2 "%1\$$x %2\$$x %3\$$x"
	@echo "\nRun 'make' to compile all examples"

.PHONY: all clean test 32bit
