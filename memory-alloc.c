#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define PER_BYTE				(1)
#define PER_KB					(1024 * PER_BYTE)
#define PER_MB					(1024 * PER_KB)
#define PER_GB					(1024 * PER_MB)
#define KB(b)					((b) >> 10)			// b字节等于多少KB
#define MB(b)					((b) >> 20)			// b字节等于多少MB
#define MB_MASK					((1 << 20) - 1)		// 总字节数去掉整数个GB后，剩多少MB
#define GB(b)					((b) >> 30)			// b字节等于多少GB
#define MAX_GRANULARITY			(1024 * PER_MB)		// 最大粒度，单位MB
#define MIN_GRANULARITY			(64 * PER_MB)		// 最小粒度，单位MB

#define MAX_ALLOC				(72UL * PER_GB)		// 最大分配多少内存


static inline unsigned long generate_64bit_rand_num()
{
	unsigned long num;

	num = rand();
	num <<= 16;
	num |= rand();
	num <<= 16;
	num |= rand();
	num <<= 16;
	num |= rand();

	return num;
}

int main()
{
	unsigned long alloc_size = MAX_GRANULARITY;
	unsigned long *ptr = NULL;
	unsigned long cnt;
	unsigned long total_size = 0;
	unsigned long i;

	srand(time(NULL));
	while (1)
	{
		ptr = NULL;
		ptr = (unsigned long *)malloc(alloc_size);
		if (NULL != ptr)
		{
			total_size += alloc_size;
			printf("Allocated \033[42;1m%lu MB\033[0m space success(total size:\033[42;1m%lu MB\033[0m), clear and set random values.\n", MB(alloc_size), MB(total_size));
			/* 为了触发缺页中断，而对被分配的空间执行清零操作，
			 * 并且对被分配的空间中的每个字节都分配一个随机值
			 */
			memset(ptr, 0, alloc_size);
			cnt = alloc_size >> 3;
			for (i=0; i<cnt; i++)
			{
				ptr[i] = generate_64bit_rand_num();
				if (!((i + 1) % PER_MB))
				{
					printf("\r%0.02f%% random values setted.\033[K", ((float)(i+1)/cnt)*100);
					fflush(stdout);
				}
			}
			printf("\n");
		}
		else
		{
			// 上一次内存申请失败，则缩小下一次申请的内存大小
			printf("Allocate %lu MB space failed, change alloc_size to %lu MB.\n", MB(alloc_size), MB(alloc_size >> 1));
			alloc_size >>= 1;
			if (alloc_size < MIN_GRANULARITY)
			{
				// 不满足最小申请粒度，退出
				printf("alloc_size is too small, exit.\n");
				printf("Total alloc size:\033[42;1m%lu GB %lu MB\033[0m.\n", GB(total_size), MB(total_size & MB_MASK));

				break;
			}
		}

#if 0
		if (total_size >= MAX_ALLOC)
		{
			printf("Total alloc size \033[42;1m%lu GB\033[0m, exit.\n", GB(total_size));
			break;
		}
#endif
	}

	return 0;
}
