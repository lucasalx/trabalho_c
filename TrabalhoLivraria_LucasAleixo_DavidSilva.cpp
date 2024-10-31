#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define LIVROS "Livros.dat"
#define CLIENTES "Clientes.dat"
#define VENDAS "Vendas.dat"
#define AUTOR "Autor.dat"
#define RELACIONAMENTO "Relacionamento.dat"

struct reg_livro {
    int codigo;
    char titulo[30];
    float preco;
};

struct reg_cliente {
    int codigo;
    char nome[40];
    char fone[15];
    char email[30];
};

struct reg_venda {
    int codigo; //Código da Venda
    int codcli; //Código do Cliente
    int codliv; //Código do Livro a ser vendido
    int qtde; //Quantidade vendida
    float desconto; //Desconto
};

struct reg_autor {
	char nome[40];
	int codigo;
};

struct reg_relacionamento {
	int codigo;
	int codliv;
	int codaut;
};


void converteParaMaiusculo(char string[]) {
    for(int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
} //Fim converteParaMaiusculo()

void cadastrarLivro(){
    FILE *fplivro;
    struct reg_livro livro;
    char opc;
  
    //Entrada de Dados - Pedir os dados do Livro
    printf("\nDigite o Codigo:");
    fflush(stdin); scanf("%i",&livro.codigo);
    printf("Titulo: ");
    fflush(stdin); gets(livro.titulo);
    printf("Preco: ");
    fflush(stdin); scanf("%f", &livro.preco);
  
    printf("\nGravar Livro?(S/N)");
    fflush(stdin); scanf("%c",&opc);
  
    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao menu principal
    }
  
    //Abrir o Arquivo
    fplivro = fopen(LIVROS, "ab");
  
    //Gravar o Registro
    fwrite(&livro, sizeof(livro), 1, fplivro);
  
    //Fechar o Arquivo
    fclose(fplivro);
  
    printf("\nLivro gravado com sucesso.");
} //Fim cadastrarLivro()

void cadastrarCliente() {
    FILE *fpcliente;
    struct reg_cliente cliente;
    char opc;
  
    //Entrada de Dados - Pedir os dados do cliente
    printf("\nDigite o Codigo:");
    fflush(stdin); scanf("%i", &cliente.codigo);
    printf("Nome: ");
    fflush(stdin); gets(cliente.nome);
    printf("Fone: ");
    fflush(stdin); gets(cliente.fone);
    printf("Email: ");
    fflush(stdin); gets(cliente.email);
  
    printf("\nGravar Cliente?(S/N)");
    fflush(stdin); scanf("%c", &opc);
  
    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao menu principal
    }
  
    //Abrir o Arquivo
    fpcliente= fopen(CLIENTES, "ab");
  
    //Gravar o Registro
    fwrite(&cliente, sizeof(cliente), 1, fpcliente);
  
    //Fechar o Arquivo
    fclose(fpcliente);
  
    printf("\nCliente gravado com sucesso.");
} //Fim cadastrarCliente()

struct reg_autor buscarAutorPeloCodigo(int codbuscar) {
	FILE *fpautor;
	int achou;
	struct reg_autor autor;
	
	fpautor = fopen(AUTOR, "rb");
	achou = 0;
	while((achou == 0) && (fread(&autor, sizeof(autor), 1, fpautor) == 1)) {
        if(autor.codigo == codbuscar) { //Achei o autor
            achou = 1;
        }
    }

    //Fechar o Arquivo
    fclose(fpautor);

    if(achou != 1) {
        autor.codigo = -1;
    }
    
    return autor;
}

