/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*Reset iniziale*/
void prepare_tt(){
	unsigned int c,i;
	TT_LIST p={0};

	for (i=0;i<SLOT_IN_TT;i++) {
		p.slot[i].valid=TT_INVALID;
		p.slot[i].best_move.mi=0;
	}
	
	for (c=0;c<mizar.o.hash_size;c++){
		hash_table[c]=p;
	}
}

/*Reset tra una iterazione e l'altra*/
void reset_tt(){
	unsigned int c,i;
		
	for (c=0;c<mizar.o.hash_size;c++){
		for (i=0;i<SLOT_IN_TT;i++) {
			if(hash_table[c].slot[0].valid==TT_VALID)
				hash_table[c].slot[0].valid=TT_OLD;
		}
	}
}

/*recupera l'indice dello slot della tabella*/
int probe_tt(const unsigned int index) {
	int c,val;
	
	val=-1;
	for (c=0;c<SLOT_IN_TT;c++) {
		if (hash_table[index].slot[c].valid) {
			if (Hash==hash_table[index].slot[c].key) {
				hash_table[index].slot[c].valid=TT_VALID;
				val=c;
				/*if (depth<=hash_table[index].slot[c].depth){
					val=c;
				}*/
			}
		}
	}
	return val;
}

/*memorizza la posizione nella tabella di trasposizione*/
void store_tt(const int index,const int score, const int depth,const int flag,const MOVE best){
	int c,mem,d,p;
	c=0;
	mem=0;
	/*Calcolata la posizione, può accadere:
	- la posizione è libera : memorizza
	- la posizione è occupata da dati riguardanti la stessa posizione: memorizza se depth>=hash_depth
	- la posizione è occupata da dati non riguardanti la stessa posizione: memorizza nell'ultimo slot*/
	while (c<SLOT_IN_TT) {
		if (hash_table[index].slot[c].valid!=TT_VALID) {
			hash_table[index].slot[c].valid=TT_VALID;
			hash_table[index].slot[c].best_move=best;
			hash_table[index].slot[c].depth=depth;
			hash_table[index].slot[c].flag=flag;
			hash_table[index].slot[c].key=Hash;
			hash_table[index].slot[c].score=score;
			mem=1;
		}
		else {
			if (hash_table[index].slot[c].key==Hash) {
				if (depth>=hash_table[index].slot[c].depth) {
					hash_table[index].slot[c].best_move=best;
					hash_table[index].slot[c].depth=depth;
					hash_table[index].slot[c].flag=flag;
					hash_table[index].slot[c].score=score;
				}
				mem=1;
			}
		}
		c++;
	}
	if (mem==0) {
		c=1;
		d=hash_table[index].slot[0].depth;
		p=0;
		do {
			if (hash_table[index].slot[c].depth<d) {
				d=hash_table[index].slot[c].depth;
				p=c;
			}
			c++;
		} while (c<SLOT_IN_TT);
		hash_table[index].slot[p].valid=TT_VALID;
		hash_table[index].slot[p].best_move=best;
		hash_table[index].slot[p].depth=depth;
		hash_table[index].slot[p].flag=flag;
		hash_table[index].slot[p].key=Hash;
		hash_table[index].slot[p].score=score;
	}
}