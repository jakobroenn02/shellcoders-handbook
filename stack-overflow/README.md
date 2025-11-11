# Stack Overflow Vulnerabilities - Chapter 2 Practice

This directory contains practice exercises for stack overflow vulnerabilities from the Shellcoder's Handbook Chapter 2.

## Overview

These exercises progressively teach stack-based buffer overflow exploitation, from basic concepts to advanced techniques. Each level builds upon the previous one.

## Exercises

### Level 1: Basic Buffer Overflow (vuln1.c)
**Difficulty: Beginner**

- Learn about stack layout and how local variables are stored
- Practice overflowing a buffer to modify adjacent variables
- **Goal**: Overflow the buffer to set `auth = 0xdeadbeef`
- **Concepts**: Stack layout, variable ordering, strcpy vulnerability

### Level 2: Overwriting Return Address (vuln2.c)
**Difficulty: Intermediate**

- Understand stack frames (saved base pointer and return address)
- Learn to calculate the offset to the return address
- Practice redirecting execution flow
- **Goal**: Redirect execution to `secret_function()`
- **Concepts**: Return address, instruction pointer control, stack frame structure

### Level 3: Shellcode Injection (vuln3.c)
**Difficulty: Advanced**

- Learn to inject and execute arbitrary code
- Understand NOP sleds and their purpose
- Practice address calculation for shellcode placement
- **Goal**: Inject and execute shellcode to spawn a shell
- **Concepts**: Shellcode, NOP sleds, executable stack, address calculation

### Level 4: Return-to-libc Attack (vuln4.c)
**Difficulty: Expert**

- Understand non-executable stack protections (DEP/NX)
- Learn return-to-libc technique
- Practice chaining library functions
- **Goal**: Spawn a shell using return-to-libc (no shellcode execution)
- **Concepts**: Return-to-libc, DEP/NX bypass, function chaining

## Compiling

### Disable Modern Protections for Practice

For maximum learning, disable security features:

```bash
# 32-bit (easier for learning)
gcc -o vuln1 vuln1.c -m32 -fno-stack-protector -z execstack -no-pie -g

# 64-bit
gcc -o vuln1 vuln1.c -fno-stack-protector -z execstack -no-pie -g
```

### Compilation Flags Explained

- `-m32`: Compile as 32-bit (optional, but makes offsets easier)
- `-fno-stack-protector`: Disable stack canaries
- `-z execstack`: Make stack executable (needed for Level 3)
- `-no-pie`: Disable Position Independent Executable (makes addresses predictable)
- `-g`: Include debug symbols (helpful for GDB debugging)

### For Level 4 (Return-to-libc)

```bash
# Compile with non-executable stack
gcc -o vuln4 vuln4.c -fno-stack-protector -z noexecstack -no-pie -g
```

### Quick Compile with Makefile

```bash
make          # Compile all exercises
make vuln1    # Compile specific exercise
make clean    # Remove compiled binaries
```

## Tools and Techniques

### Essential Tools

- **gdb**: GNU Debugger for examining memory and execution
- **objdump**: Disassemble binaries to find addresses
- **readelf**: View ELF file information
- **python/python3**: Create payloads with binary data
- **perl**: Quick command-line payload generation

### GDB Commands

```bash
# Start debugging
gdb ./vuln1

# Set breakpoints
(gdb) break vulnerable_function
(gdb) break *0x080484ab

# Run with arguments
(gdb) run $(python -c 'print "A"*100')

# Examine memory
(gdb) x/32wx $esp       # 32 words from stack pointer (32-bit)
(gdb) x/32gx $rsp       # 32 giant words (64-bit)
(gdb) x/32xb $buffer    # 32 bytes in hex

# View registers
(gdb) info registers
(gdb) print $eip        # Instruction pointer (32-bit)
(gdb) print $rip        # Instruction pointer (64-bit)

# Disassemble
(gdb) disassemble main
```

### Creating Payloads

