/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*Calculate a margin for static beta pruning*/
int margin(void) {
	int c;

	/*Build table attack*/
	if (Hang(Side)==-1) {
		Hang(Side)=0;
		gen_knowledge();
		for (c=1;c<16;c++) {
			if (Active(Side,c)) {
				if (ATable(Xside, Position(Side,c))) {
					Hang(Side) = piece_value[Name(Side,c)];
					Hang_p(Side) = Position(Side,c);
					break;
					}
				}
			}
		}

	if (Hang(Side)==0)
		return 0;

	/*Hanging passed pawn in 7th or an enemy passed pawn in 7th is like hanging queen */
	if (Hang(Side)==QUEEN_VALUE) {
		if (Color(Hang_p(Side)==Side)) {
			if (Name(Side,Hang_p(Side))!=queen) {
				if (!(ATable(Side, (Hang_p(Side)))& pw_attack_pattern[Side]))
					return (QUEEN_VALUE*2/3);
				}
			}
		else {
			if (Name(Xside,Hang_p(Side))!=queen) {
				return (QUEEN_VALUE);
				}
			}
		}

	if (ATable(Xside, (Hang_p(Side)))& attacked_by_low[Side][Plist(Hang_p(Side))])
		return (Hang(Side)*2/3);

	if (ATable(Xside, (Hang_p(Side)))& attacked_by_eq[Side][Plist(Hang_p(Side))]){
		if (count_attack(Hang_p(Side),Xside)>count_attack(Hang_p(Side),Side))
			return (Hang(Side)*2/3);
		}

	if (ATable(Xside, (Hang_p(Side)))& attacked_by_hi[Side][Plist(Hang_p(Side))]) {
		if (!(ATable(Side, (Hang_p(Side)))& pw_attack_pattern[Side]))
			return  (Hang(Side)*2/3);
		}

	return 0;
}
