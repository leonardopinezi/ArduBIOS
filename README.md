# ArduBIOS
ArduBios is a simple 8 bits micro computer code system for Arduino.

---

## News
New v3.0 arrived and now contains several powerful features for low-level emulation:
- **Liquid Crystal 16x2 Display support**: Full integration with I2C displays.
- **New SysCall System**: Added `0x08` opcode to natively clear the LCD screen.
- **Buzzer Support**: Port 13 now has a dedicated opcode (`0x07`) for simple beeps.
- **Timing Control**: Added `0x60` opcode for precise hardware delays.
- **Logic Expansion**: Added a full suite of Comparison opcodes (`0x50`-`0x53`) for advanced conditional branching.
- **Subroutine Support**: Implementation of `Call` and `Ret` functions using a dedicated Call Stack (`CStack`).

## Features
- **Dual Output System**: Synchronized output to both Serial Monitor and LCD 16x2.
- **Memory Architecture**:
    - **ROM/Card**: Variable size for program storage.
    - **User RAM**: 512 Bytes reserved for data manipulation.
    - **Data Stack**: 128 Bytes for arithmetic and logic operations.
    - **Call Stack**: 128 Bytes dedicated to subroutine return addresses.
- **Registers & Flags**: 
    - 4 General Purpose Registers (A, B, C, D).
    - Status Register (SR) containing **Zero**, **Carry**, and **Negative** (Borrow) flags.
- **Full Opcode Set**:
    - **Arithmetic**: `Add` and `Sub` (supporting both constants and registers).
    - **Comparison**: `Cmp` instructions to update flags without altering registers.
    - **Flow Control**: Unconditional `Jmp`, Conditional `JZ` (Jump if Zero), `Call`, and `Ret`.
    - **Hardware Control**: `Delay`, `Beep`, `Clear LCD`, and `Halt`.

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

| Opcode | Usage | Bytes | Description |
| :--- | :--- | :---: | :--- |
| **0x00** | `null` | 1 | No operation (NOP). |
| **0x01** | `Print char from stack` | 1 | Pop value from stack and print to LCD/Serial. |
| **0x02** | `Push N` | 2 | Push a constant byte `N` onto stack. |
| **0x03** | `Push A` | 1 | Push Register A to stack. |
| **0x04** | `Push B` | 1 | Push Register B to stack. |
| **0x05** | `Push C` | 1 | Push Register C to stack. |
| **0x06** | `Push D` | 1 | Push Register D to stack. |
| **0x07** | `Beep` | 1 | Make a beep (1kHz, 100ms) at **Port 13**. |
| **0x08** | `Clear LCD` | 1 | **SysCall:** Clear the 16x2 LCD display. |
| **0x10** | `Store generic to ram` | 3 | Store value `N` at RAM address `ADR`. |
| **0x11** | `Push from ram to stack` | 2 | Load value from RAM `ADR` and push to stack. |
| **0x12** | `Pop from stack to ram` | 2 | Pop value from stack and store at RAM `ADR`. |
| **0x20** | `Store generic to Reg A` | 2 | Move constant value `N` to Register A. |
| **0x21** | `Store generic to Reg B` | 2 | Move constant value `N` to Register B. |
| **0x22** | `Store generic to Reg C` | 2 | Move constant value `N` to Register C. |
| **0x23** | `Store generic to Reg D` | 2 | Move constant value `N` to Register D. |
| **0x24** | `Store stack to Reg A` | 1 | Pop value from stack to Register A. |
| **0x25** | `Store stack to Reg B` | 1 | Pop value from stack to Register B. |
| **0x26** | `Store stack to Reg C` | 1 | Pop value from stack to Register C. |
| **0x27** | `Store stack to Reg D` | 1 | Pop value from stack to Register D. |
| **0x30** | `Add N to A` | 2 | Add constant `N` to A (Updates Flags). |
| **0x31** | `Add B to A` | 1 | Add B to A (Updates Flags). |
| **0x32** | `Add C to A` | 1 | Add C to A (Updates Flags). |
| **0x33** | `Add D to A` | 1 | Add D to A (Updates Flags). |
| **0x34** | `Sub N from A` | 2 | Subtract constant `N` from A (Updates Flags). |
| **0x35** | `Sub B from A` | 1 | Subtract B from A (Updates Flags). |
| **0x36** | `Sub C from A` | 1 | Subtract C from A (Updates Flags). |
| **0x37** | `Sub D from A` | 1 | Subtract D from A (Updates Flags). |
| **0x40** | `Jmp N` | 2 | Unconditional Jump to address `N`. |
| **0x41** | `JZ N` | 2 | Jump to address `N` if **Zero Flag** is set. |
| **0x42** | `Call N` | 2 | Call subroutine at address `N` (Push return to CStack). |
| **0x43** | `Return` | 1 | Return from subroutine (Pop from CStack). |
| **0x50** | `Cmp A with N` | 2 | Compare A with constant `N` (Updates Flags only). |
| **0x51** | `Cmp A with B` | 1 | Compare A with B (Updates Flags only). |
| **0x52** | `Cmp A with C` | 1 | Compare A with C (Updates Flags only). |
| **0x53** | `Cmp A with D` | 1 | Compare A with D (Updates Flags only). |
| **0x60** | `Delay N` | 2 | Wait for `N` milliseconds. |
| **0xFF** | `Halt` | 1 | Stop execution. |
