#!/bin/sh

GCOV_DIR="${MESON_SOURCE_ROOT}/gcov"
mkdir "${GCOV_DIR}"

cd "${MESON_BUILD_ROOT}"
# -r consider only relative paths
# -s remove .. prefix (we are running gcov from build folder)
gcov-10 -p -s '..' -r src/*/*.g* src/*/*/*.g* test/*/*.g* \
                     test/*/..*.g* > /dev/null 2>&1
# Remove coverage files for ext folder
rm ext*.gcov
mv *.gcov "${GCOV_DIR}"