void listarLivros_Autores() { // Alterado
    FILE *fplivro, *fprelacionamento;

    struct reg_livro livro;
    struct reg_relacionamento relacionamento;
    struct reg_autor autor;
    int codbuscar;
  
    //Abrir o arquivo
    fprelacionamento = fopen (RELACIONAMENTO, "rb");
    fplivro = fopen(LIVROS, "rb");
  
    printf("\nRelatório de Todos os Livros e seus Autores:\n");

    // Ler registro por registro de livros
    while(fread(&livro, sizeof(livro), 1, fplivro) == 1) {
        printf("\nCódigo: %i", livro.codigo);
        printf("\nTítulo: %s", livro.titulo);
        printf("\nPreço: %.2f", livro.preco);
        printf("\nAutores: ");

        // Inicializa uma variável para controlar se há autores para o livro
        int encontrouAutores = 0;

        // Reinicia a leitura da tabela de relacionamento
        fseek(fprelacionamento, 0, SEEK_SET);

        // Ler a tabela de relacionamento para encontrar autores
        while(fread(&relacionamento, sizeof(relacionamento), 1, fprelacionamento) == 1) {
            if (livro.codigo == relacionamento.codliv) {
                // Buscar autor pelo código
                autor = buscarAutorPeloCodigo(relacionamento.codaut);
                printf("%s; ", autor.nome); // Listar autores
                encontrouAutores = 1; // Indica que pelo menos um autor foi encontrado
            }
        }

        // Se não encontrou autores, exibir mensagem
        if(!encontrouAutores) {
            printf("Nenhum autor encontrado.");
        }

        printf("\n--------------------------------------------");
    }
  
    //Fechar o Arquivo
    fclose(fplivro);
    fclose(fprelacionamento);
	
} //Fim listarLivros_Autores()

void listarClientes() {
    FILE *fpcliente;
    struct reg_cliente cliente;

    //Abrir o arquivo
    fpcliente = fopen(CLIENTES, "rb");

    printf("\nRelatorio Todos os Clientes: ");
    printf("\nCodigo Nome                           Fone            Email");

    //Ler registro por registro e mostar na tela
    while(fread(&cliente, sizeof(cliente), 1, fpcliente) == 1) {
        printf("\n%-6i %-30s %-15s %s", cliente.codigo, cliente.nome, cliente.fone, cliente.email);
    }

    //Fechar o Arquivo
    fclose(fpcliente);
	
} //Fim listarClientes()

void consultarPeloCodigo() {
    FILE *fplivro;
    struct reg_livro livro;
    int codbuscar, achou;
  
    //Entrada de Dados - Pedir o Código do Livro
    printf("\nDigite o Codigo do Livro a ser Localizado: ");
    fflush(stdin); scanf("%i", &codbuscar);
  	
    //Abrir o Arquivo para Leitura
    fplivro = fopen(LIVROS, "rb");
  
    //Localizar e Mostrar o Livro
    achou = 0;
    while((achou == 0) && (fread(&livro, sizeof(livro), 1, fplivro) == 1)) {
        //printf("\nEntrou");
        if(livro.codigo == codbuscar) { //Achei o Livro
            printf("\nCodigo: %i",livro.codigo);
            printf("\nTitulo: %s",livro.titulo);
            printf("\npreco: %5.2f",livro.preco);
            printf("\n--------------------------------------------");
            achou = 1;
        }
    }

    if(achou == 0) {
        printf("\nLivro Nao Localizado!");
    }

    //Fechar o Arquivo
    fclose(fplivro);
  	
} //Fim consultarPeloCodigo()

struct reg_relacionamento buscarRelacionamentoPeloCodigo(int codbuscar) {
    FILE *fprelacionamento;
    int achou;
    struct reg_relacionamento relacionamento;

    fprelacionamento = fopen(RELACIONAMENTO, "rb");
    achou = 0;
    while((achou == 0) && (fread(&relacionamento, sizeof(relacionamento), 1, fprelacionamento) ==1 )){
        if(relacionamento.codigo==codbuscar) { //Achei o venda
            achou = 1;
        }
    }

    //Fechar o Arquivo
    fclose(fprelacionamento);

    if(achou != 1) {//Venda NÃO Localizado
        relacionamento.codigo = -1;
    }
    
    return relacionamento;
}

struct reg_venda buscarVendaPeloCodigo(int codbuscar) {
	FILE *fpvenda;
	int achou;
	struct reg_venda venda;
	
	fpvenda = fopen(VENDAS, "rb");
	achou = 0;
	while((achou == 0) && (fread(&venda, sizeof(venda), 1, fpvenda) == 1)) {
        if(venda.codigo == codbuscar) { //Achei o venda
            achou=1;
        }
    }

    //Fechar o Arquivo
    fclose(fpvenda);

    if(achou != 1) { //Venda NÃO Localizado
        venda.codigo = -1;
    }
    
    return venda;
}

struct reg_cliente buscarClientePeloCodigo(int codbuscar) {
    FILE *fpcliente;
    struct reg_cliente cliente;
    int achou;
  
