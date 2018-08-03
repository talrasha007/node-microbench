#!/bin/sh

emcc -Os -o functions.js functions.cc
emcc --bind -Os -o bind.js bind.cc