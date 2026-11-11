/* 
            Trabalho 1 - Grupo 08
            Trabalho feito por: NICOLLY PELEGRINI - 2333481
                                JÚLIA MERSCHBACHER - 2376150
*/

#include <iostream> 
#include <string>    
#include <fstream>   
#include <cstdlib>   // rand, srand e exit
#include <ctime>     // time(NULL)
#include <iomanip>  
#include <sstream>   // stringstream, usado na conversÃo e no .csv


#include "fila.h"
#include "pilha.h"

using namespace std;

const int maxCargas = 40;
const int minMaquinistas = 2;
const int maxMaquinistas = 4;
const int vagoesIniciais = 3;
const int limiteVagoes = 5;
const int maxPontos = 20;

// Registro do catalogo:
struct Carga{
    string nome;
    string destino;
    int valor;
    string tipo;
};

// Registro de cada maquinista:
struct Maquinista{
    string nome;
    int pontos;
    int colocacao;
    bool emJogo;
    Fila trem;
};

/* 
                    Preparação da partida:
*/


// VaAlidações.
// Converte texto para inteiro:
bool converterInteiro(const string &texto, int &valor){
    stringstream conversor(texto);
    char sobra;

    if (!(conversor >> valor)) return false;

    if (conversor >> sobra) return false;

    return true;
}

// Le um inteiro, verifica se é uma entrada válida:
int lerInteiro(const string &texto, int min, int max){
    string entrada;
    int valor;

    while(true){
        cout << texto;

        if (!getline(cin, entrada)){ // Programa não consegiu uma entrada
            cout << "\nEntrada encerrada. O programa sera finalizado.\n";
            exit(0);
        }

        if (converterInteiro(entrada, valor) && valor >= min && valor <= max) return valor;

        cout << "Entrada invalida. Digite um numero de " << min << " a " << max << ".\n";
    }
}

// Verifica se o nome do maquinista é valido:
string nomeMaquinista(const string &texto){
    string nome;

    while(true){
        cout << texto;

        if (!getline(cin, nome)){
            cout << "\nEntrada encerrada. O programa sera finalizado.\n";
            exit(0);
        }

        for (int i = 0; i < (int)nome.length(); i++){
            if (nome[i] != ' ' && nome[i] != '\t') return nome;
        }

        cout << "Nome invalido. Digite pelo menos um caractere.\n";
    }
}

// Verifica se o destino lido no .csv é valido:
bool destinoValido(const string &destino){
    return destino == "Norte" || destino == "Sul" || destino == "Leste" || destino == "Oeste";
}

// Verifica se o tipo lido no .csv é valido:
bool tipoValido(const string &tipo){
    return tipo == "CARGA" || tipo == "DESVIO" || tipo == "DESCARRILAMENTO" || tipo == "GUINDASTE";
}