    //Abrir o Arquivo para Leitura
    fpcliente = fopen(CLIENTES, "rb");
  
    //Localizar o cLIENTE
    achou = 0;
    while((achou == 0) && (fread(&cliente, sizeof(cliente), 1, fpcliente) == 1)) {
        if(cliente.codigo == codbuscar) { //Achei o cliente
            achou = 1;
        }
    }

    //Fechar o Arquivo
    fclose(fpcliente);

    if(achou != 1) { //cliente NÃO Localizado
        cliente.codigo = -1;
    }
    
    return cliente;
 	
} //Fim buscarClientePeloCodigo()

struct reg_livro buscarLivroPeloCodigo(int codbuscar) {
    FILE *fplivro;
    struct reg_livro livro;
    int achou;
  
    //Abrir o Arquivo para Leitura
    fplivro = fopen(LIVROS, "rb");
  
  //Localizar o Livro
    achou = 0;
    while((achou==0) && (fread(&livro, sizeof(livro), 1, fplivro) == 1)) {
        if(livro.codigo == codbuscar) { //Achei o Livro
            achou=1;
        }
    }

    //Fechar o Arquivo
    fclose(fplivro);

    if(achou != 1) { //lIVRO NÃO Localizado
        livro.codigo = -1;
    }
    
    return livro;
 	
} //Fim buscarlIVROPeloCodigo()

void consultarPeloTitulo() {
    FILE *fplivro;
    struct reg_livro livro;
    int achou;
    char titbuscar[30];
  
    //Entrada de Dados - Pedir o Título do Livro
    printf("\nDigite o Titulo do Livro a ser Localizado: ");
    fflush(stdin); gets(titbuscar);
    converteParaMaiusculo(titbuscar);
  	
    //Abrir o Arquivo para Leitura
    fplivro = fopen(LIVROS,"rb");

    //Localizar e Mostrar o Livro
    achou = 0;
    while((achou==0) && (fread(&livro, sizeof(livro), 1, fplivro) == 1)) {
        converteParaMaiusculo(livro.titulo);
        if(strcmp(livro.titulo,titbuscar) == 0) { //Achei o Livro
            printf("\nCodigo: %i",livro.codigo);
            printf("\nTitulo: %s",livro.titulo);
            printf("\npreco: %5.2f",livro.preco);
            printf("\n--------------------------------------------");
            achou = 1;
        }
    }

    if(achou == 0) {
        printf("\nLivro Nao Localizado!");
    }

    //Fechar o Arquivo
    fclose(fplivro);
} //Fim consultarPeloTitulo()

void consultarPalavraChaveTitulo() {
    FILE *fplivro;
    struct reg_livro livro;
    int achou;
    char palavra[30];

    //Entrada de Dados - Pedir a Palavra-Chave
    printf("\nDigite a Palavra-Chave: ");
    fflush(stdin); gets(palavra);
    converteParaMaiusculo(palavra);

    //Abrir o Arquivo para Leitura
    fplivro = fopen(LIVROS, "rb");

    //Localizar e Mostrar o Livro
    printf("\nRelatorio Livros com Palavra-Chave %s:", palavra);
    printf("\nCodigo Titulo                         Preco");
    achou = 0;

    while(fread(&livro, sizeof(livro), 1, fplivro) == 1) {
        converteParaMaiusculo(livro.titulo);
        if(strstr(livro.titulo, palavra) != NULL) { //Achei o Livro
            printf("\n%-6i %-30s %-5.2f",livro.codigo, livro.titulo, livro.preco);
            achou = 1;
        }
    }

    if(achou == 0) {
        printf("\nNenhum Livro com esta Palavra-Chave no Titulo!");
    }

    //Fechar o Arquivo
    fclose(fplivro);
} //Fim consultarPalavraChaveTitulo()

