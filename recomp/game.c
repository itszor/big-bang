/* Big Bang (Psycore): what libroger needs to know about the game.
 *
 * !Run sets BigBang$Dir, RMLoads z15 (a module defining screen mode 112),
 * loads fun (a BBC font file: the game's character set), turns the pointer
 * off, *ScreenLoads sprytes.z1, takes a 640K WimpSlot, sets *Tuning and
 * *Volume, *Dirs to the application and runs buuni, an absolute at &8000.
 *
 * buuni is position-independent (it finds its workspace from its PC and
 * keeps it in r12), times its frames with OS_ReadMonotonicTime, flips two
 * screen banks in MODE 13 and plays its samples with a voice generator of
 * its own ("sonik"). M switches between MODE 13 and z15's mode 112: 640
 * pixels at mode 15's rate on 128 lines, so the game's 320-byte rows show
 * in pairs side by side, two quarter-size pictures (the "mini mode").
 *
 * The translated images: buuni (gen/), z15 (genmod/). */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <roger.h>

#define APP "../game/!BigBang"

extern const struct roger_image recomp_image, recompM_image;

/* a file in !BigBang, whatever ,load-exec suffix the extraction gave it
 * (its filetype and date stamp) */
static const char *in_app(const char *leaf)
{
    DIR *d = opendir(APP);
    struct dirent *e;
    char *path = NULL;
    while (d && !path && (e = readdir(d)))
        if (strcspn(e->d_name, ",") == strlen(leaf) && !strncasecmp(e->d_name, leaf, strlen(leaf))) {
            path = malloc(strlen(APP) + strlen(e->d_name) + 2);
            sprintf(path, "%s/%s", APP, e->d_name);
        }
    if (d) closedir(d);
    if (!path) { fprintf(stderr, "no %s/%s: build.sh extracts it\n", APP, leaf); exit(1); }
    return path;
}

static struct roger_module modules[] = {
    {NULL, &recompM_image, 0},                     /* rmload <BigBang$Dir>.z15 */
    {NULL, NULL, 0}};

static const char *const variables[] = {
    "BigBang$Dir", "!BigBang",                     /* Set BigBang$Dir <Obey$Dir> */
    NULL};

/* the rest of !Run, before buuni runs */
static void init(void)
{
    roger_cli("Load <BigBang$Dir>.fun");
    roger_cli("Pointer 0");
    roger_cli("ScreenLoad <BigBang$Dir>.sprytes.z1");
    roger_cli("Tuning -&3FFF");
    roger_cli("Tuning &3FFF");
    roger_cli("Tuning 0");
    roger_cli("Volume 127");
}

/* The headless scenario (--limit runs; instruction counts, 2000 a ms):
 * Return past the intro to the side select, X moves the bar from the info
 * icon to Dr. Subtech, Return picks him, Return the first world
 * (Wormwood), Return past its title card, then X walks right and ' jumps */
static const struct roger_key keys[] = {
    {74, 19000000, 19400000},                      /* Return: the side select */
    {67, 22000000, 22400000},                      /* X: the bar to Dr. Subtech */
    {74, 26000000, 26400000},                      /* Return: him */
    {74, 32000000, 32400000},                      /* Return: Wormwood */
    {74, 38000000, 38400000},                      /* Return: start */
    {67, 52000000, 58000000},                      /* X: walk right */
    {80, 52000000, 52800000},                      /* ': jump */
};

static struct roger_config config = {
    .title = "Big Bang (recompiled)",
    .image = &recomp_image,
    .modules = modules,
    .appdir = "../game",
    .variables = variables,
    .directory = "<BigBang$Dir>",                  /* dir <BigBang$Dir> */
    .command_line = "<BigBang$Dir>.buuni",
    .initial_mode = 12,                            /* the desktop's */
    .wimpslot = 640 * 1024,                        /* WimpSlot -min 640k -max 640k */
    .init = init,
    /* a controller: Z X walk, ' jumps (and is up in the bonus stage), /
     * down there, Return throws a teddy (and picks on the menus), P pauses,
     * M the mini mode */
    .pad_map = "left=Z,right=X,up=',down=/,a=',b=Return,x=Return,y=M,start=P,back=Escape",
    .keys = keys,
    .nkeys = sizeof keys / sizeof keys[0],
};

int main(int argc, char **argv)
{
    config.image_path = in_app("buuni");
    modules[0].path = in_app("z15");
    roger_main(argc, argv, &config);
}
