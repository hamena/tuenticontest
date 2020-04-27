#!/bin/bash

dir=$1

[ -z "$dir" ] && echo "ERROR: A target dir must be provided." && exit 1
[ -d "$dir" ] && echo "ERROR: Directory '$dir' already exists." && exit 1

mkdir -p $dir
cp main.cpp $dir/main.cpp
