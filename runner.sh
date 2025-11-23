#!/bin/bash

GREEN="\033[1;32m"
RED="\033[1;31m"
NC="\033[0m" # No color

show_usage() {
    echo -e "${GREEN}Usage:${NC} ./runner.sh [mode|file_path]"
    echo
    echo "Modes:"
    echo "  auto       : Interactive mode."
    echo "               Navigate through all maps in maps/invalid/ and maps/valid/ using"
    echo "               → (next) and ← (previous). Press q to quit."
    echo
    echo "  file_path  : Run cub3D on a single map file, e.g.:"
    echo "               ./runner.sh maps/valid/1.cub"
    echo
    echo "What the script does:"
    echo "  1. Runs 'make -j' to compile cub3D."
    echo "     - Output is suppressed unless there is a compilation error."
    echo "     - If compilation succeeds, a green header 'cub3D compiled successfully!'"
    echo "       will be visible throughout auto mode."
    echo "  2. Runs cub3D on the specified map(s)."
    echo "     - In auto mode, shows current map index and path."
    echo "     - In single file mode, runs only the provided map."
    echo
}

# If no arguments provided, show usage
if [[ $# -eq 0 ]]; then
    show_usage
    exit 0
fi

# Compile cub3D
if ! make -j > /tmp/cub3d_build.log 2>&1; then
    echo -e "${RED}Compilation failed!${NC}"
    cat /tmp/cub3d_build.log
    exit 1
fi

# Success header
header="${GREEN}======================================
       cub3D compiled successfully!
======================================${NC}"

# Mode selection
if [[ $1 == "auto" ]]; then
    maps=(maps/invalid/*.cub maps/valid/*.cub)
    total=${#maps[@]}
    index=0

    while true; do
        clear
        echo -e "$header"
        echo -e "${GREEN}Map $((index+1))/$total: ${maps[index]}${NC}"
        echo "Press → to next, ← to previous, q to quit"

        ./build/cub3D "${maps[index]}"

        read -rsn1 key
        if [[ $key == $'\x1b' ]]; then
            read -rsn2 key
            if [[ $key == "[C" ]]; then
                ((index = (index + 1) % total))
            elif [[ $key == "[D" ]]; then
                ((index = (index - 1 + total) % total))
            fi
        elif [[ $key == "q" ]]; then
            break
        fi
    done
else
    if [[ -f "$1" ]]; then
        echo -e "$header"
        echo "Running ./build/cub3D on $1"
        ./build/cub3D "$1"
    else
        echo -e "${RED}File not found: $1${NC}"
        show_usage
        exit 1
    fi
fi
