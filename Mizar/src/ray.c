/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

static int bishop_mob[64];
static int knight_mob[64];


static const int knight_pmob[64] = {
                                       2, 3, 4, 4, 4, 4, 3, 2,
                                       3, 4, 6, 6, 6, 6, 4, 3,
                                       4, 6, 8, 8, 8, 8, 6, 4,
                                       4, 6, 8, 8, 8, 8, 6, 4,
                                       4, 6, 8, 8, 8, 8, 6, 4,
                                       4, 6, 8, 8, 8, 8, 6, 4,
                                       3, 4, 6, 6, 6, 6, 4, 3,
                                       2, 3, 4, 4, 4, 4, 3, 2,
                                   };

static const int bishop_pmob[64] = {
                                       7, 7, 7, 7, 7, 7, 7, 7,
                                       7, 9, 9, 9, 9, 9, 9, 7,
                                       7, 9, 11, 11, 11, 11, 9, 7,
                                       7, 9, 11, 13, 13, 11, 9, 7,
                                       7, 9, 11, 13, 13, 11, 9, 7,
                                       7, 9, 11, 11, 11, 11, 9, 7,
                                       7, 9, 9, 9, 9, 9, 9, 7,
                                       7, 7, 7, 7, 7, 7, 7, 7
                                   };

