/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>
#define ORDER_TIME 90
#define ORDER_DEPTH 9

/*
E' una funzione che serve a contenere i vari test per il debugging di Mizar
*/
void test() {
	char input[256];
	char c = 0, exit, dep;
	exit = 1;
	printf("============\n");
	printf("TESTING MODE\n");
	printf("============\n");

	do {
			puts("\nType quit to return to main menu\noptions: bench, check, eval, order, perf, perft, quit\n");
			printf("Mizar> ");
			scanf("%s", input);

			if (!strcmp(input, "quit"))
				c = 0;

			if (!strcmp(input, "perf"))
				c = 1;

			if (!strcmp(input, "check"))
				c = 2;

			if (!strcmp(input, "perft"))
				c = 3;

			if (!strcmp(input, "bench"))
				c = 4;

			if (!strcmp(input, "order"))
				c = 5;

			if (!strcmp(input, "eval"))
				c = 6;



			switch (c) {
					case 0:
						exit = 0;
						break;
					case 1:
						perf_gen();
						break;
					case 2:
						check_correct();
						break;
					case 3:

						while (1) {
								printf("depth(0=quit)?:");
								scanf("%d", &dep);

								if (dep == 0)
									break;

								perft(dep);
								}

						break;
					case 4:
						bench();
						break;
					case 5:
						order();
						break;
					case 6:
						test_eval();
						break;
					default:
						puts("That is not one of the listed options.");

					}

			}

	while (exit);
	}

/*Verifica le prestazioni del motore di generazione delle mosse
*/
void perf_gen() {
	char p[] = "r2qk2r/ppp2ppp/2np1n2/2b1p1B1/2B1P1b1/2NP1N2/PPP2PPP/R2QK2R w KQkq - 0 1";
	/*char p[]="rnbqkbnr/ppp2ppp/8/3pp3/3PP3/8/PPP2PPP/RNBQKBNR w KQkq ";
	char p[]="8/8/p1r5/6k1/KP6/8/8/5R2 b - - ";*/
	int i, moves, start_time, end_time;
	float time, speed;

	set_position(p);
	init_data();
	print_board();
	moves = 0;
	start_time = time_elapsed();

	for (i = 0;i <= 2000000;i++) {
			gen_all();
			moves += (tree.first_move[Ply + 1]);
			}

	end_time = time_elapsed();
	time = ((end_time - start_time) / (float)1000);
	speed = moves / time;
	printf("generated %d moves in %.3f sec.\n", moves, time);
	printf("move generator speed: %.3f mov/sec\n", speed);
	/*catture*/
	set_position(p);
	init_data();
	print_board();
	moves = 0;
	start_time = time_elapsed();

	for (i = 0;i <= 4000000;i++) {
			gen_all_captures();
			moves += (tree.first_move[Ply + 1]);
			}

	end_time = time_elapsed();
	time = ((end_time - start_time) / (float)1000);
	speed = moves / time;
	printf("generated %d captures in %.3f sec.\n", moves, time);
	printf("capture generator speed: %.3f mov/sec\n", speed);

	}
/*Verifica la correttezza della funzione di scacco*/
void check_correct() {
	char p0[] = "1k1r4/pp1b1R2/3qp1pp/8/3B4/4Q3/PPP2B2/3K4 w - - 0 1";
	char p1[] = "1k1r4/pp1b1R2/3qp1pp/8/3B4/3Q4/PPP2B2/3K4 w - - 0 1";

	set_position(p0);
	init_data();
	print_board();
	printf("In this position white king is under x attack\n");
	printf("Mizar saies:\n");
	check_condition();

	if (tree.check[Side][Ply])
		printf("king is in check\n");
	else
		printf("king is not in check\n");

	if (tree.verify[Ply])
		printf("do legality test in makemove\n");
	else
		printf("No legality test in makemove\n");

	set_position(p1);

	init_data();

	print_board();

	printf("In this position white king is not under x attack\n");

	printf("Mizar saies:\n");

	check_condition();

	if (tree.check[Side][Ply])
		printf("king is in check\n");
	else
		printf("king is not in check\n");

	if (tree.verify[Ply])
		printf("do legality test in makemove\n");
	else
		printf("No legality test in makemove\n");

	}

