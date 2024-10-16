#!/bin/bash

# Check if the folder is provided as an argument
if [ -z "$1" ]; then
  echo "Usage: $0 <folder>"
  exit 1
fi

# Target folder
TARGET_FOLDER="$1"

# Find and delete all files that are not .md files
find "$TARGET_FOLDER" -type f ! -name "*.md" -exec rm -v {} \;

echo "All non-md files have been deleted from $TARGET_FOLDER."
