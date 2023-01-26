#ifndef PROJETOIAPG_UTIL_H
#define PROJETOIAPG_UTIL_H
#ifdef _WIN32
#include <windows.h>
#else

#include <stdio.h>
#include <unistd.h>
#endif

// quando se responde a uma pergunta esta funçao faz com que se espere um tempo para dps passar para a proxima
void sleep_quizz(unsigned int sleep_ms);

void limpar_ecra();

#endif //PROJETOIAPG_UTIL_H
