#include "vrp.h"
#include <stdbool.h>
#include <assert.h>


int myError(const char* err, int errType){
	printf("\nFATAL ERROR:\n%s", err);
	return errType;
} /*myError*/

int read_fileIn(instance* inst) {
	
	char line[180];
	char *par_name;   
	char *token1;
	char *token2;
	
	bool node_sect = false;
	
	FILE* fileIn = fopen(inst->fileIn, "r");
	if(fileIn == NULL) return myError("Couldn't open the input file!", FILE_OPEN_ERR);
	
	inst->nnodes = -1;

	while (fgets(line, sizeof(line), fileIn) != NULL) 
	{
		if (strlen(line) <= 1 ) continue; /*skip empty lines*/
	    par_name = strtok(line, " :");
		
		if (strncmp(par_name, "NAME", 4) == 0 || strncmp(par_name, "COMMENT", 7) == 0) continue;
		if (strncmp(par_name, "TYPE", 4) == 0) 
		{
			token1 = strtok(NULL, " :");  
			if(strncmp(token1, "TSP", 3) != 0) return myError("Format error: only TYPE == CSP allowed!", FORMAT_ERR); 
			continue;
		} /*if*/
		if (strncmp(par_name, "DIMENSION", 9) == 0) 
		{
			if(inst->nnodes >= 0) return myError("Repeated DIMENSION section in input file!", FILE_STRUCT_ERR);
			token1 = strtok(NULL, " :");
			inst->nnodes = atoi(token1); 
			inst->pts = malloc(inst->nnodes * sizeof(point));
			assert(inst->pts != NULL);
			inst->best_sol = malloc(inst->nnodes * sizeof(int));
			assert(inst->best_sol != NULL);
			inst->costs = malloc(inst->nnodes * inst->nnodes * sizeof(double));
			assert(inst->costs != NULL);
			continue;
		} /*if*/
		if (strncmp(par_name, "EDGE_WEIGHT_TYPE", 16) == 0) 
		{
			token1 = strtok(NULL, " :");
			if (strncmp(token1, "ATT", 3) != 0) return myError("Format error: only EDGE_WEIGHT_TYPE == ATT allowed!", FORMAT_ERR); 
			continue;
		} /*if*/            
		if (strncmp(par_name, "NODE_COORD_SECTION", 18) == 0) 
		{
			if (inst->nnodes <= 0) return myError("DIMENSION section should appear before NODE_COORD_SECTION section!", FILE_STRUCT_ERR);
			node_sect = true;   
			continue;
		} /*if*/
		if (strncmp(par_name, "EOF", 3) == 0) 
		{
			node_sect = false;
			break;
		} /*if*/

		if (node_sect)
		{
			int i = atoi(par_name) - 1; 
			if (i < 0 || i >= inst->nnodes) return myError("Unknown node in NODE_COORD_SECTION section!", INDEX_ERR);     
			token1 = strtok(NULL, " :,");
			token2 = strtok(NULL, " :,");
			inst->pts[i].x = atof(token1);
			inst->pts[i].y = atof(token2); 
			continue;
		} /*if*/
		
	} /*while*/
	
	fclose(fileIn);
	return 0;
} /*read_fileIn*/

void parse_command_line(int argc, char** argv, instance *inst){
    
    if ( VERBOSE >= 100 ) printf(" running %s with %d parameters \n", argv[0], argc-1);

    // default
	inst->model_type = 0;
	strcpy(inst->fileIn, "NULL");
	inst->randomseed = 0;

    int help = 0; if ( argc < 1 ) help = 1;

    for (int i=0; i<argc; i++){
        if (strcmp(argv[i], "-f") == 0){ strcpy(inst->fileIn, argv[++i]); continue;}
		if ( strcmp(argv[i],"-time_limit") == 0 ) { inst->timelimit = atof(argv[++i]); continue; }		// total time limit
		if ( strcmp(argv[i],"-model_type") == 0 ) { inst->model_type = atoi(argv[++i]); continue; } 	// model type
		if ( strcmp(argv[i],"-model") == 0 ) { inst->model_type = atoi(argv[++i]); continue; } 			// model type
		if ( strcmp(argv[i],"-seed") == 0 ) { inst->randomseed = abs(atoi(argv[++i])); continue; } 		// random seed
		if ( strcmp(argv[i],"-help") == 0 ) { help = 1; continue; } 									// help
		if ( strcmp(argv[i],"--help") == 0 ) { help = 1; continue; } 
    }

    if ( help || (VERBOSE >= 10) )		// print current parameters
	{
		printf("\n\navailable parameters --------------------------------------------------\n");
		printf("-file %s\n", inst->fileIn);
		printf("-time_limit %lf\n", inst->timelimit);
		printf("-model_type %d\n", inst->model_type);
		printf("-seed %d\n", inst->randomseed);
		printf("\nenter -help or --help for help\n");
		printf("----------------------------------------------------------------------------------------------\n\n");
	}

	if ( help ) exit(1);
}

void compute_costs(instance* inst, cost fc){
	int i, j;
	for(i = 0; i < inst->nnodes; i++){
		for(j = 0; j < i; j++)
			inst->costs[i * inst->nnodes + j] = fc(i, j, inst->pts);
		inst->costs[i * inst->nnodes + i] = INFINITE_DBL;
	} /*for*/
	
	for(i = 0; i < inst->nnodes; i++){
		for(j = i + 1; j < inst->nnodes; j++)
			inst->costs[i * inst->nnodes + j] = inst->costs[j * inst->nnodes + i];
	} /*for*/

	//out file
	FILE* cost_f = fopen("costs.txt", "w");

	for (int i = 0; i < inst->nnodes; i++)
	{
		for (int j = 0; j < inst->nnodes; j++)
		{
			fprintf(cost_f, "[%d][%d]: %f\n",i, j, inst->costs[i * inst->nnodes + j]);
		}fprintf(cost_f, "\n");
	}
} /*compute_costs*/

double sq_dist(int i, int j, const point* pts) {
	return (pts[i].x - pts[j].x) * (pts[i].x - pts[j].x) + (pts[i].y - pts[j].y) * (pts[i].y - pts[j].y); //to be sure about the 
} /*dist*/
