#include "common.h"
#include "ascii.h"

const char* taglines[] = {
    "Now with 100% more atoms!",
    "Powered by questionable physics!",
    "May cause spontaneous combustion!",
    "No particles were harmed!",
    "Contains trace amounts of awesome!",
    "Warning: Results may vary!",
    "Definitely not a war crime!",
    "Now in Technicolor!",
    "Your scientists were so preoccupied...",
    "Achievement unlocked: Nuclear fusion!",
    "Still better than Fission 2!",
    "Patent pending!",
    "As seen on TV!",
    "Quantum mechanics approved!",
    "Error 404: Stability not found!",
    "Trust me, I'm an engineer!",
    "Hold my beer and watch this!",
    "What could possibly go wrong?",
    "Breaking the laws of thermodynamics!",
    "Schrödinger's favorite!",
    "Compiles on my machine!",
    "TODO: Fix this later",
    "It's not a bug, it's a feature!",
    "Segmentation fault (core dumped)",
    "Works 60% of the time, every time!",
    "git commit -m 'YOLO'",
    "Stack Overflow approved!",
    "Deprecated since forever!",
    "// No comments needed, code is self-documenting",
    "while(true) { party(); }",
    "Powered by copy-paste engineering!",
    "99 bugs in the code, 99 bugs...",
    "Undefined behavior is just extra features!",
    "Memory leaks build character!",
    "There are only 10 types of people...",
    "Turning slurpees into code since 2014!",
    "rm -rf / --no-preserve-root vibes",
    "Held together with duct tape and prayers",
    "Fusing atoms and merging branches!",
    "Critical mass of bugs achieved!",
    "Now with more nuclear decay!",
    "Half-life measured in sprints!",
    "Chain reaction of bad decisions!",
    "Splitting atoms, splitting strings!",
    "Reactor meltdown in 3... 2... 1...",
    "Enriching uranium and depleting sanity!",
    "Plasma contained (probably)",
    "Fusion confusion!",
    "More unstable than my production server!",
    "Critical temperature: 100 million Kelvin",
    "Magnetic confinement of technical debt!",
    "Deuterium-tritium powered development!",
    "Achieving ignition... eventually",
    "ITER? I hardly know her!",
    "Fission? That's so 1940s!",
    "Sun envy intensifies"
};

static uint32_t rand_seed = 0;

uint32_t simple_rand() {
    if (rand_seed == 0)
    {
        // Initialize with timestamp or other source
        rand_seed = (uint32_t)sceKernelGetProcessTime();
    }
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7fffffff;
    return rand_seed;
}


void ascii()
{
    // Get random tagline
    uint64_t seed = sceKernelGetProcessTime(); // or your random method
    int index = (seed / 1000000) % (sizeof(taglines) / sizeof(taglines[0]));
    const char* tagline = taglines[index];
    int tagline_len = strlen(tagline);

    // Calculate required width (tagline + padding on both sides)
    int min_content_width = 47; // Width needed for "FUSION" ASCII art
    int content_width = (tagline_len > min_content_width) ? tagline_len : min_content_width;
    content_width += 8; // Add some padding

    int tagline_padding = (content_width - tagline_len) / 2;
    int fusion_padding = (content_width - min_content_width) / 2;

    // Build top border
    char top_border[256];
    char bottom_border[256];
    char empty_line[256];

    top_border[0] = '\0';
    strcat(top_border, "┏");
    for (int i = 0; i < content_width + 2; i++) strcat(top_border, "━");
    strcat(top_border, "┓\n");

    bottom_border[0] = '\0';
    strcat(bottom_border, "┗");
    for (int i = 0; i < content_width + 2; i++) strcat(bottom_border, "━");
    strcat(bottom_border, "┛\n");

    sprintf(empty_line, "┃%*s┃\n", content_width + 2, "");

    // Print the banner
    klog(top_border);
    klog(empty_line);
    klog("┃%*s███████╗██╗   ██╗███████╗██╗ ██████╗ ███╗   ██╗%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog("┃%*s██╔════╝██║   ██║██╔════╝██║██╔═══██╗████╗  ██║%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog("┃%*s█████╗  ██║   ██║███████╗██║██║   ██║██╔██╗ ██║%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog("┃%*s██╔══╝  ██║   ██║╚════██║██║██║   ██║██║╚██╗██║%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog("┃%*s██║     ╚██████╔╝███████║██║╚██████╔╝██║ ╚████║%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog("┃%*s╚═╝      ╚═════╝ ╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝%*s┃\n", fusion_padding + 1, "", fusion_padding + 1, "");
    klog(empty_line);
    klog("┃%*s%s%*s┃\n", tagline_padding + 1, "", tagline, content_width - tagline_padding - tagline_len + 1, "");
    klog(empty_line);
    klog(bottom_border);
}