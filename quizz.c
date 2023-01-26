#include "quizz.h"

QUIZ inicializar_quiz() {
	srand(time(NULL));  // tempo atual
	// criar estrutura quiz
	QUIZ quiz;
	quiz.data_inicio = time(0); // tempo atual
	quiz.index_categoria_a_jogar = -1;  // -1 pois o utilizador nao escolheu nenhum
	quiz.perguntas = carregar_perguntas_do_ficheiro();
	limpar_ecra();
	inserir_jogadores(&quiz);
	limpar_ecra();
	escolher_categorias_a_jogar(&quiz);
	limpar_ecra();
	gerar_ordem_perguntas(&quiz);   // gerar perguntas de forma random
	comecar_quiz(&quiz);
	limpar_ecra();
	quiz.data_fim = time(0); // guarda a data em q acabei o quiz
	print_resultados(&quiz, stdout);    // print do resultado na consola do compilador
	exportar_resultados(&quiz);
	puts("Obrigado por jogar!");
	return quiz;
}

void exportar_resultados(QUIZ *quiz) {
	char buffer;
	char nome_base[] = "quizz_apg_resultados.txt";  // nome do ficheiro em que vai ser guardado os resultados
	puts("Pretende exportar os resultados? s/n");
	scanf(" %1c", &buffer); // aquilo que e escrito é copiado para a variavel buffer ------ %1c -> é so para 1 carater
	while ((buffer != 's' && buffer != 'n' && buffer != 'S' && buffer != 'N')) {
		puts("Selecao invalida, tente denovo.");
		scanf(" %1c", &buffer);
	}
	FILE *ficheiro;
	ficheiro = fopen(nome_base, "a+");  // escreve apartir do fim pois posso ter varios resultados no ficheiro
	if (ficheiro == NULL) {
		puts("Nao foi possivel exportar os dados.");
		return;
	}
	print_resultados(quiz, ficheiro);
	fclose(ficheiro);
}

void print_resultados(QUIZ *quiz, FILE *stream) {   // o parametro stream decide se escreve para uma consola ou para um ficheiro
	fputs("-----------\n", stream);
	// imprime tanto na consola ou no ficheiro
	fprintf(stream, "Data Inicio: %s\n", ctime(&quiz->data_inicio)); // ctime -> converte a data em segundos para uma string legivel
	fprintf(stream, "Data Fim: %s\n", ctime(&quiz->data_fim));
	if (quiz->numero_jogadores > 1) {
		JOGADOR resultado_ordenados[quiz->numero_jogadores];    // cria um array com o tamanho = ao nr de jogadores
		for (int i = 0; i < quiz->numero_jogadores; ++i) {
			resultado_ordenados[i] = quiz->jogadores[i];    // copia de um array para outro
		}
		qsort(resultado_ordenados, sizeof(resultado_ordenados) / sizeof(*resultado_ordenados),  // funçao da biblioteca C para ordenar ----- sizeof = nr de elementos do array
			  sizeof(*resultado_ordenados), comparar_resultados);   // ver o tamanho do primeiro e compara o resultado
		for (int i = 0; i < quiz->numero_jogadores; ++i) {
			fprintf(stream, "%d lugar - %s - %d / %d certas\n", i + 1, resultado_ordenados[i].nome,
					resultado_ordenados[i].respostas_corretas, quiz->numero_perguntas_a_jogar);
		}
	} else {
		fprintf(stream, "Jogador: %s\n", quiz->jogadores[0].nome);
		fprintf(stream, "Resultado: %d / %d\n", quiz->jogadores[0].respostas_corretas, quiz->numero_perguntas_a_jogar);
		if (quiz->jogadores[0].token_disponivel == true) {
			fputs("Nao usou o token de ajuda.", stream);
		} else {
			fputs("Usou o token de ajuda", stream);
		}
	}
}

// a funçao qsort quer que os 2 elementos a comparar sejam do tipo void*
int comparar_resultados(const void *elemento1, const void *elemento2) {
	JOGADOR x = *((JOGADOR *) elemento1);   // converte de void* para jogador* e vai buscar o valor
	JOGADOR y = *((JOGADOR *) elemento2);
	return y.respostas_corretas - x.respostas_corretas;
}

