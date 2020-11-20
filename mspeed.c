#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

void print_size(uint64_t size, char * out)
{
    if (size < 1024)
        sprintf(out, "%lu Bytes", size);
    else if (size < 1024*1024)
        sprintf(out, "%.2lf KiB", ((double)size)/1024);
    else if (size < 1024*1024*1024)
        sprintf(out, "%.2lf MiB", ((double)size)/(1024*1024));

}

static inline void mem_test(uint64_t msize)
{
    int64_t iterations = ((1024*1024*1024) / msize) + 10;
    // iterations -= msize;
    if (iterations < 10) iterations = 10;

    uint8_t * mem1 = malloc(msize);
    uint8_t * mem2 = malloc(msize);

    memset(mem1, 69, msize);
    memset(mem2, 69, msize);
    for (volatile int i = 0; i < 1; ++i) {
        memcpy(mem1, mem2, msize);
    }

    // sleep(1);

    clock_t start, end;

    volatile uint64_t result1 = 69;
    start = clock();
    for (volatile uint64_t i = 0; i < iterations; ++i)
    {
        memcpy(mem1, mem2, msize);
        result1 = (result1-i) * (i+result1);
    }
    end = clock();
    double time_copy = ((double) (end - start)) / CLOCKS_PER_SEC;


    volatile uint64_t result2 = 69;
    start = clock();
    for (volatile uint64_t i = 0; i < iterations; ++i)
    {
        // memcpy(mem1, mem2, msize/2);
        result2 = (result2-i) * (i+result2);
    }
    end = clock();
    double time_other = ((double) (end - start)) / CLOCKS_PER_SEC;

    if (result1 != result2) puts("problem");
    else {
        char size_str[32];
        double actual_copying_time = time_copy;
        double mib_copied = ((double)iterations * msize) / (double)(1024*1024);
        double speed = mib_copied / actual_copying_time;
        print_size(msize, size_str);
        printf("%13s : %.1lf GiB/s\n", size_str, speed/1024);

        // for (int i = 0; i < speed/1024; ++i) printf("*");
        // puts("");
    }

    free(mem1);
    free(mem2);
}


int main()
{
    for (float i = 3; i < 29; i +=1)
        mem_test(pow(2, i));
}