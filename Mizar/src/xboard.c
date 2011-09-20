/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>

/*gestisce la comunicazione con winboard*/
void xboard() {
	char parse[256], action[256];
	int mps, base, inc, cg,s,old_side;
	MOVE m,mtp,mfp;

	/*risposta al comando 'xboard'*/
	printf("\n");

	/*inizializzazioni*/
	signal(SIGINT, SIG_IGN);
	init_board();
	init_data();
	mizar.output = IO_WINBOARD;
	mizar.side = FORCE_MODE;
	old_side = mizar.side;
	mizar.protocol = 0;
	s=NEW_SEARCH;
	mtp.mi=0;
	tree.ponder_search=0;

	do {
			fflush(stdout);

			if (Side == mizar.side) {
					tree.ponder_search=0;
					m.mi = find_best_move(mizar.output,s);

					if (m.mi == 0) {
							mizar.side = FORCE_MODE;
							continue;
							}

					printf("move %s\n", move2str(m));
					makemove(m,0);
					cg=check_game();
					if (cg) mizar.side = FORCE_MODE;

					if (mizar.o.use_ponder) {
						old_side = mizar.side;
						mizar.side = PONDER_MODE;
						mtp.mi=tree.old_pv[1].mi;
					}
					
					continue;
					}
			/*Ponder*/
			if (mizar.o.use_ponder) {
				if (mtp.mi && (mizar.side == PONDER_MODE)) {
					check_condition();
					if (makemove(mtp,0)){
						/*controlla che non sia già finita*/
						cg=check_game();
						if (cg) {
							mizar.side = FORCE_MODE;
							unmakemove();
							continue;
							}
						/*altrimenti cominciamo a pensare*/
						tree.ponder_search=1;
						mfp.mi = find_best_move(mizar.output,NEW_SEARCH);
						/*siamo usciti dal ponder perchè l'avversario ha inserito un comando*/
						unmakemove();
						if (mfp.mi==0) {
							mizar.side = FORCE_MODE;						
							continue;
							}
						mizar.side=old_side;
						}
					}			
			}

			/*Input*/
			if (!fgets(parse, 256, stdin))
				return ;

			if (parse[0] == '\n')
				continue;

			sscanf(parse, "%s", action);

			if (!strcmp(action, "xboard"))
				continue;

			if (!strcmp(action, "protover")) {
					printf("feature ping=0 setboard=1 san=0 usermove=0\n");
					printf("feature playother=0 time=1 draw=0 analyze=0\n");
					printf("feature sigint=0 sigterm=0 colors=1 pause=0\n");
					printf("feature myname=\"Mizar %s\" done=1\n", MIZAR_VERSION);
					mizar.protocol = 1;
					continue;
					}


			if (!strcmp(action, "new")) {
					init_board();
					init_data();
					mizar.side = BLACK;
					continue;
					}


			if (!strcmp(action, "quit"))
				break;


			if (!strcmp(action, "force")) {
					mizar.side = FORCE_MODE;
					continue;
					}


			if (!strcmp(action, "go")) {
					mizar.side = Side;
					continue;
					}

			if (!strcmp(action, "hard")) {
					/*???*/
					continue;
					}

			if (!strcmp(action, "easy")) {
					/*???*/
					continue;
					}

			if (!strcmp(action, "white")) {
					Side = WHITE;
					Xside = BLACK;
					mizar.side = BLACK;
					continue;
					}

			if (!strcmp(action, "black")) {
					Xside = WHITE;
					Side = BLACK;
					mizar.side = WHITE;
					continue;
					}

			if (!strcmp(action, "st")) {
					sscanf(parse, "st %d", &(tree.delta_time));
					tree.delta_time *= 1000;
					/*tree.max_depth = MAX_PLY;*/
					mizar.timer = TIME_EXAT;
					continue;
					}


			if (!strcmp(action, "sd")) {
					sscanf(parse, "sd %d", &(tree.max_depth));
					/*tree.stop_time = MAX_TIME;
					mizar.timer=TIME_EXAT;*/
					continue;
					}

			if (!strcmp(action, "level")) {
					sscanf(parse, "level %d %d %d", &mps, &base, &inc);

					if (mps == 0) {
							mizar.timer = TIME_INCR;
							tree.delta_time = (base * 60 * 1000);
							tree.delta_time /= 30;
							tree.max_n_moves = 0;
							}

					else {
							mizar.timer = TIME_CONV;
							tree.delta_time = (base * 60000);
							tree.delta_time /= (mps + 1);
							tree.max_n_moves = mps;
							}

					continue;
					}


			if (!strcmp(action, "time")) {
					sscanf(parse, "time %d", &base);

					if (mizar.timer == TIME_INCR) {
							tree.delta_time = (base * 10);
							tree.delta_time /= 30;
							}

					continue;
					}

			if (!strcmp(action, "otim")) {
					continue;
					}
			
			if (!strcmp(action, "setboard")) {
				char *wh = parse;
				wh += strlen("setboard");
				while (*wh && isspace(*wh)) wh++;
				set_position(wh);
				init_data();
				mizar.side=FORCE_MODE;
            continue;
			}

			if (!strcmp(action, "post")) {
					mizar.output = IO_WINBOARD;
					continue;
					}

			if (!strcmp(action, "nopost")) {
					mizar.output = IO_MUTE;
					continue;
					}

			/*if (!strcmp(action, "usermove")) {
					m.mi = str2move(parse, 0);

					if (m.mi == 0) {
							printf("Illegal move: %s\n", move2str(m));
							}

					else {
							check_condition();

							if (!makemove(m)) {
									printf("Illegal move: %s\n", move2str(m));
									}

							else {
									
									cg=check_game();
									if (cg) mizar.side = FORCE_MODE;
									}

							}

					}*/


			
					m.mi = str2move(parse, 1);

					if (m.mi == 0) {
							printf("Error (unknown command): %s\n", action);
							}

					else {
							check_condition();

							if (!makemove(m,0)) {
									printf("Illegal move: %s\n", move2str(m));
									}

							else {
									/*Ora Side e Xside sono invertiti*/
									cg=check_game();
									if (cg) mizar.side = FORCE_MODE;
									/*confronta con la mossa ponder*/
									if (mizar.o.use_ponder) {
										if (m.mi==mtp.mi) s=CONTINUE_SEARCH;
										else s=NEW_SEARCH;
										}
									}

							}

					

			}

	while (1);
	}
