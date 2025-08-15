# Sketch File (.sk) Viewer

A C program that loads and displays `.sk` (Sketch) files in a 200×200 pixel SDL2 window.  
It parses each byte of the file, decodes opcodes and operands, and renders lines, blocks, and colours according to the Sketch file format.

---

## Features

- **Sketch File Parsing** — Reads `.sk` files byte by byte and interprets drawing commands.
- **Opcode/Operand Decoding** — Extracts 2-bit opcodes and signed 6-bit operands.
- **Drawing Tools**:
  - `LINE`: Draw a line from the current position to the target position.
  - `BLOCK`: Draw a filled block defined by current and target coordinates.
  - `COLOUR`: Set the current drawing colour.
  - `TARGETX` / `TARGETY`: Update target coordinates for future commands.
- **State Management** — Maintains the current drawing state (position, tool, target, data).
- **SDL2 Display** — Uses `displayfull.c` and SDL2 to create a simple interactive viewer.

---

## Technologies Used

- **Language:** C (C11 standard)
- **Libraries:** SDL2
- **Files:**
  - `sketch.c` — Core implementation of the Sketch interpreter
  - `displayfull.c` / `displayfull.h` — SDL2 wrapper for drawing
  - `sketch.h` — Header for Sketch state and function declarations
  - `test.c` — Test harness for unit testing

---

## Building

The repository includes a `Makefile` with multiple targets:

**Build and run tests:**
```bash
make test
```

**Build the viewer:**
```bash
make sketch
```

**Compile any `.c` file as a standalone program:**
```bash
make <filename-without-.c>
```

---

## Running

Once compiled, run the viewer with:
```bash
./sketch <file.sk>
```

Example:
```bash
./sketch example.sk
```

This will open a 200×200 pixel SDL2 window and render the sketch.

---

## How It Works

1. **newState()**: Allocates and initializes a new drawing state.
2. **getOpcode()**: Extracts the 2-bit opcode from the byte.
3. **getOperand()**: Extracts the signed 6-bit operand.
4. **obey()**: Executes a drawing command based on the opcode and operand.
5. **processSketch()**: Opens a Sketch file, processes it byte-by-byte, and calls `obey()` for each instruction.
6. **view()**: Creates an SDL2 display, loads the file, runs the render loop, and frees resources.

---

## Example Opcode Table

| Opcode | Meaning     |
|--------|-------------|
| `00`   | DX (change X) |
| `01`   | DY (change Y and draw) |
| `10`   | TOOL (set drawing mode) |
| `11`   | DATA (load extra data) |