void alterarLivro() {
    FILE *fplivro;
    struct reg_livro livro, liv;
    int cod,achou; //Código do Livro a ser Alterado
    char opc;

    //Pedir o código do Livro a ser alterado
    printf("\nDigite o Codigo do Livro a ser Alterado: ");
    fflush(stdin); scanf("%i", &cod);

    //Localizar o Livro e Mostrar os dados
    livro = buscarLivroPeloCodigo(cod);
    if(livro.codigo == -1) {
        printf("\nLivro NAO Localizado!");
        return;  //Volta para o Menu Principal
    }

    //Mostrar dados do Livro Localizado
    printf("\nCodigo: %i",livro.codigo);
    printf("\nTitulo: %s",livro.titulo);
    printf("\npreco: %5.2f",livro.preco);

    printf("\nConfirna Livro(S/N)? ");
    fflush(stdin); scanf("%c",&opc);

    if((opc == 'n') || (opc == 'N')) {
        printf("\n Livro NAO confirmado!");
        return; //Volta para o menu principal
    }

    //Solicitar novos dados
    printf("\nDeseja alterar o Titulo(S/N)? ");
    fflush(stdin); scanf("%c", &opc);

    if((opc == 's') || (opc == 'S')) {
        printf("\nDigite o novo Titulo: ");
        fflush(stdin); gets(livro.titulo);
    }

    printf("\nDeseja alterar o Preco(S/N)? ");
    fflush(stdin); scanf("%c",&opc);

    if((opc == 's') || (opc == 'S')) {
        printf("\nDigite o novo preco: ");
        fflush(stdin); scanf("%f", &livro.preco);
    }

    printf("\nGravar Livro(S/N)? ");
    fflush(stdin); scanf("%c",&opc);

    if((opc == 'n') || (opc == 'N')) {
        printf("\n Operacao Cancelada!");
        return; //Volta para o menu principal
    }

    fplivro = fopen(LIVROS, "rb+");

    //Posicionar o ponteiro do arquivo no registro a ser alterado
    achou = 0;
    while((achou == 0) && (fread(&liv, sizeof(liv), 1, fplivro) == 1)) {
        if(liv.codigo == cod) {
            achou = 1;
        }
    }

    //Preciso voltar o ponteiro para o regitro anterior
    fseek(fplivro,-sizeof(livro), 1);

    //Gravar livro alterado
    fwrite(&livro,sizeof(livro), 1, fplivro);

    //Fechar o arquivo
    fclose(fplivro);

    printf("\nLivro alterado com sucesso.");
} //Fim alterarLivro()

void  excluirLivro() {
    FILE *fplivro, *fplivronew;
    struct reg_livro livro;
    int cod;
    char opc;

    //Solicitar o código do Livro a ser excluído
    printf("\nDigite o Codigo do Livro a ser Excluido: ");
    fflush(stdin); scanf("%i", &cod);

    //Localizar o Livro e Mostrar os dados
    livro = buscarLivroPeloCodigo(cod);
    if(livro.codigo == -1) {
        printf("\nLivro NAO Localizado!");
        return;  //Volta para o Menu Principal
    }

    //Mostrar dados do Livro Localizado
    printf("\nCodigo: %i", livro.codigo);
    printf("\nTitulo: %s", livro.titulo);
    printf("\npreco: %5.2f", livro.preco);

    //Pedir Confirmação
    printf("\nConfirna Exclusao do Livro(S/N)? ");
    fflush(stdin); scanf("%c", &opc);

    if((opc == 'n') || (opc == 'N')) {
        printf("\n Exlusao NAO confirmada!");
        return; //Volta para o menu principal
    }

    //Se confirmado - Excluir o Livro do Arquivo
    //Criar um novo arquivo (livrosnew.dat) e copiar todos os livros
    // menos o Livro sendo Excluído
    fplivro = fopen(LIVROS, "rb");
    fplivronew = fopen("livrosnew.dat","wb");
    while(fread(&livro, sizeof(livro), 1, fplivro) == 1) {
        if(livro.codigo != cod) {
            fwrite(&livro, sizeof(livro), 1, fplivronew);
        }
    }

    fclose(fplivro);
    fclose(fplivronew);

    //Excluir o arquivo antigo livros.dat do sistema
    system("del livros.dat");

    //Renomear o novo arquivo (livrosnew.dat) para (livros.dat)
    system("ren livrosnew.dat livros.dat");

    printf("\nLivro excluido com sucesso.");
} //Fim excluirLivro()

