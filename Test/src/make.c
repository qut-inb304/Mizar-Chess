/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*
Aggiorna la scacchiera e le altre strutture dati eseguendo la mossa
La verifica di legalita' viene effettuata quando:
-la mossa è una mossa di re
-il re è sotto scacco
-il re è sotto attacco x-ray
*/
int makemove(const MOVE m, const unsigned char s) {

	unsigned char from=0; /*da dove parte il pezzo*/
	unsigned char to=0; /*dove copiare il pezzo*/
	char was=0; /*cosa era il pezzo?*/
	char wh=0;
	unsigned char del=0; /*dove cancellare*/

	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

	

	if (Piece(m.ms.to)==king) return 0;

	/*copia la chiave hash per l'unmake()*/
	BHash(N_moves) = Hash;

	/*copia la castle_exe per l'unmake()*/
	BCste(N_moves) = Cste;

	/*copia la bitboard per l'unmake()*/
	BBitb(N_moves, Side) = Bitboard(Side);


	/*Controlla che l'arrocco sia legale e sposta la torre
*/
	if (m.ms.flag & (M_OO | M_OOO)) {
			if (tree.check[Side][Ply])
				return 0;

			switch (m.ms.to) {
					case C8:

						if (attacked(C8, WHITE) || attacked(D8, WHITE))
							return 0;
						from = A8;
						to = D8;
						Cste |= cas_booo;
						break;

					case G8:
						if (attacked(F8, WHITE) || attacked(G8, WHITE))
							return 0;
						from = H8;
						to = F8;
						Cste |= cas_boo;
						break;

					case C1:
						if (attacked(C1, BLACK) || attacked(D1, BLACK))
							return 0;
						from = A1;
						to = D1;
						Cste |= cas_wooo;
						break;

					case G1:
						if (attacked(F1, BLACK) || attacked(G1, BLACK))
							return 0;
						from = H1;
						to = F1;
						Cste |= cas_woo;
						break;

					default:
						puts("Bad castle request.");
						assert(0);
					}
			/*sposta torre aggiornando la chiave hash*/
			UpdateHash(from);

			Piece(to) = Piece(from);

			Color(to) = Color(from);

			Plist(to) = Plist(from);

			Position((Color(to)), (Plist(to))) = to;

			Piece(from) = no_piece;

			Color(from) = EMPTY;

			Plist(from) = no_list;

			UpdateHash(to);

			SetBB(Side, to);

			DelBB(Side, from);
			}
	/*sposta il pezzo*/
	from = m.ms.from;

	to = m.ms.to;

	was = Piece(from);

	wh = Plist(from);
	assert(was>=0);
	assert(was<=king);

	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);
	if (wh>0){
		assert(Name(Side,wh)!=king);
	}

	

	/*Se la mossa e' una cattura cancella il pezzo dalla casella di arrivo*/
	if (m.ms.flag & M_CAP) {
			del = to;

			/*se è una cattura en passant aggiustiamo casella*/
			if (m.ms.flag & M_ENP) {
					if (Side)
						del += DOWN;
					else
						del += UP;
			}

			

			BCapP(N_moves) = Piece(del);
			BCapPos(N_moves) = Plist(del);
			BCapC(N_moves) = Xside;
			BBitb(N_moves, Xside) = Bitboard(Xside);
			Active(Xside, (Plist(del))) = CAPTURED;
			Piece(del) = no_piece;
			Color(del) = EMPTY;
			Plist(del) = no_list;
			DelBB(Xside, del);
			}

	Piece(to) = Piece(from);
	if (m.ms.piece!=was)Piece(to)=m.ms.piece;
	Color(to) = Side;
	Plist(to) = wh;
	Position((Side), (wh)) = to;
	Piece(from) = no_piece;
	Color(from) = EMPTY;
	Plist(from) = no_list;
	SetBB(Side, to);
	DelBB(Side, from);
	/*effettuato il minimo indispensabile verifica, se opportuno, la legalità della mossa*/
	
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);
	if (wh>0){
		assert(Name(Side,wh)!=king);
	}
	

	if ( tree.verify[Ply] || tree.check[Side][Ply] || (m.ms.flag & M_KNG) ||(m.ms.flag & M_ENP)) {
			if (in_check(Side)) {
					/*unmake() locale*/
					Piece(from) = Piece(to);
					if (Piece(from)!=was)Piece(from)=was;
					Color(from) = Side;
					Plist(from) = wh;
					Position((Side), (wh)) = from;
					Piece(to) = no_piece;
					Color(to) = EMPTY;
					Plist(to) = no_list;
					Bitboard(Side) = BBitb(N_moves, Side);

					if (m.ms.flag & M_CAP) {
							Piece(del) = BCapP(N_moves);
							Color(del) = BCapC(N_moves);
							Plist(del) = BCapPos(N_moves);
							Active((Xside), (Plist(del))) = FREE;
							Position((Xside), (Plist(del))) = del;
							Bitboard(Xside) = BBitb(N_moves, Xside);
							assert(Name(Side,0)==king);
							assert(Name(Xside,0)==king);
							if (Plist(del)>0){
								assert(Name(Xside,Plist(del))!=king);
								}
							}
					assert(Name(Side,0)==king);
					assert(Name(Xside,0)==king);
					if (wh>0){
						assert(Name(Side,wh)!=king);
						}
					return 0;
					}

			}
	/*Selectivity*/
	if (s && ((!in_check(Xside)) || ((m.ms.flag& M_CAP)==0))) {
		/*unmake() locale*/
					Piece(from) = Piece(to);
					if (Piece(from)!=was)Piece(from)=was;
					Color(from) = Side;
					Plist(from) = wh;
					Position((Side), (wh)) = from;
					Piece(to) = no_piece;
					Color(to) = EMPTY;
					Plist(to) = no_list;
					Bitboard(Side) = BBitb(N_moves, Side);

					if (m.ms.flag & M_CAP) {
							Piece(del) = BCapP(N_moves);
							Color(del) = BCapC(N_moves);
							Plist(del) = BCapPos(N_moves);
							Active((Xside), (Plist(del))) = FREE;
							Position((Xside), (Plist(del))) = del;
							Bitboard(Xside) = BBitb(N_moves, Xside);
							assert(Name(Side,0)==king);
							assert(Name(Xside,0)==king);
							if (Plist(del)>0){
								assert(Name(Xside,Plist(del))!=king);
								}
							}
					assert(Name(Side,0)==king);
					assert(Name(Xside,0)==king);
					if (wh>0){
						assert(Name(Side,wh)!=king);
						}
					return 0;
		}
	/*back up per l'unmakemove()
	i dati per la cattura e la hash della posizione sono gia' memorizzati*/
	tree.history[N_moves].m.mi = m.mi;
	tree.history[N_moves].cast.castle_right = Cstr;
	tree.history[N_moves].enp = Enp;
	tree.history[N_moves].fifty = tree.fifty;
	tree.history[N_moves].was = was;
	tree.history[N_moves].material[Side] = Material(Side);
	tree.history[N_moves].material[Xside] = Material(Xside);
	BPBitb(N_moves, Side) = Pbitboard(Side);
	BPBitb(N_moves, Xside) = Pbitboard(Xside);

	/*aggiorna diritti arrocco*/
	UpdateHashCastle(Cstr);
	Cstr &= castle_mask[b256to64[from]] & castle_mask[b256to64[to]];
	UpdateHashCastle(Cstr);

	Hash ^= zobrist_enp[b256to64[Enp]];
	Enp = 0;
	tree.fifty++;
	/*se irreversibile...*/

	if (m.ms.flag&(M_PAW | M_CAP)) {
			tree.fifty = 0;
			

			if (m.ms.flag&M_DBP) {
					if (Side)
						Enp = to + DOWN;
					else
						Enp = to + UP;
					}

			if (m.ms.flag&M_PAW) {
					DelPBB(Side, from);

					if ((m.ms.flag&M_PRO) == 0)
						SetPBB(Side, to);
					}

			if (m.ms.flag&M_CAP) {
					tree.history[N_moves].del = del;
					Hash ^= zobrist[Xside][BCapP(N_moves)][b256to64[del]];
					Material(Xside) -= piece_value[BCapP(N_moves)];
					Num_piece(Xside, BCapP(N_moves))--;
					Pbitboard(Xside) &= (~(bit_square[del]));
					}

			Material(Side) -= piece_value[was];
			Material(Side) += piece_value[m.ms.piece];
			Num_piece(Side, was)--;
			Num_piece(Side, m.ms.piece)++;
			}

	Hash ^= zobrist_enp[b256to64[Enp]];

	Hash ^= zobrist[Side][was][b256to64[from]];
	Hash ^= zobrist[Side][m.ms.piece][b256to64[to]];
	Hash ^= zobrist_side;



	Ply++;
	N_moves++;

	Change(Side);
	Change(Xside);

	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

	return 1;

	}
