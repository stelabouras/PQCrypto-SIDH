#!/usr/bin/env bash

dir=cmake-build-debug

for tst_dir in P434 P503 P610 P751; do
  for tst in ${dir}/${tst_dir}/*_test; do
    printf "running ${tst} "
    ./${tst} >/dev/null
    if test ! $?; then
      printf "... FAILED.\n"
    else
      printf "... PASSED.\n"
    fi
  done
done