//      1. Ler o cargas.csv para o catálogo (uma única vez, ao iniciar o programa):
bool lerCatalogo(Carga catalogo[], string nomeArquivo){
    ifstream arquivo(nomeArquivo);
    string linha;
    int quantidade = 0; // Variaveis para guardar os itens lidos.
    int totalCarga = 0;
    int totalDesvio = 0;
    int totalDescarrilamento = 0;
    int totalGuindaste = 0;

    if (!arquivo.is_open()){
        cout << "\nErro: Arquivo " << nomeArquivo << " nao encontrado ou esta inacessivel!";
        return false;
    }

    if (!getline(arquivo, linha)){
        cout << "\nErro: Arquivo " << nomeArquivo << " vazio!";
        return false;
    }

    //
    while (getline(arquivo, linha)){ 
        string campoNome;
        string campoDestino;
        string campoValor;
        string campoTipo;
        string campoExtra;
        stringstream linhaCsv(linha);
        int valor;

        if (quantidade >= maxCargas){
            cout << "\nErro: Arquivo " << nomeArquivo << " com mais de " << maxCargas << "!";
            return false;
        }

        // Le e para quando encontra o ";":
        getline(linhaCsv, campoNome, ';');
        getline(linhaCsv, campoDestino, ';');
        getline(linhaCsv, campoValor, ';');
        getline(linhaCsv, campoTipo);

        // Remove possiveis '\r'
        if (!campoTipo.empty() && campoTipo[campoTipo.length() - 1] == '\r'){
            campoTipo.erase(campoTipo.length() - 1, 1);
        }

        if (campoNome.empty() || !destinoValido(campoDestino) || !converterInteiro(campoValor, valor) || !tipoValido(campoTipo) ||
            getline(linhaCsv, campoExtra, ';')){
            cout << "Erro: Linha invalida no CSV: " << linha;
            return false;
        }

        if ((campoTipo == "CARGA" && (valor < 1 || valor > 6)) || (campoTipo != "CARGA" && valor != 0)){
            cout << "Erro: Valor invalido na linha: " << linha;
            return false;
        }

        catalogo[quantidade].nome = campoNome; // Coloca os itens no catalogo
        catalogo[quantidade].destino = campoDestino;
        catalogo[quantidade].valor = valor;
        catalogo[quantidade].tipo = campoTipo;

        if (campoTipo == "CARGA"){
            totalCarga++;
        }else if (campoTipo == "DESVIO"){
            totalDesvio++;
        }else if (campoTipo == "DESCARRILAMENTO"){
            totalDescarrilamento++;
        }else{
            totalGuindaste++;
        }

        quantidade++;
    }

    if (quantidade != maxCargas){
        cout << "Erro: Arquivo " << nomeArquivo << " deve conter exatamente " << maxCargas << " cargas.";
        return false;
    }

    if (totalCarga != 28 || totalDesvio != 4 || totalDescarrilamento != 4 || totalGuindaste != 4){
    cout << "Erro: Arquivo " << nomeArquivo<< " deve conter exatamente 28 cargas, 4 desvios, 4 descarrilamentos e 4 guindastes";
    return false;
    }

    return true;
}

// Limpa o vetor de maquinistas antes de uma nova partida:
void inicializarMaquinistas(Maquinista maquinistas[]){
    for (int i = 0; i < maxMaquinistas; i++){
        maquinistas[i].nome = "";
        maquinistas[i].pontos = 0;
        maquinistas[i].colocacao = 0;
        maquinistas[i].emJogo = false;
        maquinistas[i].trem.esvaziar();
    }
}

//      2. Pedir o número de maquinistas (2 a 4) e o nome de cada um:
void cadastrarMaquinistas(Maquinista maquinistas[], int quantidade){
    for (int i = 0; i < quantidade; i++) {
        stringstream mensagem;
        mensagem << "Nome do maquinista " << (i + 1) << ": ";

        maquinistas[i].nome = nomeMaquinista(mensagem.str());
        maquinistas[i].pontos = 0;
        maquinistas[i].colocacao = 0;
        maquinistas[i].emJogo = true;
        maquinistas[i].trem.esvaziar();
    }
}

// Procedimento que embaralha vetor para auxiliar quando embaralharmos/reembaralharmos os IDs:
void embaralharVetor(int itens[], int quantidade){
    for (int i = quantidade - 1; i > 0; i--){
        int sorteado = rand() % (i + 1);
        int auxiliar = itens[i];
        itens[i] = itens[sorteado];
        itens[sorteado] = auxiliar;
    }
}

//      3. Embaralhar os 40 IDs e empilhá-los no depósito:
void prepararDeposito(Pilha &deposito){
    int ids[maxCargas];

    deposito.esvaziar();

    for (int i = 0; i < maxCargas; i++){
        ids[i] = i;
    }

    embaralharVetor(ids, maxCargas);

    for (int i = 0; i < maxCargas; i++){
        deposito.empilhar(ids[i]);
    }
}

//      4. Cada maquinista, na ordem de cadastro, recebe 3 cargas: desempilhadas do depósito e enfileiradas no seu trem:
void distribuirCargasIniciais(Maquinista maquinistas[], int quantidade, Pilha &deposito){
    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < vagoesIniciais; j++) {
            int idCarga;

            if (deposito.desempilhar(idCarga)){
                maquinistas[i].trem.enfileirar(idCarga);
            }
        }
    }
}

