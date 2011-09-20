/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*init_board() sets starting position*/
void init_board() {
	char start[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	set_position(start);
	}

/*init_data() resets all data used in search()*/
void init_data() {
	int c;
	Ply = 0;
	tree.first_move[0] = 0;
	N_moves = 0;
	tree.fifty = 0;
	Nodes = 0;
	tree.max_depth = MAX_PLY;
	if (mizar.o.use_hash) prepare_tt();
	/*memset(tree.old_pv, 0, sizeof(tree.old_pv));*/
	for (c=0;c<MAX_PLY;c++)tree.old_pv[c].mi=0;
	if (mizar.o.use_ev_cache)/* memset(eval_hash,0,sizeof(eval_hash));*/
		for (c=0;c<mizar.o.ev_cache_size;c++) {
			eval_hash[c].hash=0;
			eval_hash[c].score=0;
		}
	}

/*init_zobrist() reads zobrist numbers from 'zobrist.dat'*/
void init_zobrist() {
	FILE* in;
	unsigned char c, p, sq;
	unsigned __int64 temp;
	time_t seed;
	size_t retry_count = 0;
	size_t items_read;
retry:
	/*Cerca di aprire il file "zobrist.dat"*/

	if (mizar.o.verbose) printf("Init Zobrist number...");

	if ((in = fopen("zobrist.dat", "rb")) == NULL) {
			/*se non c'e' lo crea*/

			if (mizar.o.verbose) printf("building...");

			srand ((unsigned) time(&seed));

			in = fopen("zobrist.dat", "wb");

			if (in == NULL) {
					puts("Fatal error opening zobrist.dat data file.");
					puts("Check permissions in this directory.");
					exit(EXIT_FAILURE);
					}

			for (c = 0; c < 2; c++) {
					for (p = 0; p < 8; p++) {
							for (sq = 0; sq < 64; sq++) {
									temp = rand64();
									zobrist[c][p][sq] = temp;
									fwrite(&temp, sizeof (unsigned __int64), 1, in);
									}

							}

					}
			fclose(in);

			for (sq = 0;sq < 64;sq++)
				zobrist_enp[sq] = zobrist[0][0][sq];

			for (sq = 0;sq < 16;sq++)
				zobrist_castle[sq] = zobrist[1][0][sq];

			zobrist_side = zobrist[1][0][20];

			if (mizar.o.verbose) printf("done\n");

			return ;
			}

	for (c = 0; c < 2; c++) {
			for (p = 0; p < 8; p++) {
					for (sq = 0; sq < 64; sq++) {
							items_read = fread(&temp, sizeof(unsigned __int64), 1, in);

							if (items_read != 1) {
									puts("I/O error reading zobrist data file.");
									system("del zobrist.dat");
									retry_count++;

									if (retry_count < 5) goto retry;
									else {
											puts("Error recreating zobrist random number data file.  Recreating memory version.");

											for (c = 0; c < 2; c++) {
													for (p = 0; p < 8; p++) {
															for (sq = 0; sq < 64; sq++) {
																	temp = rand64();
																	zobrist[c][p][sq] = temp;
																	}

															}

													}
											break;
											}

									}
							zobrist[c][p][sq] = temp;
							}

					}

			}
	fclose(in);

	for (sq = 0;sq < 64;sq++)
		zobrist_enp[sq] = zobrist[0][0][sq];

	for (sq = 0;sq < 16;sq++)
		zobrist_castle[sq] = zobrist[1][0][sq];

	zobrist_side = zobrist[1][0][20];

	if (mizar.o.verbose) printf("done\n");
	}
/*init_ttable() allocates memory for transposition tables*/
void init_ttable(){
	size_t hs,u;
	
	u=sizeof(TT_LIST);
	hs=((mizar.o.hash_size*1000000)/u);
	hash_table=((TT_LIST *)malloc((hs*u)));
	if(hash_table==NULL) {
		printf("Insufficient memory available\n");
	}
	else {
		if (mizar.o.verbose)
		printf("Requested %d bytes; allocated %d items of %d bytes (%d bytes)\n",(mizar.o.hash_size*1000000),hs,u,(hs*u));
	}
	mizar.o.hash_size=hs;
}
/*init_evcache() allocates memory for evaluation's cache*/
void init_evcache(){
	size_t hs,u;
	unsigned int min_req,max_req,d;
	
	u=sizeof(EVAL_HASH);
	/*controlla che sia una potenza di due*/
	if (mizar.o.ev_cache_size && !((mizar.o.ev_cache_size-1)&(mizar.o.ev_cache_size))) {
		hs=(mizar.o.ev_cache_size*u);
	}
	else {
		hs=mizar.o.ev_cache_size;
		while (!(hs && !((hs-1)&hs)))
			hs--;
		min_req=hs;
		hs=mizar.o.ev_cache_size;
		if (hs<2147483648) {
			while (!(hs && !((hs-1)&hs)))
				hs++;
		} else hs=2147483648;
		max_req=hs;
		d=(max_req-min_req)/2;
		if ((mizar.o.ev_cache_size-min_req) > d) hs = max_req;
		else hs = min_req;
		printf("Eval cache size is not a power of two; %u is near your request and it is a power of two\n",hs);
		mizar.o.ev_cache_size=hs; 
		hs*=u;
	}
	
	eval_hash=((EVAL_HASH *)malloc(hs));
	if(eval_hash==NULL) {
		printf("Insufficient memory available\n");
	}
	else {
		if (mizar.o.verbose)
		printf("Allocated %d bytes for evaluation cache\n",hs);
	}
	eval_hash_mask=mizar.o.ev_cache_size-1;
	
}
/*on_quit() is called before mizar quits. it frees memory.*/
void on_quit(){
	if (mizar.o.use_hash)
		free( hash_table );
	if (mizar.o.use_ev_cache)
		free( eval_hash );
}