```bash
# Python 2
./vuln1 $(python -c 'print "A"*72 + "\xef\xbe\xad\xde"')

# Python 3
./vuln1 $(python3 -c 'import sys; sys.stdout.buffer.write(b"A"*72 + b"\xef\xbe\xad\xde")')

# Perl
./vuln1 $(perl -e 'print "A"x72 . "\xef\xbe\xad\xde"')

# Using files for complex payloads
python3 -c 'import sys; sys.stdout.buffer.write(b"A"*100)' > payload.bin
./vuln1 "$(cat payload.bin)"
```

### Finding Addresses

```bash
# Find function addresses at runtime
objdump -d ./vuln2 | grep secret_function

# Find libc function addresses
ldd ./vuln4                    # See which libc is loaded
readelf -s /lib/libc.so.6 | grep system

# Using GDB to find addresses
gdb ./vuln4
(gdb) break main
(gdb) run
(gdb) print system
(gdb) print &system
(gdb) find "/bin/sh"
```

## Understanding the Stack

### 32-bit Stack Layout (grows downward)

```
High Memory
+------------------+
| Command line     |
| Environment vars |
+------------------+
| Stack            |
|  ...             |
|  Return address  | <- EIP will load this
|  Saved EBP       | <- Frame pointer
|  Local vars      |
|  Buffer[64]      | <- Overflow starts here
+------------------+
| Heap             |
+------------------+
| BSS              |
| Data             |
| Text (code)      |
+------------------+
Low Memory
```

### Typical Stack Frame

```
Higher addresses
+------------------+
| Function args    |
+------------------+
| Return address   | <- Target for overwrite
+------------------+
| Saved EBP/RBP    | <- Base pointer
+------------------+
| Local variables  |
+------------------+
| Buffer           | <- Overflow origin
+------------------+
Lower addresses
```

## Common Pitfalls

1. **Endianness**: x86/x64 use little-endian (least significant byte first)
   - Address `0xdeadbeef` → `\xef\xbe\xad\xde`

2. **Alignment**: Some architectures require address alignment
   - May need to adjust padding

3. **Null bytes**: `strcpy()` stops at null bytes (`\x00`)
   - Avoid null bytes in addresses when possible
   - Use alternative methods if needed

4. **ASLR**: Address Space Layout Randomization randomizes addresses
   - Disable with: `echo 0 | sudo tee /proc/sys/kernel/randomize_va_space`
   - Or use relative offsets/techniques

5. **Bad characters**: Some functions filter certain characters
   - Test your payload for character restrictions

## Security Protections (Modern Systems)

Understanding what you're bypassing:

- **Stack Canaries**: Random values placed before return address
  - Detected when overwritten on function return
  - Disabled with `-fno-stack-protector`

- **DEP/NX** (Data Execution Prevention): Non-executable stack
  - Prevents shellcode execution
  - Bypassed with return-to-libc (Level 4)
  - Disabled with `-z execstack`

- **ASLR**: Randomizes memory addresses
  - Makes exploitation harder
  - Disabled system-wide or with `-no-pie`

- **PIE** (Position Independent Executable): Code at random addresses
  - Similar to ASLR for code section
  - Disabled with `-no-pie`

## Learning Resources

- **The Shellcoder's Handbook** - Chapter 2
- **Hacking: The Art of Exploitation** by Jon Erickson
- **Smashing The Stack For Fun And Profit** by Aleph One
- **LiveOverflow YouTube Channel** - Binary exploitation tutorials
- **pwntools** - Python framework for exploit development

## Ethical Guidelines

These exercises are for **educational purposes only**:

- Only practice on systems you own or have explicit permission to test
- Never use these techniques maliciously or on unauthorized systems
- Understand the law regarding computer security in your jurisdiction
- Use knowledge responsibly to improve security, not break it

## Getting Help

If you're stuck:

1. Check `SOLUTIONS.md` for hints (but try yourself first!)
2. Use GDB to examine what's happening on the stack
3. Draw out the stack layout on paper
4. Calculate offsets carefully
5. Test incrementally (check if you can crash it first)

Remember: The best learning comes from struggling with the problem before looking at solutions!

## Progress Tracking

- [ ] Level 1: Basic Buffer Overflow
- [ ] Level 2: Overwriting Return Address
- [ ] Level 3: Shellcode Injection
- [ ] Level 4: Return-to-libc Attack

Good luck, and happy hacking!
