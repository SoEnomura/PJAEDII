# Projeto - Estruturas de Dados Dinâmicas em C

Estruturas dinâmicas:

O sistema desenvolvido manipula uma ou mais estrutura de dados dinâmica; no código, são utilizadas as funcionalidades chave discutidas durante as aulas:

- Inserção
- Remoção
- Visualização
- Verificações adicionais, como: checar se a estrutura está vazia, checar seu tamanho, checar topo/inicio/fim, etc. 

Manipulação de arquivos:

O sistema armazena e recupera todos os dados relevantes em arquivos. O sistema captura e salva o estado corrente, para que, ao ser executado novamente, ele possa retomar do ponto onde parou. O sistema é capaz de salvar os dados alocados em estruturas dinâmicas ao ser encerrado ou ao ser solicitado um salvamento. Ao inicializar, o sistema é capaz de ler os dados de um arquivo e alocá-los novamente em estruturas dinâmicas.

# Situação escolhida: League of Legends Build

O jogador pode registrar os itens que gostaria de comprar ao longo de uma partida. Esses itens ficam armazenados em um inventário de interesse, implementado através de uma lista, permitindo inserção, remoção, listagem e busca de itens conforme necessário. A cada rodada, o jogador recebe uma quantidade aleatória de ouro, que ele poderá usar para comprar os itens registrados. O valor mínimo e máximo de ouro recebido por rodada aumenta de acordo com o valor total da build atual, simulando um sistema de progressão econômica. Assim, quanto mais valiosa for a build que o jogador já montou, mais ouro ele recebe por rodada, incentivando a compra de itens e o fortalecimento gradual do personagem.

Quando o jogador decide comprar um item, o programa verifica se ele possui ouro suficiente, e adiciona o item à build atual, que também é gerenciada por uma lista encadeada dinâmica. Caso o jogador queira, ele também pode vender um item da build, recuperando o valor pago por ele em ouro, o que adiciona flexibilidade estratégica ao sistema. Além disso, o jogador pode consultar sua build a qualquer momento, ver o valor total investido e a quantidade de ouro disponível.
