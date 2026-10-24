#include <stdio.h>
#include <string.h>

#define HDR 4 /* bytes of header in front of every block */
#define MAXB 4096

static long long heap = 0;
static long long b_addr[MAXB]; /* data address, in address order */
static long long b_size[MAXB];
static int b_free[MAXB];
static int nb = 0;

/* returns the data address, or -1 for OOM */
static long long do_alloc(long long size)
{
    int i;

    if (size <= 0)
        return -1;

    /* Reuse the lowest-address free block of exactly this size. */
    for (i = 0; i < nb; i++)
    {
        if (b_free[i] && b_size[i] == size)
        {
            b_free[i] = 0;
            return b_addr[i];
        }
    }

    /* Allocate a new block after the last block. */
    long long start = (nb == 0) ? 0 : b_addr[nb - 1] + b_size[nb - 1];

    /* Check heap capacity, including the header. */
    if (nb >= MAXB || start > heap || HDR > heap - start ||
        size > heap - start - HDR)
        return -1;

    b_addr[nb] = start + HDR;
    b_size[nb] = size;
    b_free[nb] = 0;
    nb++;

    return b_addr[nb - 1];
}

static int do_free(long long addr)
{
    int i;

    for (i = 0; i < nb; i++)
    {
        if (b_addr[i] == addr && !b_free[i])
        {
            b_free[i] = 1;
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    char line[4096];
    while (fgets(line, sizeof line, stdin))
    {
        char cmd[32];
        long long arg = 0;
        if (sscanf(line, "%31s %lld", cmd, &arg) < 1)
            continue;
        if (strcmp(cmd, "INIT") == 0)
        {
            heap = arg;
            nb = 0;
            printf("OK\n");
        }
        else if (strcmp(cmd, "ALLOC") == 0)
        {
            long long r = do_alloc(arg);
            if (r < 0)
                printf("OOM\n");
            else
                printf("%lld\n", r);
        }
        else if (strcmp(cmd, "FREE") == 0)
        {
            printf("%s\n", do_free(arg) ? "OK" : "BAD");
        }
        else if (strcmp(cmd, "BLOCKS") == 0)
        {
            int i;
            for (i = 0; i < nb; i++)
                printf("%lld:%lld:%s\n", b_addr[i], b_size[i], b_free[i] ? "free" : "used");
        }
    }
    return 0;
}
