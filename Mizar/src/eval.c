/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*#define DEBUG_EVAL*/

#define TotNumPiece(x) (Num_piece((x),knight)+Num_piece((x),bishop)+Num_piece((x),rook)+Num_piece((x),queen)+1)


int piece_value[8];
/*nxp[y]:= numero di pedoni di colore x su casella di colore y*/
static int nwp[2], nbp[2], maj[2], psh[2][8];
static unsigned int double_bishop[2];
static int center_control[2];
static const int w_adv[64] = {
	3, 3, 4, 3, 3, 4, 3, 3,
	2, 3, 3, 3, 3, 3, 3, 2,
	2, 2, 3, 3, 3, 3, 2, 2,
	1, 2, 2, 4, 4, 2, 2, 1,
	1, 1, 2, 4, 4, 2, 1, 1,
	0, 1, 1, 2, 2, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
	};

static const int b_adv[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 2, 2, 1, 1, 0,
	1, 1, 2, 4, 4, 2, 1, 1,
	1, 2, 2, 4, 4, 2, 2, 1,
	2, 2, 3, 3, 3, 3, 2, 2,
	2, 3, 3, 3, 3, 3, 3, 2,
	3, 3, 4, 3, 3, 4, 3, 3
	};

static const int wking_dvlp[64] = {
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 3, 3, 3, 3, 3, 3, 4,
	2, 1, 1, 1, 1, 1, 1, 2,
	2, 0, 0, 0, 1, 0, 0, 2
	};

static const int bking_dvlp[64] = {
	2, 0, 0, 0, 1, 0, 0, 2,
	2, 1, 1, 1, 1, 1, 1, 2,
	4, 3, 3, 3, 3, 3, 3, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4
	};

static const int king_endgame[64] = {
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 4, 3, 2, 2, 3, 4, 5,
	4, 3, 2, 1, 1, 2, 3, 4,
	4, 2, 1, 0, 0, 1, 2, 4,
	4, 2, 1, 0, 0, 1, 2, 4,
	4, 3, 2, 1, 1, 2, 3, 4,
	5, 4, 3, 2, 2, 3, 4, 5,
	5, 5, 5, 5, 5, 5, 5, 5
	};
/*wking_pawn_shield[Side (w) pawn rank][Xside (b) pawn rank]
no white pawn = Riga8 (4)->0
no black pawn = Riga1 (11)->8
*/
static const int wking_pawn_shield[8][8] = {
/*black pawn on rank
	 8 7 6 5 4 3 2 N*/
	{0,2,3,4,4,4,4,5},/*no white pawn*/
	{0,2,2,2,3,3,3,4},/*white pawn on Riga7*/
	{0,2,2,2,3,3,3,4},/*white pawn on Riga6*/
	{0,2,2,2,3,3,3,4},/*white pawn on Riga5*/
	{0,2,2,2,3,3,3,4},/*white pawn on Riga4*/
	{0,1,1,2,2,2,2,3},/*white pawn on Riga3*/
	{0,0,0,1,1,1,1,1},/*white pawn on Riga2*/
	{0,0,0,0,0,0,0,0} /*white pawn on Riga1*/	
	};
/*bking_pawn_shield[Side (b) pawn rank][Xside (w) pawn rank]
no white pawn = Riga8 (4)->0
no black pawn = Riga1 (11)->8
*/
static const int bking_pawn_shield[8][8] = {
	/*white pawn on rank
	 N 7 6 5 4 3 2 1*/
	{0,0,0,0,0,0,0,0},/*black pawn on Riga8*/
	{1,1,1,1,1,0,0,0},/*black pawn on Riga7*/
	{3,2,2,2,2,1,1,0},/*black pawn on Riga6*/
	{4,3,3,3,2,2,2,0},/*black pawn on Riga5*/
	{4,3,3,3,2,2,2,0},/*black pawn on Riga4*/
	{4,3,3,3,2,2,2,0},/*black pawn on Riga3*/
	{4,3,3,3,2,2,2,0},/*black pawn on Riga2*/
	{5,4,4,4,4,3,2,0} /*no black pawn*/	
	};

static const int danger[32] = {
	0, 1, 2, 4, 15, 30, 60, 70,
	80, 90, 100, 125, 150, 175, 200, 225,
	250, 275, 300, 325, 350, 375, 400, 425,
	450, 475, 500, 525, 550, 575, 600, 600
	};

static const int centr[64] = {
	-5,-4,-4,-4,-4,-4,-4,-5,
	-4,-4, 0, 0, 0, 0,-4,-4,
	-4, 0, 3, 3, 3, 3, 0,-4,
	-4, 0, 3, 4, 4, 3, 0,-4,
	-4, 0, 3, 4, 4, 3, 0,-4,
	-4, 0, 3, 3, 3, 3, 0,-4,
	-4,-4, 0, 0, 0, 0,-4,-4,
	-5,-4,-4,-4,-4,-4,-4,-5
	};

static const int qcentr[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 2, 2, 2, 2, 1, 0,
	0, 1, 2, 2, 2, 2, 1, 0,
	0, 1, 2, 2, 2, 2, 1, 0,
	0, 1, 2, 2, 2, 2, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0
	};

static const int zone[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
	};

static int knight_mob[64];
static int bishop_mob[64];


static const int of_w[16] = {0, 0, 0, 0, 5, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0};
static const int of_b[16] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 5, 0, 0, 0, 0};
static const int wksq[9] = {8, 0, 1, 1, 2, 2, 3, 3, 4};

/*Inizializza le strutture dati per la valutazione*/
void set_board_eval() {
	int c;

	Material(BLACK) = 0;
	Material(WHITE) = 0;

	for (c = 0; c < 8; c++) {
			Num_piece(BLACK, c) = 0;
			Num_piece(WHITE, c) = 0;
			}

	for (c = 0;c < 16;c++) {
			if (Active(BLACK, c)) {
					Material(BLACK) += piece_value[Name(BLACK, c)];
					Num_piece(BLACK, Name(BLACK, c))++;
					}

			if (Active(WHITE, c)) {
					Material(WHITE) += piece_value[Name(WHITE, c)];
					Num_piece(WHITE, Name(WHITE, c))++;
					}

			}
	}

