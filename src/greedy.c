#include "vrp.h"

/*output procedures*/

void plot_sol(instance* inst, int arr[], int size) {

	FILE* gnuplotPipe = popen("gnuplot -persist", "w");
	fprintf(gnuplotPipe, "set xlabel 'X'\n");
	fprintf(gnuplotPipe, "set ylabel 'Y'\n");

	fprintf(gnuplotPipe, "plot '-' using 2:3:1 with linespoints\n");
	for (int i = 0; i < size; i++) {
		fprintf(gnuplotPipe, "%d %lf %lf\n", i, inst->pts[arr[i]].x, inst->pts[arr[i]].y);
	}
	fprintf(gnuplotPipe, "e\n");

	// Close the Gnuplot pipe
	pclose(gnuplotPipe);
}


void greedy_solver(instance* inst){
    int* succ = malloc(sizeof(int)*inst->nnodes);
    int* uncov = malloc(sizeof(int)*inst->nnodes);
    int unc_len = inst->nnodes;
    for (int i = 0; i < inst->nnodes; i++)
    {
        succ[i] = -1;
        uncov[i] = i;
    }

    
    int curr_i = 0;
    while(unc_len!=0){
        double minCost_i = INFINITE_DBL;
        int unc_ind;
        for(int j=1; j<unc_len; j++){
            if(inst->costs[curr_i * inst->nnodes + uncov[j]] < minCost_i) {
                minCost_i = inst->costs[curr_i * inst->nnodes + uncov[j]];
                unc_ind = j;
            }
        } 
        succ[curr_i] = uncov[unc_ind];
        uncov[unc_ind] = uncov[unc_len-1];
        unc_len = unc_len -1;

        curr_i = succ[curr_i];

    }
    succ[curr_i] = uncov[0];
    plot_sol(inst, succ, inst->nnodes);

    free(succ);
    free(uncov);
}