void efetuarVenda() {
    FILE *fpvenda;
    struct reg_livro livro;
    struct reg_cliente cliente;
    struct reg_venda venda;
    int codcli, codliv;
    char opc;

    //Entrada de Dados
    //Pedir o código da Venda
    printf("\nDigite o Codigo da Venda: ");
    fflush(stdin); scanf("%i",&venda.codigo);

    //Pedir o Codigo do Cliente
    printf("\nDigite o Codigo do Cliente: ");
    fflush(stdin); scanf("%i", &codcli);
    //Localizar e Mostrar os dados do Cliente e pedir confirmação
    cliente = buscarClientePeloCodigo(codcli);

    if(cliente.codigo == -1) {
        printf("\nCliente Nao Localizado!");
        return; //Retorna para o menu Principal
    }

    printf("\nCodigo: %i - Nome: %s", cliente.codigo,cliente.nome);

    //Pedir o Código do Livro
    printf("\nDigite o Codigo do Livro: ");
    fflush(stdin); scanf("%i", &codliv);
    //Localizar e Mostrar os dados do Livro e pedir confirmação
    livro = buscarLivroPeloCodigo(codliv);

    if(livro.codigo == -1) {
        printf("\nLivro Nao Localizado!");
        return; //Retorna para o menu Principal
    }

    printf("\nCodigo: %i - Titulo: %s", livro.codigo, livro.titulo);

    printf("\nConfirma Cliente e Livro(S/N)? ");
    fflush(stdin); scanf("%c", &opc);

    if((opc != 's') && (opc != 'S')) {
        printf("\nNao confirmado.");
        return; //Retorna ao Menu principal
    }

    //Pedir quantidade e Desconto
    printf("\nDigite a Quantidade: ");
    fflush(stdin); scanf("%i",&venda.qtde);

    printf("\nDigite o Desconto: ");
    fflush(stdin); scanf("%f",&venda.desconto);

    //Gravar a Venda
    printf("\nGravar Venda(S/N)? ");
    fflush(stdin); scanf("%c",&opc);

    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao Menu principal
    }

    venda.codcli = cliente.codigo;
    venda.codliv = livro.codigo;

    fpvenda = fopen(VENDAS, "ab");

    fwrite(&venda, sizeof(venda), 1, fpvenda);

    fclose(fpvenda);

    printf("\nVenda registrada com sucesso.");
} //Fim efetuarVenda()

void listarVendas() {
    FILE *fpvenda;
    struct reg_venda venda;

    //Abrir o Arquivo de Vendas
    fpvenda = fopen(VENDAS,"rb");

    //Ler registro por registro e mostrar na tela
    while(fread(&venda, sizeof(venda), 1, fpvenda) == 1) {
        printf("\nCodigo Venda: %i",venda.codigo);
        printf("\nCodigo Cliente: %i",venda.codcli);
        printf("\nCodigo Livro: %i",venda.codliv);
        printf("\nQuantidade: %i",venda.qtde);
        printf("\nDesconto: %5.2f",venda.desconto);
        printf("\n----------------------------------------");
    }

    //Fechar o Arquivo
    fclose(fpvenda);
} //Fim listarVendas()

void relatorioVendasDetalhado() {
    FILE *fpvenda;
    struct reg_venda venda;
    struct reg_cliente cliente;
    struct reg_livro livro;
    int vendaanterior;

    //Abrir o Arquivo de Vendas
    fpvenda = fopen(VENDAS, "rb");

    printf("\n         Relatorio de Vendas - Detalhado");
    printf("\nVenda Cliente                      Livro                        Preco  Desc  Qtde Cobrado");
    //Ler registro por registro e mostrar na tela
    vendaanterior = -1;
    while(fread(&venda, sizeof(venda), 1, fpvenda) == 1) {
        cliente = buscarClientePeloCodigo(venda.codcli);
        livro = buscarLivroPeloCodigo(venda.codliv);

        if(venda.codigo == vendaanterior) {
            printf("\n                                   %-2i-%-25s %-6.2f %-6.2f %-4i %5.2f",livro.codigo,livro.titulo,livro.preco,venda.desconto,venda.qtde,(livro.preco*venda.qtde)-venda.desconto);
        } else{
            printf("\n%-5i %-2i-%-25s %-2i-%-25s %-6.2f %-6.2f %-4i %5.2f", venda.codigo,cliente
            .codigo,cliente.nome,livro.codigo,livro.titulo,livro.preco,venda.desconto,venda.qtde,(livro.preco*venda.qtde)-venda.desconto);
        }
        vendaanterior = venda.codigo;
    }

    //Fechar o Arquivo
    fclose(fpvenda);
} //Fim relatorioVendasDetalhado()