//      5. A ordem de turnos:
void montarOrdemTurnos(Fila &turnos, int quantidade){
    turnos.esvaziar();

    for (int i = 0; i < quantidade; i++){
        turnos.enfileirar(i);
    }
}


/* 
                    Reembaralhamento:
*/

void reembaralharPatio(Pilha &deposito, Pilha &patio){
    int cargaGuardada;
    int temporario[maxCargas];
    int quantidade = 0;

    if (!patio.desempilhar(cargaGuardada)){
        return; // Interrompe o procedimento.
    }

    while (!patio.vazia()){
        int idCarga;
        patio.desempilhar(idCarga);
        temporario[quantidade] = idCarga;
        quantidade++;
    }

    embaralharVetor(temporario, quantidade);

    for (int i = 0; i < quantidade; i++){
        deposito.empilhar(temporario[i]);
    }

    patio.empilhar(cargaGuardada);
    cout << "\nDeposito vazio: o patio foi reembaralhado para formar novo deposito.\n";
}



/* 
                    Exibição:
*/

// Mostra os dados de uma carga a partir do ID:
void mostrarCarga(const Carga catalogo[], int id){
    cout << "#" << setw(2) << id << " - "
         << catalogo[id].nome << " | "
         << catalogo[id].destino << " | valor "
         << catalogo[id].valor << " | "
         << catalogo[id].tipo;
}

// Mostra a ordem de turnos comecando pelo maquinista atual:
void mostrarFilaTurnos(Fila &turnos, int maquinistaAtual, Maquinista maquinistas[]){
    int quantidade = turnos.tamanho();

    cout << maquinistas[maquinistaAtual].nome;
    for (int i = 0; i < quantidade; i++){
        int idMaquinista;

        turnos.desenfileirar(idMaquinista);
        cout << " -> " << maquinistas[idMaquinista].nome;
        turnos.enfileirar(idMaquinista);
    }

    cout << "\n";
}

// Mostra pontos e quantidade de vagoes de cada maquinista ainda em jogo:
void mostrarMaquinistasEmJogo(Maquinista maquinistas[], int quantidade){
    for (int i = 0; i < quantidade; i++){
        if (maquinistas[i].emJogo) {
            cout << "\n - " << maquinistas[i].nome << ": " << maquinistas[i].pontos << " ponto(s), " << maquinistas[i].trem.tamanho() << " vagao(oes)\n";
        }
    }
}

// Mostra o trem da frente para o fim:
void mostrarTrem(Fila &trem, const Carga catalogo[]){
    int quantidade = trem.tamanho();

    if (quantidade == 0) {
        cout << "(trem vazio)\n";
        return;
    }

    for (int i = 0; i < quantidade; i++){
        int idCarga;

        trem.desenfileirar(idCarga);

        cout << (i + 1) << ". ";
        mostrarCarga(catalogo, idCarga);
        cout << "\n";

        trem.enfileirar(idCarga);
    }
}

// Mostra as 3 cargas do topo do patio:
void mostrarTopoPatio(Pilha &patio, const Carga catalogo[]){
    Pilha auxiliar;
    int mostradas = 0;

    while (mostradas < 3){
        int idCarga;

        if (!patio.desempilhar(idCarga)) break;

        cout << (mostradas + 1) << ". ";
        mostrarCarga(catalogo, idCarga);
        cout << "\n";

        auxiliar.empilhar(idCarga);
        mostradas++;
    }

    if (mostradas == 0){
        cout << "(patio vazio)\n";
    }

    while(!auxiliar.vazia()){
        int idCarga;
        auxiliar.desempilhar(idCarga);
        patio.empilhar(idCarga);
    }
}


