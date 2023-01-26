#include "util.h"

void sleep_quizz(unsigned int sleep_ms) {
#ifdef __linux__
	usleep(sleep_ms * 1000);
#endif
#ifdef _WIN32
	Sleep(sleep_ms);
#endif
}

void limpar_ecra() {
#ifdef __linux__
	printf("\e[1;1H\e[2J");
#endif
#ifdef _WIN32
	system("cls");
#endif
}

