/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*The main function of alpha beta program that implements*/
int search(int alpha, int beta, int depth) {

	int c, score,fscore, legal, old_depth,s=0;
	unsigned int tt_index=0,use_hash_move=0;
	unsigned char my_flag,nws,selective;
	MOVE m,best={0};

	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);


	/*controls whether to go dormant*/

	if (depth == HORIZON) {
			return quiesce(alpha, beta, QDEPTH);
		}

	/*this and 'a new node to search*/
	Nodes++;

	/*checks whether it is time to abort the search*/
	if (stop_search()) {
			tree.abort_search = 1;
			return 0;
			}

	/*updates the length of the current pv*/
	tree.pv_lenght[Ply] = Ply;

	/*checks whether the move is a repetition*/
	if (Ply && repetition())
		return DRAW;

	/*check if we have reached the limits of implementation*/
	if (Ply >= MAX_PLY - 1)
		return beta;

	/*We are under attack?*/
	check_condition();

	/*extensions*/
	old_depth = depth;

	/*Stand Pat Score*/
	fscore = Material(Side) - Material(Xside);
	/*Static Pruning*/
	selective=0;
	/*Alpha*/
	if (mizar.o.use_futility) {
		/*Try pruning only if...*/
		if (alpha == beta-1 && !tree.check[Side][Ply] &&  fscore < alpha) {
			do {
				if (tree.check[Xside][Ply-1])
					break;
				/*Futility Pruning*/
				if (old_depth==1) {
					fscore += FUTILITY_MAR;
					if (fscore <= alpha)
						return fscore;
					}
				/*Extended Futility Pruning*/
				else if (old_depth==2) {
					fscore += EXT_FUTI_MAR;
					if (fscore <= alpha) /*selective = 1;*/
						return fscore;
					}
				} 
			while (0);
			}
		}

	/*Beta*/
	/*It is an idea of Don Dailey, taken from an old post in CCC and adapted by me*/
	if (mizar.o.use_beta_prune) {
		if (alpha == beta-1 && !tree.check[Side][Ply] &&  fscore > beta) {
			do {
				if (tree.check[Xside][Ply-1])
					break;
				if (old_depth>2)
					break;
				fscore=eval(1);
				if (fscore < beta) break;
				if (fscore-margin() >=beta)
					return fscore;

				}
			while (0);
			}
		}

	

	/*controlla la tabella di trasposizione*/
	if (mizar.o.use_hash) {
		/*calcola la posizione*/
		best.mi=0;
		tt_index=(unsigned int)(Hash % mizar.o.hash_size);
		/*trova l'indice se esiste*/
		s=probe_tt(tt_index);
		use_hash_move=0;
		if (s!=-1) {
			/*La posizione è in tabella*/
			if(old_depth<=hash_table[tt_index].slot[s].depth && alpha==beta-1){
			/*la ricerca è stata effettuata su su sottoalbero maggiore*/
			score=hash_table[tt_index].slot[s].score;
			switch (hash_table[tt_index].slot[s].flag) {
				case TT_UPPER:
					if (score<=alpha ) return score;
					beta=min(beta,score);
					break;
				case TT_LOWER:
					if (score>=beta ) return score;
					alpha=max(alpha,score);
					break;
				case TT_EXACT:
					if(hash_table[tt_index].slot[s].best_move.mi && verify_move(hash_table[tt_index].slot[s].best_move)) {
						tree.pv[Ply][Ply]=hash_table[tt_index].slot[s].best_move;
						if (tree.pv_lenght[Ply+1]) {
							for(c=Ply+1;c<tree.pv_lenght[Ply+1];c++)
								tree.pv[Ply][c]=tree.pv[Ply+1][c];
							tree.pv_lenght[Ply]=tree.pv_lenght[Ply+1];
						}
					}
					return score;
					break;
				default:
					puts("Bad flag in transposition table");
					assert(0);
				}
			}/*end if depth*/
			use_hash_move=1;
		}		
	}

	/*Inizializza flag per la tabella di trasposizione*/
	my_flag=TT_UPPER;	

	/*1-se sotto scacco: estendi la ricerca!*/
	if (tree.check[Side][Ply])
		depth++;

	/*Inizializziamo la "pompa" delle mosse*/
	tree.status[Ply] = ST_PV_MOVE;

	legal = 0;
	nws = 0;

	/*negascout fail-soft alpha beta*/
	while ((m.mi = select_move(use_hash_move,tt_index,s)) != 0) {

			if (!makemove(m,selective))
				continue;

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

			if (tree.abort_search)
				return 0;
			assert(Name(WHITE,0)==king);
			assert(Name(BLACK,0)==king);
			

			if (score > alpha) {
					if (score >= beta) {
							heuristic(m, old_depth);
							if (mizar.o.use_hash)
								store_tt(tt_index,score,old_depth,TT_LOWER,m);
							return (score);
							}

					alpha = score;
					best.mi=m.mi;
					my_flag=TT_EXACT;
					nws=1;


					/*aggiorna la pv*/
					tree.pv[Ply][Ply] = m;
					heuristic(tree.pv[Ply][Ply], old_depth);

					for (c = Ply + 1;c < tree.pv_lenght[Ply + 1];c++)
						tree.pv[Ply][c] = tree.pv[Ply + 1][c];

					tree.pv_lenght[Ply] = tree.pv_lenght[Ply + 1];
					}

			} /*ciclo mosse*/

	/*Se non ci sono mosse legali*/

	if (legal == 0 && selective == 0) {
			if (tree.check[Side][Ply])
				return -INF + Ply;
			else
				return DRAW;
			}

	if (selective) {
		my_flag=TT_UPPER;
		}

	if (mizar.o.use_hash)
		store_tt(tt_index,alpha,old_depth,my_flag,best);
	return alpha;
	}
