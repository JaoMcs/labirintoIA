//Autor: João Marcos Pereira Tavares
//Algoritmo de busca informada
//MARK: - Estrategias
//Player1: Busca gulosa
//Player2: Busca A*

//MARK: - Player 1
// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 1 ***
//	Implementacao da primeira estrategia de jogo.


//Structs feitas para aplicaçao da estrategia, um espelho do mapa onde é informado o caminhos ja passados e uma struct Path referente as informaçoes do caminho, como seu valor heuristico e a direçao desse valor
typedef struct {
    int passei = 0;
    int antCaminho = -1;
    bool checkPoint = 0;
} InfoMapa;

typedef struct {
    int direcao;
    float valor;
} Path;

//Globais
InfoMapa mapaEspelho[35][60];
int posicao[2];

//Funcao utilizada para verificar se o proximo passo será um caminho que ja foi utilizado, ou seja, ve se ele vai retornar
bool verRetorno(int direcao) {
    int auxX = posicao[0];
    int auxY = posicao[1];
    if (direcao == 0) {
        auxX--;
    } else if (direcao == 1) {
        auxX++;
    } else if (direcao == 2) {
        auxY--;
    } else {
        auxY++;
    }
    if (mapaEspelho[auxX][auxY].passei == 1) {
        return true;
    } else {
        return false;
    }
}
//posAtualP1
//posAtualp2

//Ordena os caminhos de ordem crescente
void ordenaCaminhos(Path* caminhos) {
    Path aux;
    for (int i = 0; i < NUMCAMINHOS; i++) {
        for (int j = 0; j < NUMCAMINHOS; j++) {
            if (caminhos[i].valor < caminhos[j].valor) {
                
                aux.valor = caminhos[i].valor;
                aux.direcao = caminhos[i].direcao;
                
                caminhos[i].valor = caminhos[j].valor;
                caminhos[i].direcao = caminhos[j].direcao;
                
                caminhos[j].valor = aux.valor;
                caminhos[j].direcao = aux.direcao;
            }
        }
    }
}

//Evitar voltas
//Evitar becos
//Sair fora do bolsonaro ops minotauro

//Escolhe o melhor caminho de acordo com a distancia heuristica, levando em consideraçao se tem minotauro perto ou se ja passou pelo caminho
int escolheCaminho() {
    Path caminhos[NUMCAMINHOS];
    int escolha = -1;
    
    //printf("Desordenado:\n");
    for (int i = 0; i < NUMCAMINHOS; i++) {
        caminhos[i].valor = maze_HeuristicaDistEuclidiana(id_Caminhos[i]);
        caminhos[i].direcao = i;
        //printf("Direcao: %d, Valor: %.2f\n", caminhos[i].direcao, caminhos[i].valor);
    }
    
    ordenaCaminhos(caminhos);
    
    //Escolhe o melhor caminho, caso nao encontre ele retorna -1
    for (int i = 0; i < NUMCAMINHOS; i++) {
        if (maze_VerCaminho(id_Caminhos[caminhos[i].direcao]) && !maze_VerMinotauro(id_Caminhos[caminhos[i].direcao]) && !verRetorno(caminhos[i].direcao)) {
            escolha = caminhos[i].direcao;
            printf("Menor Player 1: %d ", escolha);
            return escolha;
        }
    }
    return escolha;
}
//Incrementa o mapa deacordo com a posicao do jogador
void movimentaMapa(int direcao, bool retorno) {
    mapaEspelho[posicao[0]][posicao[1]].passei = 1;
    if (!retorno) {
        mapaEspelho[posicao[0]][posicao[1]].antCaminho = direcao;
    }
}

//Calcula qual posiçao do mapa o player esta, para depois incrementar no mapa os valores
void calculaPosicao(int direcao, bool retorno) {
    if (direcao == 0) {
        posicao[0]--;
    } else if (direcao == 1) {
        posicao[0]++;
    } else if (direcao == 2) {
        posicao[1]--;
    } else {
        posicao[1]++;
    }
    movimentaMapa(direcao, retorno);
}

//Calcula o valor inverso em relaçao a direçao que esta no mapa,
int inversoDirecao(int direcao) {
    if (mapaEspelho[posicao[0]][posicao[1]].antCaminho == 0) {
        direcao = 1;
    } else if (mapaEspelho[posicao[0]][posicao[1]].antCaminho == 1) {
        direcao = 0;
    } else if (mapaEspelho[posicao[0]][posicao[1]].antCaminho == 2) {
        direcao = 3;
    } else {
        direcao = 2;
    }
    return direcao;
}
//Iniciador
void init_Player1() {
    posicao[0] = 0;
    posicao[1] = 0;
	
}

//Chamada das funcoes
const char *run_Player1() {
    const char *movimento;
    int direcao = escolheCaminho();
    if (direcao == -1) { //Quando a direçao é -1 o algoritmo retorna até achar novamnete um caminho valido
        movimento = id_Retornos[mapaEspelho[posicao[0]][posicao[1]].antCaminho];
        direcao = inversoDirecao(direcao);
        calculaPosicao(direcao, true);
    } else {
        movimento = id_Caminhos[direcao];
        calculaPosicao(direcao, false);
    }
    printf("\nDirecao Player 1: %d\n", direcao);
    printf("Posicao Player 1x: %d, y: %d\n", posicao[0], posicao[1]);
	
	return movimento;
}

