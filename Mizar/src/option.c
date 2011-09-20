/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*Parts copyright (c) 1991-2000 iMatix Corporation*/
/*--------------------------------------*/


#include <mizar.h>

static char iniline[MAX_LINE + 1]; /*linea del file ini*/
static char ini_section[MAX_LINE + 1]; /*nome sezione*/
static char ini_keyword[MAX_LINE + 1]; /*nome valore*/
static char ini_value[MAX_LINE + 1]; /*valore*/


int find_ini_section(FILE *inifile, char *section, const int reset);
int scan_ini_section(FILE *inifile, char **keyword, char **value);

/*gestisce il file di opzioni*/
void load_option() {
	FILE* in;
	char *keyword = NULL;
	char *val = NULL;
	int c;


	/*Cerca di aprire il file "option.ini"*/
	printf("Load option file...");

	if ((in = fopen("option.ini", "rt")) == NULL) {
			printf("cannot open option file.\n");
			return ;
			}

	/*Opzioni generali*/
	if (find_ini_section(in, "General", 1)) {
			while (scan_ini_section(in, &keyword, &val)) {
					if (!strcmp(keyword, "verbose")) {
							c = atoi(val);
							mizar.o.verbose = c;
							}					

					else if (!strcmp(keyword, "use_transposition")) {
							c = atoi(val);
							mizar.o.use_hash = c;
							}

					else if (!strcmp(keyword, "show_only_pv")) {
							c = atoi(val);
							mizar.o.only_pv = c;
							}

					else if (!strcmp(keyword, "thinking_on_opponent_time")) {
							c = atoi(val);
							mizar.o.use_ponder = c;
							if (mizar.o.use_ponder) mizar.ponder_init=0;
							}

					else if (!strcmp(keyword, "transposition_tables_size")) {
							c = atoi(val);
							mizar.o.hash_size = c;
							}

					else if (!strcmp(keyword, "use_evaluation_cache")) {
							c = atoi(val);
							mizar.o.use_ev_cache = c;
							}

					else if (!strcmp(keyword, "evaluation_cache_size")) {
							c = atoi(val);
							mizar.o.ev_cache_size = c;
							}
					 
					}

			}

	/*Opzioni di Search*/
	if (find_ini_section(in, "Search", 1)) {
			while (scan_ini_section(in, &keyword, &val)) {
					if (!strcmp(keyword, "aspiration_window")) {
							c = atoi(val);
							ASPIRATION_W = c;
							}

					else if (!strcmp(keyword, "pessimistic_depth")) {
							c = atoi(val);
							mizar.o.q_depth = c;
							}

					else if (!strcmp(keyword, "do_futility_pruning")) {
							c = atoi(val);
							mizar.o.use_futility = c;
							}

					else if (!strcmp(keyword, "futility_margin")) {
							c = atoi(val);
							FUTILITY_MAR = c;
							}

					else if (!strcmp(keyword, "ext_futility_margin")) {
							c = atoi(val);
							EXT_FUTI_MAR = c;
							}

					else if (!strcmp(keyword, "do_beta_pruning")) {
							c = atoi(val);
							mizar.o.use_beta_prune = c;
							}
					}
			}

	/*funzione di valutazione*/

	for (c = 0;c < 32;c++)
		value[c] = 0;

	if (find_ini_section(in, "Evaluation", 1)) {
			while (scan_ini_section(in, &keyword, &val)) {
					if (!strcmp(keyword, "pawn")) {
							c = atoi(val);
							PAWN_VALUE = c;
							}

					else if (!strcmp(keyword, "knight")) {
							c = atoi(val);
							KNIGHT_VALUE = c;
							}

					else if (!strcmp(keyword, "bishop")) {
							c = atoi(val);
							BISHOP_VALUE = c;
							}

					else if (!strcmp(keyword, "rook")) {
							c = atoi(val);
							ROOK_VALUE = c;
							}

					else if (!strcmp(keyword, "queen")) {
							c = atoi(val);
							QUEEN_VALUE = c;
							}

					else if (!strcmp(keyword, "opening_phase")) {
							c = atoi(val);
							OPENING_PHASE = c;
							}

					else if (!strcmp(keyword, "ending_phase")) {
							c = atoi(val);
							ENDING_PHASE = c;
							}

					else if (!strcmp(keyword, "castling")) {
							c = atoi(val);
							CASTLING_BONUS = c;
							}

					else if (!strcmp(keyword, "castle_impossible")) {
							c = atoi(val);
							LOSE_CASTLING_RIGHT = c;
							}

					else if (!strcmp(keyword, "doubled_pawn")) {
							c = atoi(val);
							DOUBLED_PAWN = c;
							}

					else if (!strcmp(keyword, "isolated_pawn")) {
							c = atoi(val);
							ISOLATED_PAWN = c;
							}

					else if (!strcmp(keyword, "backward_pawn")) {
							c = atoi(val);
							BACKWARD_PAWN = c;
							}

					else if (!strcmp(keyword, "passed_pawn")) {
							c = atoi(val);
							PASSED_PAWN = c;
							}

					else if (!strcmp(keyword, "weakness_degree")) {
							c = atoi(val);
							WEAK_DEGREE = c;
							}

					else if (!strcmp(keyword, "double_bishop")) {
							c = atoi(val);
							DOUBLE_BISHOP = c;
							}

					else if (!strcmp(keyword, "blocked_passed_pawn")) {
							c = atoi(val);
							BLOCKED_PASSED_PAWN = c;
							}

					else if (!strcmp(keyword, "tandem")) {
							c = atoi(val);
							TND_RB = c;
							}

					else if (!strcmp(keyword, "outpost")) {
							c = atoi(val);
							OUTPOST = c;
							}

					else if (!strcmp(keyword, "knight_bad_mobility")) {
							c = atoi(val);
							BAD_N = c;
							}

					else if (!strcmp(keyword, "bishop_bad_mobility")) {
							c = atoi(val);
							BAD_B = c;
							}

					else if (!strcmp(keyword, "superprotection")) {
							c = atoi(val);
							SUPERPROT = c;
							}

					else if (!strcmp(keyword, "bad_knight")) {
							c = atoi(val);
							BAD_PIECE_N = c;
							}

					else if (!strcmp(keyword, "bad_bishop")) {
							c = atoi(val);
							BAD_PIECE_B = c;
							}

					else if (!strcmp(keyword, "pawn_in_center")) {
							c = atoi(val);
							PAWN_CENTR = c;
							}

					else if (!strcmp(keyword, "piece_on_strong_square")) {
							c = atoi(val);
							PIECE_ON_STRONG_SQUARE = c;
							}

					else if (!strcmp(keyword, "piece_on_open_file")) {
							c = atoi(val);
							PIECE_ON_OPENF = c;
							}

					else if (!strcmp(keyword, "piece_on_half_open_file")) {
							c = atoi(val);
							ROOK_ON_HOF = c;
							}

					else if (!strcmp(keyword, "hanging_pawns")) {
							c = atoi(val);
							HANG_P = c;
							}

					else if (!strcmp(keyword, "pawn_majority")) {
							c = atoi(val);
							MAJORITY = c;
							}

					else if (!strcmp(keyword, "weak_square_of_one_color")) {
							c = atoi(val);
							WEAK_SQUARES = c;
							}

					else if (!strcmp(keyword, "queen_vs_bad_king")) {
							c = atoi(val);
							QUEEN_STRONG = c;
							}

					else if (!strcmp(keyword, "center_control")) {
							c = atoi(val);
							CENTER_CONTROL = c;
							}

					}

			piece_value[0] = 0;
			piece_value[1] = PAWN_VALUE;
			piece_value[2] = PAWN_VALUE;
			piece_value[3] = KNIGHT_VALUE;
			piece_value[4] = BISHOP_VALUE;
			piece_value[5] = ROOK_VALUE;
			piece_value[6] = QUEEN_VALUE;
			piece_value[7] = 2000;
			TND_QN = (BISHOP_VALUE - KNIGHT_VALUE + TND_RB);
			}

	printf("done\n");
	}

