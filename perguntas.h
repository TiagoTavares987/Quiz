#ifndef PROJETOIAPG_PERGUNTAS_H
#define PROJETOIAPG_PERGUNTAS_H

#include <stdbool.h>

#define CARATERES_MAXIMO_RESPOSTA 64
#define CARATERES_MAXIMO_PERGUNTA 256
#define NUMERO_MAXIMO_PERGUNTAS 16
#define CARATERES_MAXIMO_CATEGORIAS 16
#define NUMERO_MAXIMO_CATEGORIAS 8

typedef struct Pergunta {
	int index_categoria;
	int index_resposta_certa;
	char pergunta[CARATERES_MAXIMO_PERGUNTA];
	char respostas[3][CARATERES_MAXIMO_RESPOSTA];   // array de 3 strings das respostas
} PERGUNTA;

typedef struct Perguntas {
	int numero_perguntas;   // nr total de perguntas
	int numero_categorias;  // nr total de categorias
	char categorias[NUMERO_MAXIMO_CATEGORIAS][CARATERES_MAXIMO_CATEGORIAS]; // array de strings para as categorias
	PERGUNTA perguntas[NUMERO_MAXIMO_PERGUNTAS];    // array da estrutura pergunta que guarda as perguntas
} PERGUNTAS;

int adicionar_categoria(char *categoria, PERGUNTAS *perguntas);

PERGUNTAS carregar_perguntas_do_ficheiro();

FILE *abrir_ficheiro_pelo_nome();


#endif //PROJETOIAPG_PERGUNTAS_H
