//
// Created by Tiago on 1/14/21.
//

#include <stdio.h>
#include <string.h>
#include "perguntas.h"

PERGUNTAS carregar_perguntas_do_ficheiro() {
	char buffer[1024]; // começa por alocar um buffer com 1024 carateres para ler o tamanho de uma linha inteira
	FILE *ficheiro = abrir_ficheiro_pelo_nome(); // chama a funçao abrir_ficheiro que o trata de abrir
	// criar estrutura
	PERGUNTAS perguntas;
	perguntas.numero_perguntas = 0;
	perguntas.numero_categorias = 0;
	//Ignorar a marca BOM (identificador) única aos ficheiros csv
	fseek(ficheiro, 3 * sizeof(char), SEEK_SET);
	int linha = 0;
	// ler as linhas do ficheiro
	while (fgets(buffer, 1024, ficheiro) != NULL) {
		linha++;
		char *token = strtok(buffer, ";"); // dividir a str em substrings pelo delimitador
		bool erro_ao_adicionar = false; // caso deia erro passa a frente a linha
		perguntas.perguntas[perguntas.numero_perguntas].index_resposta_certa = -1;  // incializar a -1 o index_resposta_certa
		for (int i = 0; i < 6; ++i) {
		    // se o token for igual a NULL é pq deu erro adicionar
			if (token == NULL) {
				erro_ao_adicionar = true;
				break;
			}
			switch (i) {
				case 0: {
					int index_categoria = adicionar_categoria(token, &perguntas); // index_categoria vai ser igual ao resultado da funçao adicionar..
					perguntas.perguntas[perguntas.numero_perguntas].index_categoria = index_categoria;
					if (index_categoria == -1) {
						erro_ao_adicionar = true;
						goto cleanup;
					}
					break;
				}
				case 1:
					strncpy(perguntas.perguntas[perguntas.numero_perguntas].pergunta, token, CARATERES_MAXIMO_PERGUNTA); // copia o token para a pergunta atual
					break;
					// case 2, 3 e 4 fazem a mesma coisa
				case 2:
				case 3:
				case 4:
					strncpy(perguntas.perguntas[perguntas.numero_perguntas].respostas[i - 2], token,    // copiar o token para a respetiva resposta
							CARATERES_MAXIMO_RESPOSTA);
					break;
				case 5:
				    // substitui o fim da linha pelo terminador para nao ter um espaço a mais
					if (token[strlen(token) - 1] == '\n') {
						token[strlen(token) - 1] = '\0';
					}
                    // substitui o fim da linha pelo terminador para nao ter um enter a mais
					if (token[strlen(token) - 1] == '\r') {
						token[strlen(token) - 1] = '\0';
					}
					// percorrer respostas
					for (int j = 0; j < 3; ++j) {
					    // vai comparar o numero de respostas com o token
						if (strncmp(perguntas.perguntas[perguntas.numero_perguntas].respostas[j], token,
									CARATERES_MAXIMO_RESPOSTA) == 0) {
							perguntas.perguntas[perguntas.numero_perguntas].index_resposta_certa = j; // j vai ser o indice da resposta correta
							break;
						}
					}
					// se o token nao for igual a nenhuma resposta é pq deu erro
					if (perguntas.perguntas[perguntas.numero_perguntas].index_resposta_certa == -1) {
						erro_ao_adicionar = true;
						goto cleanup;
					}
					break;
				default:
					break;
			}
			token = strtok(NULL, ";"); // percorre o token, vai para o proximo dependendo da linha atual
			//ignorar espaços vazios e o delimitador se tiver dentro das aspas
			while (token != NULL && (token[0] == '"' && strlen(token) == 1)) {
				token = strtok(NULL, ";");
			}
		}
		cleanup:
		if (erro_ao_adicionar == true) {
			printf("Ocorreu um erro ao adicionar a pergunta da linha %d\n", linha);
		} else {
			perguntas.numero_perguntas++;
		}
	}
	return perguntas;
}

// retorna o indice da categoria ou -1 se nao conseguir adicionar
int adicionar_categoria(char *categoria, PERGUNTAS *perguntas) {
	if (perguntas->numero_categorias == 0) {
		perguntas->numero_categorias++;
		strncpy(perguntas->categorias[0], categoria, CARATERES_MAXIMO_CATEGORIAS); // copia a string ou copia o CARATERES_MAXIMO_CATEGORIAS
		return perguntas->numero_categorias - 1;
	} else {
		for (int i = 0; i < perguntas->numero_categorias; ++i) {
		    // para nao adicionar categorias duplicadas
			if (strcmp(perguntas->categorias[i], categoria) == 0)
				return perguntas->numero_categorias - 1;
		}
        if (perguntas->numero_categorias == NUMERO_MAXIMO_CATEGORIAS) {
            puts("Nao foi possivel adicionar uma pergunta pois excedeu o numero maximo de categorias");
            return -1;
        }
        // se nao tiver nenhuma categoria igual mas ainda tiver espaço adiciona a categoria
		perguntas->numero_categorias++;
		strncpy(perguntas->categorias[perguntas->numero_categorias - 1], categoria, CARATERES_MAXIMO_CATEGORIAS);
	}
	return perguntas->numero_categorias - 1;
}


FILE *abrir_ficheiro_pelo_nome() {
	FILE *ficheiro;
	char nome[16];
	puts("Introduza o nome do ficheiro que contêm as perguntas");
	scanf(" %16s", nome);
	ficheiro = fopen(nome, "r"); // abrir para ler
	while (ficheiro == NULL) {
		puts("O ficheiro cujo nome introduziu não pode ser encontrado.");
		puts("Introduza o nome do ficheiro denovo.");
		scanf(" %16s", nome);
		ficheiro = fopen(nome, "r");
	}
    return ficheiro;
}
