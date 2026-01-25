#include "common.h"
#include "process.h"

int GetPidByName(char* processName)
{
    size_t length;
    static int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0 };

    // Get the size of buffer needed.
    if (sysctl(name, 3, 0, &length, 0, 0) < 0)
        return -1;

    struct kinfo_proc* processList = (struct kinfo_proc*)malloc(length);
    if (processList == NULL)
        return -1;

    /* Get process list */
    if (sysctl(name, 3, processList, &length, 0, 0) < 0)
    {
        free(processList);
        return 0;
    }

    /* Search by name */
    int pid = -1;
    for (size_t i = 0; i < length / sizeof(struct kinfo_proc); i++)
    {
        if (strstr(processList[i].ki_comm, processName))
        {
            pid = processList[i].ki_pid;
            break;
        }
    }

    free(processList);
    return pid;
}