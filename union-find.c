#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define debug 1

typedef struct edge {
  struct edge *next;
  int vertex_number;
  int weight;
} 
edge;

edge *free_edges = NULL;

typedef struct vertex {
  struct edge *out_edges; 
  int mark;               
  int representative;     
}
vertex;

typedef struct graph {
  struct vertex *vertices;    
  int n_vertices;             
  int n_connected_components; 
}
graph;

edge *create_edge(void) {
  edge *e;
  int i;

  if(free_edges == NULL) {
    free_edges = (edge *)malloc((size_t)1000 * sizeof(edge));
    assert(free_edges != NULL);
    for(i = 0;i < 999;i++)
      free_edges[i].next = &free_edges[i + 1];
    free_edges[i].next = NULL;
  }
  e = free_edges;
  free_edges = free_edges->next;
  return e;
}

graph *create_graph(int n_vertices) {
  graph *g;
  int i;

  assert(n_vertices >= 1 && n_vertices <= 1000000);
  g = (graph *)malloc(sizeof(graph));
  assert(g != NULL);
  g->vertices = (vertex *)malloc((size_t)n_vertices * sizeof(vertex));
  assert(g->vertices != NULL);
  g->n_vertices = n_vertices;
  g->n_connected_components = n_vertices;
  for(i = 0;i < n_vertices;i++) {
    g->vertices[i].out_edges = NULL;
    g->vertices[i].representative = i;
  }
  return g;
}

int find_representative(graph *g,int vertex_number) {
  int i,j,k;

  // find
  for(i = vertex_number;i != g->vertices[i].representative;i = g->vertices[i].representative)
    ;
  // path compression
  for(j = vertex_number;j != i;j = k) {
    k = g->vertices[j].representative;
    g->vertices[j].representative = i;
  }
  return i;
}

int add_edge(graph *g,int from,int to,int weight) {
  int fi,ti;
  edge *e;

  assert(from >= 0 && from < g->n_vertices && to >= 0 && to < g->n_vertices && from != to);
  for(e = g->vertices[from].out_edges;e != NULL && e->vertex_number != to;e = e->next)
    ;
  if(e != NULL)
    return 0;
  e = create_edge();
  e->next = g->vertices[from].out_edges;
  g->vertices[from].out_edges = e;
  e->vertex_number = to;
  e->weight = weight;
  fi = find_representative(g,from);
  ti = find_representative(g,to);
  if(fi != ti) { // union
    g->vertices[ti].representative = fi;
    g->n_connected_components--;
  }
  return 1;
}

int calc(int c, int l) { 

	graph *g = create_graph(l*c);

	int j,k,p,found=0,white=0;

	while (found==0) {

		p = rand()%(l*c);
		while (g->vertices[p].mark == 1) {p = rand()%(l*c);}
		 g->vertices[p].mark = 1; white++;

		if (p-c>0 && g->vertices[p-c].mark == 1)
		  add_edge(g,p-c,p,1); 
		if (p+c<(l*c) && g->vertices[p+c].mark == 1)
		  add_edge(g,p+c,p,1); 
		if (p-1>0 && (p-1)%c==p%c-1 && g->vertices[p-1].mark == 1) 
		  add_edge(g,p-1,p,1); 
		if (p+1<(l*c) && (p+1)%c==p%c+1 && g->vertices[p+1].mark == 1)
		  add_edge(g,p+1,p,1); 


		#if debug==1´
		int i;
		for (i=0;i<l*c;i++) { 
		  if (i%c==0) {printf("\n%d ",g->vertices[i].mark);}
		  else {printf("%d ",g->vertices[i].mark);}
		}
		printf("\n");
		#endif

		for (j=0;j<c;j++) { 
		  for (k=l*c-1;k>l*c-1-c;k--) {
			if (find_representative(g,j) == find_representative(g,k)) {
			  found=1; break;
			}
		  }
		}

	} 


	#if debug==1
	printf("%d brancos.\n\n",white);
	#endif

	return white;

}


int main(int argc, char **argv) {

	time_t t;
	srand((unsigned) time(&t));

	int l=atoi(argv[1]); //amount of rows
	int c=atoi(argv[2]); //amount of columns
	int tr=atoi(argv[3]); //amount of tests
	int i,cur,max=0,min=c*l;
	double med=0;

	for (i=0;i<tr;i++) {
	  cur = calc(c,l);
	  if (cur>c*l-c+1 || cur<l) {i--;} 
	  else
	  {
		  med+=cur;
		  if (cur<min) min=cur;
		  if (cur>max) max=cur;
	  }
	  if (tr>=100000) printf("%.0f%s\r",100.0*i/tr*1.0,"%"); //progress
	}
	printf("\nCampo %d linhas x %d colunas.\n",l,c);
	printf("%d experiência(s).\n",tr);
	printf("Min: %d\n",min);
	printf("Med: %.2f\n",med/tr);
	printf("Max: %d\n",max);

	return 0;

}



