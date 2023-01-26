#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "perguntas.h"
#include <time.h>
#include <string.h>

#ifndef PROJETOIAPG_QUIZZ_H
#define PROJETOIAPG_QUIZZ_H
#define NUMERO_MAXIMO_JOGADORES 4
#define CARATERES_MAXIMO_NOME_JOGADOR 20

typedef struct Jogador {
	int respostas_corretas;
	int ordem_perguntas[NUMERO_MAXIMO_PERGUNTAS];
	bool token_disponivel;
	char nome[CARATERES_MAXIMO_NOME_JOGADOR];
} JOGADOR;

typedef struct Quiz {
	time_t data_inicio; // guardar data inicio
	time_t data_fim;    // guardar data fim
	int numero_jogadores;   // nr total de jogadores
	int index_categoria_a_jogar;    // determina quais as perguntas sao apresentadas ao jogador
	int numero_perguntas_a_jogar;   // quando escolher a categoria escolher quantas perguntas
	JOGADOR jogadores[NUMERO_MAXIMO_JOGADORES]; // vai a estrutura JOGADOR e guarda os jogadores
	PERGUNTAS perguntas;    // guarda as perguntas
} QUIZ;

QUIZ inicializar_quiz();

void exportar_resultados(QUIZ *quiz);

int comparar_resultados(const void *elemento1, const void *elemento2);

void print_resultados(QUIZ *quiz, FILE *stream);

void escolher_numero_de_perguntas(QUIZ *quiz);

void comecar_quiz(QUIZ *quiz);

void mostrar_pergunta(QUIZ *quiz, int index_jogador, int index_pergunta, int numero_pergunta_atual);

void shuffle_array(int *array, int tamanho);

void gerar_ordem_perguntas(QUIZ *quiz);

void escolher_categorias_a_jogar(QUIZ *quiz);

void inserir_jogadores(QUIZ *quiz);

#endif //PROJETOIAPG_QUIZZ_H
