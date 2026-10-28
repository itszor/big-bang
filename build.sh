#!/bin/sh
# Build Big Bang from your own copy of it:
#     ./build.sh bigbang.zip   (a RISC OS zip of !BigBang; or a directory holding it)
# then: cd recomp && ./bigbang
# The steps are recipe.json's "build", run by SuperSpider's tools/weave.py.
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
[ $# -gt 0 ] || { echo "usage: $0 bigbang.zip | DIRECTORY" >&2; exit 2; }
(cd "$SUPERSPIDER" && dune build)
WEAVE_RECIPES="$HERE" exec python3 -I "$SUPERSPIDER/tools/weave.py" build "$@"