void perft(char dep) {
	int start_time, end_time;
	char num[65];
	float time;
	/*char p0[]="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	char p0[]="8/PPP4k/8/8/8/8/4Kppp/8 w - - ";*/
	/*8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - - 0 1*/
	/*char p0[]="8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - - 0 1";*/
	/*8/5kpp/2K5/p5q1/6P1/5P2/8/2q5 w - - 0 53*/
	/*char p0[]="8/5kpp/2K5/p5q1/6P1/5P2/8/2q5 w - - 0 53";

	char p0[]="8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";
	

		
	char p0[]="8/2p5/K2p4/1P5r/1R3p1k/8/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/1P5r/KR3p1k/8/4P1P1/8 b - - 0 1";//15----30


	char p0[]="8/2p5/3p4/KP5r/R4p1k/8/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/KP5r/5p1k/1R6/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/KP5r/5p1k/8/1R2P1P1/8 b - - 0 1";//16
	char p0[]="8/2p5/3p4/KP5r/5p1k/8/4P1P1/1R6 b - - 0 1";//16
	char p0[]="8/2p5/3p4/KP5r/2R2p1k/8/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/KP5r/3R1p1k/8/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/KP5r/4Rp1k/8/4P1P1/8 b - - 0 1";//15
	char p0[]="8/2p5/3p4/KP5r/5R1k/8/4P1P1/8 b - - 0 1";//2-----109

	char p0[]="8/2p5/3p4/KP5r/1R3p1k/4P3/6P1/8 b - - 0 1";//15 
	char p0[]="8/2p5/3p4/KP5r/1R2Pp1k/8/6P1/8 b - e3 0 1";//17----32 ERROR???

	char p0[]="8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - - 0 1";//4
	char p0[]="8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1";//18-----22
	*/
	init_board();
	/*set_position(p0);*/

	init_data();
	print_board();

	print_bitboard();
	start_time = time_elapsed();
	perf_t(dep);
	end_time = time_elapsed();
	_ui64toa(Nodes,num,10);
	time = ((end_time - start_time) / (float)1000);
	printf("\nmoves %s in %.3f sec.\n", num, time);
	}

void perf_t(char dep) {
	unsigned int i;
	/*char dummy;
	gen_all_captures();*/
	gen_all();
	check_condition();
	dep--;

	for (i = tree.first_move[Ply];i < tree.first_move[Ply + 1];i++) {
		if (verify_move(tree.move_list[i].move)==0) continue;
			if (makemove(tree.move_list[i].move,0)) {
					/*print_board();
					printf("Press any key to continue:");
					 while (!kbhit());
					dummy=getch();*/
					Nodes++;

					if (dep)
						perf_t(dep);

					unmakemove();
					}

			}
	}

void bench() {
	
	/*char p[] = "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - - 0 1";*/
	char p[] = "r2q1rk1/1ppnbppp/p2p1nb1/3Pp3/2P1P1P1/2N2N1P/PPB1QP2/R1B2RK1 b - - ";
	/*char p[] = "8/pR1b1kpp/8/8/3p2n1/3P4/P4qPP/Q6K b - - 0 28";
	char p[]="8/5kpp/2K5/p5q1/6P1/5P2/8/2q5 w - - 0 53";
	2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - - 0 1
	*/
	double nps;
	unsigned int start_time, end_time, delta, c;
	nps = 0;
	delta = 0;

	for (c = 0;c < 5;c++) {
			set_position(p);
			init_data();
			print_board();
			tree.max_depth = 8;
			tree.delta_time = (10 * 60 * 1000);
			start_time = time_elapsed();
			find_best_move(IO_CONSOLE,NEW_SEARCH);
			end_time = time_elapsed();
			nps += Nodes;
			delta += (end_time - start_time);
			}

	nps /= (double)(delta);
	nps *= 1000.0;
	printf("Nodes per second: %d\n", (int)nps);
	}

/*268999693
cn    1 1772675 - 2 158835411 - 3 264778321
no cn 1 4041820 - 2 47941220  - 3 130971796
96492907  (9) 1876773365
11453430  (9)  551555736
*/

