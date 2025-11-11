# Makefile for Stack Overflow Exercises
# Chapter 2 - The Shellcoder's Handbook

CC = gcc
CFLAGS_COMMON = -fno-stack-protector -g -Wall

# Detect OS and set appropriate flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS_COMMON += -no-pie
    CFLAGS_EXEC = -z execstack
    CFLAGS_NOEXEC = -z noexecstack
    CFLAGS_32 = -m32
else ifeq ($(UNAME_S),Darwin)
    # macOS doesn't support -z flag, stack is executable by default
    CFLAGS_EXEC = -Wl,-no_pie
    CFLAGS_NOEXEC = -Wl,-no_pie
    CFLAGS_32 =
    $(info Note: macOS detected. Stack protections differ from Linux.)
    $(info 32-bit builds disabled on macOS (not supported on Apple Silicon))
endif

# Targets
TARGETS = vuln1 vuln2 vuln3 vuln4
TARGETS_32 = vuln1_32 vuln2_32 vuln3_32 vuln4_32

.PHONY: all clean 32bit help

# Default: compile all 64-bit versions
all: $(TARGETS)

# Compile all 32-bit versions
32bit: $(TARGETS_32)

# Level 1: Basic buffer overflow (64-bit)
vuln1: vuln1.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln1 (64-bit) - Basic buffer overflow"

# Level 2: Overwriting return address (64-bit)
vuln2: vuln2.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln2 (64-bit) - Return address overwrite"

# Level 3: Shellcode injection (64-bit)
vuln3: vuln3.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln3 (64-bit) - Shellcode injection"

# Level 4: Return-to-libc (64-bit, non-executable stack)
vuln4: vuln4.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_NOEXEC) -o $@ $<
	@echo "Built vuln4 (64-bit) - Return-to-libc (NX enabled)"

# 32-bit versions (easier for learning)
vuln1_32: vuln1.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_32) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln1_32 (32-bit) - Basic buffer overflow"

vuln2_32: vuln2.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_32) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln2_32 (32-bit) - Return address overwrite"

vuln3_32: vuln3.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_32) $(CFLAGS_EXEC) -o $@ $<
	@echo "Built vuln3_32 (32-bit) - Shellcode injection"

vuln4_32: vuln4.c
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_32) $(CFLAGS_NOEXEC) -o $@ $<
	@echo "Built vuln4_32 (32-bit) - Return-to-libc (NX enabled)"

# Clean up binaries
clean:
	rm -f $(TARGETS) $(TARGETS_32)
	@echo "Cleaned all binaries"

# Help target
help:
	@echo "Stack Overflow Exercises - Makefile"
	@echo "===================================="
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build all 64-bit exercises"
	@echo "  make 32bit    - Build all 32-bit exercises"
	@echo "  make vuln1    - Build only vuln1 (64-bit)"
	@echo "  make vuln1_32 - Build only vuln1 (32-bit)"
	@echo "  make clean    - Remove all compiled binaries"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Individual targets: vuln1, vuln2, vuln3, vuln4"
	@echo "32-bit versions: vuln1_32, vuln2_32, vuln3_32, vuln4_32"
	@echo ""
	@echo "Compilation flags used:"
	@echo "  -fno-stack-protector : Disable stack canaries"
	@echo "  -no-pie              : Disable position independent executable"
	@echo "  -z execstack         : Make stack executable (vuln1-3)"
	@echo "  -z noexecstack       : Make stack non-executable (vuln4)"
	@echo "  -g                   : Include debug symbols"
	@echo "  -m32                 : Compile as 32-bit (32bit target)"
	@echo ""
	@echo "Note: 32-bit compilation requires multilib support"
	@echo "Install with: sudo apt-get install gcc-multilib (Ubuntu/Debian)"
	@echo "           or: sudo dnf install glibc-devel.i686 (Fedora)"