// Exibição no inicio de cada turno:
void mostrarEstado(Maquinista maquinistas[], int quantidade, int atual, Fila &turnos, Pilha &deposito, Pilha &patio, const Carga catalogo[]){
    cout << "\n========================================\n";
    cout << "Maquinista da vez: " << maquinistas[atual].nome << "\n\n";

    cout << "Ordem de turnos: ";
    mostrarFilaTurnos(turnos, atual, maquinistas);

    cout << "\nMaquinistas em jogo:\n";
    mostrarMaquinistasEmJogo(maquinistas, quantidade);

    cout << "\nTrem de " << maquinistas[atual].nome << ":\n";
    mostrarTrem(maquinistas[atual].trem, catalogo);

    cout << "\nTopo do patio (ate 3 cargas):\n";
    mostrarTopoPatio(patio, catalogo);

    cout << "\nCargas no deposito: " << deposito.tamanho() << "\n";
    cout << "\n========================================\n";
}


/* 
                    O turno:
*/

//      2. COmpra de cargas:
// Se o trem tiver 5 vagões, a carga comprada vai obrigatoriamente para o pátio.
// Senão, o jogador escolhe engatar (enfileirar no seu trem) ou descartar (empilhar no pátio).
void comprarCarga(Maquinista &maquinista, Pilha &deposito, Pilha &patio, const Carga catalogo[]){
    int idCarga;

    if (deposito.vazia()){
        reembaralharPatio(deposito, patio);
    }

    if (!deposito.desempilhar(idCarga)){
        cout << "\nNao temos carga disponivel para compra neste momento.\n";
        return;
    }

    cout << "\nCarga comprada: ";
    mostrarCarga(catalogo, idCarga);
    cout << "\n";

    if (maquinista.trem.tamanho() >= limiteVagoes){
        cout << "\nO trem ja tem 5 vagoes. A carga vai para o patio.\n";
        patio.empilhar(idCarga);
        return;
    }

    cout << "1 - Engatar no trem\n";
    cout << "2 - Descartar para o patio\n";

    if (lerInteiro("Escolha: ", 1, 2) == 1){
        maquinista.trem.enfileirar(idCarga);
        cout << "\nCarga engatada no fim do trem.\n";
    }else{
        patio.empilhar(idCarga);
        cout << "\nCarga descartada para o patio.\n";
    }
}

//      3. Ações

// Inverte uma fila para mudarmos a ordem dos vagões:
void inverterFilaComPilha(Fila &fila){
    Pilha auxiliar;

    while (!fila.vazia()){
        int item;
        fila.desenfileirar(item);
        auxiliar.empilhar(item);
    }

    while (!auxiliar.vazia()){
        int item;
        auxiliar.desempilhar(item);
        fila.enfileirar(item);
    }
}

// Açãoo manobrar:
void manobrarTrem(Fila &trem){
    int idCarga;

    if (trem.desenfileirar(idCarga)){
        trem.enfileirar(idCarga);
        cout << "\nManobra realizada: o vagao da frente foi para o fim do trem.\n";
    }
}

// Efeito CARGA:
void efeitoCarga(Maquinista &maquinista, int idCarga, Pilha &patio, const Carga catalogo[]){
    int pontosGanhos = catalogo[idCarga].valor;
    int topoPatio;

    if (patio.consultarTopo(topoPatio) && catalogo[topoPatio].destino == catalogo[idCarga].destino){
        pontosGanhos = pontosGanhos * 2;
        cout << "\nBonus aplicado: destino igual ao topo do patio.\n";
    }

    maquinista.pontos = maquinista.pontos + pontosGanhos;
    cout << maquinista.nome << " ganhou " << pontosGanhos << " ponto(s). Total: " << maquinista.pontos << ".\n";
}

// Efeito desvio:
void efeitoDesvio(Fila &turnos){
    inverterFilaComPilha(turnos);
    cout << "DESVIO: a ordem dos demais maquinistas foi invertida.\n";
}

// Efeito descarrilamento:
void efeitoDescarrilamento(Maquinista maquinistas[], Fila &turnos, Pilha &patio, const Carga catalogo[]){
    int proximo;
    int idCarga;

    if (!turnos.consultarFrente(proximo)){
        cout << "DESCARRILAMENTO: nao ha proximo maquinista.\n";
        return;
    }

    if (maquinistas[proximo].trem.desenfileirar(idCarga)){
        patio.empilhar(idCarga);
        cout << "DESCARRILAMENTO: o vagao da frente de "
             << maquinistas[proximo].nome << " foi para o patio: ";
        mostrarCarga(catalogo, idCarga);
        cout << "\n";
    }else{
        cout << "DESCARRILAMENTO: o trem do proximo maquinista esta vazio.\n";
    }
}

