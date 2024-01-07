#!/bin/bash

git remote add teacher-upstream https://github.com/bopan-uw/css432-tftp.git
git fetch teacher-upstream main:teacher-main

CHANGES=$(git diff teacher-main -- .github/ main -- .github/)

if [ -z "$CHANGES" ]
then
  echo "No changes to .github"
else
  echo "Detected changes to .github:"
  echo "$CHANGES"
  exit 1
fi
