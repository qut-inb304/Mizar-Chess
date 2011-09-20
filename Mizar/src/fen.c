/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

static FEN_POSITION fen;


/*set_position() fills the POSITION struct with data taken from a FEN string*/
void set_position(char *p) {
	int c, d, cont[2], square, end[2];
	PIECES_LIST m;
	set_fen(p);
	Side = fen.side;
	Xside = Side ^ 1;
/*	azzera il bordo*/

	for (c = 0; c < B_SIZE; c++) {
			if (IsLeft(c, FilaA) || IsRight(c, FilaH) || IsUp(c, Riga8) || IsDown(c, Riga1)) {
					Piece(c) = out_board;
					Color(c) = OUT_BOARD;
					Plist(c) = no_list;
					}

			else {
					Piece(c) = no_piece;
					Color(c) = EMPTY;
					Plist(c) = no_list;
					}

			}
	/*riempie la scacchiera*/

	for (c = 0; c < B2_SIZE; c++) {
			if (fen.piece[c] != no_piece) {
					Piece(b64to256[c]) = fen.piece[c];
					Color(b64to256[c]) = fen.col[c];
					}

			}
	/*azzera lista pezzi*/

	for (c = 0; c < 16; c++) {
			Active(BLACK, c) = CAPTURED;
			Active(WHITE, c) = CAPTURED;
			}

	cont[0] = 1;
	cont[1] = 1;
/*	riempie la lista dei pezzi*/

	for (c = 0; c < B2_SIZE; c++) {
			square = b64to256[c];

			if (Piece(square) == king) {
					Position(Color(square), 0) = square;
					Active(Color(square), 0) = FREE;
					Plist(square) = 0;
					}

			else if (Piece(square) != no_piece) {
					Position(Color(square), cont[Color(square)]) = square;
					Active(Color(square), cont[Color(square)]) = FREE;
					Plist(square) = cont[Color(square)];
					cont[Color(square)]++;
					}

			}
	Enp = 0;
	if (fen.enp) Enp = b64to256[fen.enp];
	Cstr = fen.castle;
	Cste = 0;

	if (Position(WHITE, 0) != E1) {
			if (IsRight(Position(WHITE, 0), FilaE))
				Cste |= cas_woo;
			else if (IsLeft(Position(WHITE, 0), FilaE))
				Cste |= cas_wooo;
			}

	if (Position(BLACK, 0) != E8) {
			if (IsRight(Position(BLACK, 0), FilaE))
				Cste |= cas_boo;
			else if (IsLeft(Position(BLACK, 0), FilaE))
				Cste |= cas_booo;
			}

	end[WHITE]=0;
	for (c = 0; c < 16; c++) {
		if (Active(WHITE,c)) end[WHITE]++;
		else break;
	}

	d=1;
	do {
			square=0;
			for (c = 1; c < (end[WHITE]-d); c++) {
				if(piece_value[Name(WHITE,c)]<piece_value[Name(WHITE,c+1)]) {
					m=tree.position.piece_list[WHITE][c];
					tree.position.piece_list[WHITE][c]=tree.position.piece_list[WHITE][c+1];
					tree.position.piece_list[WHITE][c+1]=m;
					square=1;
				} 
			}
			d++;
	} while (square);

	end[BLACK]=0;
	for (c = 0; c < 16; c++) {
		if (Active(BLACK,c)) end[BLACK]++;
		else break;
	}

	d=1;
	do {
			square=0;
			for (c = 1; c < (end[BLACK]-d); c++) {
				if(piece_value[Name(BLACK,c)]<piece_value[Name(BLACK,c+1)]) {
					m=tree.position.piece_list[BLACK][c];
					tree.position.piece_list[BLACK][c]=tree.position.piece_list[BLACK][c+1];
					tree.position.piece_list[BLACK][c+1]=m;
					square=1;
				} 
			}
			d++;
	} while (square);

	for (c = 0; c < end[WHITE]; c++){
		if(Active(WHITE,c)){
			Plist(Position(WHITE,c))=c;
		}
	}

	for (c = 0; c < end[BLACK]; c++){
		if(Active(BLACK,c)){
			Plist(Position(BLACK,c))=c;
		}
	}
	
	tree.position.pawns_start[WHITE]=16;
	tree.position.pawns_start[BLACK]=16;

	for (c = 0; c < 16; c++){
		if (Name(WHITE,c) == wpawn) {
			tree.position.pawns_start[WHITE]=c;
			break;
		} 
	}

	for (c = 0; c < 16; c++){
		if (Name(BLACK,c) == bpawn) {
			tree.position.pawns_start[BLACK]=c;
			break;
		} 
	}

	set_hashkey();
	set_board_eval();
	set_bitboard_position();
	}

