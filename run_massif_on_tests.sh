#!/bin/bash

TEST_DIR="test"
TEST_FILES=("test1.c" "test2.c")
OUTPUT_DIR="massif_files"
OUTPUT_FILE_BASE="massif.out"

mkdir -p $OUTPUT_DIR 

for i in "${!TEST_FILES[@]}"; do
  TEST_FILE="${TEST_DIR}/${TEST_FILES[$i]}"
  OUTPUT_FILE="${OUTPUT_DIR}/${OUTPUT_FILE_BASE}.$((i+1))"

  EXE_FILE="${TEST_DIR}/$(basename ${TEST_FILE%.c})"
  if [ ! -f "$EXE_FILE" ]; then
    echo "Compiling $TEST_FILE..."
    gcc -o "$EXE_FILE" "$TEST_FILE"
  fi

  echo "Running valgrind on $EXE_FILE..."
  valgrind --tool=massif --massif-out-file="$OUTPUT_FILE" ./$EXE_FILE
done
