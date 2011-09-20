#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#define B_SIZE		256
#define B2_SIZE		64
#define MOVE_SIZE   2048
#define MAX_PLY     128
#define MAX_TIME    INT_MAX
#define MAX_N_MOVES 400
#define TIME_CHECK  1023
#define SLOT_IN_TT  3

#define ROOT_MOVE_SIZE   128
#define QDEPTH      mizar.o.q_depth

#define MAX_LINE 1024

#define IO_MUTE          0
#define IO_WINBOARD      1
#define IO_CONSOLE       2

#define TIME_CONV        0
#define TIME_INCR        1
#define TIME_EXAT        2

#define HORIZON    0
#define INF        10000
#define DRAW       0

#define TT_UPPER   0
#define TT_LOWER   1
#define TT_EXACT   2

#define TT_INVALID 0
#define TT_VALID   1
#define TT_OLD     2

#define WHITE      1
#define BLACK      0

#define FORCE_MODE  (-1)
#define PONDER_MODE (-2)

#define NEW_SEARCH      0
#define CONTINUE_SEARCH 1


#define CAPTURED   0
#define FREE       1

#define OUT_BOARD -1
#define EMPTY      2

#define BB         1
#define WB         2
#define EB         4
#define OB         8

#define out_board -1
#define no_piece   0
#define wpawn      1
#define bpawn      2
#define knight     3
#define bishop     4
#define rook       5
#define queen      6
#define king       7

#define no_list    16


#define bitwp      1
#define bitbp      2
#define bitn       4
#define bitb       8
#define bitr       16
#define bitq       32
#define bitk       64

#define cas_woo     1
#define cas_wooo    2
#define cas_boo     4
#define cas_booo    8

#define UP       (-16)
#define DOWN     (+16)
#define RIGHT    (+1)
#define LEFT     (-1)
#define UP_R     (-15)
#define UP_L     (-17)
#define DOWN_R   (+17)
#define DOWN_L   (+15)
#define KNIGHT1  (-33)
#define KNIGHT2  (-31)
#define KNIGHT3  (-18)
#define KNIGHT4  (-14)
#define KNIGHT5  (+14)
#define KNIGHT6  (+18)
#define KNIGHT7  (+31)
#define KNIGHT8  (+33)

#define BUP       (0)
#define BDOWN     (1)
#define BRIGHT    (2)
#define BLEFT     (3)
#define BUP_R     (4)
#define BUP_L     (5)
#define BDOWN_R   (6)
#define BDOWN_L   (7)

#define Riga8  4
#define Riga7  5
#define Riga6  6
#define Riga5  7
#define Riga4  8
#define Riga3  9
#define Riga2  10
#define Riga1  11

#define FilaA  4
#define FilaB  5
#define FilaC  6
#define FilaD  7
#define FilaE  8
#define FilaF  9
#define FilaG  10
#define FilaH  11

#define A1  180
#define B1  181
#define C1  182
#define D1  183
#define E1  184
#define F1  185
#define G1  186
#define H1  187

#define A2  164
#define B2  165
#define C2  166
#define D2  167
#define E2  168
#define F2  169
#define G2  170
#define H2  171

#define A3  148
#define B3  149
#define C3  150
#define D3  151
#define E3  152
#define F3  153
#define G3  154
#define H3  155

#define A4  132
#define B4  133
#define C4  134
#define D4  135
#define E4  136
#define F4  137
#define G4  138
#define H4  139

#define A5  116
#define B5  117
#define C5  118
#define D5  119
#define E5  120
#define F5  121
#define G5  122
#define H5  123

#define A6  100
#define B6  101
#define C6  102
#define D6  103
#define E6  104
#define F6  105
#define G6  106
#define H6  107

#define A7  84
#define B7  85
#define C7  86
#define D7  87
#define E7  88
#define F7  89
#define G7  90
#define H7  91

#define A8  68
#define B8  69
#define C8  70
#define D8  71
#define E8  72
#define F8  73
#define G8  74
#define H8  75

#define M_STD 0
#define M_CAP 1
#define M_OO  2
#define M_OOO 4
#define M_ENP 8
#define M_DBP 16
#define M_PAW 32
#define M_PRO 64
#define M_KNG 128

#define ST_PV_MOVE  0
#define ST_GEN_ALL  1
#define ST_TT_MOVE  2
#define ST_CPT_MOV  3
#define ST_CNT_MOV  4
#define ST_KL1_MOV  5
#define ST_KL2_MOV  6
#define ST_SRT_ALL  7
#define ST_ALL_MOV  8

#define PAWN_VALUE             (value[0])
#define KNIGHT_VALUE           (value[1])
#define BISHOP_VALUE           (value[2])
#define ROOK_VALUE             (value[3])
#define QUEEN_VALUE            (value[4])
#define OPENING_PHASE          (value[5])
#define ENDING_PHASE           (value[6])
#define CASTLING_BONUS         (value[7])
#define LOSE_CASTLING_RIGHT    (value[8])
#define DOUBLED_PAWN           (value[9])
#define ISOLATED_PAWN          (value[10])
#define BACKWARD_PAWN          (value[11])
#define PASSED_PAWN            (value[12])
#define WEAK_DEGREE            (value[13])
#define DOUBLE_BISHOP          (value[14])
#define BLOCKED_PASSED_PAWN    (value[15])
#define TND_RB                 (value[16])
#define TND_QN                 (value[17])
#define OUTPOST                (value[18])
#define BAD_B                  (value[19])
#define BAD_N                  (value[20])
#define PAWN_CENTR             (value[21])
#define BAD_PIECE_B            (value[22])
#define BAD_PIECE_N            (value[23])
#define SUPERPROT              (value[24])
#define PIECE_ON_STRONG_SQUARE (value[25])
#define PIECE_ON_OPENF         (value[26])
#define ROOK_ON_HOF            (value[27])
#define HANG_P                 (value[28])
#define MAJORITY               (value[29])
#define WEAK_SQUARES           (value[30])
#define QUEEN_STRONG           (value[31])
#define CENTER_CONTROL         (value[32])


#define EXT_FUTI_MAR           (value[61])
#define FUTILITY_MAR           (value[62])
#define ASPIRATION_W           (value[63])

#endif          /* DEF_H_INCLUDED */
