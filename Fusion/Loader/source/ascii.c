#include "common.h"
#include "ascii.h"

uint32_t simple_rand() 
{
    int rand_seed = (uint32_t)sceKernelGetProcessTime();
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7fffffff;
    return rand_seed;
}

void ascii()
{
    const char* tagline;

    int r = simple_rand() % 57;

    switch (r)
    {
        case 0: tagline = "Now with 100% more atoms!"; break;
        case 1: tagline = "Powered by questionable physics!"; break;
        case 2: tagline = "May cause spontaneous combustion!"; break;
        case 3: tagline = "No particles were harmed!"; break;
        case 4: tagline = "Contains trace amounts of awesome!"; break;
        case 5: tagline = "Warning: Results may vary!"; break;
        case 6: tagline = "Definitely not a war crime!"; break;
        case 7: tagline = "Now in Technicolor!"; break;
        case 8: tagline = "Your scientists were so preoccupied..."; break;
        case 9: tagline = "Achievement unlocked: Nuclear fusion!"; break;
        case 10: tagline = "Still better than Fission 2!"; break;
        case 11: tagline = "Patent pending!"; break;
        case 12: tagline = "As seen on TV!"; break;
        case 13: tagline = "Quantum mechanics approved!"; break;
        case 14: tagline = "Error 404: Stability not found!"; break;
        case 15: tagline = "Trust me, I'm an engineer!"; break;
        case 16: tagline = "Hold my beer and watch this!"; break;
        case 17: tagline = "What could possibly go wrong?"; break;
        case 18: tagline = "Breaking the laws of thermodynamics!"; break;
        case 19: tagline = "Schrödinger's favorite!"; break;
        case 20: tagline = "Compiles on my machine!"; break;
        case 21: tagline = "TODO: Fix this later"; break;
        case 22: tagline = "It's not a bug, it's a feature!"; break;
        case 23: tagline = "Segmentation fault (core dumped)"; break;
        case 24: tagline = "Works 60% of the time, every time!"; break;
        case 25: tagline = "git commit -m 'YOLO'"; break;
        case 26: tagline = "Stack Overflow approved!"; break;
        case 27: tagline = "Deprecated since forever!"; break;
        case 28: tagline = "// No comments needed, code is self-documenting"; break;
        case 29: tagline = "while(true) { party(); }"; break;
        case 30: tagline = "Powered by copy-paste engineering!"; break;
        case 31: tagline = "99 bugs in the code, 99 bugs..."; break;
        case 32: tagline = "Undefined behavior is just extra features!"; break;
        case 33: tagline = "Memory leaks build character!"; break;
        case 34: tagline = "There are only 10 types of people..."; break;
        case 35: tagline = "Turning slurpees into code since 2014!"; break;
        case 36: tagline = "rm -rf / --no-preserve-root vibes"; break;
        case 37: tagline = "Held together with duct tape and prayers"; break;
        case 38: tagline = "Fusing atoms and merging branches!"; break;
        case 39: tagline = "Critical mass of bugs achieved!"; break;
        case 40: tagline = "Now with more nuclear decay!"; break;
        case 41: tagline = "Half-life measured in sprints!"; break;
        case 42: tagline = "Chain reaction of bad decisions!"; break;
        case 43: tagline = "Splitting atoms, splitting strings!"; break;
        case 44: tagline = "Reactor meltdown in 3... 2... 1..."; break;
        case 45: tagline = "Enriching uranium and depleting sanity!"; break;
        case 46: tagline = "Plasma contained (probably)"; break;
        case 47: tagline = "Fusion confusion!"; break;
        case 48: tagline = "More unstable than my production server!"; break;
        case 49: tagline = "Critical temperature: 100 million Kelvin"; break;
        case 50: tagline = "Magnetic confinement of technical debt!"; break;
        case 51: tagline = "Deuterium-tritium powered development!"; break;
        case 52: tagline = "Achieving ignition... eventually"; break;
        case 53: tagline = "ITER? I hardly know her!"; break;
        case 54: tagline = "Fission? That's so 1940s!"; break;
        case 55: tagline = "Sun envy intensifies"; break;
        default: tagline = "Fusion!"; break;
    }

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