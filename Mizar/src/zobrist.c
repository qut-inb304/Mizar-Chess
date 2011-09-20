/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*crea numeri casuali a 64 bit*/
unsigned __int64 rand64() {
	return (rand() ^ ((unsigned __int64)rand() << 15) ^ ((unsigned __int64)rand() << 30) ^
	        ((unsigned __int64)rand() << 45) ^ ((unsigned __int64)rand() << 60));
	}

/*crea la chiave hash per la posizione*/
void set_hashkey() {
	char c, i;

	Hash = 0;

	for (i = 0;i < 2;i++)
		for (c = 0;c < 16;c++) {
				if (Active(i,c))
					Hash ^= zobrist[i][Name(i, c)][b256to64[Position(i, c)]];
				}

	if (Side)
		Hash ^= zobrist_side;

	UpdateHashCastle(Cstr);

	Hash ^= zobrist_enp[b256to64[Enp]];
	}
