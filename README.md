# ArduBIOS
ArduBios is a simple 8 bits micro computer code system for Arduino.

## News
New v2.0 arrived and now contains some new features:
- Liquid Cristal 16x2 Display support
- Port 13 now have a opcode for simple beeps using buzzers
- Fixed bugs
- Call and Ret functions

## Features
- Basic Output system in Serial Monitor and LCD 16x2 (50% of a BIOS)
- Reserve 256 Bytes for the Stack
- Reserve 512 Bytes for the user RAM (Ram for the programs)
- Registers n Flags (50% done) : (A,B,C,D Regs n Zero,Carry,Negative Flags)
- Basic Opcodes
- Jumps and Conditional Jumps (jmp n; jz n; call; ret)
- Simple math operators (add and sub)
- Halt and Null values (0x00; 0xFF)
- Beeps for buzzers support at port 8

## Ideas for the future
- Full BIOS (Input n Output)
- Sdcard as Floppy Disk support
- Generic keyboard input support
- File system like Fat12
- Public Assembler made in NodeJS
- Interrupt system

## Memory map
| Size in Bytes | Usage |
| --- | --- |
| n | Rom/Card |
| 512 | User RAM |
| 256 | Stack |

## Flags, SR and IsBitSet

### Flags
In ArduBios, you have the basics flags Zero, Carry and Negative for your operations and conditions. All the flags are stored in a unique single byte called SR where Bit 0 = Zero, Bit 1 = Carry and Bit 2 = Negative.

#### Map
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
| Empty | Empty | Empty | Empty | Empty | Negative | Carry | Zero |

### IsBitSet
```c
unsigned char IsBitSet(unsigned char bytevalue, int pos) {
  unsigned char mask = (1<<pos);
  if((bytevalue & mask) != 0) return 1;
  return 0;
}
```

This is a function that verify if a specific byte is SET, this can be used to verify if a bit in SR (Status Register) is 1.

## Opcodes
| Opcode | Usage | Bytes |
| --- | --- | --- |
| 0x00 | null | 1 |
| 0x01 | Print char from stack | 1 |
| 0x02 | Push a byte on stack | 2 |
| 0x03 | Push A reg to Stack | 1 |
| 0x04 | Push B reg to Stack | 1 |
| 0x05 | Push C reg to Stack | 1 |
| 0x06 | Push D reg to Stack | 1 |
| 0x07 | Make a beep for Buzzers at port 3 | 1 |
| 0x10 | Store a value generic to ram | 3 |
| 0x11 | Push a value from ram to stack | 2 |
| 0x12 | Pop a value from stack to ram | 2 |
| 0x20 | Store a generic value for register A | 2 |
| 0x21 | Store a generic value for register B | 2 |
| 0x22 | Store a generic value for register C | 2 |
| 0x23 | Store a generic value for register D | 2 |
| 0x24 | Store a stack value for register A | 1 |
| 0x25 | Store a stack value for register B | 1 |
| 0x26 | Store a stack value for register C | 1 |
| 0x27 | Store a stack value for register D | 1 |
| 0x30 | Add N to A | 2 |
| 0x31 | Add B to A | 1 |
| 0x32 | Add C to A | 1 |
| 0x33 | Add D to A | 1 |
| 0x34 | Sub N to A | 2 |
| 0x35 | Sub B to A | 1 |
| 0x36 | Sub C to A | 1 |
| 0x37 | Sub D to A | 1 |
| 0x40 | Jmp N | 2 |
| 0x41 | JZ N (Jump if Zero Flag set) | 2 |
| 0x42 | Call Byte | 2 |
| 0x43 | Return | 1 |
| 0xFF | Halt | 1 |
