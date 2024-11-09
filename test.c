#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Variables partagées pour le mouvement
//pthread_mutex_t input_mutex;

int x = 1, y = 1;

void deplacer_curseur(int x, int y) {
	printf("\033[%d;%dH", y + 1, x + 1);
}

void* input_thread_func(void* arg) {
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); // Désactive l'echo et le mode canonique
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Met stdin en mode non-bloquant
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

	while (1) {
		char c;
		if (read(STDIN_FILENO, &c, 1) > 0) {
			//pthread_mutex_lock(&input_mutex);
			//printf("Touche pressée : %c\n", c);
			if (c == 'z') y--;
			if (c == 's') y++;
			if (c == 'd') x++;
			if (c == 'q') x--;
			printf("%d %d", x, y);
			//pthread_mutex_unlock(&input_mutex);
		}
		usleep(10000); // Évite de surcharger le CPU
	}

	// Rétablit les paramètres de terminal originaux
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return NULL;
}

int main() {
	pthread_t input_thread;
	//pthread_mutex_init(&input_mutex, NULL);

	// Démarre le thread de gestion d'entrée
	if (pthread_create(&input_thread, NULL, input_thread_func, NULL) != 0) {
		perror("pthread_create");
		return 1;
	}
	int lx = x, ly = y;
	int cycle = 0;
	char c[] = {'|', '/', '-', '\\'};

	// Boucle principale : ici on peut faire le rendu
	while (1) {
		// Simulation d'un rendu (par exemple un déplacement)
		//printf("Rendu en cours...\n");
		if (lx != x) {
			deplacer_curseur(lx, ly);
			printf("x");
			lx = x;
		}

		if (ly != y) {
			deplacer_curseur(lx, ly);
			printf("x");
			ly = y;
		}

		deplacer_curseur(x, y);
		printf("%c", c[cycle++]);
		if (cycle >= 4) cycle = 0;

		usleep(5000000); // Simulation du temps de rendu
	}

	//pthread_mutex_destroy(&input_mutex);
	return 0;
}
