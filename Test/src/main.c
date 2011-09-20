/*--------------------------------------*/
/*                                      */
/* Mizar - Winboard Chess Engine        */
/*                                      */
/* 2006 (c) Nicola Rizzuti - Italy      */
/*                                      */
/*--------------------------------------*/

#include <mizar.h>


void shell();

/*Funzione principale*/
__cdecl main(int argc, char* argv[]) {

	printf("Initialization, please wait\n");
	load_option();
	init_zobrist();
	init_bitboard();
	if (mizar.o.use_hash) init_ttable();
	if (mizar.o.use_ev_cache) init_evcache();

	
	printf("\n");
	printf("===========================\n");
	printf("Mizar %s chess engine\n", MIZAR_VERSION);
	printf("%s Nicola Rizzuti - Italy\n", MIZAR_YEAR);
	printf("===========================\n");
	printf("\n");
	printf("%s \n", MIZAR_WEB);
	printf("%s \n", MIZAR_EMAIL);

	shell();

	on_quit();

	return 0;
	}

/*Interfaccia*/
void shell() {
	char c[256];

	/*ciclo principale*/

	while (1) {
			printf("\nType test to enter in testing mode\n");
			printf("Type quit to exit\n");
			printf("Mizar> ");
			scanf("%s", c);

			if (!strcmp(c, "test")) {
					test();
					}

			/*From:Chess Engine Communication Protocol
			This command will be sent once immediately
			after your engine process is started. You can
			use it to put your engine into "xboard mode" if
			that is needed. If your engine prints a prompt to ask
			for user input, you must turn off the prompt and output
			a newline when the "xboard" command comes in.
			*/

			if (!strcmp(c, "xboard")) {
					xboard();
					return ;
					}

			if (!strcmp(c, "quit")) {
					printf("\nBye!!\n");
					break;
					}

			} /*end while*/
	}
