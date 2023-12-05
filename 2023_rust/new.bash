#!/bin/bash -e

[[ "$#" -ne 1 ]] && exit 1
[[ "$1" != [0-9][0-9] ]] && exit 1

cp -r ./template/ "./day${1}"
find "./day${1}" -type f -exec sed -i "s/<TODO>/${1}/g" {} \;

tac Cargo.toml | sed "2i"$'\t'"\"day${1}\"," | tac > Cargo.toml.1
mv Cargo.toml.1 Cargo.toml

sed -i "1s/^DAYS = /DAYS = ${1} /" Makefile

echo "modify ./main/src/main.rs on your own"
