#!/bin/bash

# Compile the program
gcc -I include -o lexer tests/lexer/test_lexer.c src/lexer.c src/token.c src/errors.c
if [ $? -ne 0 ]; then
    echo "Compilation failed. Please fix the errors and try again."
    exit 1
fi

# Define paths
CASES_DIR="tests/lexer/cases_lexer"
EXPECTED_DIR="tests/lexer/expected_lexer"
ACTUAL_DIR="tests/lexer/actual_lexer"

# Ensure the actual_lexer directory exists
mkdir -p "$ACTUAL_DIR"

# Run tests
for i in {1..20}; do
    TEST_CASE="$CASES_DIR/test_lexer_$i.txt"
    EXPECTED_OUTPUT="$EXPECTED_DIR/expected_lexer_$i.txt"
    ACTUAL_OUTPUT="$ACTUAL_DIR/actual_lexer_$i.txt"

    echo "Running Test $i..."
    ./lexer < "$TEST_CASE" > "$ACTUAL_OUTPUT"

    if diff -q "$ACTUAL_OUTPUT" "$EXPECTED_OUTPUT" > /dev/null; then
        echo "Test $i PASSED!"
    else
        echo "Test $i FAILED!"
        echo "Diff:"
        diff "$ACTUAL_OUTPUT" "$EXPECTED_OUTPUT"
    fi
done