// Efeito guindaste:
void efeitoGuindaste(Maquinista &maquinista, Pilha &patio, const Carga catalogo[]){
    int idCarga;

    if (patio.desempilhar(idCarga)){
        maquinista.trem.enfileirar(idCarga);
        cout << "GUINDASTE: carga resgatada do patio para o fim do trem: ";
        mostrarCarga(catalogo, idCarga);
        cout << "\n";
    }else{
        cout << "GUINDASTE: o patio esta vazio, entao nada acontece.\n";
    }
}

// Escolhe o efeito da carga pela coluna tipo do catalogo:
void aplicarEfeito(int idCarga, int atual, Maquinista maquinistas[], Fila &turnos, Pilha &patio, const Carga catalogo[]){
    string tipo = catalogo[idCarga].tipo;

    if (tipo == "CARGA"){
        efeitoCarga(maquinistas[atual], idCarga, patio, catalogo);
    }else if (tipo == "DESVIO"){
        efeitoDesvio(turnos);
    }else if (tipo == "DESCARRILAMENTO"){
        efeitoDescarrilamento(maquinistas, turnos, patio, catalogo);
    }else if (tipo == "GUINDASTE"){
        efeitoGuindaste(maquinistas[atual], patio, catalogo);
    }
}

// Quando o maquinista chega a 20 pontos ou mais, ele conclui a rota (1ª parte de "Conclusão de rota e fim da partida"):
// Será chamado em "entregarCarga"
void concluirRota(Maquinista &maquinista, int &proximaColocacao, int idCargaEntregue, Pilha &patio){
    maquinista.colocacao = proximaColocacao;
    maquinista.emJogo = false;
    proximaColocacao++;

    cout << "\n" << maquinista.nome << " concluiu a rota em " << maquinista.colocacao << "a colocacao!\n";

    while(!maquinista.trem.vazia()){
        int idCarga;
        maquinista.trem.desenfileirar(idCarga);
        patio.empilhar(idCarga);
    }

    patio.empilhar(idCargaEntregue);
}

// Ação entregar:
bool entregarCarga(int atual, Maquinista maquinistas[], Fila &turnos, Pilha &patio, const Carga catalogo[], int &proximaColocacao){
    int idCarga;

    if (!maquinistas[atual].trem.desenfileirar(idCarga)){
        return false;
    }

    cout << "\nCarga entregue/desengatada: ";
    mostrarCarga(catalogo, idCarga);
    cout << "\n";

    aplicarEfeito(idCarga, atual, maquinistas, turnos, patio, catalogo);

    if (maquinistas[atual].pontos >= maxPontos){
        concluirRota(maquinistas[atual], proximaColocacao, idCarga, patio);
        return true;
    }

    patio.empilhar(idCarga);
    return false;
}

// Verifica se sobrou apenas um maquinista (2ª parte de "Conclusão de rota e fim da partida", é chamado em "executarTurno")
// Quando isso acontece, ele recebe a ultima colocacao, a partida termina e a classificação e mostrada:
void ultimoMaquinista(Maquinista maquinistas[], Fila &turnos, int &proximaColocacao, bool &partidaAtiva){
    int ultimo;

    if (turnos.tamanho() != 1){
        return; // Interrompe esse procedimento aqui caso tiver mais de 1 maquinista.
    }

    turnos.desenfileirar(ultimo);
    maquinistas[ultimo].colocacao = proximaColocacao;
    maquinistas[ultimo].emJogo = false;
    maquinistas[ultimo].trem.esvaziar();

    cout << "\nResta apenas um maquinista. "
         << maquinistas[ultimo].nome << " fica com a "
         << proximaColocacao << "a colocacao.\n";

    proximaColocacao++;
    partidaAtiva = false;
}

