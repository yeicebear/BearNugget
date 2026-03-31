#!/bin/zsh

printf '%s\n' "$@" \
| sed 's|^.*/src/||; s|/|_|g; s|\.|_|g; s|^|.obj/src_|; s|$|.o|' \
| paste -sd' ' - \
| tr -d '\n'