void fecharVendaCaixa() { // Feito!
	FILE *fpvenda;
	struct reg_livro livro;
	struct reg_cliente cliente;
	struct reg_venda venda;
	int codbuscar;
	float total=0;

	
	//Abrir o arquivo de vendas.
	fpvenda = fopen(VENDAS, "rb");
	
	printf("\n\n###### Fechamento De Vendas ######");
	
	//Usuario entrar com o codigo de venda.
	printf("\nDigite o codigo da venda: ");
	fflush(stdin);  scanf("%i", &codbuscar);
	
	venda = buscarVendaPeloCodigo(codbuscar);
	if(venda.codigo == -1){
		printf("Venda NAO Localizada");
		return;
	}
	
	cliente = buscarClientePeloCodigo(venda.codcli);
	printf("\nVenda: %i",codbuscar);
	printf("\nCliente: %s", cliente.nome);
	
	printf("\nCodigo Titulo                    Qtde. Preco Desconto ValorPago");
	
	//Imprimir todos os itens da venda.
    while(fread(&venda, sizeof(venda), 1, fpvenda) == 1) {
        if(venda.codigo == codbuscar) {
            livro = buscarLivroPeloCodigo(venda.codliv);
            printf("\n%-6i-%-25s %-5i %-5.2f %-8.2f %9.2f",livro.codigo, livro.titulo, venda.qtde,
            livro.preco, venda.desconto, (livro.preco * venda.qtde) - venda.desconto);
            total = total + ((livro.preco * venda.qtde) - venda.desconto);
        }
    }

	printf("\nTotal                                                     %-0.2f", total);
	
	//fechar o aquivo de vendas.
	fclose(fpvenda);
} //fim fecharVendaCaixa();

void aplicarAumento() { // Feito!
	//Criar um novo arquivo e dopois excluir.
	FILE *fplivro, *fpnewlivro;
	float aumento, porcentagem;
	struct reg_livro livro;
	
	fplivro = fopen(LIVROS, "rb");
	fpnewlivro = fopen("livrosnew.dat", "wb");
	
	printf("Digite a porcentagem de aumento (ex: 10 para 10%%): ");
    fflush(stdin);  scanf("%f", &porcentagem);
	
	aumento = (porcentagem / 100.0);
	
	 // Ler cada livro, aplicar o aumento e gravar no novo arquivo
	 while(fread(&livro, sizeof(livro), 1, fplivro) == 1) {
        livro.preco += livro.preco * aumento; // Aplicar aumento
        fwrite(&livro, sizeof(livro), 1, fpnewlivro); // Gravar no novo arquivo
    }
    
    fclose(fplivro);
    fclose(fpnewlivro);
    
    remove(LIVROS); // Excluir o arquivo original
    rename("livrosnew.dat", LIVROS); // Renomear o novo arquivo

    printf("Aumento de %.2f%% aplicado com sucesso.\n", porcentagem);
}

void cadastrarAutor() { // Feito!
	FILE *fpautor;
	struct reg_autor autor;
	char opc;
	
	printf("\nDigite o Codigo do Autor: ");
	fflush(stdin); scanf("%i", &autor.codigo);
	
	printf("\nDigite o Nome do Autor: ");
	fflush(stdin); gets(autor.nome);
	
	printf("\nGravar Autor?(S/N)");
    fflush(stdin); scanf("%c",&opc);
  
    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao menu principal
    }

    //Abrir o arquivo
    fpautor = fopen (AUTOR, "ab");

    //Gravar no arquivo
    fwrite(&autor, sizeof(autor), 1, fpautor);

    //Fechar arquivo
    fclose(fpautor);

    printf("\nAutor Gravado Com Sucesso!");
} //Fim cadastrarAuor();

void listarAutores() { // Feito!
	FILE *fpautor;
    struct reg_autor autor;

    //Abrir o arquivo
    fpautor = fopen(AUTOR, "rb");

    printf("\n###### Relatório de Autores ######\n");

    //Ler registro por registro e mostar na tela
    while(fread(&autor, sizeof(autor), 1, fpautor) == 1) {
        printf("\nCodigo: %i",autor.codigo);
        printf("\nNome: %s",autor.nome);

        printf("\n--------------------------------------------");
    }

    //Fechar o Arquivo
    fclose(fpautor);
} //Fim listarAutores();