/*Valuta i fattori statici di una posizione*/
int eval(const char fast) {
	int score[2], c, wsq[2], ks[2], qs[2];
	size_t hash_index;

#ifdef DEBUG_EVAL
	int tmp[2], tnd[2], db_bish[2], heur[2], ps[2], wsqarec[2];
#endif /*DEBUG_EVAL*/
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

	Hang(WHITE) = -1;
	Hang(BLACK) = -1;

	if (mizar.o.use_ev_cache && fast) {
		hash_index = (size_t)(eval_hash_mask & Hash);
		if ((eval_hash[hash_index].hash ) == (Hash>>32))
			return eval_hash[hash_index].score;
	}

	gen_knowledge();

	Hang(WHITE) = 0;
	Hang(BLACK) = 0;

	qs[WHITE] = 0;
	qs[BLACK] = 0;

	psh[WHITE][0]=Riga8;
	psh[WHITE][1]=Riga8;
	psh[WHITE][2]=Riga8;
	psh[WHITE][3]=Riga8;
	psh[WHITE][4]=Riga8;
	psh[WHITE][5]=Riga8;
	psh[WHITE][6]=Riga8;
	psh[WHITE][7]=Riga8;

	psh[BLACK][0]=Riga1;
	psh[BLACK][1]=Riga1;
	psh[BLACK][2]=Riga1;
	psh[BLACK][3]=Riga1;
	psh[BLACK][4]=Riga1;
	psh[BLACK][5]=Riga1;
	psh[BLACK][6]=Riga1;
	psh[BLACK][7]=Riga1;

	/*-------------------*/
	/*Materiale          */
	/*-------------------*/

	score[WHITE] = Material(WHITE);
	score[BLACK] = Material(BLACK);

#ifdef DEBUG_EVAL
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];
	printf("------[ Material ]------\n");
	printf("Material W: %d \n", tmp[WHITE]);
	printf("Material B: %d \n", tmp[BLACK]);
#endif /*DEBUG_EVAL*/

	/*tandem*/
	if (TotNumPiece(WHITE) == 3 && TotNumPiece(BLACK) == 3) {
			do {
					/*tandem: torre e alfiere meglio di torre e cavallo*/
					if (Num_piece(WHITE, rook) == 1 && Num_piece(WHITE, bishop) == 1 &&
					        Num_piece(BLACK, rook) == 1 && Num_piece(BLACK, knight) == 1) {
							score[WHITE] += TND_RB;
							break;
							}

					if (Num_piece(BLACK, rook) == 1 && Num_piece(BLACK, bishop) == 1 &&
					        Num_piece(WHITE, rook) == 1 && Num_piece(WHITE, knight) == 1) {
							score[BLACK] += TND_RB;
							break;
							}
					/*tandem: regina e cavallo meglio di regina e alfiere*/
					if (Num_piece(WHITE, queen) == 1 && Num_piece(WHITE, knight) == 1 &&
					        Num_piece(BLACK, queen) == 1 && Num_piece(BLACK, bishop) == 1) {
							score[WHITE] += TND_QN;
							break;
							}

					if (Num_piece(BLACK, queen) == 1 && Num_piece(BLACK, knight) == 1 &&
					        Num_piece(WHITE, queen) == 1 && Num_piece(WHITE, bishop) == 1) {
							score[BLACK] += TND_QN;
							break;
							}

					}

			while (0);

#ifdef DEBUG_EVAL
			tnd[WHITE] = score[WHITE] - tmp[WHITE];
			tnd[BLACK] = score[BLACK] - tmp[BLACK];
			printf("Tandem W: %d \n", tnd[WHITE]);
			printf("Tandem B: %d \n", tnd[BLACK]);
			tmp[WHITE] = score[WHITE];
			tmp[BLACK] = score[BLACK];
#endif /*DEBUG_EVAL*/

			}


	/*Coppia di alfieri e gruppi di caselle deboli di un colore*/
	wsq[WHITE] = -1;
	wsq[BLACK] = -1;

	switch (double_bishop[WHITE]) {
			case 0: /*no alfiere*/
				if (double_bishop[BLACK] == 3) score[BLACK] += DOUBLE_BISHOP;
				else if (double_bishop[BLACK] == 1) wsq[WHITE] = BLACK;
				else if (double_bishop[BLACK] == 2) wsq[WHITE] = WHITE;
				break;
			case 1: /*alfiere su bianco*/
				if (double_bishop[BLACK] == 3) {
						score[BLACK] += DOUBLE_BISHOP;
						wsq[WHITE] = BLACK;
						}
				else if (double_bishop[BLACK] == 2){
					wsq[WHITE] = BLACK;
					wsq[BLACK] = WHITE;
					}
				else if (double_bishop[BLACK] == 0) wsq[BLACK] = WHITE;					
				break;
			case 2: /*alfiere su nero*/
				if (double_bishop[BLACK] == 3) {
						score[BLACK] += DOUBLE_BISHOP;
						wsq[WHITE] = WHITE;
						}
				else if (double_bishop[BLACK] == 1){
					wsq[WHITE] = WHITE;
					wsq[BLACK] = BLACK;
					}
				else if (double_bishop[BLACK] == 0) wsq[BLACK] = BLACK;
				break;
			case 3: /*doppio alfiere*/
				score[WHITE] += DOUBLE_BISHOP;
				if (double_bishop[BLACK] == 3) score[BLACK] += DOUBLE_BISHOP;
				else if (double_bishop[BLACK] == 2) wsq[BLACK] = WHITE;
				else if (double_bishop[BLACK] == 1) wsq[BLACK] = BLACK;
				break;
			default:
				puts("Unexpected double_bishop[WHITE] contents.");
				assert(0);
			}

#ifdef DEBUG_EVAL
	db_bish[WHITE] = score[WHITE] - tmp[WHITE];
	db_bish[BLACK] = score[BLACK] - tmp[BLACK];
	printf("Double Bishop W: %d \n", db_bish[WHITE]);
	printf("Double Bishop B: %d \n", db_bish[BLACK]);
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];
#endif /*DEBUG_EVAL*/

	if (wsq[WHITE] != -1) {
			if (wsq[WHITE] == WHITE) {
				/*se ci sono troppi pedoni sul nero penalizzo*/
					if (nwp[BLACK] > wksq[Num_piece(WHITE, wpawn)]) score[WHITE] -= WEAK_SQUARES;
					}
			else {
					if (nwp[WHITE] > wksq[Num_piece(WHITE, wpawn)]) score[WHITE] -= WEAK_SQUARES;
					}
			}

	if (wsq[BLACK] != -1) {
			if (wsq[BLACK] == WHITE) {
					if (nbp[BLACK] > wksq[Num_piece(BLACK, bpawn)]) score[BLACK] -= WEAK_SQUARES;
					}
			else {
					if (nbp[WHITE] > wksq[Num_piece(BLACK, bpawn)]) score[BLACK] -= WEAK_SQUARES;
					}
			}

