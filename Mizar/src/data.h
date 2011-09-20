#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

extern const unsigned char bit_mask[8];
extern const unsigned char castle_mask[64];

extern const int b64to256[B2_SIZE];
extern const int b256to64[B_SIZE];

extern int knight_mob[64];
extern int bishop_mob[64];

extern unsigned __int64 zobrist[2][8][64];
extern unsigned __int64 zobrist_enp[64];
extern unsigned __int64 zobrist_castle[16];
extern unsigned __int64 zobrist_side;

extern const unsigned char table_attack[256];
extern const unsigned __int16 i2b[16];
extern unsigned __int8 b2n[256];

extern const int direction[256];

extern unsigned __int64 bit_square[256];
extern unsigned __int64 bit_ray[256][256];
extern unsigned __int64 bit_scan[256][8];
extern unsigned __int64 bit_file[256];
extern unsigned __int64 bit_rank[256];
extern unsigned __int64 bit_isolated[256];
extern unsigned __int64 bit_backward[2][256];
extern unsigned __int64 bit_passed[2][256];
extern unsigned __int64 bit_weaksquare[2][256];

extern const int sq_color[64];

extern int piece_value[8];
extern const int piece_value2[8];
extern int value[64];

extern TREE tree;
extern ENGINE mizar;

extern EVAL_HASH* eval_hash;
extern unsigned eval_hash_mask;
extern TT_LIST* hash_table;
extern unsigned __int16 attacked_by_low[2][16];
extern unsigned __int16 attacked_by_hi[2][16];
extern unsigned __int16 attacked_by_eq[2][16];
extern unsigned __int16 pw_attack_pattern[2];




#endif          /* DATA_H_INCLUDED */
