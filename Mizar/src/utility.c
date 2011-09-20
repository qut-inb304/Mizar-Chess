/*------------------------------------------------*/
/*                                                */
/* Mizar - Winboard Chess Engine                  */
/*                                                */
/* 2006 (c) Nicola Rizzuti - Italy                */
/*                                                */
/*Parts copyright (c) 1991-2000 iMatix Corporation*/
/*------------------------------------------------*/

#include <mizar.h>

/*-----------------------------------------------------------------------------*/
unsigned int time_elapsed() {

	struct timeb t;
	ftime(&t);
	return ((unsigned int)(((t.time*1000) + t.millitm)&0xFFFFFFFF));
	}
/*-----------------------------------------------------------------------------*/
void print_board() {
	int a, sq, rig;
	char ptoch[3][8] = {
	                       {'.', 'p', 'p', 'n', 'b', 'r', 'q', 'k'},
	                       {'.', 'P', 'P', 'N', 'B', 'R', 'Q', 'K'},
	                       {'.', '.', '.', '.', '.', '.', '.', '.'}
	                   };
	rig = 8;

	for (a = 0;a < B2_SIZE;a += 8) {
			sq = b64to256[a];
			printf("\n%d ", rig);
			printf(" %c", ptoch[Color(sq)][Piece(sq)]);
			printf(" %c", ptoch[Color(sq + 1)][Piece(sq + 1)]);
			printf(" %c", ptoch[Color(sq + 2)][Piece(sq + 2)]);
			printf(" %c", ptoch[Color(sq + 3)][Piece(sq + 3)]);
			printf(" %c", ptoch[Color(sq + 4)][Piece(sq + 4)]);
			printf(" %c", ptoch[Color(sq + 5)][Piece(sq + 5)]);
			printf(" %c", ptoch[Color(sq + 6)][Piece(sq + 6)]);
			printf(" %c", ptoch[Color(sq + 7)][Piece(sq + 7)]);
			rig--;
			}

	printf("\n\n   a b c d e f g h\n\n");
	}
/*-----------------------------------------------------------------------------*/
void print_bitboard() {
	int a, rig;

	unsigned __int64 w, b;
	rig = 8;

	for (a = 7;a >= 0;a--) {
			w = Bitboard(WHITE);
			b = Bitboard(BLACK);
			w >>= (a*8);
			b >>= (a*8);
			printf("\n%d ", rig);

			if ((w&128) == 0) {
					if ((b&128))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&64) == 0) {
					if ((b&64))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&32) == 0) {
					if ((b&32))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&16) == 0) {
					if ((b&16))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&8) == 0) {
					if ((b&8))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&4) == 0) {
					if ((b&4))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&2) == 0) {
					if ((b&2))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&1) == 0) {
					if ((b&1))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			rig--;
			}

	printf("\n\n   a b c d e f g h\n\n");
	rig = 8;

	for (a = 7;a >= 0;a--) {
			w = Pbitboard(WHITE);
			b = Pbitboard(BLACK);
			w >>= (a*8);
			b >>= (a*8);
			printf("\n%d ", rig);

			if ((w&128) == 0) {
					if ((b&128))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&64) == 0) {
					if ((b&64))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&32) == 0) {
					if ((b&32))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&16) == 0) {
					if ((b&16))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&8) == 0) {
					if ((b&8))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&4) == 0) {
					if ((b&4))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&2) == 0) {
					if ((b&2))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			if ((w&1) == 0) {
					if ((b&1))
						printf(" b");
					else
						printf(" 0");
					}

			else
				printf(" w");

			rig--;
			}

	printf("\n\n   a b c d e f g h\n\n");
	}
/*-----------------------------------------------------------------------------*/
void print_bit(const unsigned __int64 bit) {
	int a, rig;

	unsigned __int64 w;
	rig = 8;

	for (a = 7;a >= 0;a--) {
			w = bit;
			w >>= (a*8);
			printf("\n%d ", rig);

			if ((w&128) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&64) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&32) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&16) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&8) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&4) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&2) == 0)
				printf(" 0");
			else
				printf(" 1");

			if ((w&1) == 0)
				printf(" 0");
			else
				printf(" 1");



			rig--;
			}

	printf("\n\n   a b c d e f g h\n\n");
	}
/*-----------------------------------------------------------------------------*/
int stop_search() {
	
	if (mizar.timer == TIME_EXAT && time_elapsed() >= tree.stop_time)
		return 1;

	if (mizar.o.use_ponder && tree.ponder_search && ponder_stop())
		return 1;

	/*temporizzatore...dovremmo crearne uno indipendente dalla macchina*/
	if (Nodes&TIME_CHECK)
		return 0;	

	if (time_elapsed() >= tree.stop_time)
		return 1;

	return 0;
	}
