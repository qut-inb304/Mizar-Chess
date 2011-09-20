/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>




/*genera tutte le mosse*/
void gen_all() {
	int c;

	c = 0;
	tree.first_move[Ply + 1] = tree.first_move[Ply];
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);

	do {
			if (Active(Side, c))
				gen_piece_moves(Position(Side, c));

			c++;
			}

	while (c < 16);

	/*arrocco ed en-passant*/
	switch (Side) {
			case BLACK:

				if (Cstr&cas_boo)
					if (IsEmpty(F8) && IsEmpty(G8))
						ins_b_oo(); /*ins_move(E8,G8);*/

				if (Cstr&cas_booo)
					if (IsEmpty(B8) && IsEmpty(C8) && IsEmpty(D8))
						ins_b_ooo(); /*ins_move(E8,C8);*/

				if (Enp) {
						if (Piece(Enp + UP_R) == bpawn)
							ins_pawn_enp((Enp + UP_R), Enp,bpawn);

						if (Piece(Enp + UP_L) == bpawn)
							ins_pawn_enp((Enp + UP_L), Enp,bpawn);
						}

				break;
			case WHITE:

				if (Cstr&cas_woo)
					if (IsEmpty(F1) && IsEmpty(G1))
						ins_w_oo(); /*ins_move(E1,G1);*/

				if (Cstr&cas_wooo)
					if (IsEmpty(B1) && IsEmpty(C1) && IsEmpty(D1))
						ins_w_ooo(); /*ins_move(E1,C1);*/

				if (Enp) {
						if (Piece(Enp + DOWN_R) == wpawn)
							ins_pawn_enp((Enp + DOWN_R), Enp,wpawn);

						if (Piece(Enp + DOWN_L) == wpawn)
							ins_pawn_enp((Enp + DOWN_L), Enp,wpawn);
						}

				break;
			default:
				puts("Bad side to move (neither black nor white).");
				assert(0);
			}
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);
	}

/*genera tutte le catture e le promozioni a donna*/
void gen_all_captures() {
	int c;
	c = 0;
	tree.first_move[Ply + 1] = tree.first_move[Ply];
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);

	do {
			if (Active(Side, c))
				gen_piece_captures(Position(Side, c));

			c++;
			}

	while (c < 16);

	/*en-passant*/
	switch (Side) {
			case BLACK:

				if (Enp) {
						if (Piece(Enp + UP_R) == bpawn)
							ins_pawn_enp((Enp + UP_R), Enp,bpawn);

						if (Piece(Enp + UP_L) == bpawn)
							ins_pawn_enp((Enp + UP_L), Enp,bpawn);
						}

				break;
			case WHITE:

				if (Enp) {
						if (Piece(Enp + DOWN_R) == wpawn)
							ins_pawn_enp((Enp + DOWN_R), Enp,wpawn);

						if (Piece(Enp + DOWN_L) == wpawn)
							ins_pawn_enp((Enp + DOWN_L), Enp,wpawn);
						}

				break;
			default:
				puts("Bad side to move (neither black nor white).");
				assert(0);
			}
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);
	}




