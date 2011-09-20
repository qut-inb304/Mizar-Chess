#ifndef MACRO_H_INCLUDED
#define MACRO_H_INCLUDED
/*Tree Macro*/
#define Side (tree.side)
#define Xside (tree.xside)
#define Ply (tree.ply)
#define N_moves (tree.n_moves)
#define Nodes (tree.nodes)

/*Position Macro*/
#define Enp (tree.position.enp)
#define Cstr (tree.position.castle.castle_right)
#define Cste (tree.position.castle.castle_exe)
#define Hash (tree.position.hash_position)
#define Material(x) (tree.position.board_eval.material[(x)])
#define Phase(x) (tree.position.board_eval.root_phase[(x)])
#define Hang(x) ( tree.position.board_eval.hang[(x)].piece)
#define Hang_p(x) ( tree.position.board_eval.hang[(x)].pos)

#define Num_piece(x,y) (tree.position.board_eval.num_pieces[(x)][(y)])
#define Color(x) (tree.position.board[(x)].col) /*dato numero casella->colore pezzo*/
#define Piece(x) (tree.position.board[(x)].piece) /*dato numero casella->tipo pezzo*/
#define Plist(x) (tree.position.board[(x)].poslist) /*dato numero casella->posizione pezzo nella lista pezzi*/
#define Position(x,y) (tree.position.piece_list[(x)][(y)].pos) /*data posizione pezzo nella lista pezzi->casella*/
#define Active(x,y) (tree.position.piece_list[(x)][(y)].active)/*data posizione pezzo nella lista pezzi->è in gioco?*/
#define Name(x,y) (Piece(Position(x,y)))/*data posizione pezzo nella lista pezzi->tipo pezzo*/
#define Bitboard(x) (tree.position.bitboard[(x)])
#define Pbitboard(x) (tree.position.pbitboard[(x)])
#define ATable(x,y) (tree.position.board_eval.ab[(x)][b256to64[(y)]].ci)

#define SetBB(x,y) (tree.position.bitboard[(x)]|=bit_square[(y)])
#define DelBB(x,y) (tree.position.bitboard[(x)]^=bit_square[(y)])
#define SetPBB(x,y) (tree.position.pbitboard[(x)]|=bit_square[(y)])
#define DelPBB(x,y) (tree.position.pbitboard[(x)]^=bit_square[(y)])

/*Backup Macro*/
#define BHash(x) (tree.history[(x)].hash)
#define BBitb(x,y) (tree.history[(x)].bitboard[(y)])
#define BPBitb(x,y) (tree.history[(x)].pbitboard[(y)])

#define BCste(x) (tree.history[(x)].cast.castle_exe)
#define BCapP(x) (tree.history[(x)].captured.piece)
#define BCapPos(x) (tree.history[(x)].captured.poslist)
#define BCapC(x) (tree.history[(x)].captured.col)

#define Rank(x)  ((x) >> 4) /*dato numero casella->numero riga*/
#define File(x)  ((x) & 15) /*dato numero casella->numero fila*/

#define FileDistance(x,y) (abs(File(x)-File(y)))
#define RankDistance(x,y) (abs(Rank(x)-Rank(y)))
#define Distance(x,y) (max(FileDistance(x,y),RankDistance(x,y)))




#define IsLeft(x,y)  ((File(x))<(y))
#define IsRight(x,y)  ((File(x))>(y))
#define IsUp(x,y)  ((Rank(x))<(y))
#define IsDown(x,y)  ((Rank(x))>(y))

#define IsEmpty(x) (Piece(x)==no_piece)
#define IsOnBoard(x) (Piece(x)>out_board)
#define IsFull(x) (Piece(x)>no_piece)
#define IsEnemy(x) (Color(x)==Xside)
#define IsWhite(x) (Color(x)==WHITE)
#define IsBlack(x) (Color(x)==BLACK)
#define IsWPawn(x) (Piece(x)==wpawn)
#define IsBPawn(x) (Piece(x)==bpawn)

#define Kind(x,y) (bit_mask[(Name(x,y))])
#define IsNoSlide(x,y) ((Kind(x,y))&(bitn|bitk))
#define IsSlide(x,y) ((Kind(x,y))&(bitb|bitr|bitq))

#define UpdateHash(x) (Hash ^= zobrist[Color(x)][Piece(x)][b256to64[(x)]])
#define UpdateHashCastle(x) (Hash ^= zobrist_castle[(x)])

#define UpdateAB(x,y,z) (tree.position.board_eval.ab[(x)][b256to64[(y)]]|=i2b[(z)])

#define Change(x) ((x)^=1)

#endif          /* MACRO_H_INCLUDED */
