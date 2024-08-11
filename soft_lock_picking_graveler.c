#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

unsigned long max_roll_sessions = 1;
unsigned long roll_sessions = 0;
unsigned int highest_roll = 0;
unsigned char numbers_index_to_use = 0;

void algo0() // Directly derived from Austin's code // Also called by -v
{
	unsigned int numbers[4] = {0};

	while(numbers[numbers_index_to_use] < 177 && roll_sessions < max_roll_sessions)
	{
		for(unsigned char i = 0; i < 4; i++)
		{
			numbers[i] = 0;
		}
		for(unsigned char i = 0; i < 231; i++)
		{
			numbers[(rand() / (RAND_MAX / 4 + 1))]++;
		}
		if(numbers[numbers_index_to_use] > highest_roll)
		{
			highest_roll = numbers[numbers_index_to_use];
		}
		roll_sessions++;
	}
}

/*
algo1's ones should be different to algo0 due to the /3 used to only get 1 in 4
The algo1 ones should be equal to the algo0's fours, check this using -v
*/
void algo1()
{
	while(highest_roll < 177 && roll_sessions < max_roll_sessions)
	{
		unsigned char ones = 0;
		for(unsigned char i = 0; i < 231; i++)
		{
			ones = ones + (rand() / (RAND_MAX / 4 + 1) / 3);
		}
		if(ones > highest_roll) highest_roll = ones;
		roll_sessions++;
	}
}

/*
algo2 should give same values as algo1
*/
void algo2()
{
	char* ones_array = malloc(sizeof(char) * max_roll_sessions);
	while(highest_roll < 177 && roll_sessions < max_roll_sessions)
	{
		unsigned char ones = 0;
		for(unsigned char i = 0; i < 231; i++)
		{
			ones = ones + (rand() / (RAND_MAX / 4 + 1) / 3);
		}
		ones_array[roll_sessions] = ones;
		roll_sessions++;
	}
	for(unsigned int i = 0; i < roll_sessions; i++)
	{
		if(ones_array[i] > highest_roll) highest_roll = ones_array[i];
	}
	free(ones_array);
}

int main(int argc, char** argv)
{
	unsigned char option_algo = 0;
	unsigned long long seed = time(0);

	int opt;
	while((opt = getopt(argc, argv, "m:s:012vh")) != -1)
	{
		switch(opt)
		{
			case 'm': max_roll_sessions = strtoul(optarg, 0, 10); break;
			case 's': seed = strtoull(optarg, 0, 10); break;
			case '0': option_algo |= 1 << 0; break;
			case '1': option_algo |= 1 << 1; break;
			case '2': option_algo |= 1 << 2; break;
			case 'v': option_algo |= 1 << 3; break;
			case 'h':
			default:
				fprintf(stderr, "\nUsage: %s [OPTION]...\nExample: %s -1 -m100 -s1\n\nOptions: \n\t-m MAX\t\tmax roll sessions; default=1 min=0 max=%lu\n\t-s SEED\t\tseed to use; defaults to time(0); min=0 max=%llu\n\t-0,1,2\t\tuse algorithm 0, 1, or 2; default=2\n\t-v\t\tuse algo0 but using numbers[3] instead of numbers[0] as ones; algo1 and algo2's ones should match the ones from here; this exists purely to verify that\n", argv[0], argv[0], ULONG_MAX, ULLONG_MAX);
				return 1;
		}
	}

	srand(seed);

	switch(option_algo)
	{
		case (0 | 1 << 0): algo0(); break;
		case (0 | 1 << 1): algo1(); break;
		case 0:
		case (0 | 1 << 2): algo2(); break;
		case (0 | 1 << 3): numbers_index_to_use = 3; algo0(); break;
		default:
			fprintf(stderr, "\nError: Only one algorithm can bet set\n");
			return 1;
	}

	printf("\nSeed: %llu\nHighest roll: %u\nSessions rolled: %lu\n", seed, highest_roll, roll_sessions);
	return 0;
}
