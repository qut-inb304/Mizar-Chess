/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*La ricerca quiescente*/
int quiesce(int alpha, int beta, int qdepth) {

	int i, c, score, pessimistic, optimistic;
	MOVEL m;
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);


	/*questo e' un nuovo nodo da cercare*/
	Nodes++;

	/*controlla se è tempo di abortire la ricerca*/

	if (stop_search()) {
			tree.abort_search = 1;
			return 0;
			}

	/*if (max_check_depth<0)        max_check_depth=0;*/

	/*aggiorna la lunghezza della pv corrente*/
	tree.pv_lenght[Ply] = Ply;

	/*controlla se abbiamo raggiunto i limiti di implementazione*/
	if (Ply >= MAX_PLY - 1)
		return eval(1);

	/*controlla se si è sotto scacco o se si deve verificare la legalità della mossa*/
	check_condition();

	/*valore statico della posizione*/
	score = eval(1);

/*	calcola valore pessimista (Punteggio-mio pezzo in presa-1/2 pedone)*/
	if (qdepth > 0)pessimistic = score - Hang(Side) - (PAWN_VALUE >> 1);
	else pessimistic = score;

	if (pessimistic >= beta)
		return score;

	if (score > alpha) {
			/*aggiorna alpha*/
			alpha = score;
			}

	else {
		if (Hang(Xside)!=-1) {
			/*calcola valore ottimista (Punteggio + pezzo in presa avversario + 1/2 pedone)*/
			optimistic = score + Hang(Xside) + (PAWN_VALUE >> 1);
			if (optimistic < alpha)
				return score;
			}
		else {
			optimistic = score + (PAWN_VALUE >> 1);
			if (optimistic < alpha) {
				for (c=1;c<16;c++) {
					if (Active(Xside,c)) {
						if (attacked(Position(Xside,c),Side)) {
							optimistic += piece_value[Name(Xside,c)];
							break;
							}
						}
					}
				if (optimistic < alpha)
					return score;
				}
			}
		}
	/*generiamo le catture*/
	gen_all_captures();

	
	if ((tree.first_move[Ply + 1] - tree.first_move[Ply]) > 1) {
			c = 2;

			do {
					score = 0;

					for (i = tree.first_move[Ply]; i < tree.first_move[Ply + 1] - c;i++) {
							if (tree.move_list[i].scorec < tree.move_list[i + 1].scorec) {
									m = tree.move_list[i];
									tree.move_list[i] = tree.move_list[i + 1];
									tree.move_list[i + 1] = m;
									score = 1;
									}

							}
					c++;
					}

			while (score);
			}


	for (i = tree.first_move[Ply]; i < tree.first_move[Ply + 1];i++) {

			if (!makemove(tree.move_list[i].move,0))
				continue;

			score = -quiesce( -beta, -alpha, qdepth - 1);

			unmakemove();

			if (tree.abort_search)
				return 0;

			assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

			if (score > alpha) {
					if (score >= beta)
						return (score);

					alpha = score;

					/*aggiorna la pv*/
					tree.pv[Ply][Ply] = tree.move_list[i].move;

					for (c = Ply + 1;c < tree.pv_lenght[Ply + 1];c++)
						tree.pv[Ply][c] = tree.pv[Ply + 1][c];

					tree.pv_lenght[Ply] = tree.pv_lenght[Ply + 1];
					}

			}
	return alpha;
	}