/*-----------------------------------------------------------------------------*/
int repetition() {
	int c, end;

	if (tree.fifty > 99)
		return 2;

	if (Ply > 3) {
			end = N_moves - tree.fifty;

			for (c = N_moves - 2;c >= end;c -= 2)
				if (Hash == tree.history[c].hash)
					return 1;
			}

	else {
			end = 0;

			for (c = N_moves - 2;c >= 0;c -= 2)
				if (Hash == tree.history[c].hash)
					end++;

			if (end > 1)
				return 1;
			}

	return 0;
	}
/*-----------------------------------------------------------------------------*/
char *move2str(MOVE m) {

	static char str[6];

	char p=0;

	if (m.ms.flag & M_PRO) {
			switch (m.ms.piece) {
					case no_piece:
					case wpawn:
					case bpawn:
						break;
					case knight:
						p = 'n';
						break;
					case bishop:
						p = 'b';
						break;
					case rook:
						p = 'r';
						break;
					case queen:
						p = 'q';
						break;
					}

			sprintf(str, "%c%d%c%d%c",
			        File(m.ms.from) - FilaA + 'a',
			        12 - Rank(m.ms.from),
			        File(m.ms.to) - FilaA + 'a',
			        12 - Rank(m.ms.to),
			        p);
			}

	else
		sprintf(str, "%c%d%c%d",
		        File(m.ms.from) - FilaA + 'a',
		        12 - Rank(m.ms.from),
		        File(m.ms.to) - FilaA + 'a',
		        12 - Rank(m.ms.to));

	return str;
	}
/*-----------------------------------------------------------------------------*/
unsigned int str2move(char *s, const int control) {
	unsigned char from, to, pro, i;

	if (control) {
			if (s[0] < 'a' || s[0] > 'h' || s[1] < '0' || s[1] > '9' || s[2] < 'a' || s[2] > 'h' || s[3] < '0' || s[3] > '9')
				return 0;

			from = s[0] - 'a' + FilaA;

			from += (16 * (12 - (s[1] - '0')));

			to = s[2] - 'a' + FilaA;

			to += (16 * (12 - (s[3] - '0')));

			pro = 0;

			if (Side == WHITE) {
					if (Piece(from) == wpawn && Rank(from) == Riga7) {
							switch (s[4]) {
									case 'n':
										pro = knight;
										break;
									case 'b':
										pro = bishop;
										break;
									case 'r':
										pro = rook;
										break;
									case 'q':
										pro = queen;
									}

							}

					}

			else {
					if (Piece(from) == bpawn && Rank(from) == Riga2) {
							switch (s[4]) {
									case 'n':
										pro = knight;
										break;
									case 'b':
										pro = bishop;
										break;
									case 'r':
										pro = rook;
										break;
									case 'q':
										pro = queen;
									}

							}

					}

			}

	else {
			from = s[9] - 'a' + FilaA;
			from += (16 * (12 - (s[10] - '0')));
			to = s[11] - 'a' + FilaA;
			to += (16 * (12 - (s[12] - '0')));
			pro = 0;

			if (Side == WHITE) {
					if (Piece(from) == wpawn && Rank(from) == Riga7) {
							switch (s[13]) {
									case 'n':
										pro = knight;
										break;
									case 'b':
										pro = bishop;
										break;
									case 'r':
										pro = rook;
										break;
									case 'q':
										pro = queen;
									}

							}

					}

			else {
					if (Piece(from) == bpawn && Rank(from) == Riga2) {
							switch (s[13]) {
									case 'n':
										pro = knight;
										break;
									case 'b':
										pro = bishop;
										break;
									case 'r':
										pro = rook;
										break;
									case 'q':
										pro = queen;
									}

							}

					}

			}
	/*Potrei usare verify_move, ma dovrei capire che mossa e'...meglio generarle*/
	Ply = 0;
	gen_all();

	for (i = 0;i < tree.first_move[1];i++) {
			if (tree.move_list[i].move.ms.from == from && tree.move_list[i].move.ms.to == to) {
					if (tree.move_list[i].move.ms.flag&M_PRO) {
							if (tree.move_list[i].move.ms.piece == pro)
								return (tree.move_list[i].move.mi);
							else
								continue;
							}

					return (tree.move_list[i].move.mi);
					}

			}
	return 0;
	}
