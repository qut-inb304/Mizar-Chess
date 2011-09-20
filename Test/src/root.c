/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*Genera le mosse della Root e le ordina*/
void root(){
	int d,next;
	RMOVEL m;
	unsigned int c;
	
	/*siamo sotto scacco?*/
    check_condition();

	/*generiamo le mosse della root*/
	gen_all();

	/*validiamo, inseriamo nella lista e attribuiamo un valore*/
	tree.last_root_move=0;
	for (c=0;c<tree.first_move[1];c++) {
        if(!makemove(tree.move_list[c].move,0))continue;
		tree.root_move_list[tree.last_root_move].move=tree.move_list[c].move;
		tree.root_move_list[c].scorec=-quiesce(-INF,+INF,0);
		tree.root_move_list[c].scoren=0;
		tree.last_root_move++;
		unmakemove();
	}

	/*ordiniamo*/
	d=1;
	do {
		next=0;
		for (c=0;c<((tree.last_root_move)-d);c++) {
			if (tree.root_move_list[c].scorec<tree.root_move_list[c+1].scorec) {
				m=tree.root_move_list[c];
				tree.root_move_list[c]=tree.root_move_list[c+1];
				tree.root_move_list[c+1]=m;
				next=1;
			}
		}
		d++;
	} while (next);
}

/*Ordina le mosse alla root tra una iterazione e l'altra*/
void sort_root_nodes(){
	unsigned int d,c,next;
	RMOVEL m;
	d=1;
	do {
		next=0;
		for (c=0;c<((tree.last_root_move)-d);c++) {
			if (tree.root_move_list[c].scoren<tree.root_move_list[c+1].scoren) {
				m=tree.root_move_list[c];
				tree.root_move_list[c]=tree.root_move_list[c+1];
				tree.root_move_list[c+1]=m;
				next=1;
			}
		}
		d++;
	} while (next);
}

/*Equivalente alla search() ma alcune cose non vengono effettuate*/
int search_root(int alpha,int beta,int depth){
    int i,c,score,legal,old_depth;
	unsigned int start_nodes,nws;
    MOVE m;

	start_nodes=Nodes;

    /*questo e' un nuovo nodo da cercare*/
    Nodes++;

    /*controlla se è tempo di abortire la ricerca*/
    if(stop_search()) {
        tree.abort_search=1;
        return 0;
    }

    /*aggiorna la lunghezza della pv corrente*/
    tree.pv_lenght[Ply]=Ply;

    /*estensioni*/
    old_depth=depth;
    /*1-se sotto scacco: estendi la ricerca!*/
    if(tree.check[Side][Ply])
        depth++;

    legal=0;
	nws=0;
    /*negascout fail-soft alpha beta*/
	for (i=0;i<tree.last_root_move;i++) {

		m=tree.root_move_list[i].move;

		if(!makemove(m,0)) continue;
        legal = 1;
			if (nws) {
				score = -search( -alpha-1, -alpha, (depth - 1));
				if (tree.abort_search) {
					unmakemove();
					return 0;
				}
				if (score > alpha && score < beta ) {
					score = -search( -beta, -alpha, (depth - 1));
				}
			}
			else score = -search( -beta, -alpha, (depth - 1));

        unmakemove();

		tree.root_move_list[i].scoren=Nodes-start_nodes;
		start_nodes=Nodes;

        if(tree.abort_search)
            return 0;

        if(score>alpha) {
            if(score>=beta) {
                heuristic(m,old_depth);
                return (score);
            }

            alpha=score;
			nws=1;

            /*aggiorna la pv*/
            tree.pv[Ply][Ply]=m;
            heuristic(tree.pv[Ply][Ply],old_depth);
            for(c=Ply+1;c<tree.pv_lenght[Ply+1];c++)
                tree.pv[Ply][c]=tree.pv[Ply+1][c];
            tree.pv_lenght[Ply]=tree.pv_lenght[Ply+1];
        }
    }/*ciclo mosse*/

    /*Se non ci sono mosse legali*/
    if(legal==0) {
        if(tree.check[Side][Ply])
            return -INF+Ply;
        else
            return DRAW;
    }
    return alpha;
}