void comecar_quiz(QUIZ *quiz) {
	for (int i = 0; i < quiz->numero_jogadores; ++i) {
		for (int j = 0; j < quiz->numero_perguntas_a_jogar; ++j) {
			int index_pergunta = quiz->jogadores[i].ordem_perguntas[j];     // cada jogador vai ter uma ordem de perguntas diferentes
			mostrar_pergunta(quiz, i, index_pergunta, j + 1);
		}
	}
}

void gerar_ordem_perguntas(QUIZ *quiz) {
	int numero_perguntas = 0;
	// percorrer todas as perguntas que carregaste no ficheiro
	for (int i = 0; i < quiz->perguntas.numero_perguntas; ++i) {
	    // se o indice da categoria for igual ao indice que o jgoador escolheu incrementa o nr de perguntas ----- "-1" (nenhuma categoria) se o jogador quiser jogar todas as perguntas
		if (quiz->perguntas.perguntas[i].index_categoria == quiz->index_categoria_a_jogar ||
			quiz->index_categoria_a_jogar == -1) {
			numero_perguntas++;
		}
	}
	quiz->numero_perguntas_a_jogar = numero_perguntas;
	escolher_numero_de_perguntas(quiz);
	int index_perguntas[numero_perguntas];
	int j = 0;
	// guardar o indice das perguntas no array index perguntas
	for (int i = 0; i < quiz->perguntas.numero_perguntas; ++i) {
		if (quiz->perguntas.perguntas[i].index_categoria == quiz->index_categoria_a_jogar ||
			quiz->index_categoria_a_jogar == -1) {
			index_perguntas[j++] = i;
		}
	}
	for (int i = 0; i < quiz->numero_jogadores; ++i) {
		shuffle_array(index_perguntas, numero_perguntas); // da shuffle ao array index_perguntas
		for (int k = 0; k < quiz->numero_perguntas_a_jogar; ++k) {
			quiz->jogadores[i].ordem_perguntas[k] = index_perguntas[k];     // copiar do array index_perguntas para a ordem de cada jogador
		}
	}

}

void mostrar_pergunta(QUIZ *quiz, int index_jogador, int index_pergunta, int numero_pergunta_atual) {
	limpar_ecra();
	char buffer;
	printf("Jogador atual: %s\n", quiz->jogadores[index_jogador].nome);
	printf("Categoria: %s		%d/%d\n",
		   // ver qual a categoria da pergunta atual, dps ver a pergunta atual e o nr total de peguntas nessa categoria
		   quiz->perguntas.categorias[quiz->perguntas.perguntas[index_pergunta].index_categoria], numero_pergunta_atual,
		   quiz->numero_perguntas_a_jogar);
	// dar print a pergunta atual
	printf("%s\n", quiz->perguntas.perguntas[index_pergunta].pergunta);
	// ciclo para percorrer as respostas da pergunta
	for (int k = 0; k < 3; ++k) {
		printf("%d - %s\n", k + 1, quiz->perguntas.perguntas[index_pergunta].respostas[k]); // dar print a resposta
	}
	if (quiz->jogadores[index_jogador].token_disponivel == true) {
		puts("t - Usar o token de ajuda");
	}
	puts("Introduza a sua resposta.");
	scanf(" %1c", &buffer);
	while ((buffer < '0' || buffer > '9')) {
		if (buffer == 't') {
			if (quiz->jogadores[index_jogador].token_disponivel == false) {
				puts("Já esgotou o seu token de ajuda. Tente denovo.");
			} else {
			    // ve qual é o indice da resposta correta e da print da resposta
				printf("A resposta correta é: %d - %s\n",
					   quiz->perguntas.perguntas[index_pergunta].index_resposta_certa + 1,
					   quiz->perguntas.perguntas[index_pergunta].respostas[quiz->perguntas.perguntas[index_pergunta].index_resposta_certa]);
				quiz->jogadores[index_jogador].token_disponivel = false; // jogador ja n pode utilizar mais o token
				quiz->jogadores[index_jogador].respostas_corretas++;
				sleep_quizz(1000); // tempo de espera que n faz passar logo para a pergunta seguinte
				return;
			}
		} else {
			puts("Resposta inválida, tente denovo.");
		}
		scanf(" %1c", &buffer);
	}
	int resposta_user = buffer - '0' - 1; // converter de ASCII para inteiro
	if (resposta_user == quiz->perguntas.perguntas[index_pergunta].index_resposta_certa) {
		puts("A sua resposta esta correta.");
		quiz->jogadores[index_jogador].respostas_corretas++;
	} else {
		puts("A sua resposta esta errada.");
		printf("A resposta correta é: %d - %s\n", quiz->perguntas.perguntas[index_pergunta].index_resposta_certa + 1,
			   quiz->perguntas.perguntas[index_pergunta].respostas[quiz->perguntas.perguntas[index_pergunta].index_resposta_certa]);
	}
	sleep_quizz(1000);
}

