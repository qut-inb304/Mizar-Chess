/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*attacked() returns 1 if square 'target' is attacked by color 'color'
  and 0 otherwise*/
int attacked(const int target, const int color) {
	int c;
	unsigned __int64 all;
	all = Bitboard(BLACK) | Bitboard(WHITE);

	for (c = 0;c < 16;c++) {
			if (Active(color, c)) {
				assert(Name(color,0)==king);
				if (c>0) assert(Name(color,c)!=king);					
					
					if (Kind(color, c)&table_attack[target - Position(color, c) + 128]) {
							if (IsNoSlide(color, c))
								return 1;

							if ((all&(bit_ray[Position(color, c)][target])) == 0)
								return 1;
							}

					}

			}

	switch (color) {
			case BLACK:

				if (File(target) != FilaA && IsBPawn(target + UP_L))
					return 1;

				if (File(target) != FilaH && IsBPawn(target + UP_R))
					return 1;

				break;

			case WHITE:
				if (File(target) != FilaA && IsWPawn(target + DOWN_L))
					return 1;

				if (File(target) != FilaH && IsWPawn(target + DOWN_R))
					return 1;

				break;

			case EMPTY:
				printf("Bad color board detect in attacked(): %d",color);
				assert(0);
				break;

			default:
				printf("Bad color board detect in attacked(): %d",color);
				assert(0);

				break;
			}

	return 0;
	}

/*
check_condition() sets tree.check[Side][Ply] if king of side to move is in check
and it sets tree.verify[Ply] if it is in check or under 'x-ray' attack
*/
void check_condition() {
	int c, from, dir, target, tmp_incheck, tmp_xattack, num_pieces, prob_xattack;
	unsigned __int64 all;
	tmp_incheck = 0;
	tmp_xattack = 0;
	target = Position(Side, 0);
	all = Bitboard(BLACK) | Bitboard(WHITE);
	assert(Name(Side,0)==king);
	
	for (c = 0;c < 16;c++) {
			if (Active(Xside, c)) {
				assert(Name(Xside,0)==king);
				if (c>0) assert(Name(Xside,c)!=king);

				/*controlla se il pezzo puo' attaccare*/

					if (Kind(Xside, c)&table_attack[target - Position(Xside, c) + 128]) {
							/*se e' un pezzo no-slide è sotto scacco*/

							if (IsNoSlide(Xside, c)) {
									tmp_incheck++;
									break;
									}
							/*se e' un pezzo slide dobbiamo controllare*/
							if ((all&(bit_ray[Position(Xside, c)][target])) == 0) {
									tmp_incheck++;
									break;
									}

							from = Position(Xside, c);
							dir = direction[target - from + 128];
							num_pieces = 0;
							prob_xattack = 0;
							from += dir;

							while (from != target) {
									if (Piece(from) != no_piece) {
											num_pieces++;

											if (num_pieces == 1 && Color(from) == Side)
												prob_xattack = 1;

											if (num_pieces > 1)
												break;
											}

									from += dir;
									}

							if (num_pieces == 1 && prob_xattack == 1)
								tmp_xattack++;
							}

					}

			}

	switch (Xside) {
			case BLACK:

				if (File(target) != FilaA && IsBPawn(target + UP_L)) {
						tmp_incheck++;
						break;
						}

				if (File(target) != FilaH && IsBPawn(target + UP_R))
					tmp_incheck++;

				break;

			case WHITE:
				if (File(target) != FilaA && IsWPawn(target + DOWN_L)) {
						tmp_incheck++;
						break;
						}

				if (File(target) != FilaH && IsWPawn(target + DOWN_R))
					tmp_incheck++;

				break;

			case EMPTY:
				printf("Bad color board detect in checkcondition(): %d",Xside);
				assert(0);
				break;

			default:
				printf("Bad color board detect in checkcondition(): %d",Xside);
				assert(0);

				break;
			}
	/*E' sotto scacco?*/
	tree.check[Side][Ply] = tmp_incheck;

	/*Nella makemove() dobbiamo verificare la legalita'?*/
	tree.verify[Ply] = tmp_incheck + tmp_xattack;
	}

/*in_check() returns 1 if king of color 'color' is in check, 0 otherwise*/
int in_check(const int color) {
	assert(Name(color,0)==king);
	assert(Name((color ^ 1),0)==king);
	return attacked(Position(color, 0), (color ^ 1));
	}

/*count_attack() returns number of attacks on square 'target' by color 'color'*/
int count_attack(const int target, const int color) {
	TATT temp;
	int num;

	temp.ci = ATable(color, target);
	num = b2n[temp.cb.h];
	num += b2n[temp.cb.l];
	return num;
	}

/*It sets attacked_by_hi[] and attacked_by_low[]*/
void set_attacked_by_array(void) {
	int color,xcolor;
	int c,d;

	for (c=0;c<16;c++) {
		attacked_by_low[0][c] = 0;
		attacked_by_low[1][c] = 0;
		attacked_by_hi[0][c] = 0;
		attacked_by_hi[1][c] = 0;
		attacked_by_eq[0][c] = 0;
		attacked_by_eq[1][c] = 0;	
		}

	/*White*/
	color=WHITE;
	xcolor=BLACK;
	for (c=0;c<16;c++) {
		if (Active(color,c)) {
			for (d=0;d<16;d++) {
				if (Active(xcolor,d)) {
					if (piece_value[Name(xcolor,d)]<piece_value[Name(color,c)]) {
						attacked_by_low[color][c] |= i2b[d];
						}
					if (piece_value[Name(xcolor,d)]>piece_value[Name(color,c)]) {
						attacked_by_hi[color][c] |= i2b[d];
						}
					if (piece_value[Name(xcolor,d)]==piece_value[Name(color,c)]) {
						attacked_by_eq[color][c] |= i2b[d];
						}
					}
				}
			}
		}

	/*Black*/
	color=BLACK;
	xcolor=WHITE;
	for (c=0;c<16;c++) {
		if (Active(color,c)) {
			for (d=0;d<16;d++) {
				if (Active(xcolor,d)) {
					if (piece_value[Name(xcolor,d)]<piece_value[Name(color,c)]) {
						attacked_by_low[color][c] |= i2b[d];
						}
					if (piece_value[Name(xcolor,d)]>piece_value[Name(color,c)]) {
						attacked_by_hi[color][c] |= i2b[d];
						}
					if (piece_value[Name(xcolor,d)]==piece_value[Name(color,c)]) {
						attacked_by_eq[color][c] |= i2b[d];
						}
					}
				}
			}
		}

	/*Pawns*/
	pw_attack_pattern[0]=0;
	pw_attack_pattern[1]=0;

	for (c=0;c<16;c++) {
		if (Active(WHITE,c)) {
			if (Name(WHITE,c)==wpawn)
				pw_attack_pattern[WHITE] |= i2b[c];
			}
		}

	for (c=0;c<16;c++) {
		if (Active(BLACK,c)) {
			if (Name(BLACK,c)==bpawn)
				pw_attack_pattern[BLACK] |= i2b[c];
			}
		}

	
}
