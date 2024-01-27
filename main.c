#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>
#include <conio.h>

//Wagner Gaspar


 FILE *file;


struct Pessoa{
    int telefone,codigo_do_cliente;
    char nome[40],localidade[30], bi[10];
};


struct Conta{
    int numero_de_conta,tipo_de_conta,codigo_cliente,estado;
	float saldo;
};



typedef struct Pessoa pessoa;
typedef struct Conta conta;

pessoa p, cliente[100];
conta contas[100];
int qtdCliente = 0,qtdContas = 0;

/*void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}*/

void continuar(){
    char p;
    printf("Digite qualquer tecla para continuar: ");
    scanf("%c",&p);
    getchar();
}

int buscar_indice(int codigo){
    for(int i = 0; i < qtdCliente; i++){
        if(codigo == cliente[i].codigo_do_cliente){
            return i;
        }
    }
    return -1;
}

int buscar_conta(int codigo){
    for(int i = 0; i < qtdCliente; i++){
        if(codigo == contas[i].codigo_cliente){
            return i;
        }
    }
    return -1;
}


int verificar_numero_de_conta(int num_conta){
    for(int i =0; i<qtdCliente;i++){
        if(num_conta == contas[i].numero_de_conta){
            return i;
        }
    }
    return -1;
}

bool cadastro_de_cliente(struct Pessoa p){
    cliente[qtdCliente] = p;
    cliente[qtdCliente].codigo_do_cliente = p.codigo_do_cliente + qtdCliente;
    qtdCliente++;

    return true;
}
//abertura de conta
bool abertura_de_conta(struct Pessoa p, int tipo_de_conta){

			contas[qtdContas].codigo_cliente = qtdContas;
			contas[qtdContas].tipo_de_conta = tipo_de_conta;
			contas[qtdContas].numero_de_conta = qtdContas + 1000;
			contas[qtdContas].saldo = 0.0;
			contas[qtdContas].estado = 0;

			qtdContas++;

		return true;
	}
bool depositar_dinheiro(int num_da_conta, float valor){
    int num = verificar_numero_de_conta(num_da_conta);

    if((num != -1) && (valor >= 10 && valor <12500000)){
        contas[num].saldo = valor + contas[num].saldo;
        contas[num].estado = 1;

        return true;
    }
    return false;
}

bool levantar_dinheiro(int num_da_conta, float valor){
    int num = verificar_numero_de_conta(num_da_conta);

    if((num != -1) && (valor <= contas[num].saldo && valor <= 10000000)){
        contas[num].saldo = contas[num].saldo - valor;
        return true;
    }
    return false;
}

float consultar_saldo(int num_da_conta){
    int num = verificar_numero_de_conta(num_da_conta);

    if(num != -1){
        return contas[num].saldo;
    }
    return 0.0;
}

bool transferir_dinheiro(int num_da_conta_enviar, int num_da_conta_receber, float valor){
    int indice_enviar = verificar_numero_de_conta(num_da_conta_enviar);
    int indice_receber = verificar_numero_de_conta(num_da_conta_receber);

    if((indice_receber != -1) && (indice_enviar != -1)){
        if((valor >= 10 && valor < 12500000)){
            contas[indice_enviar].saldo -= valor;
            contas[indice_receber].saldo += valor;
            return true;
        }
    }
    return false;
}

bool atualisar_dados(struct Pessoa p, int codigo_cliente){
    int indice = buscar_indice(codigo_cliente);
    cliente[indice] = p;

    return true;
}


//Criar ficheiro

void CriarFicheiro(){
    file= fopen("C:\\ProetoEmC\\ProjetoEmC\\ProjetoBD.txt","a");

    if (file == NULL){
        file = fopen("C:\\ProetoEmC\\ProjetoEmC\\ProjetoBD.txt","w");
        if(file == NULL){
            printf("Não foi possível criar ficheiro");
            exit(EXIT_FAILURE);
        }
        printf("Ficheiro criado");
    }
}


//ECREVER OS DADOS NO ARQUIVO
void copia_de_seguranca(){
        file = fopen("C:\\ProetoEmC\\ProjetoEmC\\ProjetoBD.txt", "w");

        for(int i = 0; i < qtdCliente; i++){
            fprintf(file, "%s|%d|%s|%s|%d|", cliente[i].nome, cliente[i].codigo_do_cliente, cliente[i].bi, cliente[i].localidade , cliente[i].telefone);

            int indice = buscar_indice(cliente[i].codigo_do_cliente);

            if(indice != -1){
                fprintf(file,"%d|%d|%d|%f|%d\n",contas[indice].codigo_cliente,contas[indice].tipo_de_conta,contas[indice].numero_de_conta,contas[indice].saldo,contas[indice].estado);
            }
        }
        fclose(file);
}
void restaurar_backup() {
    file = fopen("C:\\ProetoEmC\\ProjetoEmC\\ProjetoBD.txt", "r");

    int i = 0; char linha[1000];
    while (fscanf(file, "%[^\n]\n", &linha)==1) {
       char *quebra = strtok(linha,"|");
       char *linhas[10];

       while(quebra != NULL){
            linhas[i]= quebra;
            quebra = strtok(NULL,"|");
            i++;
       }

       pessoa p;

        strcpy(p.nome, linhas[0]);
        p.codigo_do_cliente = atoi(linhas[1]);
        strcpy(p.bi,linhas[2]);
        strcpy(p.localidade,linhas[3]);
        p.telefone = atoi(linhas[4]);

        cliente[qtdCliente]=p;
        qtdCliente++;

        if (i == 5){
            continue;
        }else{

            int indice =buscar_conta(p.codigo_do_cliente);
            contas[indice].codigo_cliente= atoi(linhas[5]);
            contas[indice].tipo_de_conta = atoi(linhas[6]);
            contas[indice].numero_de_conta = atoi(linhas[7]);
            contas[indice].saldo = atof(linhas[8]);
            contas[indice].estado = atoi(linhas[9]);

            qtdContas++;
            }
    }
    fclose(file);

}