void shuffle_array(int *array, int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		int temp = array[i];    // guardar na variavel temp o valor atual do indice da pergunta
		int index_random = rand() % tamanho;    // gera um numero aleatorio entre 0 e o tamanho

		array[i] = array[index_random]; // troca os lugares
		array[index_random] = temp;
	}

}

void escolher_numero_de_perguntas(QUIZ *quiz) {
	char buffer[2]; // como o tamanho max das perguntas e 64 precisa-se de 2 careteres
	int numero;
	printf("Escolha o numero de perguntas que quer jogar. Max: %d\n", quiz->numero_perguntas_a_jogar);
	input:  // é uma seccçao de codigo em que posso saltar com o goto
	scanf(" %2c", buffer);
	while ((buffer[0] < '0' || buffer[0] > '9') &&
		   (buffer[1] < '0' || buffer[1] > '9' || buffer[1] != '\n' || buffer[1] != '\r' || buffer[1] != '\0')) {
		puts("O numero de perguntas que escolheu e invalido. Tente denovo.");
		scanf(" %2c", buffer);
	}
	// caso especial para nr que sao menores que 10 converte para um formato com um 0 a esquerda
	if (buffer[1] == '\n' || buffer[1] == '\r' || buffer[1] == '\0') {
		buffer[1] = buffer[0];
		buffer[0] = '0';
	}
	numero = (buffer[0] - '0') * 10 + buffer[1] - '0'; // conversao de ASCII para inteiro
	if (numero > quiz->numero_perguntas_a_jogar) {
		puts("O numero que escolheu excede o maximo. Tente denovo");
		goto input;
	}
	quiz->numero_perguntas_a_jogar = numero; // nr max de perguntas que o jogador escolher fica guardada na variavel numero_perguntas_a_jogar
}

void escolher_categorias_a_jogar(QUIZ *quiz) {
	char buffer;
	puts("Que categoria pretende jogar?");
	puts("0 - Todas");
	for (int i = 0; i < quiz->perguntas.numero_categorias; ++i) {
		printf("%d - %s\n", i + 1, quiz->perguntas.categorias[i]);
	}
	scanf(" %1c", &buffer);
	while (buffer < '0' || buffer > '9') {
		puts("A categoria escolhida nao existe, tente denovo.");
		scanf(" %1c", &buffer);
	}
	// faz a conversao
	quiz->index_categoria_a_jogar = buffer - '0' - 1;
}

void inserir_jogadores(QUIZ *quiz) {
	char buffer;
	printf("Quantos jogadores irão jogar?(Máx: %d)\n", NUMERO_MAXIMO_JOGADORES);
	scanf(" %1c", &buffer);
	// mais uma conversao
	while (buffer < '0' || buffer > NUMERO_MAXIMO_JOGADORES + '0') {
		puts("Numero de jogadores invalido, introduza um numero valido.");
		scanf(" %1c", &buffer);
	}
	quiz->numero_jogadores = buffer - '0';
	for (int i = 0; i < quiz->numero_jogadores; ++i) {
	    // encher o array ordem_perguntas com -1 ou seja n tem nada la dentro
		for (int j = 0; j < NUMERO_MAXIMO_PERGUNTAS; ++j) {
			quiz->jogadores[i].ordem_perguntas[j] = -1;
		}
        quiz->jogadores[i].respostas_corretas = 0;
		quiz->jogadores[i].token_disponivel = true;
		printf("Introduza o nome para o jogador %d\n", i + 1);
		scanf(" %20s", quiz->jogadores[i].nome);
	}
}