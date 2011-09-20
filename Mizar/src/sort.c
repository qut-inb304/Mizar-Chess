/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>
/*
Ordine delle mosse:
1 Mossa PV iterazione precedente
2 catture positive
 - catture+promozione
 - catture
 - promozioni
3 catture uguali
4 counter move
5 killer move
6 history heuristic
*/

int verify_move(const MOVE m);
int choose (const unsigned int from, const unsigned int stop);
int choose2 (const unsigned int from, const unsigned int stop);
int choose3 (const unsigned int from, const unsigned int stop);

/*la funziona che sceglie la prossima mossa da effettuare
Ho seguito la filosofia KISS, ma va ottimizzata*/
unsigned int old_select_move(const int hm,const unsigned int index,const int sl) {
	MOVEL m;
	unsigned int i;

	switch (tree.status[Ply]) {
			case ST_PV_MOVE:
				m.move.mi = tree.old_pv[Ply].mi; /*prova la mossa della pv*/
				tree.status[Ply] = ST_GEN_ALL;
				tree.next[Ply] = 0;
				tree.first_move_played[Ply].mi=0;

				if (1 && tree.follow_pv && m.move.mi) {
						if (verify_move(m.move)) {
								tree.next[Ply]++;
								tree.first_move_played[Ply].mi=m.move.mi;
								return m.move.mi;
								}

						tree.follow_pv = 0;
						}

			case ST_GEN_ALL:
				gen_all(); /*genera tutte le mosse*/

				if ((tree.first_move[Ply + 1] == tree.first_move[Ply]))
					return 0;

			/*	tree.next[Ply]+=tree.first_move[Ply];*/
				/*Elimina la mossa PV*/
				if (1 && tree.next[Ply]) {
						for (i = tree.first_move[Ply];i < tree.first_move[Ply + 1];i++) {
								/*inserisce la PV come prima mossa*/

								if (tree.move_list[i].move.mi == tree.pv[0][Ply].mi) {
										m = tree.move_list[i];
										tree.move_list[i] = tree.move_list[tree.first_move[Ply]];
										tree.move_list[tree.first_move[Ply]] = m;
										break;
										}

								}

						}
				tree.next[Ply] += tree.first_move[Ply];
				tree.status[Ply] = ST_TT_MOVE;

			case ST_TT_MOVE:
				/*Mossa tabella hash*/
				if (mizar.o.use_hash && hm) {
					m.move=hash_table[index].slot[sl].best_move;
					if (m.move.mi && m.move.mi!=tree.first_move_played[Ply].mi) {
						for (i = tree.next[Ply]; i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == m.move.mi) {
										tree.move_list[i]=tree.move_list[tree.next[Ply]];
										tree.move_list[tree.next[Ply]] = m;
										tree.next[Ply]++;
										tree.status[Ply] = ST_CPT_MOV;
										return (m.move.mi);
										break;
							}
						}				
					}
				}
				tree.status[Ply] = ST_CPT_MOV;

			case ST_CPT_MOV:  /*Seglie tra le catture*/

				if (choose(tree.next[Ply], tree.first_move[Ply + 1])) {
						m = tree.move_list[tree.next[Ply]];
						tree.next[Ply]++;
						return m.move.mi;
						}

				if (tree.next[Ply] == tree.first_move[Ply + 1])
					return 0;

				tree.status[Ply] = ST_CNT_MOV;

			case ST_CNT_MOV:
				m.move.mi = tree.c_heuristic[b256to64[tree.history[N_moves - 1].m.ms.from]][b256to64[tree.history[N_moves - 1].m.ms.to]][tree.history[N_moves - 1].m.ms.flag].mi;

				if (m.move.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == m.move.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_KL1_MOV;
									return m.move.mi;
									}

							}

			case ST_KL1_MOV:  /*Killer Move 1*/

				if (tree.k_heuristic[Ply].m1.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == tree.k_heuristic[Ply].m1.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_KL2_MOV;
									return tree.k_heuristic[Ply].m1.mi;
									}

							}

			case ST_KL2_MOV:  /*Killer Move 2*/

				if (tree.k_heuristic[Ply].m2.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == tree.k_heuristic[Ply].m1.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_SRT_ALL;
									return tree.k_heuristic[Ply].m1.mi;
									}

							}
				tree.status[Ply] = ST_SRT_ALL;

			case ST_SRT_ALL:  /*Assegna un punteggio alle mosse rimanenti*/

				for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
						tree.move_list[i].scoren = tree.h_heuristic[tree.move_list[i].move.ms.from][tree.move_list[i].move.ms.to];
						}

				tree.status[Ply] = ST_ALL_MOV;

			case ST_ALL_MOV: /* sceglie tra le rimanenti mosse*/

				if (choose2(tree.next[Ply], tree.first_move[Ply + 1])) {
						m = tree.move_list[tree.next[Ply]];
						tree.next[Ply]++;
						return m.move.mi;
						}

				break;
			default:
				puts("Unexpected tree status.");
				assert(0);
			}

	return 0;
	}