void order() {
	char p1[] = "1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - - ";
	char p2[] = "3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w - - ";
	char p3[] = "2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b - - ";
	char p4[] = "rnbqkb1r/p3pppp/1p6/2ppP3/3N4/2P5/PPP1QPPP/R1B1KB1R w KQkq - ";
	char p5[] = "r1b2rk1/2q1b1pp/p2ppn2/1p6/3QP3/1BN1B3/PPP3PP/R4RK1 w - - ";
	char p6[] = "2r3k1/pppR1pp1/4p3/4P1P1/5P2/1P4K1/P1P5/8 w - - ";
	char p7[] = "1nk1r1r1/pp2n1pp/4p3/q2pPp1N/b1pP1P2/B1P2R2/2P1B1PP/R2Q2K1 w - - ";
	char p8[] = "4b3/p3kp2/6p1/3pP2p/2pP1P2/4K1P1/P3N2P/8 w - - ";
	char p9[] = "2kr1bnr/pbpq4/2n1pp2/3p3p/3P1P1B/2N2N1Q/PPP3PP/2KR1B1R w - - ";
	char p10[] = "3rr1k1/pp3pp1/1qn2np1/8/3p4/PP1R1P2/2P1NQPP/R1B3K1 b - - ";
	char p11[] = "2r1nrk1/p2q1ppp/bp1p4/n1pPp3/P1P1P3/2PBB1N1/4QPPP/R4RK1 w - - ";
	char p12[] = "r3r1k1/ppqb1ppp/8/4p1NQ/8/2P5/PP3PPP/R3R1K1 b - - ";
	char p13[] = "r2q1rk1/4bppp/p2p4/2pP4/3pP3/3Q4/PP1B1PPP/R3R1K1 w - - ";
	char p14[] = "rnb2r1k/pp2p2p/2pp2p1/q2P1p2/8/1Pb2NP1/PB2PPBP/R2Q1RK1 w - - ";
	char p15[] = "2r3k1/1p2q1pp/2b1pr2/p1pp4/6Q1/1P1PP1R1/P1PN2PP/5RK1 w - - ";
	char p16[] = "r1bqkb1r/4npp1/p1p4p/1p1pP1B1/8/1B6/PPPN1PPP/R2Q1RK1 w kq - ";
	char p17[] = "r2q1rk1/1ppnbppp/p2p1nb1/3Pp3/2P1P1P1/2N2N1P/PPB1QP2/R1B2RK1 b - - ";
	char p18[] = "r1bq1rk1/pp2ppbp/2np2p1/2n5/P3PP2/N1P2N2/1PB3PP/R1B1QRK1 b - - ";
	char p19[] = "3rr3/2pq2pk/p2p1pnp/8/2QBPP2/1P6/P5PP/4RRK1 b - - ";
	char p20[] = "r4k2/pb2bp1r/1p1qp2p/3pNp2/3P1P2/2N3P1/PPP1Q2P/2KRR3 w - - ";
	char p21[] = "3rn2k/ppb2rpp/2ppqp2/5N2/2P1P3/1P5Q/PB3PPP/3RR1K1 w - - ";
	char p22[] = "2r2rk1/1bqnbpp1/1p1ppn1p/pP6/N1P1P3/P2B1N1P/1B2QPP1/R2R2K1 b - - ";
	char p23[] = "r1bqk2r/pp2bppp/2p5/3pP3/P2Q1P2/2N1B3/1PP3PP/R4RK1 b kq - ";
	char p24[] = "r2qnrnk/p2b2b1/1p1p2pp/2pPpp2/1PP1P3/PRNBB3/3QNPPP/5RK1 w - -";

	unsigned __int64 n;
	n = 0;
	set_position(p1);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;
	set_position(p2);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p3);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p4);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p5);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p6);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p7);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p8);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p9);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p10);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p11);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p12);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p13);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p14);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p15);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p16);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p17);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p18);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p19);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p20);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p21);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p22);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p23);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	set_position(p24);
	init_data();
	print_board();
	tree.max_depth = ORDER_DEPTH;
	tree.delta_time = (ORDER_TIME * 60 * 1000); 
	find_best_move(IO_CONSOLE,NEW_SEARCH);
	n += Nodes;

	printf("Nodes : %llu\n", n);

	}

/*verifica che la valutazione del bianco e del nero sia simmetrica*/
void test_eval() {
	/* Materiale:equal
	 KS:equal OK
	 PS:black (White isolated and hang)*/
	char p[] = "2rq1rk1/pb2bppp/1pn1p3/2p5/3P4/P1PBPN2/1B2QPPP/3R1RK1 w - - 0 1";
	/*char fp[]="3r1rk1/1b2qppp/p1pbpn2/3p4/2P5/1PN1P3/PB2BPPP/2RQ1RK1 w - - 0 1";*/
	/*char p[] = "8/PPP4k/8/8/8/8/4Kppp/8 w - - ";*/

/*	 Materiale:black (bishop pair)
	 KS:equal
	 PS:black (white weak white squares)*/
	 /*char p[]="rnbqkb1r/pppp1ppp/8/4P3/3P4/3Q4/PP3PPP/RNB1K1NR b KQkq - 0 1";*/

	/* Materiale:black (bishop pair)
	 KS:equal
	 PS:black (white weak white squares isol back)*/
	/*char p[]="r2qkb1r/1p1b3p/p3ppp1/2p5/2P5/4P2Q/PB1P1PPP/RN3RK1 b k - 0 1";*/
	

	/* Materiale:equal
	 KS:equal
	 PS:equal*/
	/*char p[]="r2q1rk1/pb1nbppp/1p2pn2/8/1P6/P4NP1/1B1NPPBP/R2Q1RK1 w - - 0 0";*/

	/* Materiale:
	 KS:black
	 PS:black--->sbagliato*/
	/*char p[]="2br1rk1/pp2bppp/3p2n1/3P4/4P1n1/1B3NN1/PP1B2PP/R3R1K1 w - - 0 0";*/

	/* Materiale:
	 KS:white
	 PS:*/
	/*char p[]="R1n5/5rkp/3p2p1/2rNpq2/1NP4b/7P/4QPP1/5RK1 b - - 0 1";*/

	int score;
	  
	set_position(p);
	init_data();
	print_board();
	print_bitboard();
	/*Precalcoliamo*/
	Phase(WHITE) = eval_phase(WHITE);
	Phase(BLACK) = eval_phase(BLACK);
	score = eval(0);
	printf("eval %d\n", score);

	/*set_position(fp);
	init_data();
	print_board();
	print_bitboard();
	Phase(WHITE)=eval_phase(WHITE);
	Phase(BLACK)=eval_phase(BLACK);
	score=eval(0);
	printf("eval %d",score);*/
}
