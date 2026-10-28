Big Bang (Psycore) - static recompilation
==========================================

A recipe: this repository holds no game data. It rebuilds the game as a
native executable from your own copy, recognised by content (recipe.json):

    ./build.sh bigbang.zip         (or a directory holding !BigBang)
    cd recomp && ./bigbang

build.sh runs recipe.json's steps with SuperSpider's tools/weave.py
(~/stuff/superspider, or $SUPERSPIDER): it unpacks the zip to
extracted/app (its RISC OS filetypes as ,load-exec suffixes) and !BigBang
to game/ (libroger's appdir), then analyses, translates and links with
libroger (~/stuff/libroger, or make ROGER=...). Needs python3, OCaml/dune,
gcc and SDL3.

The input: a RISC OS zip of !BigBang (sha256 c5f0681e...), both discs'
files in one application (the game's "Disc 2 Please" never comes up: it
finds every world's files where it looks).

    analysis/run.sh     SuperSpider on buuni and z15:
                        analysis/buuni.json, analysis/z15.json
    recomp/             make regen (analysis + superspider gen), make, ./bigbang

Committed: recipe.json, build.sh, analysis/run.sh, analysis/buuni.roots
(empty: no gaps found), recomp/Makefile, recomp/game.c, this file.

Playing
-------

    ./bigbang [--scale=N] [--fullscreen] [--no-sound]

The Psycore picture, the spinning Big Bang planet, then demos. Return
brings up the side select: Z and X move the bar between Jellyhead, the
info screen and Dr. Subtech, Return picks; then the world (five, of three
levels each) the same way. In play: Z X walk, ' jumps, Return throws a
teddy, ' and / are up and down in the bonus stage, P pauses, M toggles the
mini mode, Escape quits. A game controller holds those keys (game.c's
pad_map: D-pad, A jump, B and X throw, Y mini mode, Start pause, Back
Escape).

    ./bigbang --limit=N     headless, deterministic: Dr. Subtech on
                            Wormwood, walking right (game.c's keys)
    ./bigbang --trace-gaps  interpret untranslated code instead of stopping
                            (none found: intro, demos, menus, worlds 1 and 5,
                            the mini mode)

What runs
---------

!Run sets BigBang$Dir, RMLoads z15, *Loads fun (a BBC font file of VDU 23
character definitions), turns the pointer off, *ScreenLoads sprytes.z1,
sets a 640K WimpSlot, *Tuning and *Volume, *Dirs to the application and
runs buuni. game.c does the same (its init hook runs the commands).

buuni (gen/, 29K, an absolute at &8000, not packed) is
position-independent: add r12,pc,#k finds its workspace, which stays in
r12 (--global-r12). It selects MODE 13, flips two screen banks (OS_Byte
112/113) and paces its frames with OS_ReadMonotonicTime. Its sounds are
samples played by a voice generator of its own ("sonik", Sound_InstallVoice
on all four channels). The files in machine/ are compressed level maps and
tables, not code, despite the name; sprites/, sprytes/, sound/ and
sounds/ hold the rest of the data.

z15 (genmod/, translated at &1800000) answers Service_ModeExtension for
mode 112: mode 15's 640 pixels at 16MHz on 128 of the 256 lines (VDSR 100
to VDER 228), 8bpp, 640-byte lines. M switches the game between MODE 13
and mode 112 without changing how it draws, so its 320-byte rows show in
pairs side by side: two quarter-size pictures in a letterbox, the mini
mode. The workspace list gives eig factors of 2 and 2, which would make
the picture twice as wide as the monitor shows it; libroger takes the
shape from the VIDC list instead (2.67:1).

Testing: the translation and the interpreter (ROGER_INTERPRET=1) give
byte-identical headless output (events, VDU stream, screen, registers)
over the 70M-instruction scenario.
