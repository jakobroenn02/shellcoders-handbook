# Solutions and Hints

**SPOILER WARNING**: Try to solve the challenges yourself first!

---

## Level 1: Basic Format String

### Concept
When you control a format string, you can use format specifiers to read from the stack.

### Hints
1. Try `./vuln1 "%x"` - prints one value from stack
2. Try `./vuln1 "%x %x %x %x"` - prints multiple values
3. Try `./vuln1 "%p %p %p %p"` - prints as pointers
4. Keep adding more %x until you find 0xdeadbeef

### Example Solution
```bash
./vuln1 "%x %x %x %x %x %x"
# Keep going until you see deadbeef
```

---

## Level 2: Reading Specific Memory

### Concept
Direct parameter access: `%N$x` accesses the Nth parameter directly instead of sequential access.

### Hints
1. Note the addresses of secret1, secret2, secret3
2. Use `%1$x` to access the 1st parameter
3. Try different numbers to find where the secrets are on the stack
4. `%5$x %6$x %7$x` might be around where the secrets are

### Example Approach
```bash
./vuln2 "%1\$x"   # Try parameter 1
./vuln2 "%5\$x"   # Try parameter 5
# Increment N until you find the secrets
```

---

## Level 3: Writing to Memory

### Concept
`%n` writes the number of bytes printed so far to the address on the stack.

### Key Steps
1. Put the target address (of `auth`) on the stack
2. Use format specifiers to position to that address
3. Print exactly 1337 characters before %n
4. Use %n to write 1337 to that address

### Hints
- Target value: 1337 (decimal)
- You need to print 1337 characters total
- Use `%Nc` to print N characters (e.g., `%1337c`)
- The address needs to be at the right position on the stack

### Example Approach (64-bit)
```bash
# Step 1: Find offset where your input buffer is
./vuln3 "AAAA %p %p %p %p %p %p"

# Step 2: Construct exploit with address + format string
# You need: [address of auth][padding][%n at right offset]
# Use python to construct the payload with the address
```

---

## Level 4: Arbitrary Write

### Concept
Overwrite a function pointer to redirect execution. This is the most advanced technique.

### Key Challenges
1. Addresses are likely 64-bit (8 bytes)
2. You need to write a full address byte-by-byte or word-by-word
3. Use `%hhn` for byte writes, `%hn` for 2-byte writes

### Strategy
1. Get addresses of secret_function and func_ptr
2. Calculate what bytes you need to write
3. Write the address of secret_function into func_ptr location
4. May need multiple %n writes for different bytes

### Hints
- `%hhn` writes 1 byte
- `%hn` writes 2 bytes
- `%n` writes 4 bytes
- For 64-bit, you'll need multiple writes
- Write to func_ptr, func_ptr+1, func_ptr+2, etc.

---

## General Tips

### Debugging
```bash
# Use ltrace to see library calls
ltrace ./vuln1 "%x"

# Use gdb to examine memory
gdb ./vuln3
(gdb) break printf
(gdb) run "AAAA %p %p %p"
(gdb) x/20wx $rsp  # examine stack
```

### Finding Offset
```bash
# Use a pattern to find offset
./vuln "AAAA %p %p %p %p %p %p %p %p"
# Look for 0x41414141 (AAAA) to find where your buffer is
```

### Python Helper
```python
# For constructing payloads with addresses
import struct

addr = 0x601060  # Example address
payload = struct.pack("<Q", addr)  # 64-bit little endian
# Then add format string
payload += b"%6$n"
```