#ifdef DEBUG_EVAL
	wsqarec[WHITE] = score[WHITE] - tmp[WHITE];
	wsqarec[BLACK] = score[BLACK] - tmp[BLACK];
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];
	printf("Total Material W: %d \n", score[WHITE]);
	printf("Total Material B: %d \n", score[BLACK]);
#endif /*DEBUG_EVAL*/

	/*-------------------*/
	/*Euristiche         */
	/*-------------------*/

	/*
	NOTA BENE:
	Penso che una funzione di valutazione debba prendere in considerazione
	solo fattori statici (che non dipendono dalla fase della partita);
	lo sviluppo in apertura e' un fattore dinamico e dovrebbe essere demandato
	alla funzione di ricerca.
	*/

	/*BIANCO*/
	/*Apertura*/
	/*penalizza se non arroccato*/
	if ((Cste & (cas_woo | cas_wooo)) == 0) {
		score[WHITE] -= CASTLING_BONUS;
		/*penalizza se ha perso il diritto di arroccare senza averlo effettuato*/
		if ((Cstr & (cas_woo | cas_wooo)) == 0) {
			score[WHITE] -= LOSE_CASTLING_RIGHT;
			}
		/*penalizza se muove la regina troppo presto*/
		if (Piece(D1) != queen && Color(D1) != WHITE) score[WHITE] -= 110;
		/*Sviluppa alfieri e cavalli*/
		if (Piece(B1) == knight && Color(B1) == WHITE) score[WHITE] -= 20;
		if (Piece(G1) == knight && Color(G1) == WHITE) score[WHITE] -= 20;
		if (Piece(C1) == bishop && Color(C1) == WHITE) score[WHITE] -= 10;
		if (Piece(F1) == bishop && Color(F1) == WHITE) score[WHITE] -= 10;
		}
	
	

	/*NERO*/
	/*Apertura*/
	/*penalizza se non arroccato*/
	if ((Cste & (cas_boo | cas_booo)) == 0) {
		score[BLACK] -= CASTLING_BONUS;
		/*penalizza se ha perso il diritto di arroccare senza averlo effettuato*/
		if ((Cstr & (cas_boo | cas_booo)) == 0) {
			score[BLACK] -= LOSE_CASTLING_RIGHT;
			}
		/*penalizza se muove la regina*/
		if (Piece(D8) != queen && Color(D1) != BLACK) score[BLACK] -= 110;
		/*Sviluppa alfieri e cavalli*/
		if (Piece(B8) == knight && Color(B8) == BLACK) score[BLACK] -= 20;
		if (Piece(G8) == knight && Color(G8) == BLACK) score[BLACK] -= 20;
		if (Piece(C8) == bishop && Color(C8) == BLACK) score[BLACK] -= 10;
		if (Piece(F8) == bishop && Color(F8) == BLACK) score[BLACK] -= 10;
		}
	
			

#ifdef DEBUG_EVAL
	heur[WHITE] = score[WHITE] - tmp[WHITE];
	heur[BLACK] = score[BLACK] - tmp[BLACK];
	printf("------[ Heuristic ]------\n");
	printf("Phase W: %d \n", Phase(WHITE));
	printf("Phase B: %d \n", Phase(BLACK));
	printf("Heuristic W: %d \n", heur[WHITE]);
	printf("Heuristic B: %d \n", heur[BLACK]);
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];
#endif /*DEBUG_EVAL*/

	
	/*-------------------*/
	/*Struttura Pedonale */
	/*-------------------*/

#ifdef DEBUG_EVAL
	printf("------[ Pawn Struct ]------\n");
#endif /*DEBUG_EVAL*/

	maj[WHITE] = 0;
	maj[BLACK] = 0;

	score[WHITE] += eval_pawn_struct(WHITE);
	score[BLACK] += eval_pawn_struct(BLACK);

#ifdef DEBUG_EVAL
	ps[WHITE] = score[WHITE] - tmp[WHITE];
	ps[BLACK] = score[BLACK] - tmp[BLACK];
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];	
#endif /*DEBUG_EVAL*/

	/*-------------------*/
	/*Sicurezza del re   */
	/*-------------------*/

	ks[WHITE] = eval_king_safety(Position(WHITE, 0), WHITE);
	ks[BLACK] = eval_king_safety(Position(BLACK, 0), BLACK);

	if (ks[WHITE] > ks[BLACK]) qs[WHITE] = 1;
	else if (ks[WHITE] < ks[BLACK]) qs[BLACK] = 1;

	score[WHITE] -= ks[WHITE];
	score[BLACK] -= ks[BLACK];

#ifdef DEBUG_EVAL
	tmp[WHITE] = score[WHITE];
	tmp[BLACK] = score[BLACK];	

#endif /*DEBUG_EVAL*/

	/*------------------------*/
	/*Interazione Pezzi-Pedoni*/
	/*------------------------*/

	/*Pezzi Bianchi*/
	c = 1;

	do {
			if (Active(WHITE, c)) {
					switch (Name(WHITE, c)) {
							case no_piece:
							case wpawn:
							case bpawn:
								break;
							case knight:
								score[WHITE] += eval_knight(Position(WHITE, c), WHITE);
								break;
							case bishop:
								score[WHITE] += eval_bishop(Position(WHITE, c), WHITE);
								break;
							case rook:
								score[WHITE] += eval_rook(Position(WHITE, c), WHITE);
								break;
							case queen:
								score[WHITE] += eval_queen(Position(WHITE, c), WHITE, qs[WHITE]);
								break;
							case king:
								printf("Bad white king detect in eval(): %d in %d",Name(WHITE, c),c);
								assert(0);
								break;
							default:
								printf("Bad white piece type detect in eval(): %d",Name(WHITE, c));
								assert(0);
								break;
							}
					}
			c++;
			}
	while (c < 16);

	/*Pezzi Neri*/
	c = 1;
	do {
			if (Active(BLACK, c)) {
					switch (Name(BLACK, c)) {
							case no_piece:
							case wpawn:
							case bpawn:
								break;
							case knight:
								score[BLACK] += eval_knight(Position(BLACK, c), BLACK);
								break;
							case bishop:
								score[BLACK] += eval_bishop(Position(BLACK, c), BLACK);
								break;
							case rook:
								score[BLACK] += eval_rook(Position(BLACK, c), BLACK);
								break;
							case queen:
								score[BLACK] += eval_queen(Position(BLACK, c), BLACK, qs[BLACK]);
								break;
							case king:
								printf("Bad black king detect in eval(): %d in %d",Name(BLACK, c),c);
								assert(0);
								break;
							default:
								printf("Bad Black piece type detect in eval(): %d",Name(BLACK, c));
								assert(0);
							}
					}
			c++;
			}
	while (c < 16);

	/*Maggioranza pedonale*/
	if (maj[WHITE] > maj[BLACK]) score[WHITE] += MAJORITY;
	else if (maj[WHITE] < maj[BLACK])	score[BLACK] += MAJORITY;

	/*Controllo del centro*/
	center_control[WHITE]=(4*(count_attack(D5,WHITE)+count_attack(E5,WHITE)))+
						  (3*(count_attack(D4,WHITE)+count_attack(E4,WHITE)))+
						  (2*(count_attack(C5,WHITE)+count_attack(C6,WHITE)+count_attack(D6,WHITE)+count_attack(E6,WHITE)+count_attack(F6,WHITE)+count_attack(F5,WHITE)))+
						  (1*(count_attack(C4,WHITE)+count_attack(C3,WHITE)+count_attack(D3,WHITE)+count_attack(E3,WHITE)+count_attack(F3,WHITE)+count_attack(F4,WHITE)));
	center_control[BLACK]=(3*(count_attack(D5,BLACK)+count_attack(E5,BLACK)))+
						  (4*(count_attack(D4,BLACK)+count_attack(E4,BLACK)))+
						  (1*(count_attack(C5,BLACK)+count_attack(C6,BLACK)+count_attack(D6,BLACK)+count_attack(E6,BLACK)+count_attack(F6,BLACK)+count_attack(F5,BLACK)))+
						  (2*(count_attack(C4,BLACK)+count_attack(C3,BLACK)+count_attack(D3,BLACK)+count_attack(E3,BLACK)+count_attack(F3,BLACK)+count_attack(F4,BLACK)));
	
	if (center_control[WHITE] > center_control[BLACK]) score[WHITE] += CENTER_CONTROL;
	else if (center_control[WHITE] < center_control[BLACK]) score[BLACK] += CENTER_CONTROL;

