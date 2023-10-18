#include "vrp.h"

double second(void){
	return 0.03;
}

int main(int argc, char **argv){
    if (argc < 2) { printf("Usage: %s -help for help\n", argv[0]); exit(1); }
    if ( VERBOSE >= 2) { for (int a = 0; a < argc; a++) printf("%s ", argv[a]); printf("\n");}

    //double t1 = second(); 
	instance* inst = malloc(sizeof(instance));

    parse_command_line(argc,argv, inst);
	read_fileIn(inst);
	compute_costs(inst, (cost)sq_dist);
	if(inst->model_type == 1){
		return printf("you're fucked!\n");
	}
    freeInst(inst);
}


void freeInst(instance *inst)
{
	free(inst->pts);
	free(inst->costs);
    //free(inst->best_sol);
}
