/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/
#include <mizar.h>

/*heuristic() updates move ordering heuristics */
void heuristic(const MOVE m, const int depth) {

	/*non premiamo catture o promozioni*/
	if (m.ms.flag & (M_CAP | M_PRO))
		return ;

	/*History Heuristic*/
	tree.h_heuristic[m.ms.from][m.ms.to] += (depth * depth);

	/*Countermove Heuristic*/
	tree.c_heuristic[b256to64[tree.history[N_moves - 1].m.ms.from]][b256to64[tree.history[N_moves - 1].m.ms.to]][tree.history[N_moves - 1].m.ms.flag].mi = m.mi;

	/*Killer Heuristic*/
	if (tree.k_heuristic[Ply].m1.mi == m.mi) {
			tree.k_heuristic[Ply].score1++;
			}

	else if (tree.k_heuristic[Ply].m2.mi == m.mi) {
			tree.k_heuristic[Ply].score2++;

			if (tree.k_heuristic[Ply].score2 > tree.k_heuristic[Ply].score1) {
					tree.k_heuristic[Ply].m2 = tree.k_heuristic[Ply].m1;
					tree.k_heuristic[Ply].m1 = m;
					}

			}

	else {
			tree.k_heuristic[Ply].m2.mi = m.mi;
			tree.k_heuristic[Ply].score2 = 1;
			}
	}
