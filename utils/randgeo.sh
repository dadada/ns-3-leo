#!/bin/sh

shuf $1 | head -n $2 | ./geo2vec.py
