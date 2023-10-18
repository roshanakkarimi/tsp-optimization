#ifndef VRP_H_

#define VRP_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <float.h>


#define VERBOSE               50   //printing limit (=10 only incumbent, =20 little output, =50-60 good, =70 verbose, >=100 cplex log)

//hard-coded variables
#define XSMALL		  		  1e-5 		// 1e-4*	// tolerance used to decide ingerality of 0-1 var.s
#define EPSILON		  		  1e-9		// 1e-9		// very small numerical tolerance 
#define TICKS_PER_SECOND 	  1000.0  	// cplex's ticks on Intel Core i7 quadcore @2.3GHZ
#define INFINITE_DBL DBL_MAX

//errors
#define FILE_OPEN_ERR 1
#define FILE_STRUCT_ERR 2
#define FORMAT_ERR 3
#define INDEX_ERR 4

//data structures

typedef struct {
	double x;
	double y;
} point;

typedef struct
{
    //input data
	int nnodes; 	
	double* costs;
	point* pts;
	
    // parameters 
	int model_type; 
	int randomseed;
	double timelimit;						// overall time limit, in sec.s
	char fileIn[100]; 						// input file

    //global data
	double tstart;								
	double zbest;							// best sol. available  
	double tbest;							// time for the best sol. available  
	double *best_sol;						// best sol. available    
    
    // model;     
	int xstart;
	int qstart;
	int ystart;
	int fstart;
	int zstart;
} instance;

//general functions
void parse_command_line(int argc, char** argv, instance* inst);
int read_fileIn(instance *inst);
void freeInst(instance *inst);

/*cost functions*/
typedef double (*cost)(int ind1, int ind2, const point* pts);
void compute_costs(instance*, cost);
double sq_dist(int i, int j, const point* pts);

//inline
inline int imax(int i1, int i2) { return ( i1 > i2 ) ? i1 : i2; } 
inline double dmin(double d1, double d2) { return ( d1 < d2 ) ? d1 : d2; } 
inline double dmax(double d1, double d2) { return ( d1 > d2 ) ? d1 : d2; } 

#endif   /* VRP_H_ */ 
