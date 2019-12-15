#!/usr/bin/env sh

dir=cmake-build-debug

for tst_dir in P434 P503 P610 P751; do    # P503 P610 P751
  for tst in ${dir}/${tst_dir}/*_test; do
    echo running ${tst}
    ./${tst} >/dev/null
    if test ! $?; then
      echo ${tst} failed.
    fi
  done
done
