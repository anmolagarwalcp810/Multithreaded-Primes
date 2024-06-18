#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	FILE* plotfile = popen("gnuplot -persistent","w");
	char* commands[] = {
		"set terminal png size 1000,400","set output './output/plot.png'","set title 'Time (s) (strategy1=baseline, strategy2=load-balanced)'",
		"set xlabel 'Thread Number'","set ylabel 'Time (s)'",
		"plot for[i=1:2] './output/file'.i.'.txt' pointsize 1.1 linecolor i title 'strategy'.i with linespoint"
	};
	int n = sizeof(commands)/sizeof(commands[0]);
	for(int i=0;i<n;i++){
		fprintf(plotfile, "%s\n", commands[i]);
	}
	if(plotfile!=NULL)fclose(plotfile);
}
