#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
typedef struct{
	int ** matrix;
	int n,m;
}MAT;
typedef struct{
	int x;
	int y;
}ARG;
MAT mat1,mat2,outMat,outMat2;
void readFile(){
  FILE *f1=fopen("input.txt","r");
  if(f1 == NULL)
     	  perror("Error!");
	int count=2;
	MAT m;
	while(count){
  	fscanf(f1,"%d %d",&m.n,&m.m);
  	fscanf(f1,"\n");
  	m.matrix=malloc(m.n*sizeof(int*));
  	for(int i=0;i<m.n;i++){
			m.matrix[i]=malloc(m.m*sizeof(int*));
			int buffer;
			for(int j=0;j<m.m;j++){
				fscanf(f1,"%d",&m.matrix[i][j]);
}
			fscanf(f1,"\n");}
		if (count==2){
			mat1.n=m.n;
			mat1.m=m.m;
			mat1.matrix=m.matrix;
		}
		else{
			mat2.n=m.n;
			mat2.m=m.m;
			mat2.matrix=m.matrix;
		}
			count--;
	}
}
void writeFile(double tp1,double tp2){
	FILE* f2=fopen("output.txt","w");
	for(int i=0;i<outMat.n;i++){
			for(int j=0;j<outMat.m;j++)
				fprintf(f2,"%d	",outMat.matrix[i][j]);
			fprintf(f2,"\n");}
	fprintf(f2,"END 1 	Elapsed time of procedure 1: %lf\n",tp1);
	for(int i=0;i<outMat2.n;i++){
			for(int j=0;j<outMat2.m;j++)
				fprintf(f2,"%d	",outMat2.matrix[i][j]);
			fprintf(f2,"\n");}
	fprintf(f2,"END 2 	Elapsed time of procedure 2: %lf",tp2);
	fclose(f2);
}
void * mulP1(void * arg){
	int sum=0;
	ARG * a= (ARG*) arg;
	int i=0;
	while(i<mat1.m){
			//printf("%d %d\n",a->x,a->y);
			sum+=mat1.matrix[a->x][i]*mat2.matrix[i][a->y];
			i++;}
	outMat.matrix[a->x][a->y]=sum;
	pthread_exit(NULL);
}
void * mulP2(void * args){
	ARG * a= (ARG*) args;
	int sum=0;
	for (int i=0;i<mat2.m;i++){
		sum=0;
		for(int j=0;j<mat1.m;j++)
			sum+=mat1.matrix[a->y][j]*mat2.matrix[j][i];
		outMat2.matrix[a->x][i]=sum;
	}
}
void main(){
	readFile();
	if(mat1.m!=mat2.n)
		perror("Cannot multiply this matrices");
	else{
		int end=mat1.n * mat2.m;
		ARG args[end],args2[mat1.n];
		pthread_t th0[end],th1[mat1.n];
		outMat.n=mat1.n; outMat.m=mat2.m;
		outMat2.n=mat1.n; outMat2.m=mat2.m;
		outMat.matrix=malloc(mat1.n*sizeof(int*));
		outMat2.matrix=malloc(mat1.n*sizeof(int*));
		for(int i=0;i<mat1.n;i++){
				outMat.matrix[i]=malloc(mat2.m*sizeof(int*));
				outMat2.matrix[i]=malloc(mat2.m*sizeof(int*));
		}
		clock_t begin1=clock();
		for(int i=0;i<end;i++){
			//printf("%d\n",i);
			args[i].x=i/mat2.m;
			args[i].y=i%mat2.m;
 			pthread_create(&th0[i],NULL,mulP1,&args[i]);
		}
		for(int j=0;j<end;j++)
			pthread_join(th0[j], NULL);
		clock_t end1=clock();
		clock_t begin2=clock();
		for(int i=0;i<mat1.n;i++){
			//printf("%d\n",i);
			args2[i].x=i;
			args2[i].y=i;
 			pthread_create(&th1[i],NULL,mulP2,&args2[i]);
		}
		for(int j=0;j<mat1.n;j++)
			pthread_join(th1[j], NULL);
		clock_t end2=clock();
		writeFile((double)(end1-begin1)/CLOCKS_PER_SEC,(double)(end2-begin2)/CLOCKS_PER_SEC);
		printf("DONE!\n");
	}
}
