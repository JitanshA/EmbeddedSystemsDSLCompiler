#!/bin/bash

# Directory containing test cases
TEST_DIR="./tests/cases"
EXPECTED_DIR="./tests/expected"
OUTPUT_DIR="./tests/output"

# Run all test cases
for TEST_FILE in $TEST_DIR/*.c; do
  TEST_NAME=$(basename "$TEST_FILE" .c)

  # Run compiler
  ./compiler "$TEST_FILE" > "$OUTPUT_DIR/$TEST_NAME.s"

  # Compare output
  diff "$OUTPUT_DIR/$TEST_NAME.s" "$EXPECTED_DIR/$TEST_NAME.s"
  if [ $? -eq 0 ]; then
    echo "$TEST_NAME: PASS"
  else
    echo "$TEST_NAME: FAIL"
  fi
done