void gen_piece_moves(const int start) {
	int target;

	switch (Piece(start)) {
			case no_piece:
				break;
			case wpawn:

				if (Rank(start) == Riga7) {
						if (IsEmpty(start - 16))
							ins_pawn_move_promo( start, (start - 16));

						if (IsEnemy(start + UP_R))
							ins_pawn_cap_promo( start, (start + UP_R));

						if (IsEnemy(start + UP_L))
							ins_pawn_cap_promo( start, (start + UP_L));

						break;
						}

				if (IsEmpty(start - 16)) {
						ins_pawn_move( start, (start - 16), wpawn);

						if (Rank(start) == Riga2 && IsEmpty(start - 32))
							ins_pawn_dbmove( start, (start - 32),wpawn);
						}

				if (IsEnemy(start + UP_R))
					ins_pawn_cap( start, (start + UP_R),wpawn);

				if (IsEnemy(start + UP_L))
					ins_pawn_cap( start, (start + UP_L),wpawn);

				break;

			case bpawn:
				if (Rank(start) == Riga2) {
						if (IsEmpty(start + 16))
							ins_pawn_move_promo( start, (start + 16));

						if (IsEnemy(start + DOWN_R))
							ins_pawn_cap_promo( start, (start + DOWN_R));

						if (IsEnemy(start + DOWN_L))
							ins_pawn_cap_promo( start, (start + DOWN_L));

						break;
						}

				if (IsEmpty(start + 16)) {
						ins_pawn_move( start, (start + 16),bpawn);

						if (Rank(start) == Riga7 && IsEmpty(start + 32))
							ins_pawn_dbmove( start, (start + 32),bpawn);
						}

				if (IsEnemy(start + DOWN_R))
					ins_pawn_cap( start, (start + DOWN_R),bpawn);

				if (IsEnemy(start + DOWN_L))
					ins_pawn_cap( start, (start + DOWN_L),bpawn);

				break;

			case knight:
				target = start + KNIGHT1;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT2;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT3;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT4;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT5;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT6;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT7;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT8;

				if (IsEmpty(target))
					ins_move( start, target,knight);
				else if (IsEnemy(target))
					ins_cap( start, target,knight);

				break;

			case bishop:
				target = start;

				while (IsEmpty(target += UP_R))
					ins_move( start, target,bishop);

				if (IsEnemy(target))
					ins_cap( start, target,bishop);

				target = start;

				while (IsEmpty(target += UP_L))
					ins_move( start, target,bishop);

				if (IsEnemy(target))
					ins_cap( start, target,bishop);

				target = start;

				while (IsEmpty(target += DOWN_R))
					ins_move( start, target,bishop);

				if (IsEnemy(target))
					ins_cap( start, target,bishop);

				target = start;

				while (IsEmpty(target += DOWN_L))
					ins_move( start, target,bishop);

				if (IsEnemy(target))
					ins_cap( start, target,bishop);

				break;

			case rook:
				target = start;

				while (IsEmpty(target += UP))
					ins_move( start, target,rook);

				if (IsEnemy(target))
					ins_cap( start, target,rook);

				target = start;

				while (IsEmpty(target += RIGHT))
					ins_move( start, target,rook);

				if (IsEnemy(target))
					ins_cap( start, target,rook);

				target = start;

				while (IsEmpty(target += DOWN))
					ins_move( start, target,rook);

				if (IsEnemy(target))
					ins_cap( start, target,rook);

				target = start;

				while (IsEmpty(target += LEFT))
					ins_move( start, target,rook);

				if (IsEnemy(target))
					ins_cap( start, target,rook);

				break;

			case queen:
				target = start;

				while (IsEmpty(target += UP_R))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += UP_L))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += DOWN_R))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += DOWN_L))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += UP))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += RIGHT))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += DOWN))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				target = start;

				while (IsEmpty(target += LEFT))
					ins_move( start, target,queen);

				if (IsEnemy(target))
					ins_cap( start, target,queen);

				break;

			case king:
				target = start + UP;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + RIGHT;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + LEFT;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + UP_R;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + UP_L;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN_R;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN_L;

				if (IsEmpty(target))
					ins_king_move( start, target);
				else if (IsEnemy(target))
					ins_king_cap( start, target);

				break;

			default:
				puts("Bad piece name.");

				assert(0);
			} /*end switch*/
	}




