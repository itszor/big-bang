#!/bin/sh
# Code/data analysis of Big Bang with SuperSpider (~/stuff/superspider):
# buuni.json and z15.json, which recomp/'s "make regen" translates to C
# with superspider gen.
#   buuni  the game, an absolute at &8000 (position-independent: it finds
#          its workspace from its own PC, add r12,pc,#k, and keeps it in r12)
#   z15    a module: Service_ModeExtension for mode 112, translated at the
#          RMA address game.c loads it to (&1800000)
# The .roots file adds entry points to the analysis (recomp/: "make roots"
# merges the gap entries a --trace-gaps run logged into buuni.roots).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
SS="$SUPERSPIDER/_build/default/bin/main.exe"
G="$HERE/../game/!BigBang"
BUUNI="$(ls "$G"/buuni,* | head -1)"   # whatever their ,load-exec suffixes
Z15="$(ls "$G"/z15,* | head -1)"
(cd "$SUPERSPIDER" && dune build)
"$SS" analyse "$BUUNI" 8000 --full --global-r12 --roots "$HERE/buuni.roots" -o "$HERE/buuni.json"
"$SS" analyse "$Z15" 1800000 --full --module -o "$HERE/z15.json"
