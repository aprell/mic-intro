#!/bin/bash

cd "$(dirname "$0")/src"

case "$1" in
	"help" | "-help" | "--help" | "-h")
		echo
		echo "\$ $(basename "$0")"
		echo "  Build for hybrid execution (offload mode)"
		echo
		echo "\$ $(basename "$0") host"
		echo "  Build for execution on host processor"
		echo
		echo "\$ $(basename "$0") mic"
		echo "  Build for native execution on Xeon Phi"
		echo
		;;
	"host")
		make clean && make CFLAGS="-no-offload -Wno-unknown-pragmas"
		;;
	"mic")
		make clean && make CFLAGS="-mmic -Wno-unknown-pragmas"
		make check
		;;
	*)
		make clean && make
		make check
		;;
esac
