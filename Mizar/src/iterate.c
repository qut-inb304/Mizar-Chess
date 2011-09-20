/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*Inizializza la ricerca e gestisce l'iterative deeping*/
unsigned int find_best_move(const int output_mode,const int search_mode) {
	int depth, score, i,c,d,start_depth;
	int alpha, beta;
	MOVE m = {0};

	/*inizializzazioni varie*/
	tree.start_time = time_elapsed();
	tree.stop_time = tree.start_time + tree.delta_time;
	Ply = 0;
	Nodes = 0;
	tree.abort_search = 0;
	start_depth=tree.ponder_start_depth-1;
	alpha = -INF;
	beta = +INF;

	do {
		if (search_mode==CONTINUE_SEARCH) break;
		start_depth=1;
		tree.ponder_start_depth=1;
	for (i=0;i<MAX_PLY;i++) {
		tree.old_pv[i].mi=0;
		tree.k_heuristic[i].m1.mi=0;
		tree.k_heuristic[i].m2.mi=0;
		tree.k_heuristic[i].score1=0;
		tree.k_heuristic[i].score2=0;
		for (c=0;c<MAX_PLY;c++) tree.pv[i][c].mi=0;
	}
	for (i=0;i<256;i++) {
		for (c=0;c<256;c++) {
			tree.h_heuristic[i][c]=0;
		}	
	}
	for (i=0;i<64;i++) {
		for (c=0;c<64;c++) {
			for (d=0;d<256;d++)tree.c_heuristic[i][c][d].mi=0;
		}	
	}
	if (mizar.o.use_hash) reset_tt();
	}while (0);

	
	

	/*Precalcoliamo*/
	Phase(WHITE) = eval_phase(WHITE);
	Phase(BLACK) = eval_phase(BLACK);

	/*Prepariamo il nodo radice*/
	root();
	Nodes=0;

	if (output_mode == IO_CONSOLE)
		printf("ply  score      time      nodes pv\n");

	/*iterative deeping*/ 
	for (depth = start_depth;depth <= tree.max_depth;depth++) {
			
			tree.ponder_start_depth=depth;

			tree.follow_pv = 1;

			score = search_root( alpha, beta, depth);

			if (tree.abort_search)
				break;

			if (score<=alpha || score >=beta) {
				alpha = -INF;
				beta = +INF;
				tree.follow_pv = 1;
				score = search_root( alpha, beta, depth);
				if (tree.abort_search)
				break;
			} 
			else {
				alpha = score - ASPIRATION_W;
				beta = score + ASPIRATION_W;				
				} 

			m.mi = tree.pv[0][0].mi;

			for (i=0;i<MAX_PLY;i++)
				tree.old_pv[i].mi=0;

			sort_root_nodes();

			for (i = 0;i < tree.pv_lenght[0];i++)
				tree.old_pv[i] = tree.pv[0][i];


			if (output_mode == IO_CONSOLE) {
					printf("%3d  %5d  %8d  %9d", depth, score, (time_elapsed() - tree.start_time) / 100, Nodes);
					}

			else if (output_mode == IO_WINBOARD) {
					printf("%d %d %d %d", depth, score, (time_elapsed() - tree.start_time) / 10, Nodes);
					}

			if (output_mode) {
				if (!mizar.o.only_pv) printf(" Phs:%d Phx:%d",Phase(Side),Phase(Xside));
					for (i = 0;i < tree.pv_lenght[0];i++)
						printf(" %s", move2str(tree.old_pv[i]));

					printf("\n");

					fflush(stdout);
					}

			}
	return (m.mi);
	}
