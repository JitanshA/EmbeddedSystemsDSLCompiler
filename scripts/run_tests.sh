#!/bin/bash

TEST_DIR="./tests/cases"
EXPECTED_DIR="./tests/expected"
OUTPUT_DIR="./tests/output"

# Create output directory if not exists
mkdir -p $OUTPUT_DIR

# Run all test cases
for TEST_FILE in $TEST_DIR/*.txt; do
  TEST_NAME=$(basename "$TEST_FILE" .txt)
  EXPECTED_FILE="$EXPECTED_DIR/$TEST_NAME.txt"

  echo "Running $TEST_NAME..."

  # Run compiler
  ./compiler "$TEST_FILE" > "$OUTPUT_DIR/$TEST_NAME.txt" 2> "$OUTPUT_DIR/$TEST_NAME.err"

  # Check if expected output is an error file or a result file
  if [ -f "$EXPECTED_FILE" ]; then
    diff "$OUTPUT_DIR/$TEST_NAME.txt" "$EXPECTED_FILE" > /dev/null
    if [ $? -eq 0 ]; then
      echo "$TEST_NAME: PASS"
    else
      echo "$TEST_NAME: FAIL (output mismatch)"
    fi
  else
    echo "$TEST_NAME: FAIL (no expected output)"
  fi
done