/*set_fen() translates FEN string to FEN struct*/
void set_fen(char *f) {
	int sq, c;
	/*Campo 1: descrizione scacchiera*/

	for (sq = 0; sq < 64; sq++) {
			fen.piece[sq] = no_piece;
			fen.col[sq] = EMPTY;
			}

	c = 0;
	sq = 0;

	do {
			if (isdigit(f[c])) {
					sq += ((f[c]) - ('1' - 1));
					c++;
					continue;
					}

			switch (f[c]) {
					case 'K':
						fen.piece[sq] = king;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'k':
						fen.piece[sq] = king;
						fen.col[sq] = BLACK;
						sq++;
						break;
					case 'Q':
						fen.piece[sq] = queen;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'q':
						fen.piece[sq] = queen;
						fen.col[sq] = BLACK;
						sq++;
						break;
					case 'R':
						fen.piece[sq] = rook;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'r':
						fen.piece[sq] = rook;
						fen.col[sq] = BLACK;
						sq++;
						break;
					case 'B':
						fen.piece[sq] = bishop;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'b':
						fen.piece[sq] = bishop;
						fen.col[sq] = BLACK;
						sq++;
						break;
					case 'N':
						fen.piece[sq] = knight;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'n':
						fen.piece[sq] = knight;
						fen.col[sq] = BLACK;
						sq++;
						break;
					case 'P':
						fen.piece[sq] = wpawn;
						fen.col[sq] = WHITE;
						sq++;
						break;
					case 'p':
						fen.piece[sq] = bpawn;
						fen.col[sq] = BLACK;
						sq++;
						break;
					default :
						break;
					}

			c++;
			}

	while (isspace(f[c]) == 0);

	/*Campo 2:Tratto*/
	c++;

	switch (f[c]) {
			case 'w':
				fen.side = WHITE;
				break;
			case 'b':
				fen.side = BLACK;
				break;
			default:
				puts("Bad color in FEN/EPD record.  Choosing white as default.");
				fen.side = WHITE;
			}
/*	Campo 3:Arrocco*/
	c+=2;

	fen.castle = 0;

	if (f[c] != '-') {
			do {
					switch (f[c]) {
							case 'K':
								fen.castle |= cas_woo;
								break;
							case 'Q':
								fen.castle |= cas_wooo;
								break;
							case 'k':
								fen.castle |= cas_boo;
								break;
							case 'q':
								fen.castle |= cas_booo;
								break;
							default:
								printf("Non king/queen symbol (%c)=%d in castle rights.\n", f[c], f[c]);
							}

					c++;
					}
			while (isspace(f[c]) == 0);
			c++;
		}else c+=2;
	
	/*	Campo 4:En-passant*/
	fen.enp=0;
	if (f[c] != '-') {
		if ((f[c+1]=='3' && fen.side==WHITE) || (f[c+1]=='6' && fen.side==BLACK))  printf("Bad en passant square\n");
		switch (f[c]) {
			case 'a':
				if (f[c+1]=='3') fen.enp=40;
				else if (f[c+1]=='6') fen.enp=16;
				break;
			case 'b':
				if (f[c+1]=='3') fen.enp=41;
				else if (f[c+1]=='6') fen.enp=17;
				break;
			case 'c':
				if (f[c+1]=='3') fen.enp=42;
				else if (f[c+1]=='6') fen.enp=18;
				break;
			case 'd':
				if (f[c+1]=='3') fen.enp=43;
				else if (f[c+1]=='6') fen.enp=19;
				break;
			case 'e':
				if (f[c+1]=='3') fen.enp=44;
				else if (f[c+1]=='6') fen.enp=20;
				break;
			case 'f':
				if (f[c+1]=='3') fen.enp=45;
				else if (f[c+1]=='6') fen.enp=21;
				break;
			case 'g':
				if (f[c+1]=='3') fen.enp=46;
				else if (f[c+1]=='6') fen.enp=22;
				break;
			case 'h':
				if (f[c+1]=='3') fen.enp=47;
				else if (f[c+1]=='6') fen.enp=23;
				break;
			default:
				printf("Bad en passant square\n");
			} 
	}


}
