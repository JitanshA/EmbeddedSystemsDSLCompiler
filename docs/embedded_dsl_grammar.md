# Embedded Systems DSL Grammar

This DSL is designed for embedded systems programming with a focus on hardware interaction and efficient execution. Below is a brief description of its structure and allowed instructions:

---

## Key Features

### 1. Variable Declarations
- Supported types: `int`, `bool`.
- **Syntax**: 
  ```c
  int a = 5;
  bool flag = true;
  ```

### 2. Control Flow
- Conditional statements: `if`, `else`.
- Loops: `while`.
- **Example**:
  ```c
  if (a < b) {
      a = a + 1;
  } else {
      a = 0;
  }
  ```

### 3. Expressions
- Arithmetic: `+`, `-`, `*`, `/`.
- Comparison: `<`, `>`, `<=`, `>=`, `==`, `!=`.
- Logical: `&&`, `||`, `!`.
- Assignment: `=`.

### 4. Hardware Interaction
- `SET_PIN(pin_number, state)`: Sets a GPIO pin to `HIGH` or `LOW`.
- `READ_PIN(pin_number)`: Reads the state of a GPIO pin.
- **Example**:
  ```c
  SET_PIN(3, HIGH);
  if (READ_PIN(5)) {
      SET_PIN(3, LOW);
  }
  ```

### 5. Functions
- **Syntax**: 
  ```c
  int func(int param1, bool param2) {
      // function body
  }
  ```
- Supports `return` for values.

### 6. Comments
- Single-line comments start with `#`.

---

## Example Program
```c
int main() {
    int a = 5;
    bool flag = false;

    SET_PIN(3, HIGH);
    if (a > 0) {
        while (a < 10) {
            a = a + 1;
        }
    } else {
        flag = true;
    }
    SET_PIN(3, LOW);
    return 0;
}
```

---

## Instructions Summary

| **Instruction**          | **Description**                                    |
|---------------------------|----------------------------------------------------|
| `int`, `bool`            | Variable declarations                              |
| `if`, `else`, `while`    | Control flow                                       |
| `SET_PIN`, `READ_PIN`    | GPIO operations                                    |
| `+`, `-`, `*`, `/`       | Arithmetic operations                              |
| `<`, `>`, `==`, `!=`     | Comparison operations                              |
| `&&`, `||`, `!`          | Logical operations                                 |
| `=`                      | Assignment                                         |
| `#`                      | Single-line comments                              |

This grammar is concise, hardware-focused, and optimized for embedded systems development.