#ifdef DEBUG_EVAL
	ps[WHITE] += (score[WHITE] - tmp[WHITE]);
	ps[BLACK] += (score[BLACK] - tmp[BLACK]);
	printf("Weak square of one color W: %d \n", wsqarec[WHITE]);
	printf("Weak square of one color B: %d \n", wsqarec[BLACK]);
	printf("Total Pawn Struct W: %d \n", (ps[WHITE] + wsqarec[WHITE]));
	printf("Total Pawn Struct B: %d \n", (ps[BLACK] + wsqarec[BLACK]));
	printf("Center Control W: %d \n", (center_control[WHITE]));
	printf("Center Control B: %d \n", (center_control[BLACK]));
	printf("------[ King Safety ]------\n");
	printf("King Safety W: %d \n", ks[WHITE]);
	printf("King Safety B: %d \n", ks[BLACK]);
	printf("\n============================\n\n");
	printf("Global (white score - black score): %d %d Total %d \n", score[WHITE], score[BLACK], score[WHITE] - score[BLACK]);
#endif /*DEBUG_EVAL*/

	if (mizar.o.use_ev_cache && fast) {
		eval_hash[hash_index].hash = (Hash>>32);
		eval_hash[hash_index].score = (score[Side] - score[Xside]);
	}
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

	return score[Side] - score[Xside];
	}

/*Valuta la fase della partita*/
int eval_phase(const int color) {
	int phase, m;

	m = WHITE;

	if (color == WHITE)
		m = BLACK;

	phase = (9 * Num_piece(m, queen)) + (5 * Num_piece(m, rook)) + (3 * (Num_piece(m, knight) + Num_piece(m, bishop)));

	phase -= 10;

	phase /= 2;

	if (phase < 0)
		phase = 0;
	else if (phase > 7)
		phase = 7;

	return phase;
	}

/*Valuta la sicurezza del re*/
int eval_king_safety(const int sq, const int color) {
	int score, counter,f;

	assert(Piece(sq)==king);
	/*invita a portare il re in posizione coperta*/
	if (color == WHITE) {
			counter = wking_dvlp[b256to64[sq]];
			f=File(sq)-FilaA;
			if (Cste & (cas_woo | cas_wooo)) {
				counter += wking_pawn_shield[psh[WHITE][f]-Riga8][psh[BLACK][f]-Riga8];
				counter += wking_pawn_shield[psh[WHITE][f+1]-Riga8][psh[BLACK][f+1]-Riga8];
				counter += wking_pawn_shield[psh[WHITE][f-1]-Riga8][psh[BLACK][f-1]-Riga8];
				}		
			}

	else {
			counter = bking_dvlp[b256to64[sq]];
			f=File(sq)-FilaA;
			if (Cste & (cas_boo | cas_booo)) {
				counter += bking_pawn_shield[psh[BLACK][f]-Riga8][psh[WHITE][f]-Riga8];
				counter += bking_pawn_shield[psh[BLACK][f+1]-Riga8][psh[WHITE][f+1]-Riga8];
				counter += bking_pawn_shield[psh[BLACK][f-1]-Riga8][psh[WHITE][f-1]-Riga8];
				}
			}

	/*nel finale induce a centralizzarlo*/
	if (Phase(color) <= ENDING_PHASE)
		counter = king_endgame[b256to64[sq]];

	/*scudo pedonale*/

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("King Safety start counter W: %d \n", counter);
	else
		printf("King Safety start counter B: %d \n", counter);

#endif /*DEBUG_EVAL*/

	/*controlla gli attacchi attorno*/
	if (color == WHITE) {
			counter += eval_kshield((sq + UP_L), BLACK, 0);
			counter += (2*eval_kshield((sq + UP), BLACK, 0));
			counter += eval_kshield((sq + UP_R), BLACK, 0);
			counter += eval_kshield((sq + DOWN_L), BLACK, 1);
			counter += eval_kshield((sq + DOWN_R), BLACK, 1);
			counter += eval_kshield((sq + DOWN), BLACK, 1);
			counter += eval_kshield((sq + RIGHT), BLACK, 1);
			counter += eval_kshield((sq + LEFT), BLACK, 1);
			}

	else {
			counter += eval_kshield((sq + DOWN_L), WHITE, 0);
			counter += eval_kshield((sq + DOWN_R), WHITE, 0);
			counter += (2*eval_kshield((sq + DOWN), WHITE, 0));
			counter += eval_kshield((sq + UP_L), WHITE, 1);
			counter += eval_kshield((sq + UP), WHITE, 1);
			counter += eval_kshield((sq + UP_R), WHITE, 1);
			counter += eval_kshield((sq + RIGHT), WHITE, 1);
			counter += eval_kshield((sq + LEFT), WHITE, 1);
			}

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("King Safety counter W: %d \n", counter);
	else
		printf("King Safety counter B: %d \n", counter);

#endif /*DEBUG_EVAL*/

	score = danger[counter];

	return score;
	}