void cadastrarRelacionamento() { // Feito!
	FILE *fprelacionamento;
	struct reg_autor autor;
	struct reg_livro livro;
	struct reg_relacionamento relacionamento;
	char opc;
	
	printf("\nCadastrar Autor no Livro.");
	
	//Entrada de dados
	printf("\nDigite o Codigo de Relacionamento: ");
	fflush(stdin);  scanf("%i", &relacionamento.codigo);
	
	//Informando o codigo do autor
	printf("\nDigite o Codigo do autor: ");
	fflush(stdin);  scanf("%i", &relacionamento.codaut);
	
	autor = buscarAutorPeloCodigo(relacionamento.codaut);
	
	if(autor.codigo == -1) {
        printf("\nAutor Nao Localizado!");
        return; //Retorna para o menu Principal
    }
   
   printf("\n\n%s", autor.nome);
   
    //Confirmando o Autor
    printf("\nEncontrou o Autor(S/N)?");
    fflush(stdin); scanf("%c", &opc);

    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao Menu principal
    }

    //Informando o codigo do Livro
    printf("\nDigite o Codigo do Livro: ");
    fflush(stdin);  scanf("%i", &relacionamento.codliv);

    livro = buscarLivroPeloCodigo(relacionamento.codliv);
    if(livro.codigo == -1) {
        printf("\nLivro Nao Localizado!");
        return; //Retorna para o menu Principal
    }

    printf("\n%s", livro.titulo);

    //Confirmando o Livro
    printf("\nEncontrou o Livro(S/N)? ");
    fflush(stdin); scanf("%c", &opc);

    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao Menu principal
    }

    //Ferificando se deseja Gravar o Relacionamento
    printf("\nGravar Relacionamento(S/N)? ");
    fflush(stdin); scanf("%c",&opc);

    if((opc != 's') && (opc != 'S')) {
        printf("\nOperacao Cancelada!");
        return; //Retorna ao Menu principal
    }

    //Gravando dados
    fprelacionamento = fopen(RELACIONAMENTO, "ab");

    fwrite(&relacionamento, sizeof(relacionamento), 1, fprelacionamento);

    fclose(fprelacionamento);

    //fedback para o usuario
    printf("\nRelacionamento de Livro e Autor Gravado com Sucesso!");
} //Fim cadastrarRelacionamento();

void listarRelacionamento() { // Feito!
	FILE *fprelacionamento;
	struct reg_relacionamento relacionamento;
	struct reg_livro livro;
	struct reg_autor autor;
	int cod_anterior;
	
	fprelacionamento = fopen(RELACIONAMENTO, "rb");
	
	printf("\nRelatorio de Relacionamento: ");
	
	printf("\nCodigo     Titulo Livro                   Nome Autor                    ");
	
	cod_anterior = -1;
	
	while(fread(&relacionamento, sizeof(relacionamento), 1, fprelacionamento) == 1) {
        autor = buscarAutorPeloCodigo(relacionamento.codaut);
        livro = buscarLivroPeloCodigo(relacionamento.codliv);

        if(relacionamento.codigo == cod_anterior) {
            printf("\n           %-30s %-30s",livro.titulo, autor.nome);
        } else {
            printf("\n%-10i %-30s %-30s", relacionamento.codigo, livro.titulo, autor.nome);
        }

        cod_anterior = relacionamento.codigo;
	}
	
    printf("\n--------------------------------------------");
	fclose(fprelacionamento);
}

