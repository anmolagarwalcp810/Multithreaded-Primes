#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

struct thread_data
{
	int N;
	int* init_num;
	int chunk;
	int* arr;
	int id;
};

double* clock_arr;

pthread_mutex_t mutex_variable = PTHREAD_MUTEX_INITIALIZER;

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
void* load_balance_primes(void* thread_data){
	struct timespec start,end;
	clock_gettime(CLOCK_MONOTONIC,&start);
	while(1){	
		int left, right, flag=0;
		//
		pthread_mutex_lock(&mutex_variable);

		struct thread_data *local_thread_data = (struct thread_data*)thread_data;
		int N = local_thread_data->N;
		int* init_num = local_thread_data->init_num;
		int chunk = local_thread_data->chunk;
		int* arr = local_thread_data->arr;
		int id = local_thread_data->id;
		left = *init_num;
		// printf("%d\n",left );
		if(left>N)flag=1;
		right = *init_num+chunk;
		if(right>N){
			right=N;
		}
		*init_num=right+1;
		pthread_mutex_unlock(&mutex_variable);
		//

		if(flag){
			clock_gettime(CLOCK_MONOTONIC,&end);
			clock_arr[id]=end.tv_sec-start.tv_sec+((double)(end.tv_nsec-start.tv_nsec))*1e-9;
			break;
		}
		for(int i=left;i<=right;i++){
			arr[i]=is_prime(i);
		}
		// printf("%d\n", id);
		
	}
}

int main(int argc, char* argv[]){

	struct timespec start,end;
	clock_gettime(CLOCK_MONOTONIC,&start);

	int N=atoi(argv[1]),num_threads=atoi(argv[2]);
	// int N=1000, num_threads=2;
	int* output_primes = (int*) malloc(sizeof(int)*(N+1));
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
	int* next_num = (int*)malloc(sizeof(int)); 
	*next_num=1;
	int chunk = 1;

	clock_arr = (double*)malloc(sizeof(double)*num_threads);
	

	// initalize the time taken to 0
	for(int i=0;i<num_threads;i++){
		clock_arr[i]=0;
	} 

	for(int i=0;i<num_threads;i++){
		struct thread_data* thread_parameters = (struct thread_data*)malloc(sizeof(struct thread_data));
		thread_parameters->N=N;
		thread_parameters->init_num=next_num;
		thread_parameters->chunk=chunk;
		thread_parameters->arr=output_primes;
		thread_parameters->id=i;
		pthread_create(&threads[i],NULL,load_balance_primes,thread_parameters);
	}

	//wait and join
	for(int i=0;i<num_threads;i++){
		pthread_join(threads[i],NULL);
	}

	int count=0;

	clock_gettime(CLOCK_MONOTONIC,&end);
	printf("Total Time taken in load_balance approach : %f s\n",end.tv_sec-start.tv_sec+((double)(end.tv_nsec-start.tv_nsec))*1e-9);

	// print the output
	for(int i=2;i<=N;i++){
		if(output_primes[i]){
			printf("%d ", i);
			count++;
		}
	}

	// printf("\n");

	FILE* file = fopen("./output/file2.txt","w");
	for(int i=0;i<num_threads;i++){
		// printf("Thread %d %f\n", i,clock_arr[i]);
		fprintf(file, "%d %f\n",i,clock_arr[i] );
	}

	fclose(file);

	printf("\nTotal Primes Found : %d\n",count);

}
