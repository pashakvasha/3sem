#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define COUNT_THREAD 1
#define COUNT 1000000

/* array ... транслит не используем */

/*
 К сожалению, я дезынформировал всю вашу группу по поводу работы clock'а:
 http://stackoverflow.com/questions/2962785/c-using-clock-to-measure-time-in-multi-threaded-programs
 
 Там написано, как это по-хорошему делается.
 Поэтому скорее всего ускорения и не наблюдалось.
 */

int mas[COUNT];
float segments_sum[COUNT_THREAD];
float segments_sum_square[COUNT_THREAD];

typedef struct Segment {
	int begin;
	int end;
	int index;
} Segment;

void* my_thread(void* arg) {
	Segment segment = *((Segment*)arg);
	int j;
	for (j = segment.begin; j < segment.end; j++)
	{
		segments_sum[segment.index] += mas[j];
		segments_sum_square[segment.index] += mas[j] * mas[j];
	}
    return NULL;
}

int main(int argc, char* argv[]) {
	int i;
    
    for(i = 0; i < COUNT; i++)
    {
		mas[i] = (1+rand()%5);
	}
	
	clock_t begin = clock();
	
	float sum = 0, sum_square = 0;
	float average, average_square;
	Segment segments[COUNT_THREAD];
	int segment_length = COUNT / COUNT_THREAD;
	
	for (i = 0; i < COUNT_THREAD; i++)
	{
		segments[i].index = i;
		segments[i].begin = segment_length * i;
		if (i != COUNT_THREAD - 1) {
			segments[i].end = segments[i].begin + segment_length;
		} else {
			segments[i].end = COUNT;
		}
	}
	
	pthread_t thread_id[COUNT_THREAD];
	
    int result;
    
	for(i = 0; i < COUNT_THREAD; i++) 
	{
		result = pthread_create(&(thread_id[i]), (pthread_attr_t *)NULL, my_thread, &(segments[i]) );
        if (result) {
			printf("Can`t create thread, returned value = %d\n" , result);
			exit(-1);
		}
	}
	
	for(i = 0; i < COUNT_THREAD; i++)
    {
		pthread_join(thread_id[i], (void **) NULL);
	}
	
	for (i = 0; i < COUNT_THREAD; i++)
	{
		sum += segments_sum[i];
		sum_square += segments_sum_square[i];
	}
	
	average = sum / COUNT;
	average_square = sum_square / COUNT;
	
	clock_t end = clock();
	double time_spent = (double)(end - begin);
	
	printf("TIME = %.1f\n", time_spent);
	printf("sum = %.2f\n", sum);
	printf("average = %.2f\n", average);
	printf("dispersion = %.2f\n", average_square - average*average);

    return 0;
}
