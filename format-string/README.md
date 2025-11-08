# Format String Vulnerabilities - Chapter 3 Practice

This directory contains practice exercises for format string vulnerabilities from the Shellcoder's Handbook.

## Exercises

### Level 1: Basic Format String (vuln1.c)
- Simple format string vulnerability
- Goal: Read values from the stack
- Practice using %x, %p, %s format specifiers

### Level 2: Reading Specific Memory (vuln2.c)
- Direct parameter access
- Goal: Read a specific variable's value
- Practice using %n$x notation

### Level 3: Writing to Memory (vuln3.c)
- Writing arbitrary values using %n
- Goal: Overwrite a variable to change program behavior
- Practice using %n to write values

### Level 4: Arbitrary Write (vuln4.c)
- Complete arbitrary memory write
- Goal: Overwrite a function pointer or return address
- Advanced exploitation technique

## Compiling

Disable modern protections for practice:
```bash
gcc -o vuln1 vuln1.c -fno-stack-protector -z execstack -no-pie -m32
```

For 64-bit:
```bash
gcc -o vuln1 vuln1.c -fno-stack-protector -z execstack -no-pie
```

## Useful Format Specifiers

- `%x` - Print hex value from stack
- `%p` - Print pointer
- `%s` - Print string (dangerous if controlled)
- `%n` - Write number of bytes written so far
- `%Nc` - Print N characters (for padding)
- `%N$x` - Access Nth parameter directly

## Tools

- `ltrace` - Trace library calls
- `gdb` - Debug and examine memory
- `objdump -d` - Disassemble binary
- `readelf -s` - View symbols
