#!/bin/bash

# shellcheck disable=SC2164
cd build
ctest --extra-verbose --repeat-until-fail 10 --timeout 20 --build-and-test
# shellcheck disable=SC2103
cd ..


