#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

struct thread_data
{
	int local_N;
	int local_t;
	int local_id;
	int* arr;
	double* clock_arr;
};

// basic sieve of eratosthenes
void sequential_seive(int n, int* arr){
	for(int i=0;i<=n;i++)arr[i]=1;
	int i=2;
	int limit = ceil(sqrt((double)n));
	for(;i<limit;i++){
		if(arr[i]){
			int j=i*i;
			for(;j<=n;j+=i){
				arr[j]=0;
			}
		}
	}
}

int is_prime(int n){
	//use sieve of eratosthenes for i<=sqrt(n)
	if(n==2)return 1;
	int limit = ceil(sqrt((double)n));
	int* arr = (int*) malloc(sizeof(int)*(limit+1));
	//gets all primes upto sqrt(n)
	sequential_seive(limit,arr);
	int flag=0;
	for(int i=2;i<=limit;i++){
		if(arr[i]){
			if(n%i==0){
				flag=1;
			}
		}
	}
	free(arr);
	if(flag)return 0;
	else return 1;
}

// now use naive approach
void* naive_primes(void* thread_data){
	struct timespec start,end;
	clock_gettime(CLOCK_MONOTONIC,&start);
	struct thread_data *local_thread_data = (struct thread_data*)thread_data;
	int N = local_thread_data->local_N;
	int t = local_thread_data->local_t;
	int id = local_thread_data->local_id;
	int* arr = local_thread_data->arr;
	double* clock_arr = local_thread_data->clock_arr;
	// assign range to the thread
	int left = id*(N/t)+1;
	int right = left+N/t;
	for(int i=left;i<=right;i++){
		arr[i]=is_prime(i);
	}
	clock_gettime(CLOCK_MONOTONIC,&end);
	clock_arr[id]=end.tv_sec-start.tv_sec+((double)(end.tv_nsec-start.tv_nsec))*1e-9;
}

int main(int argc, char* argv[]){
	
	struct timespec start,end;
	clock_gettime(CLOCK_MONOTONIC,&start);

	int N=atoi(argv[1]),num_threads=atoi(argv[2]);
	int* output_primes = (int*) malloc(sizeof(int)*(N+1));
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
	double* time_for_each_thread = (double*)malloc(sizeof(double)*num_threads);
	
	// initalize the time taken to 0
	for(int i=0;i<num_threads;i++){
		time_for_each_thread[i]=0;
	} 

	for(int i=0;i<num_threads;i++){
		// create struct to pass multiple arguments to function
		struct thread_data * thread_parameters = (struct thread_data*)malloc(sizeof(struct thread_data));

		thread_parameters->local_N=N;
		thread_parameters->local_t=num_threads;
		thread_parameters->local_id=i;
		thread_parameters->arr=output_primes;
		thread_parameters->clock_arr=time_for_each_thread;

		pthread_create(&threads[i],NULL,&naive_primes,thread_parameters);
	}

	//wait and join
	for(int i=0;i<num_threads;i++){
		pthread_join(threads[i],NULL);
	}

	// int count=0;
	
	//print the output
	// for(int i=2;i<=N;i++){
	// 	if(output_primes[i]){
	// 		printf("%d ", i);
	// 		count++;
	// 	}
	// }

	clock_gettime(CLOCK_MONOTONIC,&end);
	printf("Total Time taken in naive approach : %f s\n",end.tv_sec-start.tv_sec+((double)(end.tv_nsec-start.tv_nsec))*1e-9);


	FILE* file = fopen("./output/file1.txt","w");
	for(int i=0;i<num_threads;i++){
		// printf("Thread %d %f\n",i, time_for_each_thread[i]);
		fprintf(file, "%d %f\n",i,time_for_each_thread[i] );
	}
	fclose(file);

	// printf("%d\n", count);
}
