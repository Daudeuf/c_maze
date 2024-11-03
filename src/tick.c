#include "tick.h"

pthread_mutex_t input_mutex;

int char_count;
char* char_lst;

int running = 1;

void* input_thread_func(void* arg) {
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	while (running) {
		char c;

		if (read(STDIN_FILENO, &c, 1) > 0) {
			pthread_mutex_lock(&input_mutex);

			char_count++;

			if ((char_count/4*4+4) > ((char_count-1)/4*4+4)) {
				char_lst = (char*) realloc(char_lst, (char_count/4*4+4) * sizeof(char));
			}

			char_lst[char_count - 1] = c;

			pthread_mutex_unlock(&input_mutex);
		}

		usleep(10000);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return NULL;
}

void tick_start() {
	clock_t   start;
	pthread_t input_thread;
	pthread_mutex_init(&input_mutex, NULL);

	char_lst = (char*) malloc(4 * sizeof(char));
	char_count = 0;

	if (pthread_create(&input_thread, NULL, input_thread_func, NULL) != 0) {
		perror("pthread_create");
		return 1;
	}

	int cpt_frame = 0;

	while (running) {
		start = clock();

		pthread_mutex_lock(&input_mutex);

		// copy input buffer & clear input buffer
		int input_count = char_count;
		char* input_lst = (char*) malloc(input_count * sizeof(char));
		for (int i=0; i < input_count; i++) input_lst[i] = char_lst[i];

		free(char_lst);
		char_lst = (char*) malloc(4 * sizeof(char));
		char_count = 0;
		// end

		pthread_mutex_unlock(&input_mutex);



		// traitement touches
		if (input_count > 0) {
			printf("[%d] ", input_count);

			for (int i=0; i < input_count; i++) {
				printf("%c", input_lst[i]);
				if (input_lst[i] == 'q') running = 0;
			}

			printf("\n");
		}
		// end



		// game tick
		//printf("frame : %d\n", cpt_frame++);
		// end



		// tick graphique

		// end

		usleep((1000000 - (clock() - start) / CLOCKS_PER_SEC * 1000000) / MAX_FPS); // assure que le jeu tourne à 60 tps (tick par seconde)
	}

	pthread_mutex_destroy(&input_mutex);
}