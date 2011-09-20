#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <type.h>               /* for MOVE definition */

extern char *move2str(MOVE m);
extern char *strconvch(char *string, char from, char to);
extern char *strcrop(char *string);
extern char *strlwc(char *string);
extern char *strskp(const char *string);
extern int attacked(const int target, const int color);
extern int choose(const unsigned int from, const unsigned int stop);
extern int choose2(const unsigned int from, const unsigned int stop);
extern int choose3(const unsigned int from, const unsigned int stop);
extern int count_attack(const int target, const int color);
extern int eval(const char fast);
extern int eval_bishop(const int p, const int color);
extern int eval_king_safety(const int sq, const int color);
extern int eval_knight(const int p, const int color);
extern int eval_kshield(const int p, const int enemy_color, const int flag);
extern int eval_pawns(const int p, const int color);
extern int eval_pawn_struct(const int color);
extern int eval_phase(const int color);
extern int eval_queen(const int p, const int color, const int str);
extern int eval_rook(const int p, const int color);
extern int find_ini_section(FILE * inifile, char *section, const int reset);
extern int in_check(const int color);
extern int lexcmp(const char *string1, const char *string2);
extern int makemove(const MOVE m, const unsigned char s);
extern int myread(FILE * stream, char *string);
extern int quiesce(int alpha, int beta, int qdepth);
extern int repetition(void);
extern int scan_ini_section(FILE * inifile, char **keyword, char **value);
extern int search(int alpha, int beta, int depth);
extern int stop_search(void);
extern int verify_move(const MOVE m);
extern unsigned __int64 rand64(void);
extern unsigned int find_best_move(const int output_mode,const int search_mode);
extern unsigned int select_move(const int hm,const unsigned int index,const int sl);
extern unsigned int str2move(char *s, const int control);
extern unsigned int time_elapsed(void);
extern void bench(void);
extern void check_condition(void);
extern void check_correct(void);
extern int check_game(void);
extern void gen_all(void);
extern void gen_all_captures(void);
extern void gen_knowledge(void);
extern void gen_piece_captures(const int start);
extern void gen_piece_moves(const int start);
extern void heuristic(const MOVE m, const int depth);
extern void init_bitboard(void);
extern void init_board(void);
extern void init_data(void);
extern void init_zobrist(void);
extern void ins_b_oo(void);
extern void ins_b_ooo(void);
extern void ins_cap(const int f, const int t, const char p);
extern void ins_king_cap(const int f, const int t);
extern void ins_king_move(const int f, const int t);
extern void ins_move(const int f, const int t, const char p);
extern void ins_pawn_cap(const int f, const int t, const char p);
extern void ins_pawn_cap_promo(const int f, const int t);
extern void ins_pawn_cap_queen_promo(const int f, const int t);
extern void ins_pawn_dbmove(const int f, const int t, const char p);
extern void ins_pawn_enp(const int f, const int t, const char p);
extern void ins_pawn_move(const int f, const int t, const char p);
extern void ins_pawn_move_promo(const int f, const int t);
extern void ins_pawn_move_queen_promo(const int f, const int t);
extern void ins_w_oo(void);
extern void ins_w_ooo(void);
extern void load_option(void);
extern void order(void);
extern void perf_gen(void);
extern void perf_t(char dep);
extern void perft(char dep);
extern void print_bit(const unsigned __int64 bit);
extern void print_bitboard(void);
extern void print_board(void);
extern void set_bitboard_position(void);
extern void set_board_eval(void);
extern void set_fen(char *f);
extern void set_hashkey(void);
extern void set_position(char *p);
extern void shell(void);
extern void test(void);
extern void test_eval(void);
extern void unmakemove(void);
extern void xboard(void);

extern void root(void);
extern int search_root(int alpha,int beta,int depth);
extern void sort_root_nodes(void);

extern void init_ttable(void);
extern void on_quit(void);

extern void prepare_tt(void);
extern void reset_tt(void);
extern int probe_tt(const unsigned int index);
extern void store_tt(const int index,const int score, const int depth,const int flag,const MOVE best);
extern void init_evcache(void);

extern int ponder_stop(void);

extern int margin(void);
extern void set_attacked_by_array(void);

#endif          /* PROTO_H_INCLUDED */