void consultarLivroPeloAutor() { // Feito!
    FILE *fplivro, *fprelacionamento;
    struct reg_livro livro;
    struct reg_relacionamento relacionamento;
    struct reg_autor autor;
    int codAutor;

    // Abrir os arquivos
    fplivro = fopen(LIVROS, "rb");
    fprelacionamento = fopen(RELACIONAMENTO, "rb");

    // Solicitar ao usuário o código do autor
    printf("Digite o codigo do autor: ");
    fflush(stdin);
    scanf("%i", &codAutor);

    // Verificar se o autor existe
    autor = buscarAutorPeloCodigo(codAutor);
    if(autor.codigo == -1) {
        printf("Autor nao encontrado.\n");
        fclose(fplivro);
        fclose(fprelacionamento);
        return;
    }

    printf("\nLivros do Autor: %s\n", autor.nome);
    printf("--------------------------------------------\n");

    // Reiniciar a leitura da tabela de relacionamento
    fseek(fprelacionamento, 0, SEEK_SET);
    int encontrouLivros = 0; // Para controlar se encontramos livros

    // Ler a tabela de relacionamento para encontrar livros do autor
    while(fread(&relacionamento, sizeof(relacionamento), 1, fprelacionamento) == 1) {
        if(relacionamento.codaut == codAutor) {
            // Se encontrou o autor, buscar o livro correspondente
            if(fseek(fplivro, (relacionamento.codliv - 1) * sizeof(livro), SEEK_SET) == 0) {
                fread(&livro, sizeof(livro), 1, fplivro);
                printf("Código: %i\n", livro.codigo);
                printf("Título: %s\n", livro.titulo);
                printf("Preço: %.2f\n", livro.preco);
                printf("--------------------------------------------\n");
                encontrouLivros = 1; // Indicando que ao menos um livro foi encontrado
            }
        }
    }

    if(!encontrouLivros) {
        printf("Nenhum livro encontrado para este autor.\n");
    }

    // Fechar os arquivos
    fclose(fplivro);
    fclose(fprelacionamento);
} // Fim consultarLivroPeloAutor()

main() {
    int op;

    do {
        printf("\n\n\n");
        printf("\n#########   Livraria Alma do Saber   ###########");
        printf("\n#                                              #");
        printf("\n# 1) Cadastrar Livro                           #");
        printf("\n# 111) Cadastrar Cliente                       #");
        printf("\n# 2) Listar Todos os Livros e Autores          #"); //Alterando!
        printf("\n# 222) Listar Todos os Clientes                #");
        printf("\n# 3) Consultar Livro pelo Codigo               #");
        printf("\n# 4) Consultar Livro pelo Titulo               #");
        printf("\n# 5) Consultar Livro por Palavra-Chave         #");
        printf("\n# 6) Alterar Livro                             #");
        printf("\n# 7) Excluir Livro                             #");
        printf("\n# 8) Efetuar Venda                             #");
        printf("\n# 9) Listar Vendas                             #");
        printf("\n# 10) Relatorio de Vendas Detalhado            #");
        printf("\n# 11) Fechar Venda Caixa                       #");
        printf("\n# 12) Aplicar Aumento                          #");
        printf("\n# 13) Cadastrar Autor                          #");
        printf("\n# 14) Listar Autores                           #");
        printf("\n# 15) Cadastrar Relacinamento                  #");
        printf("\n# 16) Listar Relacionamento                    #");
        printf("\n# 17) Consultar Livro Pelo Autor               #");
        printf("\n# 0) Sair                                      #");
        printf("\n#                                              #");
        printf("\n# Opcao-> ");

        fflush(stdin); scanf("%i", &op);
        switch(op) {
            case 1: //Cadastrar um novo Livro
                cadastrarLivro();
                break;

            case 111: //Cadastrar um novo Cliente
                cadastrarCliente();
                break;

            case 2: //Listar todos os Livros
                listarLivros_Autores();
                break;

            case 222: //Listar todos os Clientes
                listarClientes();
                break;

            case 3: //Consultar Livro pelo Código
                consultarPeloCodigo();
                break;

            case 4: //Consultar Livro pelo Titulo
                consultarPeloTitulo();
                break;

            case 5: //Consultar Livro por Palavra-Chave no Titulo
                consultarPalavraChaveTitulo();
                break;

            case 6: //Alterar dados de um Livro
                alterarLivro();
                break;

            case 7: //Excluir um Livro
                excluirLivro();
                break;

            case 8: //Efetuar Venda
            efetuarVenda();
            break;

            case 9: //Listar Vendas
                listarVendas();
                break;

            case 10: //Relatorio de Vendas Detalhado
                relatorioVendasDetalhado();
                break;

            case 11:
                fecharVendaCaixa();
                break;

            case 12:
                aplicarAumento();
                break;

            case 13:
                cadastrarAutor();
                break;

            case 14:
                listarAutores();
                break;
            
            case 15:
                cadastrarRelacionamento();
                break;

            case 16:
                listarRelacionamento();
                break;

            case 17:
                consultarLivroPeloAutor();
                break;
        }
    } while(op != 0);
} //Fim main()