/*
Riporta la scacchiera e le altre strutture dati alla situazione precedente
*/
void unmakemove() {
	MOVE m;
	unsigned char from, to, del;
	char was;
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);
	Ply--;
	N_moves--;
	Change(Side);
	Change(Xside);

	Hash = BHash(N_moves);
	Cste = BCste(N_moves);
	Cstr = tree.history[N_moves].cast.castle_right;
	Enp = tree.history[N_moves].enp;
	tree.fifty = tree.history[N_moves].fifty;
	Material(Side) = tree.history[N_moves].material[Side];
	Material(Xside) = tree.history[N_moves].material[Xside];
	m.mi = tree.history[N_moves].m.mi;
	was = tree.history[N_moves].was;
	from = m.ms.from;
	to = m.ms.to;

	Num_piece(Side, was)++;
	Num_piece(Side, Piece(to))--;

	Piece(from) = Piece(to);
	if (Piece(from)!=was)Piece(from)=was;
	Color(from) = Color(to);
	Plist(from) = Plist(to);
	Position((Color(from)), (Plist(from))) = from;
	Piece(to) = no_piece;
	Color(to) = EMPTY;
	Plist(to) = no_list;
	Bitboard(Side) = BBitb(N_moves, Side);
	Pbitboard(Side) = BPBitb(N_moves, Side);
	Pbitboard(Xside) = BPBitb(N_moves, Xside);
	
	if (m.ms.flag & M_CAP) {
			del = tree.history[N_moves].del;
			Piece(del) = BCapP(N_moves);
			Color(del) = BCapC(N_moves);
			Plist(del) = BCapPos(N_moves);
			Active((Color(del)), (Plist(del))) = FREE;
			Position((Color(del)), (Plist(del))) = del;
			Num_piece(Xside, Piece(del))++;
			Bitboard(Xside) = BBitb(N_moves, Xside);
			assert(Name(Side,0)==king);
			assert(Name(Xside,0)==king);
	}
	assert(Name(Side,0)==king);
	assert(Name(Xside,0)==king);


	if (m.ms.flag & (M_OO | M_OOO)) {
			switch (m.ms.to) {
					case C8:
						from = D8;
						to = A8;
						break;
					case G8:
						from = F8;
						to = H8;
						break;
					case C1:
						from = D1;
						to = A1;
						break;
					case G1:
						from = F1;
						to = H1;
						break;
					default:
						puts("Bad castle request.");
						assert(0);
					}

			Piece(to) = rook;
			Color(to) = Side;
			Plist(to) = Plist(from);
			Position((Color(to)), (Plist(to))) = to;
			Piece(from) = no_piece;
			Color(from) = EMPTY;
			Plist(from) = no_list;
			}
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);
	}
