#include <stdio.h>
#include <stdlib.h> // Para funcoes de memoria e numeros aleatorios
#include <string.h> // Para manipulacao de strings
#include <time.h>   // Para uso do time()

// Estrutura para guardar as informacoes de um territorio
struct Territorio {
    char nome[50];
    char cor[50];
    int tropas;
};

// Declaracao das funcoes relacionadas às missoes
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const char* nomeJogador);
int verificarMissao(const char* missao, const struct Territorio* mapa, int totalTerritorios, const char* corJogador, const char* corOponente);

// Outras funcoes do sistema
void limparBuffer();
void cadastrarTerritorio(struct Territorio* territorio, int indice);
void listarTerritorios(struct Territorio* mapa, int totalTerritorios);
void executarAtaque(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa, char* missao1, char* missao2);


// Funcao principal
int main() {
    struct Territorio *mapa = NULL; // Vetor dinamico de territorios
    int totalTerritorios;
    int territoriosCadastrados = 0;
    int opcao;
    int jogoFinalizado = 0;

    // Informacoes dos jogadores
    char corJogador1[10] = "";
    char corJogador2[10] = "";
    char* missaoJogador1 = NULL;
    char* missaoJogador2 = NULL;
    int jogoConfigurado = 0; // Controle de configuracao inicial

    // Lista de missoes disponiveis
    char* missoes[] = {
        "Controlar 4 territorios no mapa.",
        "Eliminar completamente o jogador adversario.",
        "Conquistar o territorio chamado 'Brasil'.",
        "Possuir um total de 25 tropas no mapa.",
        "Manter 15 tropas em um unico territorio."
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    srand(time(NULL)); // Inicializa gerador

    // Configuracao inicial
    printf("==========================================\n");
    printf("         CONFIGURACAO DO SISTEMA\n");
    printf("==========================================\n");
    printf("Digite o numero total de territorios: ");
    scanf("%d", &totalTerritorios);
    limparBuffer();

    mapa = (struct Territorio*) calloc(totalTerritorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("Falha ao alocar memoria! Encerrando...\n");
        return 1;
    }

    // Menu principal
    do {
        printf("\n==========================================\n");
        printf("            SISTEMA DE TERRITORIOS\n");
        printf("==========================================\n");
        printf("1. Registrar um novo territorio\n");
        printf("2. Exibir lista de territorios\n");
        printf("3. Realizar ataque\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                if (territoriosCadastrados < totalTerritorios) {
                    cadastrarTerritorio(mapa + territoriosCadastrados, territoriosCadastrados);
                    
                    // Define jogadores com base nos dois primeiros territorios
                    if (territoriosCadastrados == 0) {
                        strcpy(corJogador1, (mapa + 0)->cor);
                    } else if (territoriosCadastrados == 1) {
                        strcpy(corJogador2, (mapa + 1)->cor);

                        if (strcmp(corJogador1, corJogador2) == 0) {
                            printf("\nAVISO: As cores dos jogadores nao podem ser iguais.\n");
                        } else {
                            printf("\n--- CONFIGURACAO CONCLUIDA ---\n");
                            printf("Jogador 1 -> Cor: '%s'\n", corJogador1);
                            printf("Jogador 2 -> Cor: '%s'\n", corJogador2);

                            atribuirMissao(&missaoJogador1, missoes, totalMissoes);
                            atribuirMissao(&missaoJogador2, missoes, totalMissoes);

                            printf("\n");
                            exibirMissao(missaoJogador1, "Jogador 1");
                            exibirMissao(missaoJogador2, "Jogador 2");
                            printf("------------------------------------------\n");

                            jogoConfigurado = 1;
                        }
                    }
                    territoriosCadastrados++;
                } else {
                    printf("\nNumero maximo de territorios atingido!\n");
                }
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 2:
                listarTerritorios(mapa, territoriosCadastrados);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            
            case 3:
                if (!jogoConfigurado) {
                    printf("\nCadastre 2 territorios com cores diferentes antes de atacar.\n");
                } else {
                    int idAtacante, idDefensor;
                    printf("\n--- ATAQUE EM ANDAMENTO ---\n");
                    listarTerritorios(mapa, territoriosCadastrados);

                    printf("\nEscolha o numero do territorio atacante: ");
                    scanf("%d", &idAtacante);
                    limparBuffer();

                    printf("Escolha o numero do territorio defensor: ");
                    scanf("%d", &idDefensor);
                    limparBuffer();

                    idAtacante--;
                    idDefensor--;

                    if (idAtacante < 0 || idAtacante >= territoriosCadastrados ||
                        idDefensor < 0 || idDefensor >= territoriosCadastrados) {
                        printf("\nTerritorio selecionado nao existe!\n");
                    } else if (idAtacante == idDefensor) {
                        printf("\nUm territorio nao pode atacar ele mesmo!\n");
                    } else if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
                        printf("\nNao e possivel atacar territorios da mesma cor!\n");
                    } else {
                        executarAtaque(mapa + idAtacante, mapa + idDefensor);
                        printf("\n--- Atualizacao dos Territorios ---\n");
                        listarTerritorios(mapa, territoriosCadastrados);

                        if (verificarMissao(missaoJogador1, mapa, territoriosCadastrados, corJogador1, corJogador2)) {
                            printf("\n==========================================\n");
                            printf("  JOGADOR 1 (Cor %s) CONCLUIU SUA MISSAO!\n", corJogador1);
                            printf("  Missao: %s\n", missaoJogador1);
                            printf("==========================================\n");
                            jogoFinalizado = 1;
                        }
                        if (!jogoFinalizado && verificarMissao(missaoJogador2, mapa, territoriosCadastrados, corJogador2, corJogador1)) {
                            printf("\n==========================================\n");
                            printf("  JOGADOR 2 (Cor %s) CONCLUIU SUA MISSAO!\n", corJogador2);
                            printf("  Missao: %s\n", missaoJogador2);
                            printf("==========================================\n");
                            jogoFinalizado = 1;
                        }
                    }
                }
                if (!jogoFinalizado) {
                    printf("\nPressione ENTER para continuar...");
                    getchar();
                }
                break;

            case 0:
                printf("\nEncerrando aplicacao...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
        }

    } while (opcao != 0 && !jogoFinalizado);

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    return 0;
}


