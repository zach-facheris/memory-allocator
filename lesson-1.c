#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[4096];
    long long heap = 0; /* capacity in bytes */
    long long bump = 0; /* next free offset */

    while (fgets(line, sizeof line, stdin))
    {
        char cmd[32];
        long long arg = 0;

        if (sscanf(line, "%31s %lld", cmd, &arg) < 1)
            continue;

        if (strcmp(cmd, "INIT") == 0)
        {
            // TODO: remember the capacity, put the pointer back at 0, print OK
            bump = 0;
            heap = arg;
            printf("OK\n");
        }
        else if (strcmp(cmd, "ALLOC") == 0)
        {
            // TODO: print the pointer then advance it, or print OOM and change nothing
            long long bytes_requested = arg;

            if (bump + bytes_requested > heap)
                printf("OOM\n");
            else
            {
                printf("%lld\n", bump);
                bump += bytes_requested;
            }
        }
        else if (strcmp(cmd, "RESET") == 0)
        {
            // TODO: pointer back to 0, print OK
            bump = 0;
            printf("OK\n");
        }
        else if (strcmp(cmd, "USED") == 0)
        {
            // TODO: print the current pointer
            printf("%lld\n", bump);
        }
    }
    return 0;
}