/*-----------------------------------------------------------------------------*/
int check_game() {
	int i, legal;

	legal = 0;
	Ply = 0;
	gen_all();
	check_condition();

	for (i = 0;i < tree.first_move[1];i++) {
			if (makemove(tree.move_list[i].move,0)) {
					legal = 1;
					unmakemove();
					break;
					}

			}

	if (legal == 0) {
			if (tree.check[Side][Ply]) {
				if (Side == WHITE) {
						printf("0-1 {Black mates}\n");
						return 1;
					}
				else {
						printf("1-0 {White mates}\n");
						return 1;
					}
			}

			else {
				printf("1/2-1/2 {Stalemate}\n");
				return 1;
			}
	}

	else {
			i = repetition();

			if (i == 1) {
				printf("1/2-1/2 {Draw by repetition}\n");
				return 1;
			}
			else if (i == 2) {
				printf("1/2-1/2 {Draw by fifty move rule}\n");
				return 1;
			}
	}
	return 0;
	}
/*-----------------------------------------------------------------------------*/
char * strskp (const char *string) {
	char*skip = (char *) string;

	while (*skip == ' ')
		skip++;

	return (skip);
	}
/*-----------------------------------------------------------------------------*/
int lexcmp (const char *string1, const char *string2) {
	int cmp;

	do {
			cmp = (unsigned char) tolower (*string1) - (unsigned char) tolower (*string2);
			}

	while (*string1++ && *string2++ && cmp == 0);

	return (cmp);
	}
/*-----------------------------------------------------------------------------*/
int myread(FILE *stream, char *string) {
	int ch, cnbr;

	cnbr = 0;
	memset (string, ' ', MAX_LINE);

	for (;;) {
			ch = fgetc (stream);
			/*tab*/

			if (ch == '\t')
				cnbr = ((cnbr >> 3) << 3) + 8;
			else if ((ch == '\n') || (ch == EOF)) {
					string [cnbr] = '\0';
					return (ch == '\n' || cnbr);
					}

			else if (cnbr < MAX_LINE)
				string [cnbr++] = (char) ch;

			if (cnbr >= MAX_LINE) {
					string [MAX_LINE] = '\0';
					return 1;
					}

			}
	}
/*-----------------------------------------------------------------------------*/
char * strcrop (char *string) {
	char *last;

	if (string) {
			last = string + strlen (string);

			while (last > string) {
					if (!isspace (*(last - 1)))
						break;

					last--;
					}

			*last = 0;
			}

	return (string);
	}
/*-----------------------------------------------------------------------------*/
char * strlwc (char *string) {
	char *scan;

	if (string) {
			scan = string;

			while (*scan) {
					*scan = (char) tolower (*scan);
					scan++;
					}

			}
	return (string);
	}
/*-----------------------------------------------------------------------------*/
char * strconvch (char *string, char from, char to) {
	char *scan;

	if (string) {
			scan = string;

			while (*scan) {
					if (*scan == from)
						*scan = to;

					scan++;
					}

			}
	return (string);
	}
/*-----------------------------------------------------------------------------*/
/*
serve a sapere se si deve interrompere il ponder: copiata da Crafty e adattata
ai miei scopi; invece di usare un thread apposito per l'input usa le funzioni
GetNumberOfConsoleInputEvents() o PeekNamedPipe() per sapere se ci sono caratteri
nell'input standard.
*/
int ponder_stop(void) {
	DWORD dw;
   /*inizializzazione*/
   if (mizar.ponder_init==0) {
	   mizar.ponder_init=1;
	   /*Recupera l'handle dello standard input*/
	   mizar.inh=GetStdHandle(STD_INPUT_HANDLE);
	   /*ritorna in dw il modo corrente dell'input*/
	   mizar.ponder_pipe=!GetConsoleMode( mizar.inh, &dw);
	   /*se la funzione precedente NON ha fallito...*/
	   if (mizar.ponder_pipe==0) {
		   /*...settiamo noi la modalità di input dell'input standard
		     disabilitando il mouse ed eventuali cambiamenti di grandezza del buffer*/
		   SetConsoleMode(mizar.inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
           /*svuotiamo il buffer*/
	       FlushConsoleInputBuffer(mizar.inh);
		   }
	  }
   /*controlla l'input standard senza bloccarsi*/
   if (mizar.ponder_pipe) {
	   /*copia i dati da una pipe anonima ad un buffer senza cancellarli dalla pipe:
	     a noi interessa solo il numero di byte disponibili */
	   if (!PeekNamedPipe(mizar.inh, NULL, 0, NULL, &dw, NULL)) {
		   /*in caso di errore ferma il ponder per sicurezza*/
		   return 1;
		   }
	   return dw;
	   }
   else {
	   GetNumberOfConsoleInputEvents(mizar.inh, &dw);
	   return dw <= 1 ? 0 : dw;
	   }
}
/*-----------------------------------------------------------------------------*/