# Jogo-Ferrovia
**Trabalho de Estrutura de Dados (ADT, Circular Queue, Stack)
**
Em uma malha ferroviária, de 2 a 4 maquinistas disputam contratos de entrega. Cada maquinista monta
um trem: os vagões são engatados no fim, e só o vagão da frente pode ser desengatado para entrega. As
cargas saem de um depósito comum, onde os contêineres ficam empilhados, e as cargas entregues ou
rejeitadas vão para o pátio de manobras, de onde um guindaste pode resgatá-las. Quem completa 20
pontos conclui a sua rota; a partida termina quando resta apenas um maquinista.

Para o Trabalho foi implementado esse jogo em C++, usando os TADs Pilha e Fila estáticos
(implementados com vetor; a Fila é circular) definidos nos arquivos fornecidos pelo **Prof. Felipe Carvalho Pellison**.

**Arquivos fornecidos:**
pilha.h e fila.h: interfaces dos TADs Pilha (estática) e Fila (estática e circular) de inteiros, com
capacidade de 40 itens (CAPACIDADE). 
cargas.csv: 40 cargas, uma por linha, com cabeçalho. Campos separados por ponto e vírgula (;), fim
de linha LF, sem acentos.

**Preparação da partida:**
1. Ler o cargas.csv para o catálogo (uma única vez, ao iniciar o programa).
2. Pedir o número de maquinistas (2 a 4) e o nome de cada um.
3. Embaralhar os 40 IDs e empilhá-los no depósito.
4. Cada maquinista, na ordem de cadastro, recebe 3 cargas: desempilhadas do depósito e enfileiradas no
seu trem.
5. A ordem de turnos começa pela ordem de cadastro.

**O turno:**
O maquinista da vez é retirado da frente da ordem de turnos. Durante o turno, o próximo maquinista é
quem está na frente da ordem de turnos.
**1.** Exibição. Mostrar o estado do jogo (ver a seção Exibição).
**2.** Compra. Se o depósito estiver vazio, reembaralhar o pátio (ver a seção Reembaralhamento).
Desempilhar uma carga do depósito e mostrá-la:
- se o trem tiver 5 vagões, a carga comprada vai obrigatoriamente para o pátio;
- senão, o jogador escolhe engatar (enfileirar no seu trem) ou descartar (empilhar no pátio).
**3.** Ação. Se o trem estiver vazio, o turno passa. Senão, o jogador escolhe:
- Entregar: desenfileirar o vagão da frente e aplicar o efeito da carga (tabela abaixo);
- Manobrar: o vagão da frente vai para o fim do próprio trem;
- Encerrar a partida: volta ao menu principal.
**4.** Fim do turno. Se não concluiu a rota, o maquinista da vez volta ao fim da ordem de turnos

**Conclusão de rota e fim da partida:**
- Ao chegar a 20 pontos ou mais, o maquinista da vez conclui a rota: recebe a próxima colocação (1º,
2º, ...), os vagões do seu trem vão para o pátio, da frente para o fim, e ele não volta à ordem de turnos.
Só depois a carga entregue vai para o pátio, ficando por cima dos vagões do trem dele.
- Quando restar um único maquinista na ordem de turnos, ele recebe a última colocação e a partida
termina. O programa mostra a classificação e volta ao menu principal.