// Limpa caracteres extras do buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Cadastro de territorios
void cadastrarTerritorio(struct Territorio* territorio, int indice) {
    printf("\n--- Registro do Territorio %d ---\n", indice + 1);
    
    printf("Insira o nome do territorio: ");
    fgets(territorio->nome, 50, stdin);
    territorio->nome[strcspn(territorio->nome, "\n")] = '\0';

    printf("Cor do exercito: ");
    fgets(territorio->cor, 50, stdin);
    territorio->cor[strcspn(territorio->cor, "\n")] = '\0';

    printf("Quantidade de tropas: ");
    scanf("%d", &territorio->tropas);
    limparBuffer();
    
    printf("\nTerritorio registrado!\n");
}


// Exibe os territorios registrados
void listarTerritorios(struct Territorio* mapa, int totalTerritorios) {
    printf("\n--- Territorios Registrados ---\n");
    if (totalTerritorios == 0) {
        printf("Nenhum territorio foi registrado ainda.\n");
    } else {
        for (int i = 0; i < totalTerritorios; i++) {
            struct Territorio* atual = mapa + i;
            printf("------------------------------------------\n");
            printf("Territorio %d:\n", i + 1);
            printf("Nome: %s\n", atual->nome);
            printf("Cor: %s\n", atual->cor);
            printf("Tropas: %d\n", atual->tropas);
        }
        printf("------------------------------------------\n");
    }
}


// Simulacao de ataque
void executarAtaque(struct Territorio* atacante, struct Territorio* defensor) {
    if (atacante->tropas < 2) {
        printf("\nO atacante precisa de pelo menos 2 tropas.\n");
        return;
    }
    
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\nRolagem dos dados:\n");
    printf(" -> Ataque (%s): %d\n", atacante->nome, dadoAtaque);
    printf(" -> Defesa (%s): %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nRESULTADO: Ataque bem sucedido!\n");
        printf("O territorio %s foi dominado por %s!\n", defensor->nome, atacante->nome);
        
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1;
        atacante->tropas--;
    } else {
        printf("\nRESULTADO: Defesa bem sucedida!\n");
        printf("O ataque de %s falhou.\n", atacante->nome);
        
        atacante->tropas--;
        if (atacante->tropas < 1) atacante->tropas = 1;
    }
}


// Funcao para atribuir uma missao
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;

    *destino = (char*) malloc(strlen(missoes[indiceSorteado]) + 1);
    if (*destino == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }

    strcpy(*destino, missoes[indiceSorteado]);
}


// Exibe a missao atribuida
void exibirMissao(const char* missao, const char* nomeJogador) {
    if (missao != NULL) {
        printf("Missao de %s: %s\n", nomeJogador, missao);
    }
}


// Liberacao de memoria
void liberarMemoria(struct Territorio* mapa, char* missao1, char* missao2) {
    printf("Liberando memoria...\n");

    free(mapa);
    mapa = NULL;

    if (missao1 != NULL) {
        free(missao1);
        missao1 = NULL;
    }
    if (missao2 != NULL) {
        free(missao2);
        missao2 = NULL;
    }

    printf("Memoria liberada.\n");
}

    return 0;
}