void gen_piece_captures(const int start) {
	int target;

	switch (Piece(start)) {
			case no_piece:
				break;
			case wpawn:

				if (Rank(start) == Riga7) {
						if (IsEmpty(start - 16))
							ins_pawn_move_queen_promo( start, (start - 16));

						if (IsEnemy(start + UP_R))
							ins_pawn_cap_queen_promo( start, (start + UP_R));

						if (IsEnemy(start + UP_L))
							ins_pawn_cap_queen_promo( start, (start + UP_L));

						break;
						}

				if (IsEnemy(start + UP_R))
					ins_pawn_cap( start, (start + UP_R),wpawn);

				if (IsEnemy(start + UP_L))
					ins_pawn_cap( start, (start + UP_L),wpawn);

				break;

			case bpawn:
				if (Rank(start) == Riga2) {
						if (IsEmpty(start + 16))
							ins_pawn_move_queen_promo( start, (start + 16));

						if (IsEnemy(start + DOWN_R))
							ins_pawn_cap_queen_promo( start, (start + DOWN_R));

						if (IsEnemy(start + DOWN_L))
							ins_pawn_cap_queen_promo( start, (start + DOWN_L));

						break;
						}

				if (IsEnemy(start + DOWN_R))
					ins_pawn_cap( start, (start + DOWN_R),bpawn);

				if (IsEnemy(start + DOWN_L))
					ins_pawn_cap( start, (start + DOWN_L),bpawn);

				break;

			case knight:
				target = start + KNIGHT1;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT2;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT3;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT4;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT5;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT6;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT7;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				target = start + KNIGHT8;

				if (IsEnemy(target))
					ins_cap( start, target,knight);

				break;

			case bishop:
				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP_R]) {
						while (IsEmpty(target += UP_R))

							;
						if (IsEnemy(target))
							ins_cap( start, target,bishop);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP_L]) {
						while (IsEmpty(target += UP_L))

							;
						if (IsEnemy(target))
							ins_cap( start, target,bishop);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN_R]) {
						while (IsEmpty(target += DOWN_R))

							;
						if (IsEnemy(target))
							ins_cap( start, target,bishop);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN_L]) {
						while (IsEmpty(target += DOWN_L))

							;
						if (IsEnemy(target))
							ins_cap( start, target,bishop);
						}

				break;

			case rook:
				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP]) {
						while (IsEmpty(target += UP))

							;
						if (IsEnemy(target))
							ins_cap( start, target,rook);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BRIGHT]) {
						while (IsEmpty(target += RIGHT))

							;
						if (IsEnemy(target))
							ins_cap( start, target,rook);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN]) {
						while (IsEmpty(target += DOWN))

							;
						if (IsEnemy(target))
							ins_cap( start, target,rook);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BLEFT]) {
						while (IsEmpty(target += LEFT))

							;
						if (IsEnemy(target))
							ins_cap( start, target,rook);
						}

				break;

			case queen:
				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP_R]) {
						while (IsEmpty(target += UP_R))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP_L]) {
						while (IsEmpty(target += UP_L))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN_R]) {
						while (IsEmpty(target += DOWN_R))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN_L]) {
						while (IsEmpty(target += DOWN_L))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BUP]) {
						while (IsEmpty(target += UP))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BRIGHT]) {
						while (IsEmpty(target += RIGHT))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BDOWN]) {
						while (IsEmpty(target += DOWN))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}

				target = start;

				if (Bitboard(Xside) & bit_scan[start][BLEFT]) {
						while (IsEmpty(target += LEFT))

							;
						if (IsEnemy(target))
							ins_cap( start, target,queen);
						}


				break;

			case king:
				target = start + UP;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + RIGHT;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + LEFT;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + UP_R;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + UP_L;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN_R;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				target = start + DOWN_L;

				if (IsEnemy(target))
					ins_king_cap( start, target);

				break;

			default:
				puts("Bad piece name.");

				assert(0);
			} /*end switch*/
	}

 void ins_move (const int f, const int t, const char p) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_STD;
	m->move.ms.piece = p;
	m->scorec = -1;
	}

 void ins_cap(const int f, const int t, const char p) {

	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_CAP;
	m->move.ms.piece = p;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) - piece_value[Piece(f)];
	}

  void ins_pawn_move(const int f, const int t, const char p) {

	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PAW;
	m->move.ms.piece = p;
	m->scorec = -1;
	}



 void ins_pawn_dbmove(const int f, const int t, const char p) {

	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_DBP | M_PAW;
	m->move.ms.piece = p;
	m->scorec = -1;
	}



 void ins_pawn_cap(const int f, const int t, const char p) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_CAP | M_PAW;
	m->move.ms.piece = p;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) - 100;
	}

 void ins_pawn_enp(const int f, const int t, const char p) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_CAP | M_PAW | M_ENP;
	m->move.ms.piece = p;
	m->scorec = 1;

	}


 void ins_king_move (const int f, const int t) {

	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_KNG;
	m->move.ms.piece = king;
	m->scorec = -1;
	}

 void ins_king_cap(const int f, const int t) {

	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_CAP | M_KNG;
	m->move.ms.piece = king;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) - 1500;
	}



 void ins_pawn_move_promo(const int f, const int t) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_PAW;
	m->move.ms.piece = queen;
	m->scorec = QUEEN_VALUE - 100;


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_PAW;
	m->move.ms.piece = rook;
	m->scorec = ROOK_VALUE - 100;


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_PAW;
	m->move.ms.piece = bishop;
	m->scorec = BISHOP_VALUE - 100;


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_PAW;
	m->move.ms.piece = knight;
	m->scorec = KNIGHT_VALUE - 100;

	}

 void ins_pawn_move_queen_promo(const int f, const int t) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_PAW;
	m->move.ms.piece = queen;
	m->scorec = QUEEN_VALUE - 100;
	}




 void ins_pawn_cap_promo(const int f, const int t) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_CAP | M_PAW;
	m->move.ms.piece = queen;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) + (QUEEN_VALUE - 200);


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_CAP | M_PAW;
	m->move.ms.piece = rook;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) + (ROOK_VALUE - 200);


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_CAP | M_PAW;
	m->move.ms.piece = bishop;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) + (BISHOP_VALUE - 200);


	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_CAP | M_PAW;
	m->move.ms.piece = knight;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) + (KNIGHT_VALUE - 200);

	}

 void ins_pawn_cap_queen_promo(const int f, const int t) {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = f;
	m->move.ms.to = t;
	m->move.ms.flag = M_PRO | M_CAP | M_PAW;
	m->move.ms.piece = queen;
	m->scorec = (piece_value[Piece(t)] + piece_value2[Piece(t)]) + (QUEEN_VALUE - 200);


	}

 




 void ins_w_oo() {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = E1;
	m->move.ms.to = G1;
	m->move.ms.flag = M_OO;
	m->move.ms.piece = king;
	m->scorec = -1;
	}

 void ins_w_ooo() {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = E1;
	m->move.ms.to = C1;
	m->move.ms.flag = M_OOO;
	m->move.ms.piece = king;
	m->scorec = -1;
	}

 void ins_b_oo() {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = E8;
	m->move.ms.to = G8;
	m->move.ms.flag = M_OO;
	m->move.ms.piece = king;
	m->scorec = -1;
	}

 void ins_b_ooo() {
	MOVEL * m;

	m = &tree.move_list[tree.first_move[Ply + 1]++];
	m->move.ms.from = E8;
	m->move.ms.to = C8;
	m->move.ms.flag = M_OOO;
	m->move.ms.piece = king;
	m->scorec = -1;
	}