/*Trova la sezione [section]*/
int find_ini_section(FILE *inifile, char *section, const int reset) {
	char *first;

	if (reset)
		fseek(inifile, 0, SEEK_SET);

	while (myread(inifile, iniline)) {
			first = strskp (iniline);

			if (*first == ';' || *first == '#' || *first == 0)
				continue;
			else if (sscanf (first, "[%[^]]", ini_section) == 1 && lexcmp(ini_section, section) == 0)
				return 1;
			}

	return 0;
	}

/*Trova la coppia nome=valore e le inserisce in keyword\value*/
int scan_ini_section(FILE *inifile, char **keyword, char **value) {
	/*int remaining;*/
	char *first, *valueptr;

	while (myread(inifile, iniline)) {
			strcrop (iniline);

			if (*(iniline) == 0)
				continue;

			/*remaining = MAX_LINE - strlen (iniline);*/

			first = strskp (iniline);

			if (*first == ';' || *first == '#' || *first == 0)
				continue;

			else if ((valueptr = strchr (first, '=')) != NULL) {
					*valueptr++ = '\0';
					strcpy (ini_keyword, strcrop (strlwc (first)));

					while (*valueptr == ' ')
						valueptr++;

					strconvch (valueptr, ';', '\0');

					strcrop (valueptr);

					strcpy (ini_value, valueptr);

					*keyword = ini_keyword;

					*value = ini_value;

					return 1;
					}

			else
				if (sscanf (first, "[%[^]]", ini_section) == 1) {
						*keyword = strlwc (ini_section);
						*value = NULL;
						return 0;
						}

			}
	*keyword = NULL;
	return 0;
	}
