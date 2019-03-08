#!/bin/bash

set -e
set -x

find . -type f -name \*.h -o -name \*\.cc | \
    xargs clang-format \
        --style=file \
        --fallback-style=none \
        -i