unsigned int select_move(const int hm,const unsigned int index,const int sl) {
	MOVEL m;
	unsigned int i;

	switch (tree.status[Ply]) {
			case ST_PV_MOVE:
				gen_all(); /*genera tutte le mosse*/
				if ((tree.first_move[Ply + 1] == tree.first_move[Ply]))
					return 0;
				m.move.mi = tree.old_pv[Ply].mi; /*prova la mossa della pv*/
				tree.status[Ply] = ST_TT_MOVE;
				tree.next[Ply] = tree.first_move[Ply];
				tree.first_move_played[Ply].mi=0;			

				if (tree.follow_pv && m.move.mi) {
					for (i = tree.first_move[Ply];i < tree.first_move[Ply + 1];i++) {
						/*inserisce la PV come prima mossa*/
						if (tree.move_list[i].move.mi == m.move.mi) {
							m = tree.move_list[i];
							tree.move_list[i] = tree.move_list[tree.first_move[Ply]];
							tree.move_list[tree.first_move[Ply]] = m;
							tree.next[Ply]++;
							tree.first_move_played[Ply].mi=m.move.mi;
							return m.move.mi;
						}
					}
					tree.follow_pv = 0;
				}

			case ST_GEN_ALL:
				
			case ST_TT_MOVE:
				/*Mossa tabella hash*/
				if (mizar.o.use_hash && hm) {
					m.move=hash_table[index].slot[sl].best_move;
					if (m.move.mi && m.move.mi!=tree.first_move_played[Ply].mi) {
						for (i = tree.next[Ply]; i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == m.move.mi) {
										tree.move_list[i]=tree.move_list[tree.next[Ply]];
										tree.move_list[tree.next[Ply]] = m;
										tree.next[Ply]++;
										tree.status[Ply] = ST_CPT_MOV;
										return (m.move.mi);
										break;
							}
						}				
					}
				}
				tree.status[Ply] = ST_CPT_MOV;

			case ST_CPT_MOV:  /*Seglie tra le catture*/

				if (choose(tree.next[Ply], tree.first_move[Ply + 1])) {
						m = tree.move_list[tree.next[Ply]];
						tree.next[Ply]++;
						return m.move.mi;
						}

				if (tree.next[Ply] == tree.first_move[Ply + 1])
					return 0;

				tree.status[Ply] = ST_CNT_MOV;

			case ST_CNT_MOV:
				m.move.mi = tree.c_heuristic[b256to64[tree.history[N_moves - 1].m.ms.from]][b256to64[tree.history[N_moves - 1].m.ms.to]][tree.history[N_moves - 1].m.ms.flag].mi;

				if (m.move.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == m.move.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_KL1_MOV;
									return m.move.mi;
									}

							}

			case ST_KL1_MOV:  /*Killer Move 1*/

				if (tree.k_heuristic[Ply].m1.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == tree.k_heuristic[Ply].m1.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_KL2_MOV;
									return tree.k_heuristic[Ply].m1.mi;
									}

							}

			case ST_KL2_MOV:  /*Killer Move 2*/

				if (tree.k_heuristic[Ply].m2.mi)
					for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
							if (tree.move_list[i].move.mi == tree.k_heuristic[Ply].m1.mi) {
									tree.move_list[i] = tree.move_list[tree.next[Ply]];
									tree.next[Ply]++;
									tree.status[Ply] = ST_SRT_ALL;
									return tree.k_heuristic[Ply].m1.mi;
									}

							}
				tree.status[Ply] = ST_SRT_ALL;

			case ST_SRT_ALL:  /*Assegna un punteggio alle mosse rimanenti*/

				for (i = tree.next[Ply];i < tree.first_move[Ply + 1];i++) {
						tree.move_list[i].scoren = tree.h_heuristic[tree.move_list[i].move.ms.from][tree.move_list[i].move.ms.to];
						}

				tree.status[Ply] = ST_ALL_MOV;

			case ST_ALL_MOV: /* sceglie tra le rimanenti mosse*/

				if (choose2(tree.next[Ply], tree.first_move[Ply + 1])) {
						m = tree.move_list[tree.next[Ply]];
						tree.next[Ply]++;
						return m.move.mi;
						}

				break;
			default:
				puts("Unexpected tree status.");
				assert(0);
			}

	return 0;
	}

