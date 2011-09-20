#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED
/*Casella della scacchiera*/

typedef struct {
		char piece; /*pezzo*/
		char poslist; /*posizione lista*/
		char col; /*colore*/
		}
SQUARE;
typedef SQUARE BOARD;
/*
union BOARD{
 SQUARE sq;
 unsigned int u;
	};
*/

/*Lista dei pezzi*/

typedef struct {
		unsigned char pos; /*posizione*/
		/*char piece;pezzo
		unsigned char kind;pezzobit*/
		char active; /*attivo o catturato*/
		}
PIECES_LIST;

/*Situazione arrocco*/

typedef struct {
		unsigned char castle_right; /*diritti ad eseguirlo*/
		unsigned char castle_exe; /*eseguito*/
		}
CASTLING;

/*Mossa*/

typedef struct {
		unsigned char from; /*da*/
		unsigned char to; /*a*/
		unsigned char flag; /*flag(tipo di mossa)*/
		char piece; /*pezzo nella casella di arrivo*/
		}
MOVES;

typedef union {
		MOVES ms; /*struttura*/
		unsigned int mi;/* intero*/
		} MOVE;

/*Lista mosse*/

typedef struct {
		MOVE move; /*mossa*/
		int scorec; /*punteggio catture*/
		int scoren; /*punteggio normali*/
		}
MOVEL;


typedef struct {
    MOVE move;/*mossa*/
    int scorec;/*punteggio quiescenza*/
    unsigned int scoren;/*punteggio nodi*/
}
RMOVEL;

/*Mosse eseguite*/

typedef struct {
		unsigned __int64 hash; /*chiave hash*/
		unsigned __int64 bitboard[2]; /*la scacchiera in bit*/
		unsigned __int64 pbitboard[2]; /*la scacchiera in bit (solo pedoni)*/
		MOVE m; /*mossa fatta*/
		char was; /*pezzo da rimettere*/
		unsigned char del; /*casella su cui avviene la cattura*/
		BOARD captured; /*pezzo catturato*/
		CASTLING cast; /*arrocco*/
		unsigned char enp; /*en passant*/
		int fifty; /*50 mosse*/
		int material[2]; /*materiale*/
		}
MOVEBACK;

/*Tavole di attacco*/

typedef	struct {
    unsigned __int8 h;
    unsigned __int8 l;
}
COUNT;

typedef union {
		COUNT cb;
		unsigned __int16 ci;
		}
TATT;

typedef struct {
	int piece;
	unsigned char pos;
	}
HANG;

/*Dati per la valutazione*/

typedef struct {
		int material[2]; /*valore del materiale*/
		char num_pieces[2][8]; /*numero dei pezzi*/
		char root_phase[2]; /*fase della partita alla radice*/
		/*tavole di attacco:ogni bit indica quale pezzo
		nella lista dei pezzi attacca la casella*/
		TATT ab[2][64];
		HANG hang[2];/* valore e posizione del maggior pezzo in presa*/
		}
KNOWLEDGE;

/*Killer Heuristic*/

typedef struct {
		MOVE m1, m2;
		int score1, score2;
		}
KILLER;

/*Tabella di trasposizione*/
typedef struct {
	unsigned __int64 key;/*chiave hash per la posizione*/
	MOVE best_move;
	int score;
	unsigned char depth;
	unsigned char flag;
	unsigned char valid;
}
TT_SLOT;

typedef struct {
	TT_SLOT slot[SLOT_IN_TT];	
}
TT_LIST;

/*La posizione*/

typedef struct {
		unsigned char enp; /*cattura en-passant*/
		BOARD board[B_SIZE];/* la scacchiera*/
		PIECES_LIST piece_list[2][16]; /*lista dei pezzi*/
		unsigned char pawns_start[2];
		CASTLING castle; /*arrocco*/
		unsigned __int64 hash_position; /*chiave hash per la posizione*/
		unsigned __int64 bitboard[2]; /*la scacchiera in bit*/
		unsigned __int64 pbitboard[2]; /*la scacchiera in bit(solo pedoni)*/
		KNOWLEDGE board_eval; /*conoscenze sulla scacchiera*/
		}
POSITION;

/*Albero delle varianti*/

typedef struct {
		POSITION position;
		int side; /*tratto*/
		int xside; /*avversario*/
		unsigned int ply; /*ply(semimossa) corrente*/
		int n_moves; /*numero di mosse fatte*/
		int fifty; /*regola 50 mosse*/
		unsigned int nodes; /*numero di nodi cercati*/

		unsigned int first_move[MAX_PLY]; /*prima mossa ply corrente*/
		MOVEL move_list[MOVE_SIZE]; /*lista delle mosse*/
		unsigned int last_root_move;/*prima mossa ply corrente*/
		RMOVEL root_move_list[ROOT_MOVE_SIZE];/*lista delle mosse*/


		int check[2][MAX_PLY]; /*target-piecepos +128*/
		int verify[MAX_PLY]; /*verifica di legalita'*/
		int pv_lenght[MAX_PLY]; /*lunghezza pv*/
		MOVE pv[MAX_PLY][MAX_PLY]; /*pv*/
		MOVE old_pv[MAX_PLY]; /*pv iterazione precedente*/
		MOVEBACK history[MAX_N_MOVES]; /*mosse fatte*/

		unsigned int h_heuristic [B_SIZE][B_SIZE];/* history*/
		KILLER k_heuristic[MAX_PLY]; /*killer*/
		MOVE c_heuristic[64][64][256]; /*countermove*/

		unsigned char status[MAX_PLY]; /*stato del generatore di mosse*/
		unsigned int next[MAX_PLY]; /*numero della mossa*/
		unsigned char follow_pv; /*siamo nella pv?*/
		MOVE first_move_played[MAX_PLY];/*prima mossa giocata*/

		unsigned int start_time;
		unsigned int stop_time;
		unsigned int delta_time;
		int max_n_moves;
		char abort_search;
		int max_depth;
		char ponder_search;
		int ponder_start_depth;
		}
TREE;

typedef struct {
		char verbose;
		char use_hash;
		char use_ev_cache;
		char use_ponder;
		char only_pv;
		char use_futility;
		char use_beta_prune;
		unsigned int ev_cache_size;
		unsigned int hash_size;
		int q_depth;
		}
OPT;

 
typedef struct {
		int side;
		int output;
		int timer;
		int protocol;
		int ponder_init;
		int ponder_pipe;
		HANDLE inh;
		OPT o;
		}
ENGINE;

/*Struttura per parsing di una stringa FEN
*/
typedef struct {
		char piece[64]; /*pezzo*/
		char col[64]; /*colore*/
		char side; /*tratto*/
		unsigned char castle; /*arrocco*/
		char enp; /*en passant*/
		char ply; /*ply*/
		char nm; /*numero mosse*/
		}
FEN_POSITION;

typedef struct EVAL_HASH {
	unsigned int hash;
	int score;
		}
EVAL_HASH;
#endif          /* TYPE_H_INCLUDED */