// Mostra a classificacao em ordem de colocacao (3ª parte de "Conclusão de rota e fim da partida", é chamado em "executarTurno"):
void mostrarClassificacao(Maquinista maquinistas[], int quantidade){
    cout << "\n========== CLASSIFICACAO FINAL ==========\n";

    for (int posicao = 1; posicao <= quantidade; posicao++){
        for (int i = 0; i < quantidade; i++) {
            if (maquinistas[i].colocacao == posicao){
                cout << posicao << "a - " << maquinistas[i].nome << " (" << maquinistas[i].pontos << " ponto(s))\n";
            }
        }
    }

    cout << "\n=========================================\n\n";
}

// Executa um turno completo:
// O maquinista da vez e removido da frente de turnos logo no inicio.
// Durante esse turno, o proximo maquinista é quem ficou na frente da fila.
bool executarTurno(Maquinista maquinistas[], int quantidade, Fila &turnos, Pilha &deposito, Pilha &patio, const Carga catalogo[], int &proximaColocacao){
    int atual;
    bool concluiu = false;
    bool partidaAtiva = true;

    if (!turnos.desenfileirar(atual)){
        return false;
    }

    mostrarEstado(maquinistas, quantidade, atual, turnos, deposito, patio, catalogo);

    comprarCarga(maquinistas[atual], deposito, patio, catalogo);

    if (maquinistas[atual].trem.vazia()){
        cout << "\nO trem esta vazio. O turno passa.\n";
    }else{
        cout << "\nAcao do turno:\n";
        cout << "1 - Entregar o vagao da frente\n";
        cout << "2 - Manobrar: frente vai para o fim\n";
        cout << "3 - Encerrar a partida e voltar ao menu\n";

        int opcao = lerInteiro("Escolha: ", 1, 3);

        if (opcao == 1){
            concluiu = entregarCarga(atual, maquinistas, turnos, patio, catalogo, proximaColocacao);
        }else if (opcao == 2){
            manobrarTrem(maquinistas[atual].trem);
        }else{
            cout << "\nPartida encerrada pelo jogador. Voltando ao menu principal.\n";
            return false;
        }
    }

    if (concluiu){
        ultimoMaquinista(maquinistas, turnos, proximaColocacao, partidaAtiva);
    }else{
        turnos.enfileirar(atual);
    }

    if (!partidaAtiva){
        mostrarClassificacao(maquinistas, quantidade);
    }

    return partidaAtiva;
}


//      Menus e entradas

// Começa o jogo, junta os passos de preparação e depois chama os turnos:
void jogarPartida(const Carga catalogo[]){
    Maquinista maquinistas[maxMaquinistas];
    Pilha deposito;
    Pilha patio;
    Fila turnos;
    int quantidadeMaquinistas;
    int proximaColocacao = 1;
    bool partidaAtiva = true;

    inicializarMaquinistas(maquinistas);
    deposito.esvaziar();
    patio.esvaziar();
    turnos.esvaziar();

    quantidadeMaquinistas = lerInteiro("Numero de maquinistas (2 a 4): ", minMaquinistas, maxMaquinistas);

    cadastrarMaquinistas(maquinistas, quantidadeMaquinistas);
    prepararDeposito(deposito);
    distribuirCargasIniciais(maquinistas, quantidadeMaquinistas, deposito);
    montarOrdemTurnos(turnos, quantidadeMaquinistas);

    cout << "\nPartida começou, bom jogo!\n";

    while (partidaAtiva){
        partidaAtiva = executarTurno(maquinistas, quantidadeMaquinistas, turnos, deposito, patio, catalogo, proximaColocacao);
    }
}

// Menu:
void menu(const Carga catalogo[]){
    bool executando = true;

    while (executando){
        cout << "\n========== FERROVIA DE CARGA ==========\n";
        cout << "1 - Nova partida\n";
        cout << "2 - Sair\n";

        int opcao = lerInteiro("Escolha: ", 1, 2);

        if (opcao == 1){
            jogarPartida(catalogo);
        } else {
            cout << "Programa finalizado.\n";
            executando = false;
        }
    }
}


int main(){
    Carga catalogo[maxCargas];
    srand((unsigned int)time(NULL)); 

    if (!lerCatalogo(catalogo, "cargas.csv")) return 1;

    menu(catalogo);
    
    return 0;
}