/*verifica che la mossa è una mossa possibile*/
int verify_move(const MOVE m) {
	unsigned __int64 all;
	all = Bitboard(BLACK) | Bitboard(WHITE);

	if (Color(m.ms.from) != Side)
		return 0;
	if (Piece(m.ms.to) == king)
		return 0;
	if (Piece(m.ms.from) == out_board || Piece(m.ms.from) == no_piece)
		return 0;

	switch (Piece(m.ms.from)) {
			case no_piece:
				return 0;
			case wpawn:
				if (m.ms.piece != wpawn && ((m.ms.flag & M_PRO)==0))
					return 0;
				if (Plist(m.ms.from)==0)
					return 0;
				if (Active(Side,Plist(m.ms.from))==CAPTURED)
					return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from)
					return 0;

				if (m.ms.flag & M_PAW) {
						if (m.ms.flag & M_CAP) {
								if (m.ms.flag & M_ENP) {
										if (Piece(m.ms.to + DOWN) != bpawn)
											return 0;
										if (Active(Xside,Plist(m.ms.to + DOWN))==CAPTURED)
											return 0;
										if (Position(Xside,Plist(m.ms.to + DOWN))!=(m.ms.to + DOWN))
											return 0;
										if (Rank(m.ms.from) != Riga5)
											return 0;
										return 1;
										}
								if (Plist(m.ms.to)==0) return 0;
								if (Active(Xside,Plist(m.ms.to))==CAPTURED) return 0;
								if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;

								if (Color(m.ms.to) != Xside)
									return 0;

								if (Piece(m.ms.to) == king)
									return 0;

								if (m.ms.to == m.ms.from + UP_R || m.ms.to == m.ms.from + UP_L)
									return 1;

								if (m.ms.flag & M_PRO && Rank(m.ms.from) != Riga7)
									return 0;

								return 1;
								}

						if (Color(m.ms.to) != EMPTY)
							return 0;

						if (m.ms.flag & M_DBP) {
								if (Rank(m.ms.from) != Riga2)
									return 0;

								if (Color(m.ms.to + DOWN) != EMPTY)
									return 0;

								return 1;
								}

						if (m.ms.to == m.ms.from + UP)
							return 1;

						return 0;
						}

				return 0;
			case bpawn:

				if (m.ms.piece != bpawn && ((m.ms.flag & M_PRO)==0))
					return 0;
				if (Plist(m.ms.from)==0) return 0;

				if (Active(Side,Plist(m.ms.from))==0) return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) return 0;

				if (m.ms.flag & M_PAW) {
						if (m.ms.flag & M_CAP) {
								if (m.ms.flag & M_ENP) {
										if (Piece(m.ms.to + UP) != wpawn)
											return 0;
										if (Active(Xside,Plist(m.ms.to + UP))==CAPTURED)
											return 0;
										if (Position(Xside,Plist(m.ms.to + UP))!=(m.ms.to + UP))
											return 0;

										if (Rank(m.ms.from) != Riga4)
											return 0;

										return 1;
										}

								if (Plist(m.ms.to)==0) return 0;
								if (Active(Xside,Plist(m.ms.to))==0) return 0;
								if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;


								if (Color(m.ms.to) != Xside)
									return 0;

								if (Piece(m.ms.to) == king)
									return 0;

								if (m.ms.to == m.ms.from + DOWN_R || m.ms.to == m.ms.from + DOWN_L)
									return 1;

								if (m.ms.flag & M_PRO && Rank(m.ms.from) != Riga2)
									return 0;

								return 1;
								}

						if (Color(m.ms.to) != EMPTY)
							return 0;

						if (m.ms.flag & M_DBP) {
								if (Rank(m.ms.from) != Riga7)
									return 0;

								if (Color(m.ms.to + UP) != EMPTY)
									return 0;

								return 1;
								}

						if (m.ms.to == m.ms.from + DOWN)
							return 1;

						return 0;
						}

				return 0;
			case knight:
				if (m.ms.piece != knight)
					return 0;

				if (Plist(m.ms.from)==0)
					return 0;
				if (Active(Side,Plist(m.ms.from))==0) 
					return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) 
					return 0;
				if ((m.ms.flag == M_STD) && Color(m.ms.to) != EMPTY)
					return 0;
				if ((m.ms.flag & M_CAP) && Color(m.ms.to) != Xside)
					return 0;
				if ((m.ms.flag & M_CAP) && Piece(m.ms.to) == king)
					return 0;

				if (m.ms.flag & M_CAP) {
					if (Plist(m.ms.to)==0) return 0;
					if (Active(Xside,Plist(m.ms.to))==0) return 0;
					if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;
					}


				if (bitn&table_attack[m.ms.to - m.ms.from + 128])
					return 1;

				return 0;

			case bishop:
				if (m.ms.piece != bishop)
					return 0;

				if (Plist(m.ms.from)==0) return 0;
				if (Active(Side,Plist(m.ms.from))==0) return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) return 0;

				if ((m.ms.flag == M_STD) && Color(m.ms.to) != EMPTY)
					return 0;

				if (m.ms.flag & M_CAP && Color(m.ms.to) != Xside)
					return 0;
				if ((m.ms.flag & M_CAP) && Piece(m.ms.to) == king)
									return 0;
				if (m.ms.flag & M_CAP) {
					if (Plist(m.ms.to)==0) return 0;
					if (Active(Xside,Plist(m.ms.to))==0) return 0;
					if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;
					}

				if (bitb&table_attack[m.ms.to - m.ms.from + 128]) {
						if ((all&(bit_ray[m.ms.from][m.ms.to])) == 0)
							return 1;
						}

				return 0;
			case rook:

				if (m.ms.piece != rook)
					return 0;

				if (Plist(m.ms.from)==0) return 0;
				if (Active(Side,Plist(m.ms.from))==0) return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) return 0;

				if ((m.ms.flag == M_STD) && Color(m.ms.to) != EMPTY)
					return 0;

				if ((m.ms.flag & M_CAP) && Color(m.ms.to) != Xside)
					return 0;
				if ((m.ms.flag & M_CAP) && Piece(m.ms.to) == king)
									return 0;
				if (m.ms.flag & M_CAP) {
					if (Plist(m.ms.to)==0) return 0;
					if (Active(Xside,Plist(m.ms.to))==0) return 0;
					if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;
					}

				if (bitr&table_attack[m.ms.to - m.ms.from + 128]) {
						if ((all&(bit_ray[m.ms.from][m.ms.to])) == 0)
							return 1;
						}

				return 0;
			case queen:

				if (m.ms.piece != queen)
					return 0;

				if (Plist(m.ms.from)==0) return 0;
				if (Active(Side,Plist(m.ms.from))==0) return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) return 0;

				if ((m.ms.flag == M_STD) && Color(m.ms.to) != EMPTY)
					return 0;

				if ((m.ms.flag & M_CAP) && Color(m.ms.to) != Xside)
					return 0;

				if ((m.ms.flag & M_CAP) && Piece(m.ms.to) == king)
									return 0;
				if (m.ms.flag & M_CAP) {
					if (Plist(m.ms.to)==0) return 0;
					if (Active(Xside,Plist(m.ms.to))==0) return 0;
					if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;
					}

				if (bitq&table_attack[m.ms.to - m.ms.from + 128]) {
						if ((all&(bit_ray[m.ms.from][m.ms.to])) == 0)
							return 1;
						}

				return 0;
			case king:

				if (m.ms.piece != king)
					return 0;

				if (Plist(m.ms.from)!=0) return 0;
				if (Active(Side,Plist(m.ms.from))==0) return 0;
				if (Position(Side,Plist(m.ms.from))!=m.ms.from) return 0;

				if (m.ms.flag & M_KNG) {
						if (m.ms.flag & M_CAP) {
							
								if (Plist(m.ms.to)==0) return 0;
								if (Active(Xside,Plist(m.ms.to))==0) return 0;
								if (Position(Xside,Plist(m.ms.to))!=m.ms.to) return 0;
					
								if (Color(m.ms.to) != Xside)
									return 0;
								if (Piece(m.ms.to) == king)
									return 0;
								}

						else if (Color(m.ms.to) != EMPTY)
							return 0;

						if (bitk&table_attack[m.ms.to - m.ms.from + 128])
							return 1;

						return 0;
						}

				if (Side == BLACK) {
						if ((m.ms.flag&M_OO) && (Cstr&cas_boo) && IsEmpty(F8) && IsEmpty(G8))
							return 1;

						if ((m.ms.flag&M_OOO) && (Cstr&cas_booo) && IsEmpty(B8) && IsEmpty(C8) && IsEmpty(D8))
							return 1;
						}

				else {
						if ((m.ms.flag&M_OO) && (Cstr&cas_woo) && IsEmpty(F1) && IsEmpty(G1))
							return 1;

						if ((m.ms.flag&M_OOO) && (Cstr&cas_wooo) && IsEmpty(B1) && IsEmpty(C1) && IsEmpty(D1))
							return 1;
						}

				return 0;
			default:
				puts("Unexpected piece name.");
				assert(0);
			}

	return 0;
	}

