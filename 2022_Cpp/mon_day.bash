#!/bin/bash

eval $(luarocks path)

luamon -l cpp -t -x make