void listar_clientes(){

    for(int i =0; i < qtdCliente; i++){
        printf("Nome: %s Codigo cliente: %d BI/NIF: %s Localidade: %s Telefone: %d \n",cliente[i].nome,cliente[i].codigo_do_cliente,cliente[i].bi,cliente[i].localidade,cliente[i].telefone);

        printf("Codigo conta: %d Tipo de Conta: %d Nº de conta: %d Estado: %d \n",contas[i].codigo_cliente,contas[i].tipo_de_conta,contas[i].numero_de_conta,contas[i].estado);
        printf("________________________________________________________________________________________\n");

    }
}

int main()
{
    CriarFicheiro();
    restaurar_backup();
     setlocale(LC_ALL,"portuguese");
     int opcao = 0,tipo_de_conta = 0,indice = 0,codigo = 0,i =0, numero_de_conta = 0;
     int num_c_enviar, num_c_receber ;
     float valor = 0.0;

   while(opcao != 9){
     system("cls");
     printf("Menu\n1: Cadastrar Cliente \n2: Listar Clientes \n3: Abertura de Conta \n4: Depositar Dinheiro \n5: Levantar Dinheiro \n6: Consultar Saldo \n7: Transferir Dinheiro \n8: Actualizar Dados do Cliente \n9: Terminar\n ");
     scanf("%d",&opcao);
     getchar();

     switch (opcao){
     case 1:
         system("cls");
         printf("Insira o nome: ");
         scanf(" %[^\n]", p.nome);

        printf("Insira o número do BI/NIF: ");
        scanf(" %[^\n]", p.bi);

        printf("Insira o contacto telefonico: ");
        scanf("%d",&p.telefone);

        printf("Insira a sua localidade: ");
        scanf(" %[^\n]", p.localidade);

        if (cadastro_de_cliente(p)){
            printf("Cliente cadastrado com sucesso \n");
        }
        continuar();
        break;
     case 2:
         //Listar clientes
         system("cls");
         listar_clientes();
         continuar();
        break;
     case 3:
         //Abertura de conta
         system("cls");
         printf("Digite \n0- Caso seja conta pessoal \n1- Caso seja conta empresarial\n:");
         scanf("%d",&tipo_de_conta);
         getchar();

         printf("Insira o codigo: ");
         scanf("%d",&codigo);
         getchar();

         indice = buscar_indice(codigo);

         if(indice != -1){
            abertura_de_conta(cliente[i], tipo_de_conta);
            printf("Conta aberta com sucesso \n");
         }else{
             printf("Codigo inexistente \n");
         }
         continuar();
        break;
     case 4:
         //Depósito
         system("cls");

         printf("nsira o numero de conta: ");
         scanf("%d",&numero_de_conta);

         printf("Insira o valor a depositar: ");
         scanf("%f",&valor);

         if(depositar_dinheiro(numero_de_conta,valor)){
            printf("%0.2f depositado com sucesso\n",valor);
         }else{
             printf("Opção inválida");
         }
         continuar();
         break;
     case 5:
        //Levantar dinheiro
        system("cls");
        printf("Insira o número da conta: ");
        scanf("%d",&numero_de_conta);

        printf("Insira o valor a levantar: ");
        scanf("%f",&valor);

        if(levantar_dinheiro(numero_de_conta,valor)){
            printf("O valor %0.2f foi levantado com sucesso\n",valor);
        }else{
            printf("Não foi possível fazer o levantamento\n");
        }
        continuar();
        break;
     case 6:
        //Consultar saldo
        system("cls");
        printf("Insira o número de conta: ");
        scanf("%d",&numero_de_conta);

        float saldo = consultar_saldo(numero_de_conta);
        printf("Saldo atual KZ %.2f\n",saldo);

        continuar();
        break;
     case 7:
         //Transferir saldo
         system("cls");
         printf("Insira o seu número de conta: ");
         scanf("%d",&num_c_enviar);

         printf("Insira o número de conta destinatário: ");
         scanf("%d",&num_c_receber);

         printf("Insira o valor: ");
         scanf("%f",&valor);

         if(transferir_dinheiro(num_c_enviar, num_c_receber, valor)){
            printf("Transferência de %0.2f realisada com sucesso \n",valor);
         }else{
             printf("Falha na transferência \n");
         }
         continuar();
         break;
     case 8:
        //Atualisar dados
        system("cls");

        printf("Insira o código do cliente: ");
        scanf("%d",&codigo);

        if(buscar_indice(codigo) != -1){
            printf("Insira o nome: ");
            scanf(" %[^\n]", p.nome);

            printf("Insira o número do BI/NIF: ");
            scanf(" %[^\n]", p.bi);

            printf("Insira o contacto telefonico: ");
            scanf("%d",&p.telefone);

            printf("Insira a sua localidade: ");
            scanf(" %[^\n]", p.localidade);

            if(atualisar_dados(p,codigo)){
                printf("Dados atualisados com sucesso");
            }else{
                printf("Cliente inexistente");
            }
        }
        break;

    default:
        printf("Opção invalida");
    }

   }

    copia_de_seguranca();
    return 0;
}