/*Spinge la mossa giusta in cima alla lista(catture)*/
int choose (const unsigned int from, const unsigned int stop) {
	int best, index, i;
	MOVEL m;
	best = -1;
	index = from;

	for (i = from;i < stop;i++) {
			if (tree.move_list[i].scorec > best) {
					best = tree.move_list[i].scorec;
					index = i;
					}

			}

	if (best == -1)
		return 0; /*non ci sono più mosse*/

	m = tree.move_list[from];

	tree.move_list[from] = tree.move_list[index];

	tree.move_list[index] = m;

	return 1;
	}

/*Spinge la mossa giusta in cima alla lista(rimanenti)*/
int choose2 (const unsigned int from, const unsigned int stop) {
	int best, index, i;
	MOVEL m;
	best = -1;
	index = from;

	for (i = from;i < stop;i++) {
			if (tree.move_list[i].scoren > best) {
					best = tree.move_list[i].scoren;
					index = i;
					}

			}

	if (best == -1)
		return 0; /*non ci sono più mosse*/

	m = tree.move_list[from];

	tree.move_list[from] = tree.move_list[index];

	tree.move_list[index] = m;

	return 1;
	}


/*Spinge la mossa giusta in cima alla lista(catture)*/
int choose3 (const unsigned int from, const unsigned int stop) {
	int best, index, i;
	MOVEL m;
	best = -1000000;
	index = from;

	for (i = from;i < stop;i++) {
			if (tree.move_list[i].scoren == -1)
				continue;

			if (tree.move_list[i].scoren > best) {
					best = tree.move_list[i].scorec;
					index = i;
					}

			}

	if (best == -1000000)
		return 0; /*non ci sono più mosse*/

	m = tree.move_list[from];

	tree.move_list[from] = tree.move_list[index];

	tree.move_list[index] = m;

	return 1;
	}