/*Inizializza dati e bitboard*/
void init_bitboard() {
	unsigned __int64 i;
	int c, a, from, to, dir, p;
	unsigned __int8 t, z;
	const char temp_board[256] = {
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	                             };

	if (mizar.o.verbose) printf("Init Data and Bitboard stuff...");

	/*passo A: bit count*/
	for (p = 0;p < 256;p++) {
			t = 0;
			z = p;

			while (z) {
					if (z & ((unsigned __int8)1))
						t++;

					z >>= 1;
					}

			b2n[p] = t;
			}

	/*dati per la funzione di valutazione*/
	for (c = 0;c < 64;c++) {
			knight_mob[c] = (int)ceil(((double)knight_pmob[c] / (double)100) * (double)BAD_N);
			bishop_mob[c] = (int)ceil(((double)bishop_pmob[c] / (double)100) * (double)BAD_B);
			}

	/*Passo B: bitboard
	passo 1:caselle*/
	i = 1;

	for (c = 0;c < 256;c++)
		bit_square[c] = 0;

	for (c = 63;c >= 0;c--) {
			bit_square[b64to256[c]] = i;
			i <<= 1;
			}

	/*passo 2: i raggi tra caselle*/
	for (c = 0;c < 256;c++) {
			for (a = 0;a < 256;a++) {
					bit_ray[c][a] = 0;
					}

			}

	for (c = 0;c < 64;c++) {
			for (a = 0;a < 64;a++) {
					if (a == c)
						continue;

					from = b64to256[c];

					to = b64to256[a];

					if (bitq&table_attack[to - from + 128]) {
							dir = direction[to - from + 128];

							for (;;) {
									from += dir;

									if (from == to)
										break;

									bit_ray[b64to256[c]][to] |= bit_square[from];
									}

							}

					}

			}

	/*passo 3: raggi "radar"*/

	for (c = 0;c < 256;c++) {
			for (a = 0;a < 8;a++) {
					bit_scan[c][a] = 0;
					}

			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			to = b64to256[c];

			while (temp_board[to += UP])
				bit_scan[a][BUP] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += DOWN])
				bit_scan[a][BDOWN] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += RIGHT])
				bit_scan[a][BRIGHT] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += LEFT])
				bit_scan[a][BLEFT] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += UP_R])
				bit_scan[a][BUP_R] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += UP_L])
				bit_scan[a][BUP_L] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += DOWN_R])
				bit_scan[a][BDOWN_R] |= bit_square[to];

			to = b64to256[c];

			while (temp_board[to += DOWN_L])
				bit_scan[a][BDOWN_L] |= bit_square[to];
			}

	/*passo 4: isolated*/
	for (c = 0;c < 256;c++) {
			bit_isolated[c] = 0;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			i = 0;

			if (temp_board[a - 1]) {
					i |= bit_square[a - 1];
					i |= bit_scan[a - 1][BDOWN];
					i |= bit_scan[a - 1][BUP];
					}

			if (temp_board[a + 1]) {
					i |= bit_square[a + 1];
					i |= bit_scan[a + 1][BDOWN];
					i |= bit_scan[a + 1][BUP];
					}

			bit_isolated[a] = i;
			}

	/*passo 5:backward*/
	for (c = 0;c < 256;c++) {
			bit_backward[0][c] = 0;
			bit_backward[1][c] = 0;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];

			if (Rank(a) == Riga1)
				continue;

			i = 0;

			if (Rank(a) == Riga2) {
					if (temp_board[a - 1])
						i |= bit_square[a - 1];

					if (temp_board[a + 1])
						i |= bit_square[a + 1];

					bit_backward[WHITE][a] = i;

					continue;
					}

			if (temp_board[a - 1]) {
					i |= bit_square[a - 1];
					i |= bit_scan[a - 1][BDOWN];
					}

			if (temp_board[a + 1]) {
					i |= bit_square[a + 1];
					i |= bit_scan[a + 1][BDOWN];
					}

			bit_backward[WHITE][a] = i;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];

			if (Rank(a) == Riga8)
				continue;

			i = 0;

			if (Rank(a) == Riga7) {
					if (temp_board[a - 1])
						i |= bit_square[a - 1];

					if (temp_board[a + 1])
						i |= bit_square[a + 1];

					bit_backward[BLACK][a] = i;

					continue;
					}

			if (temp_board[a - 1]) {
					i |= bit_square[a - 1];
					i |= bit_scan[a - 1][BUP];
					}

			if (temp_board[a + 1]) {
					i |= bit_square[a + 1];
					i |= bit_scan[a + 1][BUP];
					}

			bit_backward[BLACK][a] = i;
			}

	/*passo 6:passed*/
	for (c = 0;c < 256;c++) {
			bit_passed[0][c] = 0;
			bit_passed[1][c] = 0;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];

			if (Rank(a) == Riga1)
				continue;

			i = bit_scan[a][BUP];

			if (temp_board[a - 1]) {
					i |= bit_scan[a - 1][BUP];
					}

			if (temp_board[a + 1]) {
					i |= bit_scan[a + 1][BUP];
					}

			bit_passed[WHITE][a] = i;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];

			if (Rank(a) == Riga8)
				continue;

			i = bit_scan[a][BDOWN];

			if (temp_board[a - 1]) {
					i |= bit_square[a - 1];
					i |= bit_scan[a - 1][BDOWN];
					}

			if (temp_board[a + 1]) {
					i |= bit_square[a + 1];
					i |= bit_scan[a + 1][BDOWN];
					}

			bit_passed[BLACK][a] = i;
			}

	/*passo 7: fila*/
	for (c = 0;c < 256;c++) {
			bit_file[c] = 0;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			i = 0;

			if (temp_board[a]) {
					i |= bit_square[a];
					i |= bit_scan[a][BDOWN];
					i |= bit_scan[a][BUP];
					}

			bit_file[a] = i;
			}

	/*passo 8: riga*/
	for (c = 0;c < 256;c++) {
			bit_rank[c] = 0;
			}

	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			i = 0;

			if (temp_board[a]) {
					i |= bit_square[a];
					i |= bit_scan[a][BLEFT];
					i |= bit_scan[a][BRIGHT];
					}

			bit_rank[a] = i;
			}

	/*passo 9: case deboli*/
	for (c = 0;c < 256;c++) {
			bit_weaksquare[0][c] = 0;
			bit_weaksquare[1][c] = 0;
			}
	/*Bianco: contiene tutte le posizioni in cui dovrebbero trovarsi i pedoni bianchi
	che dovrebbero difendere la casa in questione; limitatamente alla propria metà campo*/
	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			i = 0;

			if (Rank(a) == Riga3) {
					if (File(a) != FilaH) i |= bit_square[a + DOWN_R];

					if (File(a) != FilaA) i |= bit_square[a + DOWN_L];

					bit_weaksquare[WHITE][a] = i;
					}

			else if (Rank(a) == Riga4) {
					if (File(a) != FilaH) {
							i |= bit_square[a + DOWN_R];
							i |= bit_square[a + DOWN_R + DOWN];
							}

					if (File(a) != FilaA) {
							i |= bit_square[a + DOWN_L];
							i |= bit_square[a + DOWN_L + DOWN];
							}

					bit_weaksquare[WHITE][a] = i;
					}

			}
	/*Nero: contiene tutte le posizioni in cui dovrebbero trovarsi i pedoni neri
	che dovrebbero difendere la casa in questione; limitatamente alla propria metà campo*/

	for (c = 0;c < 64;c++) {
			a = b64to256[c];
			i = 0;

			if (Rank(a) == Riga6) {
					if (File(a) != FilaH) i |= bit_square[a + UP_R];

					if (File(a) != FilaA) i |= bit_square[a + UP_L];

					bit_weaksquare[BLACK][a] = i;
					}

			else if (Rank(a) == Riga5) {
					if (File(a) != FilaH) {
							i |= bit_square[a + UP_R];
							i |= bit_square[a + UP_R + UP];
							}

					if (File(a) != FilaA) {
							i |= bit_square[a + UP_L];
							i |= bit_square[a + UP_L + UP];
							}

					bit_weaksquare[BLACK][a] = i;
					}

			}


	if (mizar.o.verbose) printf("done\n");
	}

void set_bitboard_position() {
	int c;

	Bitboard(BLACK) = 0;
	Bitboard(WHITE) = 0;
	Pbitboard(BLACK) = 0;
	Pbitboard(WHITE) = 0;

	for (c = 0; c < 16; c++) {
			if (Active(BLACK, c)) {
					SetBB(BLACK, Position(BLACK, c));

					if (Name(BLACK, c) == bpawn)
						SetPBB(BLACK, Position(BLACK, c));
					}

			if (Active(WHITE, c)) {
					SetBB(WHITE, Position(WHITE, c));

					if (Name(WHITE, c) == wpawn)
						SetPBB(WHITE, Position(WHITE, c));
					}

			}
	}
