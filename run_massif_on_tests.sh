#!/bin/bash

TEST_DIR="test"
TEST_FILES=("test1.c" "test2.c" "test3.cpp" "test4.cpp" "test5.cpp")
OUTPUT_DIR="massif_files"
OUTPUT_FILE_BASE="massif.out"

mkdir -p $OUTPUT_DIR 

for i in "${!TEST_FILES[@]}"; do
  TEST_FILE="${TEST_DIR}/${TEST_FILES[$i]}"
  OUTPUT_FILE="${OUTPUT_DIR}/${OUTPUT_FILE_BASE}.$((i+1))"
  EXE_FILE="${TEST_DIR}/$(basename "${TEST_FILE%.*}")"


  # compiler based on extension
  EXT="${TEST_FILE##*.}"
  if [ "$EXT" == "c" ]; then
    COMPILER="gcc"
  elif [ "$EXT" == "cpp" ]; then
    COMPILER="g++"
  else
    echo "Unknown extension: $TEST_FILE"
    continue
  fi


  # compiling if exe don't exist
  if [ ! -f "$EXE_FILE" ]; then
    echo "Compiling $TEST_FILE with $COMPILER..."
    $COMPILER -o "$EXE_FILE" "$TEST_FILE"
  fi

  echo "Running valgrind on $EXE_FILE..."
  valgrind --tool=massif --massif-out-file="$OUTPUT_FILE" ./$EXE_FILE  > /dev/null 2>&1
done
