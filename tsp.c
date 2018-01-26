#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/resource.h>

int city_num[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

double city_x[16]; //x coordinates
double city_y[16]; //y coordinates
double city_d[16][16]; //distances

double mindist=999999; //highest possible distance
double maxdist=0; //lowest possible distance

int bestpath[16];
int worstpath[16]; 

#define max_n_cities 11 //amount of cities, maximum allowed is 16

void rand_city_coords(int seed) {
  int i,j;

  srand((unsigned int)seed);
  for(i = 0;i < max_n_cities;i++)
    for(;;) {
		city_x[i] = 0.01 * floor(1000.0 * (double)rand() / (double)RAND_MAX);       city_y[i] = 0.01 * floor(1000.0 * (double)rand() / (double)RAND_MAX);
		for(j = 0;j < i;j++)
			if(fabs(city_x[i] - city_x[j]) + fabs(city_y[i] - city_y[j]) < 2.0)
           		break;
		if(j == i)
      	break;
    }
}

void perm(int n,int m,int a[]) {
  int i;
  int j;

  if(m < n - 1) { 
    for(i = m;i < n;i++) {
#define swap(i,j)  do { int t = a[i]; a[i] = a[j]; a[j] = t; } while(0)
      swap(i,m);                         
      perm(n,m + 1,a);
      swap(i,m);                         
#undef swap
    }
  }
  else
  { 
	int previous = -1;
	int current = -1;
	int first = -1;
	double total = 0;
	for(i = 0;i < n;i++) { 
		if (first==-1) {first=a[i];}
		current=a[i];
		if (previous!=-1) {
			total=total+city_d[previous][current];
		}
		previous=current;
	}
	total=total+city_d[first][current];
	if (total<mindist) {  
		mindist=total;
		for (j=0;j<n;j++) {
			bestpath[j]=a[j];
		}
	}

	if (total>maxdist) { 
		maxdist=total; 		
		for (j=0;j<n;j++) {
			worstpath[j]=a[j];
			}
		}
	}
}

void print_tour(int n_cities,int *a,char *file_name) {
  FILE *fp;
  int i;

  if(n_cities < 2 || n_cities > max_n_cities)
  {
    fprintf(stderr,"bad number of cities\n");
    exit(1);
  }

  fp = fopen(file_name,"w");
  if(fp == NULL)
  {
    fprintf(stderr,"unable to create file %s\n",file_name);
    exit(1);
  }

  fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(fp,"%%%%Creator: TOS\n");
  fprintf(fp,"%%%%BoundingBox: 0 0 312 312\n");
  fprintf(fp,"72 25.4 div dup scale 1 setlinejoin 1 setlinecap\n");
  fprintf(fp,"/Helvetica-Bold findfont 4 scalefont setfont\n");
  fprintf(fp,"0.5 setlinewidth 0.7 setgray\n");
  fprintf(fp,"5 10 105 { 5 moveto 0 100 rlineto } for stroke\n");
  fprintf(fp,"5 10 105 { 5 exch moveto 100 0 rlineto } for stroke\n");
  fprintf(fp,"1 setlinewidth 1 0 0 setrgbcolor\n");

  for(i = 0;i < n_cities;i++)
	fprintf(fp,"%.2f %.2f %s\n",5.0 + 10.0 * city_x[bestpath[i]],5.0 + 10.0 * city_y[bestpath [i]],(i == 0) ? "moveto" : "lineto");
  fprintf(fp,"closepath stroke 0.5 setlinewidth\n");

  for(i = 0;i < n_cities;i++) { 
    fprintf(fp,"1 setgray %.2f %.2f 3 0 360 arc fill\n",5.0 + 10.0 * city_x[i],5.0 + 10.0 * city_y[i]);
    fprintf(fp,"0 setgray %.2f %.2f 3 0 360 arc closepath stroke\n",5.0 + 10.0 * city_x[i],5.0 + 10.0 * city_y[i]);
    fprintf(fp,"(%d) dup stringwidth pop 2 div neg %.2f add %.2f 1.4 sub moveto show\n",i,5.0 + 10.0 * city_x[i],5.0 + 10.0 * city_y[i]);
  }  

  fprintf(fp,"showpage\n");
  fprintf(fp,"%%%%EOF\n");
  fclose(fp);
}

int main(void) {

	int i;
	int j;
	rand_city_coords(12345);

	for (i=0;i<max_n_cities;i++) { 
		for (j=0;j< max_n_cities;j++) {
			int dx = city_x[i] - city_x[j];
			int dy = city_y[i] - city_y[j];
			city_d[i][j] = sqrt(dx*dx+dy*dy);
		}
	}

	//might take a long time for a large amount of cities
	perm(max_n_cities, 1, city_num); 

	printf("Best path found:\n");
	for (j=0;j< max_n_cities;j++) {printf("%d ",bestpath[j]);}
	printf("- %f distance\n",mindist);

	printf("Worst path found:\n");
	for (j=0;j< max_n_cities;j++) {printf("%d ",worstpath[j]);}
	printf("- %f distance\n",maxdist);

	//create pdf
	int a[max_n_cities];
	print_tour(max_n_cities,a,"resultado.ps");

	return 0;

}