// MARK: - Player 2
// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.

InfoMapa mapaEspelho2[35][60];
bool fantasma;
int contadorPassosFantasma;
float somatorioEuristica[NUMCAMINHOS];

bool verRetorno2(int direcao) {
    int auxX = posAtualP2.x;
    int auxY = posAtualP2.y;
    if (direcao == 0) {
        auxX--;
    } else if (direcao == 1) {
        auxX++;
    } else if (direcao == 2) {
        auxY--;
    } else {
        auxY++;
    }
    if (mapaEspelho2[auxX][auxY].passei == 1) {
        return true;
    } else {
        return false;
    }
}

int escolheCaminho2() {
    Path caminhos[NUMCAMINHOS];
    int escolha = -1;
    int cont = 0;
    int primeiraVez = 0;
    
    //printf("Desordenado:\n");
    for (int i = 0; i < NUMCAMINHOS; i++) {
        caminhos[i].valor = maze_HeuristicaDistEuclidiana(id_Caminhos[i]);
        caminhos[i].direcao = i;
        //printf("Direcao: %d, Valor: %.2f\n", caminhos[i].direcao, caminhos[i].valor);
    }
    
    ordenaCaminhos(caminhos);
    
    for (int i = 0; i < NUMCAMINHOS; i++) {
        if (maze_VerCaminho(id_Caminhos[caminhos[i].direcao]) && !maze_VerMinotauro(id_Caminhos[caminhos[i].direcao]) && !verRetorno2(caminhos[i].direcao)) {
            cont ++;
            if (primeiraVez == 0) {
                primeiraVez = 1;
                escolha = caminhos[i].direcao;
                printf("Menor player 2: %d ", escolha);
            }
            
        }
    }
    if (cont > 1){
        mapaEspelho2[posAtualP2.x][posAtualP2.y].checkPoint = true;
        fantasma = true;
    }
    return escolha;
}

int escolheCaminhoFantasma(int tipoFantasma) {
    Path caminhos[NUMCAMINHOS];
    int escolha = -1;
    int cont = 0;
    int primeiraVez = 0;
    
    //printf("Desordenado:\n");
    for (int i = 0; i < NUMCAMINHOS; i++) {
        caminhos[i].valor = maze_HeuristicaDistEuclidiana(id_Caminhos[i]);
        caminhos[i].direcao = i;
        //printf("Direcao: %d, Valor: %.2f\n", caminhos[i].direcao, caminhos[i].valor);
    }
    
    ordenaCaminhos(caminhos);
    
    for (int i = 0; i < NUMCAMINHOS; i++) {
        if (maze_VerCaminho(id_Caminhos[caminhos[i].direcao]) && !maze_VerMinotauro(id_Caminhos[caminhos[i].direcao]) && !verRetorno2(caminhos[i].direcao)) {
            escolha = caminhos[i].direcao;
            printf("Menor player 2: %d ", escolha);
            return escolha;
        }
    }
    return escolha;
}

void movimentaMapa2(int direcao, bool retorno) {
    mapaEspelho2[posAtualP2.x][posAtualP2.y].passei = 1;
    if (!retorno) {
        mapaEspelho2[posAtualP2.x][posAtualP2.y].antCaminho = direcao;
    }
}

int funcaoRetornaCheckPoint() {
    movimento = id_Retornos[mapaEspelho2[posAtualP2.x][posAtualP2.y].antCaminho];
    direcao = inversoDirecao(direcao);
    movimentaMapa2(direcao, true);
    return direcao;
}

void init_Player2() {
    fantasma = false;
    contadorPassosFantasma = 0;
    for(int i = 0; i < NUMCAMINHOS; i++){
        somatorioEuristica[i] = 0;
    }
}


//Player 2 não deu pra finalizar
const char *run_Player2() {
    const char *movimento;
    int fantasma1 = 0, fantasma2 = 0;

    if (fantasma) {
        if (fantasma1 == 0) {
            if (contadorPassosFantasma < 5 || direcao == -1) {
                int direcao = escolheCaminhoFantasma(1);
                movimento = id_Caminhos[direcao];
                movimentaMapa2(direcao, false);
            } else {
                movimento = funcaoRetornaCheckPoint();
            }
            contadorPassosFantasma ++;
        } else if (fantasma2 == 0) {
            if (contadorPassosFantasma < 5 || direcao == -1) {
                int direcao = escolheCaminhoFantasma(2);
                movimento = id_Caminhos[direcao];
                movimentaMapa2(direcao, false);
            } else {
                movimento = funcaoRetornaCheckPoint();
            }
            contadorPassosFantasma ++;
        }
        
        
    } else {
        int direcao = escolheCaminho2();
        if (direcao == -1) {
            movimento = id_Retornos[mapaEspelho2[posAtualP2.x][posAtualP2.y].antCaminho];
            direcao = inversoDirecao(direcao);
            movimentaMapa2(direcao, true);
        } else {
            movimento = id_Caminhos[direcao];
            movimentaMapa2(direcao, false);
        }
        printf("\nDirecao Player 2: %d\n", direcao);
        printf("posicao P2 - x: %d, y: %d\n", posAtualP2.x, posAtualP2.y);
    }

    return movimento;
}


