/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

TREE tree;

ENGINE mizar;

const int b64to256[B2_SIZE] = { 
 68,  69,  70,  71,  72,  73,  74,  75,
 84,  85,  86,  87,  88,  89,  90,  91,
100, 101, 102, 103, 104, 105, 106, 107,
116, 117, 118, 119, 120, 121, 122, 123,
132, 133, 134, 135, 136, 137, 138, 139,
148, 149, 150, 151, 152, 153, 154, 155, 
164, 165, 166, 167, 168, 169, 170, 171, 
180, 181, 182, 183, 184, 185, 186, 187
};

const int b256to64[B_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0,
0, 0, 0, 0, 8, 9,10,11,12,13,14,15, 0, 0, 0, 0,
0, 0, 0, 0,16,17,18,19,20,21,22,23, 0, 0, 0, 0,
0, 0, 0, 0,24,25,26,27,28,29,30,31, 0, 0, 0, 0,
0, 0, 0, 0,32,33,34,35,36,37,38,39, 0, 0, 0, 0,
0, 0, 0, 0,40,41,42,43,44,45,46,47, 0, 0, 0, 0,
0, 0, 0, 0,48,49,50,51,52,53,54,55, 0, 0, 0, 0,
0, 0, 0, 0,56,57,58,59,60,61,62,63, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char castle_mask[64] = {
 7, 15, 15, 15,  3, 15, 15, 11,
15, 15, 15, 15, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15,
13, 15, 15, 15, 12, 15, 15, 14
};

const unsigned char bit_mask[8] = { 0, bitwp, bitbp, bitn, bitb, bitr, bitq, bitk };

#define H (bitr | bitq)
#define D (bitb | bitq)

/*
Per maggiori dettagli visitare il sito di Bruce Moreland.
formula per l'indirizzamento e':target-piecepos +128.
-*- x-x+128=128 =>table_attack[128]=0
pezzo attaccante e':
-*- SOPRA 134-118=16 e multipli => table_attack[128+16=144...]=re,regina,torre
                                   direction [144...]=DOWN
-*- ALTO DESTRA 134-119=15 +15 => table_attack[128+15=143 +15...]=re,regina,alf
                                   direction [143...]=DOWNL
-*- ALTO SINISTRA 134-117=17 +17 => table_attack[128+17=145 +17...]=re,regina,alf
                                   direction [145...]=DOWNR
-*- DESTRA 134-135=-1 -1 => table_attack[128-1=127 -1...]=re,regina,torre
                                   direction [127...]=LEFT
-*- SINISTRA 134-133=1 +1 => table_attack[128+1=129 +1...]=re,regina,torre
                                   direction [129...]=RIGHT
-*- SOTTO 134-150=-16 -16 => table_attack[128-16=112 -16...]=re,regina,alf
                                   direction [112...]=UP
-*- SOTTO DESTRA 134-151=-17 -17 => table_attack[128-17=111 -17...]=re,regina,alf
                                   direction [111...]=UPL
-*- SOTTO SINISTRA 134-149=-15 -15 => table_attack[128-17=113 -15...]=re,regina,alf
                                   direction [113...]=UPR
-*- cavallo : 95,97,110,114,142,146,159,161
*/
const unsigned char table_attack[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, D, 0, 0, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, 0, 0, D, 0, 0, D, 0, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, 0, D, 0, 0, 0, 0, D, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, D, 0, 0, 0, 0, 0, 0, D, 0, 0, 0,                         
    H, 0, 0, 0, D, 0, 0, 0, 0, 0, 0, 0, 0, D, 0, 0,                         
    H, 0, 0, D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, D, bitn,                      
    H, bitn, D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, bitn, bitk | D,            
    bitk | H, bitk | D, bitn, 0, 0, 0, 0, 0, 0, H, H, H, H, H, H, bitk | H, 
    0, bitk | H, H, H, H, H, H, H, 0, 0, 0, 0, 0, 0, bitn, bitk | D,        
    bitk | H, bitk | D, bitn, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, D, bitn,     
    H, bitn, D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, D, 0, 0,                      
    H, 0, 0, D, 0, 0, 0, 0, 0, 0, 0, 0, D, 0, 0, 0,                         
    H, 0, 0, 0, D, 0, 0, 0, 0, 0, 0, D, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, D, 0, 0, 0, 0, D, 0, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, 0, D, 0, 0, D, 0, 0, 0, 0, 0, 0,                         
    H, 0, 0, 0, 0, 0, 0, D, 0, 0, 0, 0, 0, 0, 0, 0 };                       

const int direction[256] = {
     0, 0, 0, 0, 0, 0, 0,    0, 0, UP_L, 0, 0, 0, 0, 0, 0,
    UP, 0, 0, 0, 0, 0, 0, UP_R, 0, 0, UP_L, 0, 0, 0, 0, 0,
    UP, 0, 0, 0, 0, 0, UP_R, 0, 0, 0, 0, UP_L, 0, 0, 0, 0,
    UP, 0, 0, 0, 0, UP_R, 0, 0, 0, 0, 0, 0, UP_L, 0, 0, 0,
    UP, 0, 0, 0, UP_R, 0, 0, 0, 0, 0, 0, 0, 0, UP_L, 0, 0,
    UP, 0, 0, UP_R, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UP_L, 0,
    UP, 0, UP_R, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, UP_L,
    UP, UP_R, 0, 0, 0, 0, 0, 0, 0, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT, LEFT,
    0, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT, 0, 0, 0, 0, 0, 0, 0, DOWN_L,
    DOWN, DOWN_R, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DOWN_L, 0,
    DOWN, 0, DOWN_R, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DOWN_L, 0, 0,
    DOWN, 0, 0, DOWN_R, 0, 0, 0, 0, 0, 0, 0, 0, DOWN_L, 0, 0, 0,
    DOWN, 0, 0, 0, DOWN_R, 0, 0, 0, 0, 0, 0, DOWN_L, 0, 0, 0, 0,
    DOWN, 0, 0, 0, 0, DOWN_R, 0, 0, 0, 0, DOWN_L, 0, 0, 0, 0, 0,
    DOWN, 0, 0, 0, 0, 0, DOWN_R, 0, 0, DOWN_L, 0, 0, 0, 0, 0, 0,
    DOWN, 0, 0, 0, 0, 0, 0, DOWN_R, 0, 0, 0, 0, 0, 0, 0, 0
};

unsigned __int64 bit_square[256];
unsigned __int64 bit_ray[256][256];
unsigned __int64 bit_scan[256][8];

unsigned __int64 bit_file[256];
unsigned __int64 bit_rank[256];

unsigned __int64 bit_isolated[256];
unsigned __int64 bit_backward[2][256];
unsigned __int64 bit_passed[2][256];
unsigned __int64 bit_weaksquare[2][256];

const int piece_value2[8]={0,1,1,3,3,5,9,0};
const unsigned __int16 i2b[16]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};
unsigned __int8  b2n[256];

int value[64];

unsigned __int64 zobrist[2][8][64];/*tabella numeri casuali*/
unsigned __int64 zobrist_enp[64];/*numero casuale per la cattura en passant*/
unsigned __int64 zobrist_castle[16];/*numero casuale per l'arrocco*/
unsigned __int64 zobrist_side;/*numero casuale per il tratto*/

const int sq_color[64] = {
	WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,
	BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,
	WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,
	BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,
	WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,
	BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,
	WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,
	BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE
};

EVAL_HASH* eval_hash;
unsigned int eval_hash_mask;
unsigned __int16 attacked_by_low[2][16];
unsigned __int16 attacked_by_hi[2][16];
unsigned __int16 attacked_by_eq[2][16];
unsigned __int16 pw_attack_pattern[2];

TT_LIST* hash_table;