/*Valuta se la casella attorno al re è pericolosa o meno*/
int eval_kshield(const int p, const int enemy_color, const int flag) {
	int score, my_color;

	if (IsOnBoard(p) == 0)
		return 0;

	score = 0;

	my_color = enemy_color ^ 1;

	switch (flag) {
			case 0: /*scudo di pedoni*/
				/*se la casella è attaccata*/
				if (ATable(enemy_color, p)) {
						score++; /*preoccupiamoci*/
						if (Color(p) != my_color) score ++;
						if (((ATable(my_color, p))&((unsigned __int16)65534)) == 0)
							score++; /*e se solo il re difende la posizione*/
						}
				break;
			case 1:
				/*se la casella è attaccata*/
				if (ATable(enemy_color, p)) {
						score++;
						if (((ATable(my_color, p))&((unsigned __int16)65534)) == 0)
							score++;
						}
				break;
			default:
				puts("Unexpected king shield flag.");
				assert(0);
				break;
			}

	return score;
	}


/*Valuta la struttura pedonale:
- controllo del centro
- spazio
-pedoni deboli

*/
int eval_pawn_struct(const int color) {
	int score,cont,p,pp;
	int k, w, f, c;
#ifdef DEBUG_EVAL
		int temp,temp1;
		temp=0;
		temp1=0;
#endif /*DEBUG_EVAL*/

	score=0;
	k = 0; /*tipo*/
	w = 0; /*pedone debole*/
	f = 0;/*falange*/
	c = 0;/*pedone centrale*/
	pp = 0;/*passato*/
	
	switch (color) {
		case BLACK:
			cont = tree.position.pawns_start[BLACK];
			while (cont < 16) {
				k = 0; /*tipo*/
				w = 0; /*pedone debole*/
				f = 0;/*falange*/
				c = 0;/*pedone centrale*/
				pp = 0;/*passato*/
				if (Active(BLACK,cont) && (Name(BLACK, cont)==bpawn)) {
					assert(Name(BLACK, cont)==bpawn);
					p=Position(BLACK,cont);
					if (IsUp(p,psh[BLACK][(File(p)-FilaA)])) psh[BLACK][(File(p)-FilaA)]=Rank(p);
					/*Verifica se il pedone deve avanzare e guadagnare spazio*/
					if (File(p) == FilaD || File(p) == FilaE) {
						k = 1; /*centrale*/
						c = 1;
					}

					else if ((File(p) == FilaA || File(p) == FilaB || File(p) == FilaC)) {						
						if (Cste&cas_boo) {
							k = 1; /*opposto all'arrocco*/
							maj[BLACK]++;
							f = 1;
							}
					}

					else if ((File(p) == FilaF || File(p) == FilaG || File(p) == FilaH)){
						if (Cste&cas_booo) {
							k = 1; /*opposto all'arrocco*/
							maj[BLACK]++;
							f = 1;
							}
						}

					/*Se non ha pedoni avversari davanti e lateralmente è passato*/
					if ((bit_passed[BLACK][p]&Pbitboard(WHITE)) == 0) {
						pp=1;
						if (ATable(color, p) || (c) || (IsDown(p, Riga5))) {
								score += (PASSED_PAWN * (Rank(p) - 4));
								k++;
								}
						}

					else {
						/*Se deve avanzare premia l'avanzamento*/

						if (k)
							score += b_adv[b256to64[p]];

						/*Se c'è un pedone dello stesso colore davanti è doppiato*/
						if (bit_scan[p][BDOWN]&Pbitboard(BLACK)) {
								w++;
								score -= DOUBLED_PAWN;

								if (f)maj[BLACK]--;
								}
						/*Se non ci sono pedoni lateralmente è isolato*/
						if ((bit_isolated[p]&Pbitboard(BLACK)) == 0) {
								w++;
								score -= ISOLATED_PAWN;

								}
						/*se non può più essere difeso è arretrato*/
						else if ((bit_backward[BLACK][p]&Pbitboard(BLACK)) == 0) {
								w++;
								score -= BACKWARD_PAWN;
								}
						/*verifica il grado di debolezza*/
						if (w) {
								/*Grado 1: bloccato*/
								if (Color(p + DOWN) == WHITE || ATable(WHITE, p + DOWN)) {
										score -= WEAK_DEGREE;
										/*Grado 2: Attaccato*/

										if (ATable(WHITE, p))
											score -= WEAK_DEGREE;
										}

							  }
						}
					/*premia pedoni centrali al centro*/
					if (c) {
						if (zone[b256to64[p]]) {
							score +=PAWN_CENTR;
							score += (SUPERPROT * count_attack(p,BLACK));
						}
						/*penalizza se bloccati*/
						if (!w) {
							if (Color(p + DOWN) == WHITE || ATable(WHITE, p + DOWN)) {
								score -= WEAK_DEGREE;
							}
						}
					}
					/*penalizza blocco pedoni non centrali*/
					if (k && !c) {
						if (Color(p + DOWN) == WHITE || ATable(WHITE, p + DOWN)) {
								score -= WEAK_DEGREE;
							}
					}
					/*Pezzo in presa*/
					if (ATable(WHITE, p)) {
						score -= HANG_P;
						if (pp && (Rank(p)==Riga2) && IsEmpty(p+DOWN) && (QUEEN_VALUE > Hang(BLACK))) {
							Hang(BLACK) = QUEEN_VALUE;
							Hang_p(BLACK) = p;
							}
						else if (PAWN_VALUE > Hang(BLACK)) {
							Hang(BLACK) = PAWN_VALUE;
							Hang_p(BLACK) = p;
							}
						}
					else {
						/*Se non è in presa puo' essere una grave minaccia*/
						if (pp && (Rank(p)==Riga2) && IsEmpty(p+DOWN) && (QUEEN_VALUE > Hang(WHITE))) {
							Hang(WHITE) = QUEEN_VALUE;
							Hang_p(WHITE) = p;
							}
						}
#ifdef DEBUG_EVAL
		temp = score-temp1;
		printf("Pawn B in %d : %d \n", p, temp);
		temp1=score;
#endif /*DEBUG_EVAL*/
				}/*end if active*/	
			cont++;
			}
			break;
		case WHITE:
			cont = tree.position.pawns_start[WHITE];
			while (cont < 16) {
				k = 0; /*tipo*/
				w = 0; /*pedone debole*/
				f = 0;/*falange*/
				c = 0;/*pedone centrale*/
				pp = 0;/*passato*/
				if (Active(WHITE,cont)&& (Name(WHITE, cont)==wpawn)) {
					assert(Name(WHITE, cont)==wpawn);
					p=Position(WHITE,cont);
					if (IsDown(p,psh[WHITE][(File(p)-FilaA)])) psh[WHITE][(File(p)-FilaA)]=Rank(p);
					/*Verifica se il pedone deve avanzare e guadagnare spazio*/

					if (File(p) == FilaD || File(p) == FilaE) {
						k = 1; /*centrale*/
						c = 1;
						}

					else if ((File(p) == FilaA || File(p) == FilaB || File(p) == FilaC)) {
						if (Cste&cas_woo){
							maj[WHITE]++;
							f = 1;
							k = 1; /*opposto all'arrocco*/
							}
						}

					else if ((File(p) == FilaF || File(p) == FilaG || File(p) == FilaH)) {
						if (Cste&cas_wooo){
							maj[WHITE]++;
							f = 1;
							k = 1; /*opposto all'arrocco*/
							}
						}

					/*Se non ha pedoni avversari davanti e lateralmente è passato*/
					if ((bit_passed[WHITE][p]&Pbitboard(BLACK)) == 0) {
						pp = 1;
						if (ATable(color, p) || (c) || (IsUp(p, Riga4))) {
								score += (PASSED_PAWN * (11 - Rank(p)));
								k++;
								}
						}

					else {
						/*Se deve avanzare premia l'avanzamento*/

						if (k)
							score += w_adv[b256to64[p]];

						/*Se c'è un pedone dello stesso colore davanti è doppiato*/
						if (bit_scan[p][BUP]&Pbitboard(WHITE)) {
								w++;
								score -= DOUBLED_PAWN;

								if (f)maj[WHITE]--;
								}
						/*Se non ci sono pedoni lateralmente è isolato*/
						if ((bit_isolated[p]&Pbitboard(WHITE)) == 0) {
								w++;
								score -= ISOLATED_PAWN;
								}
						/*se non può più essere difeso è arretrato*/
						else if ((bit_backward[WHITE][p]&Pbitboard(WHITE)) == 0) {
								w++;
								score -= BACKWARD_PAWN;
								}
						/*verifica il grado di debolezza*/
						if (w) {
								/*Grado 1: bloccato*/

								if (Color(p + UP) == BLACK || ATable(BLACK, p + UP)) {
										score -= WEAK_DEGREE;
										/*Grado 2: Attaccato*/

										if (ATable(BLACK, p))
											score -= WEAK_DEGREE;
										}

								}

						}
					/*premia pedoni centrali al centro*/
					if (c) {
						if (zone[b256to64[p]]) {
							score +=PAWN_CENTR;
							score += (SUPERPROT * count_attack(p,WHITE));
						}
						/*penalizza se bloccati*/
						if (!w) {
							if (Color(p + UP) == BLACK || ATable(BLACK, p + UP)) {
								score -= WEAK_DEGREE;
							}
						}
					}
					/*penalizza blocco pedoni non centrali*/
					if (k && !c) {
						if (Color(p + UP) == BLACK || ATable(BLACK, p + UP)) {
								score -= WEAK_DEGREE;
							}
					}
					/*Pezzo in presa*/
					if (ATable(BLACK, p)) {
						score -= HANG_P;
						if (pp && (Rank(p)==Riga7) && IsEmpty(p+UP) && (QUEEN_VALUE > Hang(WHITE))) {
							Hang(WHITE) = QUEEN_VALUE;
							Hang_p(WHITE) = p;
							}
						else if (PAWN_VALUE > Hang(WHITE)) {
							Hang(WHITE) = PAWN_VALUE;
							Hang_p(WHITE) = p;
							}
						}
					else {
						/*Se non è in presa puo' essere una grave minaccia*/
						if (pp && (Rank(p)==Riga7) && IsEmpty(p+UP) && (QUEEN_VALUE > Hang(BLACK))) {
							Hang(BLACK) = QUEEN_VALUE;
							Hang_p(BLACK) = p;
							}
						}
#ifdef DEBUG_EVAL
		temp = score-temp1;
		printf("Pawn W in %d : %d \n", p, temp);
		temp1=score;		
#endif /*DEBUG_EVAL*/
					}
			cont++;
			}
			break;
		default:
			puts("Bad pawn type on board.");
			assert(0);
			break;
	}

	return score;

}


