#!/bin/bash

# shellcheck disable=SC2164
cd build
ctest --extra-verbose --repeat-until-fail 10 --timeout 20 --build-and-test
# shellcheck disable=SC2103
cd ..

FILES_SEQ="build/bin/suffix_tree"
for file in $FILES_SEQ; do
        echo "--------------------------------"
        echo $(basename $file)
        echo "--------------------------------"
        valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./$file
done