/*Valuta i cavalli*/
int eval_knight(const int p, const int color) {
	int score, mob, t;

	/*premia centralità*/
	score = centr[b256to64[p]];

	mob = 0;

	switch (color) {
			case BLACK:
				/*premia avamposto c d e f*/

				if (File(p) >= FilaC && File(p) <= FilaF) {
						if (IsDown(p, Riga5) && ((bit_file[p]&Pbitboard(BLACK)) == 0) && (IsWPawn(p + DOWN))) {
								score += OUTPOST; /*valorizzare le difese di pedoni*/
								}

						}
				/*penalizza se cattivo (<20% di mobilità potenziale)*/
				do {
					if (Phase(color) >= OPENING_PHASE)
						break;
				t = p + KNIGHT1;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT2;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT3;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT4;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT5;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT6;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT7;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				t = p + KNIGHT8;
				if (IsEmpty(t) && !ATable(WHITE, t))
					mob++;

				if (mob <= knight_mob[b256to64[p]])
					score -= BAD_PIECE_N;
					}while(0);

				/*premia se su casella debole avversaria*/
				if (bit_weaksquare[WHITE][p] && (!(bit_weaksquare[WHITE][p]&Pbitboard(WHITE))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			case WHITE:
				/*premia avamposto c d e f*/
				if (File(p) >= FilaC && File(p) <= FilaF) {
						if (IsUp(p, Riga4) && ((bit_file[p]&Pbitboard(WHITE)) == 0) && (IsBPawn(p + UP))) {
								score += OUTPOST; /*valorizzare le difese di pedoni*/
								}

						}
				/*penalizza se cattivo (<20% di mobilità potenziale)*/
				do {
					if (Phase(color) >= OPENING_PHASE)
						break;
				t = p + KNIGHT1;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT2;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT3;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT4;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT5;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT6;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT7;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				t = p + KNIGHT8;
				if (IsEmpty(t) && !ATable(BLACK, t))
					mob++;

				if (mob <= knight_mob[b256to64[p]])
					score -= BAD_PIECE_N;
					} while (0);

				/*premia se su casella debole avversaria*/
				if (bit_weaksquare[BLACK][p] && (!(bit_weaksquare[BLACK][p]&Pbitboard(BLACK))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			default:
				puts("Knight is neither black nor white.");

				assert(0);
			}

	/*Pezzo in presa*/
	if (ATable(color ^ 1, p)) {
		if (KNIGHT_VALUE > Hang(color)) {
				Hang(color) = KNIGHT_VALUE;
				Hang_p(color) = p;
			}

		}

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("Knight W in %d : %d \n", p, score);
	else
		printf("Knight B in %d : %d \n", p, score);

#endif /*DEBUG_EVAL*/


	return score;
	}

/*Valuta gli alfieri*/
int eval_bishop(const int p, const int color) {
	int score, mob, t;

	/*premia centralità*/
	score = centr[b256to64[p]];

	mob = 0;

	switch (color) {
			case BLACK:
				/*premia avamposto c d e f*/

				if (File(p) >= FilaC && File(p) <= FilaF) {
						if (IsDown(p, Riga5) && ((bit_file[p]&Pbitboard(BLACK)) == 0) && (IsWPawn(p + DOWN))) {
								score += OUTPOST; /*valorizzare le difese di pedoni*/
								}

						}
				/*penalizza se cattivo (<20% di mobilità potenziale)*/
				do {
					if (Phase(color) >= OPENING_PHASE)
						break;
				t = p;
				while (IsEmpty(t += UP_R)) {
						if (!ATable(WHITE, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += UP_L)) {
						if (!ATable(WHITE, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += DOWN_R)) {
						if (!ATable(WHITE, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += DOWN_L)) {
						if (!ATable(WHITE, t))
							mob++;
						}

				if (mob <= bishop_mob[b256to64[p]])
					score -= BAD_PIECE_B;
					}while(0);

				/*premia se su casella debole avversaria*/
				if (bit_weaksquare[WHITE][p] && (!(bit_weaksquare[WHITE][p]&Pbitboard(WHITE))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			case WHITE:
				/*premia avamposto c d e f*/
				if (File(p) >= FilaC && File(p) <= FilaF) {
						if (IsUp(p, Riga4) && ((bit_file[p]&Pbitboard(WHITE)) == 0) && (IsBPawn(p + UP))) {
								score += OUTPOST; /*valorizzare le difese di pedoni*/
								}

						}
				/*penalizza se cattivo (<20% di mobilità potenziale)*/
				do {
					if (Phase(color) >= OPENING_PHASE)
						break;
				t = p;
				while (IsEmpty(t += UP_R)) {
						if (!ATable(BLACK, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += UP_L)) {
						if (!ATable(BLACK, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += DOWN_R)) {
						if (!ATable(BLACK, t))
							mob++;
						}

				t = p;
				while (IsEmpty(t += DOWN_L)) {
						if (!ATable(BLACK, t))
							mob++;
						}

				if (mob <= bishop_mob[b256to64[p]])
					score -= BAD_PIECE_B;
					}while (0);

				/*premia se su casella debole avversaria*/
				if (bit_weaksquare[BLACK][p] && (!(bit_weaksquare[BLACK][p]&Pbitboard(BLACK))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			default:
				puts("Bishop is neither black nor white.");

				assert(0);
			}

	/*Pezzo in presa*/
	if (ATable(color ^ 1, p)) {
		if (BISHOP_VALUE > Hang(color)){
				Hang(color) = BISHOP_VALUE;
				Hang_p(color) = p;
			}
		}

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("Bishop W in %d : %d \n", p, score);
	else
		printf("Bishop B in %d : %d \n", p, score);

#endif /*DEBUG_EVAL*/

	return score;
	}

/*Valuta le torri*/
int eval_rook(const int p, const int color) {
	int score;

	score = 0;

	switch (color) {
			case BLACK:
				/*controlla se è su una colonna semiaperta*/

				if ((Pbitboard(BLACK)&bit_scan[p][BDOWN]) == 0) {
						if ((Pbitboard(WHITE)&bit_file[p])) {
								if ((File(p) < FilaC || File(p) > FilaF) && IsDown(p, Riga5) && IsWPawn(p + DOWN))
									score += OUTPOST;

								/*else score+=ROOK_ON_HOF;dovremmo premiare solo attacchi su pedoni deboli o arrocco*/
								}

						else {
								/*Premia torre su colonna aperta solo se utile*/

								if (Rank(Position(WHITE, 0)) == Riga1 || (Pbitboard(WHITE)&bit_rank[A2]) ) {
										score += (PIECE_ON_OPENF + of_b[Rank(p)]);
										}

								}

						}
				/*premia se su casella debole avversaria*/

				if (bit_weaksquare[WHITE][p] && (!(bit_weaksquare[WHITE][p]&Pbitboard(WHITE))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			case WHITE:
				/*controlla se è su una colonna semiaperta*/
				if ((Pbitboard(WHITE)&bit_scan[p][BUP]) == 0) {
						if ((Pbitboard(BLACK)&bit_file[p])) {
								if ((File(p) < FilaC || File(p) > FilaF) && IsUp(p, Riga4) && IsBPawn(p + UP))
									score += OUTPOST;

								/*else score+=ROOK_ON_HOF;*/
								}

						else {
								/*Premia torre su colonna aperta solo se utile*/

								if (Rank(Position(BLACK, 0)) == Riga8 || (Pbitboard(BLACK)&bit_rank[A7]) ) {
										score += (PIECE_ON_OPENF + of_w[Rank(p)]);
										}

								}

						}
				/*premia se su casella debole avversaria*/

				if (bit_weaksquare[BLACK][p] && (!(bit_weaksquare[BLACK][p]&Pbitboard(BLACK))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			default:
				puts("Rook is neither black nor white.");

				assert(0);
			}

	/*Pezzo in presa*/
	if (ATable(color ^ 1, p)) {
		if (ROOK_VALUE > Hang(color)) {
				Hang(color) = ROOK_VALUE;
				Hang_p(color) = p;
			}
		}

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("Rook W in %d : %d \n", p, score);
	else
		printf("Rook B in %d : %d \n", p, score);

#endif /*DEBUG_EVAL*/

	return score;
	}

/*Valuta le regine*/
int eval_queen(const int p, const int color, const int str) {
	int score;

	/*premia centralità*/
	score = qcentr[b256to64[p]];

	switch (color) {
			case BLACK:
				/*controlla se è su una colonna semiaperta*/

				if ((Pbitboard(BLACK)&bit_scan[p][BDOWN]) == 0) {
						if ((Pbitboard(WHITE)&bit_file[p]) == 0) {
								/*Premia Regina su colonna aperta solo se utile*/

								if (Rank(Position(WHITE, 0)) == Riga1 || (Pbitboard(WHITE)&bit_rank[A2]) ) {
										score += (PIECE_ON_OPENF + of_b[Rank(p)]);
										}

								}

						}
				/*premia se su casella debole avversaria*/

				if (bit_weaksquare[WHITE][p] && (!(bit_weaksquare[WHITE][p]&Pbitboard(WHITE))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			case WHITE:
				/*controlla se è su una colonna semiaperta*/
				if ((Pbitboard(WHITE)&bit_scan[p][BUP]) == 0) {
						if ((Pbitboard(BLACK)&bit_file[p]) == 0) {
								/*Premia regina su colonna aperta solo se utile*/

								if (Rank(Position(BLACK, 0)) == Riga8 || (Pbitboard(BLACK)&bit_rank[A7]) ) {
										score += (PIECE_ON_OPENF + of_w[Rank(p)]);
										}

								}

						}
				/*premia se su casella debole avversaria*/

				if (bit_weaksquare[BLACK][p] && (!(bit_weaksquare[BLACK][p]&Pbitboard(BLACK))))
					score += PIECE_ON_STRONG_SQUARE;

				break;

			default:
				puts("Queen is neither black nor white.");

				assert(0);
			}

	if (str) score += QUEEN_STRONG;

	/*Pezzo in presa*/
	if (ATable(color ^ 1, p)) {
		if (QUEEN_VALUE > Hang(color)) {
				Hang(color) = QUEEN_VALUE;
				Hang_p(color) = p;
			}
		}

#ifdef DEBUG_EVAL
	if (color == WHITE)
		printf("Queen W in %d : %d \n", p, score);
	else
		printf("Queen B in %d : %d \n", p, score);

#endif /*DEBUG_EVAL*/
	return score;
	}

/*riempie le tavole di attacco con dati utili per la valutazione*/
void gen_knowledge() {
	int c, start, target;

	/*memset(tree.position.board_eval.ab, 0, sizeof(tree.position.board_eval.ab));*/
	for (c=0;c<64;c++) {
		tree.position.board_eval.ab[0][c].ci =0;
		tree.position.board_eval.ab[1][c].ci =0;
	}
	double_bishop[WHITE] = 0;
	double_bishop[BLACK] = 0;
	nwp[WHITE] = 0;
	nwp[BLACK] = 0;
	nbp[WHITE] = 0;
	nbp[BLACK] = 0;
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);

	c = 0;

	do {
			if (Active(Xside, c)) {
									
				start = Position(Xside, c);

					switch (Name(Xside, c)) {
							case no_piece:
							case wpawn:
								target = start + UP_R;

								if (IsFull(target))
									ATable(Xside, target) |= i2b[c];

								target = start + UP_L;

								if (IsFull(target))
									ATable(Xside, target) |= i2b[c];

								nwp[sq_color[b256to64[start]]]++;

								break;

							case bpawn:
								target = start + DOWN_R;

								if (IsFull(target))
									ATable(Xside, target) |= i2b[c];

								target = start + DOWN_L;

								if (IsFull(target))
									ATable(Xside, target) |= i2b[c];

								nbp[sq_color[b256to64[start]]]++;

								break;

							case knight:
								target = start + KNIGHT1;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT2;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT3;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT4;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT5;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT6;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT7;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + KNIGHT8;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								break;

							case bishop:
								target = start;

								while (IsEmpty(target += UP_R))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += UP_L))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += DOWN_R))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += DOWN_L))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								/*doppio alfiere;*/
								if (sq_color[b256to64[start]] == WHITE)
									double_bishop[Xside] |= 1;
								else if (sq_color[b256to64[start]] == BLACK)
									double_bishop[Xside] |= 2;

								break;

							case rook:

								target = start;
								while (IsEmpty(target += UP))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;
								while (IsEmpty(target += LEFT))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;
								while (IsEmpty(target += DOWN))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;
								while (IsEmpty(target += RIGHT))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								break;

							case queen:
								target = start;

								while (IsEmpty(target += UP_R))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += UP_L))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += DOWN_R))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += DOWN_L))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += UP))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += LEFT))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += DOWN))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start;

								while (IsEmpty(target += RIGHT))
									ATable(Xside, target) |= i2b[c];

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								break;

							case king:
								assert(c==0);
								target = start + UP;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + DOWN;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + LEFT;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + RIGHT;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + UP_R;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + UP_L;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + DOWN_R;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								target = start + DOWN_L;

								if (IsOnBoard(target))
									ATable(Xside, target) |= i2b[c];

								break;

							default:
								puts("Unexpected piece name.");

								assert(0);

								break;
							}

					}
			c++;
			}

	while (c < 16);

	c = 0;

	do {
			if (Active(Side, c)) {
					start = Position(Side, c);

					switch (Name(Side, c)) {
							case no_piece:
							case wpawn:
								target = start + UP_R;

								if (IsFull(target))
									ATable(Side, target) |= i2b[c];

								target = start + UP_L;

								if (IsFull(target))
									ATable(Side, target) |= i2b[c];

								nwp[sq_color[b256to64[start]]]++;

								break;

							case bpawn:
								target = start + DOWN_R;

								if (IsFull(target))
									ATable(Side, target) |= i2b[c];

								target = start + DOWN_L;

								if (IsFull(target))
									ATable(Side, target) |= i2b[c];

								nbp[sq_color[b256to64[start]]]++;

								break;

							case knight:
								target = start + KNIGHT1;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT2;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT3;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT4;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT5;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT6;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT7;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + KNIGHT8;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								break;
							case bishop:
								target = start;

								while (IsEmpty(target += UP_R)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += UP_L)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN_R)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN_L)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}
								/*doppio alfiere;*/
								if (sq_color[b256to64[start]] == WHITE)
									double_bishop[Side] |= 1;
								else if (sq_color[b256to64[start]] == BLACK)
									double_bishop[Side] |= 2;

								break;

							case rook:
								target = start;

								while (IsEmpty(target += UP)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += LEFT)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += RIGHT)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								break;
							case queen:
								target = start;

								while (IsEmpty(target += UP_R)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += UP_L)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN_R)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN_L)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += UP)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += LEFT)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += DOWN)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start;

								while (IsEmpty(target += RIGHT)) {
										ATable(Side, target) |= i2b[c];
										}

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								break;
							case king:
								assert(c==0);
								target = start + UP;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + DOWN;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + LEFT;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + RIGHT;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + UP_R;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + UP_L;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + DOWN_R;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								target = start + DOWN_L;

								if (IsOnBoard(target)) {
										ATable(Side, target) |= i2b[c];
										}

								break;
							default:
								puts("Unexpected piece name.");
								assert(0);
								break;
							}

					}
			c++;
			}

	while (c < 16);
	assert(Name(WHITE,0)==king);
	assert(Name(BLACK,0)==king);
	}
