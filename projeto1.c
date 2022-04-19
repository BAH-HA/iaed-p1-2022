#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IDINVAL "%s: no such airport ID\n"
#define MAXID 4
#define MAXPAIS 31
#define MAXCIDADE 51
#define MAXCODIGO_VOO 7
#define MAXDATA 11
#define MAXHORA 6
#define MAXDURACAO 6
#define MAXCAPACIDADE 4
#define MAXVOOS 30000

typedef struct {
    int Dia;
    int Mes;
    int Ano;
} data;

typedef struct {
    int Horas;
    int Minutos;
} hora;


typedef struct{
    char Codigo_voo[30000][MAXCODIGO_VOO];
    char IDpartida[30000][MAXID];
    char IDchegada[30000][MAXID];
    data Data[30000];
    hora Horas[30000];
    hora Duracao[30000];
    int Capacidade[30000];
} voo;

typedef struct {
    char ID[40][MAXID];      
    char cidade[40][MAXCIDADE];           
    char pais[40][MAXPAIS];
    int N_voos[40];
} sistema;

int contador_aeroportos = 0;
int contador_voos = 0;
sistema aeroportos = {0};
voo voos = {0};
data Data = {01,01,2022};
hora Hora = {0};

int function_a();
int function_l();
int function_v();
int function_p();
int function_c();
int function_t();


/*Devolve 1 se Hora1 > Hora2 ou 0 se Hora1 < Hora2*/
int Hora_Maior(hora Hora1, hora Hora2){
    if(Hora1.Horas == Hora2.Horas && Hora1.Minutos > Hora2.Minutos){
        return 1;
    }
    else if(Hora1.Horas > Hora2.Horas){
        return 1;
    }
    else{
        return 0;
    }
    return 0;
}

/*Devolve 1 se Data1 > Data2 ou 0 se Data1 < Data 2*/
int Data_Maior(data Data1, data Data2, hora Hora1, hora Hora2){
    if (Data1.Dia > Data2.Dia && Data1.Mes == Data2.Mes && 
            Data1.Ano == Data2.Ano){
        return 1;
    }
    else if(Data1.Mes > Data2.Mes && Data1.Ano == Data2.Ano){
        return 1;
    }
    else if(Data1.Ano > Data2.Ano){
        return 1;
    }
    else if (Data1.Dia == Data2.Dia && Data1.Mes == Data2.Mes && 
                Data1.Ano == Data2.Ano){
        if (Hora_Maior(Hora1, Hora2) == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
    return 0;
}


/*Funcao ordena os aeroportos por ordem alfabetica de IDs*/
void insertionSort_aeroportos(){
    int i, j, chave_N_voos;
    char chave[MAXID], chave_cidade[MAXCIDADE], chave_pais[MAXPAIS];
    for (i = 1; i < contador_aeroportos; i++) {
        strcpy(chave,aeroportos.ID[i]);
        strcpy(chave_cidade, aeroportos.cidade[i]);
        strcpy(chave_pais,aeroportos.pais[i]);
        chave_N_voos = aeroportos.N_voos[i];
        j = i - 1;
        while (j >= 0 && strcmp(aeroportos.ID[j],chave) > 0){
            strcpy(aeroportos.ID[j + 1], aeroportos.ID[j]);
            strcpy(aeroportos.cidade[j + 1], aeroportos.cidade[j]);
            strcpy(aeroportos.pais[j + 1], aeroportos.pais[j]);
            aeroportos.N_voos[j + 1] = aeroportos.N_voos[j];
            j--;
        }
        strcpy(aeroportos.ID[j + 1], chave);
        strcpy(aeroportos.cidade[j + 1], chave_cidade);
        strcpy(aeroportos.pais[j + 1], chave_pais);
        aeroportos.N_voos[j + 1] = chave_N_voos;
    }
}

/*Funcao ordena os voos de um unico aeroporto por data antigos -> recentes*/
void insertionSort_Voos_Data(voo *voos_p_ord, int n){
    int i, j, chave_Capacidade;
    data chave_Data = {0};
    hora chave_Horas = {0};
    char chave_Codigo_voo[MAXCODIGO_VOO], chave_IDpartida[MAXID];
    char chave_IDchegada[MAXID];
    for (i = 1; i < n; i++) {
        strcpy(chave_Codigo_voo, voos_p_ord->Codigo_voo[i]);
        strcpy(chave_IDpartida, voos_p_ord->IDpartida[i]);
        strcpy(chave_IDchegada, voos_p_ord->IDchegada[i]);
        chave_Capacidade = voos_p_ord->Capacidade[i];
        chave_Data = voos_p_ord->Data[i];
        chave_Horas = voos_p_ord->Horas[i];
        j = i - 1;
        while(j >= 0 && Data_Maior(voos_p_ord->Data[j],chave_Data,
                voos_p_ord->Horas[j], chave_Horas) != 0){
            strcpy(voos_p_ord->Codigo_voo[j + 1], voos_p_ord->Codigo_voo[j]);
            strcpy(voos_p_ord->IDpartida[j + 1], voos_p_ord->IDpartida[j]);
            strcpy(voos_p_ord->IDchegada[j + 1], voos_p_ord->IDchegada[j]);
            voos_p_ord->Capacidade[j + 1] = voos_p_ord->Capacidade[j];
            voos_p_ord->Data[j + 1] = voos_p_ord->Data[j];
            voos_p_ord->Horas[j + 1] = voos_p_ord->Horas[j];
            j--;
        }
        strcpy(voos_p_ord->Codigo_voo[j + 1], chave_Codigo_voo);
        strcpy(voos_p_ord->IDpartida[j + 1], chave_IDpartida);
        strcpy(voos_p_ord->IDchegada[j + 1], chave_IDchegada);
        voos_p_ord->Capacidade[j + 1] = chave_Capacidade;
        voos_p_ord->Data[j + 1] = chave_Data;
        voos_p_ord->Horas[j + 1] = chave_Horas;
    }
}


int verificar_data(int Dia, int Mes, int Ano){
    if (Data.Ano > Ano || (Data.Ano == Ano && Data.Mes > Mes) || 
        (Data.Ano == Ano && Data.Mes == Mes && Data.Dia > Dia)){
        return 0;
    }
    else if((Data.Ano < Ano - 1) || (Data.Ano == Ano - 1 && Data.Mes < Mes) || 
            (Data.Ano == Ano - 1 && Data.Mes == Mes && Data.Dia < Dia)){
        return 0;
    }
    else{
        return 1;
    }
}

/* Soma a Hora de Partida com a Duracao*/
void Avancar_Hora(hora *HoraP, hora *Duracao){
    HoraP->Minutos += Duracao->Minutos; 
    HoraP->Horas += Duracao->Horas;
    if(HoraP->Minutos >= 60){
        HoraP->Horas += 1;
        HoraP->Minutos -= 60;  
    }  
}

/*verifica se a hora/minutos sao menores que 24/60 e se o dia existe no mes 
    correspondente, se isso nao se verificar corrije a data */
void corrige_Data(data *DataP, hora *HoraP){
    if(HoraP->Horas >= 24){
        HoraP->Horas -= 24;
        DataP->Dia += 1;
        if(DataP->Mes == 2 && DataP->Dia > 28){
            DataP->Mes++;
            DataP->Dia -= 28; 
        }
        else if(DataP->Mes == 12 && DataP->Dia > 31){
            DataP->Mes = 1;
            DataP->Dia -= 31;
            DataP->Ano++;
        }
        else if(DataP->Mes < 7 && DataP->Mes % 2 == 0 && DataP->Dia > 30){
            DataP->Mes++;
            DataP->Dia -= 30; 
        }
        else if(DataP->Mes > 7 && DataP->Mes % 2 != 0 && DataP->Dia > 30){
            DataP->Mes++;
            DataP->Dia -= 30; 
        }
        else if(DataP->Dia > 31){
            DataP->Mes++;
            DataP->Dia -= 31; 
        }
    }
}

int main()
{   
    char cmd;
    cmd = getchar();
    if (cmd == 'q'){
        exit(0);
    }
    else if (cmd == 'a'){
        function_a(cmd);
        main();
    }

    else if (cmd == 'l'){
        function_l(cmd);
        main();   
    }
    
    else if (cmd == 'v'){
        function_v(cmd);
        main();
    }
    else if (cmd == 'p'){
        function_p(cmd);
        main();
    }
    
    else if (cmd == 'c'){
        function_c(cmd);
        main();
    }
    else if (cmd == 't'){
        function_t(cmd);
        main();
    }
    else{
        main();
    }
    return 0;
}

/*Funcao verifica a validade do input e adiciona aeroportos ao sistema*/
int function_a(){
    char id[MAXID], pais[MAXPAIS], cidade[MAXCIDADE];
    int i;
    scanf(" %s %s %[^\n]%*c", id, pais, cidade); 
    for(i = 0; i < 3; i++){ /*scanf da scan ate encontrar um \n*/
        if (!(isupper(id[i]))){
            printf("invalid airport ID\n");
            return 0;
        }
    }
    if (contador_aeroportos == 40){
        printf("too many airports\n");
        return 0;
    }
    else if(contador_aeroportos != 0){
        for (i = 0; i <= contador_aeroportos; i++){
            if (strcmp(aeroportos.ID[i], id) == 0){
                printf("duplicate airport\n");
                return 0;
            }
        }
    }
    strcpy(aeroportos.ID[contador_aeroportos], id);
    strcpy(aeroportos.pais[contador_aeroportos], pais);
    strcpy(aeroportos.cidade[contador_aeroportos], cidade);
    printf("airport %s\n", aeroportos.ID[contador_aeroportos]);
    contador_aeroportos++;
    return 0;
        
            
}
/*Funcao lista as funcoes por ordem alfabetica ou por ordem de Input*/ 
int function_l(){
    char caracter = ' ', ID[MAXID];
    int  i, index, verificador; 
    if(getchar() == '\n'){
        insertionSort_aeroportos();
        for(i = 0; i < contador_aeroportos; i++){
            printf("%s %s %s %d\n", aeroportos.ID[i], 
            aeroportos.cidade[i], aeroportos.pais[i], aeroportos.N_voos[i]);
        }
        return 0;
    }
    else{
        while(caracter == ' '){
            scanf("%s%c", ID, &caracter);
            verificador = 0;
            for(i = 0; i < contador_aeroportos; i++){
                if(strcmp(ID,aeroportos.ID[i]) == 0){
                    verificador++;
                    index = i;
                }
            }
            if(verificador == 0){
                printf(IDINVAL, ID);
                continue;
            }
            printf("%s %s %s %d\n", aeroportos.ID[index], 
                aeroportos.cidade[index], aeroportos.pais[index], 
                aeroportos.N_voos[index]);
        }
        return 0;
    }
} 

/*Funcao adiciona voos ao sistema ou imprime uma lista com todos os voos
        no fim do for loop, se Verificador_IDs = 3, existem os 2 IDs ,
                se Verificadores_IDs = 2 não existe IDpartida
                se Verificadores_IDs = 1 não existe IDchegada
                se Verificadores_IDs = 0 não existem IDs*/
int function_v(){
    char Codigo_voo[MAXCODIGO_VOO], IDpartida[MAXID], IDchegada[MAXID];
    int i, Verificador_IDs = 0, Dia, Mes, Ano, Horas_data, Minutos_data;       
    int Horas_Dur, Minutos_Dur, Capacidade;
    if(getchar() == '\n'){
        for(i = 0; i < contador_voos; i++){
            printf("%s %s %s %02d-%02d-%02d %02d:%02d\n", voos.Codigo_voo[i], 
                voos.IDpartida[i], voos.IDchegada[i], voos.Data[i].Dia, 
                voos.Data[i].Mes, voos.Data[i].Ano, voos.Horas[i].Horas, 
                voos.Horas[i].Minutos);
        }
        return 0;
    }
    else{
        scanf("%s %s %s %d-%d-%d %d:%d %d:%d %d", Codigo_voo, IDpartida, 
            IDchegada, &Dia, &Mes, &Ano, &Horas_data, &Minutos_data, 
            &Horas_Dur, &Minutos_Dur, &Capacidade);
        if(!(isupper(Codigo_voo[0]) && isupper(Codigo_voo[1]) && 
            isdigit(Codigo_voo[2]) && Codigo_voo[2] > 0 + '0')){
            printf("invalid flight code\n");
            return 0;
        }
        for(i = 3; Codigo_voo[i] != '\0'; i++){
            if (!(10 + '0' > Codigo_voo[i] && Codigo_voo[i] >= '0' + 0)){
                printf("invalid flight code\n");
                return 0;
            }
        }
        for(i = 0; i < contador_voos; i++){
            if (strcmp(Codigo_voo, voos.Codigo_voo[i]) == 0 && 
                Dia == voos.Data[i].Dia && Mes == voos.Data[i].Mes && 
                Ano == voos.Data[i].Ano){
                printf("flight already exists\n");
                return 0;
            }
        }
        for(i = 0; i < contador_aeroportos; i++){
            if (strcmp(IDpartida, aeroportos.ID[i]) == 0){ 
                Verificador_IDs += 1; 
            }                   
            if(strcmp(IDchegada, aeroportos.ID[i]) == 0){
                Verificador_IDs += 2;
            }                                                   
        }                                                           
        if (Verificador_IDs == 2 || Verificador_IDs == 0){          
            printf(IDINVAL, IDpartida);            
            return 0;
        }
        else if (Verificador_IDs == 1){
            printf(IDINVAL, IDchegada);
            return 0;
        }
        else if (contador_voos >= MAXVOOS){
            printf("too many flights\n");
            return 0;
        }
        else if (verificar_data(Dia,Mes,Ano) == 0){
            printf("invalid date\n");
            return 0;
        }
        else if ((Horas_Dur < 0) || (Horas_Dur == 12 && Minutos_Dur > 0)
                || (Horas_Dur > 12)){
            printf("invalid duration\n");
            return 0;
        }
        else if (Capacidade < 10 || Capacidade > 100){
            printf("invalid capacity\n");
            return 0;    
        }
        else{
            strcpy(voos.Codigo_voo[contador_voos], Codigo_voo);
            strcpy(voos.IDpartida[contador_voos], IDpartida);
            strcpy(voos.IDchegada[contador_voos], IDchegada);
            voos.Data[contador_voos].Ano = Ano;
            voos.Data[contador_voos].Mes = Mes;
            voos.Data[contador_voos].Dia = Dia;
            voos.Horas[contador_voos].Horas = Horas_data;
            voos.Horas[contador_voos].Minutos = Minutos_data;
            voos.Duracao[contador_voos].Horas = Horas_Dur;
            voos.Duracao[contador_voos].Minutos = Minutos_Dur;
            voos.Capacidade[contador_voos] = Capacidade;
            for(i = 0; i < contador_aeroportos; i++){
                if(strcmp(aeroportos.ID[i],IDpartida) == 0){
                    aeroportos.N_voos[i] += 1;
                }
            }
            contador_voos++;
        }
    }
    return 0; 
}

/*Imprime os voos por ordem de data/hora de partida de um ID dado no Input*/
int function_p(){
    int i, Verificador_IDs = 0, contador = 0;
    char ID[MAXID];
    voo voos_p_ord = {0};
    scanf("%s", ID);
    for(i = 0; i < contador_aeroportos; i++){
                if(strcmp(ID,aeroportos.ID[i]) == 0){
                    Verificador_IDs++;
                }
            }
    if (Verificador_IDs == 0)
    {
        printf(IDINVAL,ID);
        return 0;
    }
    else{ 
        for(i = 0; i < contador_voos; i++){
            if(strcmp(voos.IDpartida[i], ID) == 0){
                strcpy(voos_p_ord.Codigo_voo[contador], voos.Codigo_voo[i]);
                strcpy(voos_p_ord.IDpartida[contador], voos.IDpartida[i]);
                strcpy(voos_p_ord.IDchegada[contador], voos.IDchegada[i]);
                voos_p_ord.Data[contador].Ano = voos.Data[i].Ano;
                voos_p_ord.Data[contador].Mes = voos.Data[i].Mes;
                voos_p_ord.Data[contador].Dia = voos.Data[i].Dia; 
                voos_p_ord.Horas[contador].Horas = voos.Horas[i].Horas;
                voos_p_ord.Horas[contador].Minutos = voos.Horas[i].Minutos;
                voos_p_ord.Duracao[contador].Horas = voos.Duracao[i].Horas;
                voos_p_ord.Duracao[contador].Minutos = voos.Duracao[i].Minutos;
                voos_p_ord.Capacidade[contador] = voos.Capacidade[i];
                contador++;
            }
        }
        insertionSort_Voos_Data(&voos_p_ord, contador);
        for(i = 0; i < contador; i++){
            printf("%s %s %02d-%02d-%d %02d:%02d\n", voos_p_ord.Codigo_voo[i], 
                voos_p_ord.IDchegada[i], voos_p_ord.Data[i].Dia, 
                voos_p_ord.Data[i].Mes, voos_p_ord.Data[i].Ano,
                voos_p_ord.Horas[i].Horas,voos_p_ord.Horas[i].Minutos);
        }
    }
    return 0;
}
/*Imprime os voos por ordem de data/hora de chegada de um ID dado no Input*/
int function_c(){
    int i, Verificador_IDs = 0, contador = 0;
    char ID[MAXID];
    voo voos_p_ord = {0};
    scanf("%s", ID);
    for(i = 0; i < contador_aeroportos; i++){
                if(strcmp(ID,aeroportos.ID[i]) == 0){
                    Verificador_IDs++;
                }
            }
    if (Verificador_IDs == 0)
    {
        printf(IDINVAL, ID);
        return 0;
    }
    else{ 
        for(i = 0; i < contador_voos; i++){
            if(strcmp(voos.IDchegada[i], ID) == 0){
                strcpy(voos_p_ord.Codigo_voo[contador], voos.Codigo_voo[i]);
                strcpy(voos_p_ord.IDpartida[contador], voos.IDpartida[i]);
                strcpy(voos_p_ord.IDchegada[contador], voos.IDchegada[i]);
                voos_p_ord.Data[contador].Ano = voos.Data[i].Ano;
                voos_p_ord.Data[contador].Mes = voos.Data[i].Mes;
                voos_p_ord.Data[contador].Dia = voos.Data[i].Dia; 
                voos_p_ord.Horas[contador].Horas = voos.Horas[i].Horas;
                voos_p_ord.Horas[contador].Minutos = voos.Horas[i].Minutos;
                voos_p_ord.Duracao[contador].Horas = voos.Duracao[i].Horas;
                voos_p_ord.Duracao[contador].Minutos = voos.Duracao[i].Minutos;
                voos_p_ord.Capacidade[contador] = voos.Capacidade[i];
                contador++;
            }
        }
        for(i = 0; i < contador; i++){
            Avancar_Hora(&voos_p_ord.Horas[i], &voos_p_ord.Duracao[i]);
            corrige_Data(&voos_p_ord.Data[i],&voos_p_ord.Horas[i]);
        }
        insertionSort_Voos_Data(&voos_p_ord, contador);
        for(i = 0; i < contador; i++){
            printf("%s %s %02d-%02d-%d %02d:%02d\n", voos_p_ord.Codigo_voo[i], 
                voos_p_ord.IDpartida[i], voos_p_ord.Data[i].Dia, 
                voos_p_ord.Data[i].Mes, voos_p_ord.Data[i].Ano,
                voos_p_ord.Horas[i].Horas,voos_p_ord.Horas[i].Minutos);
        }
    }
    return 0;
}

/*Funcao altera a data do sistema*/
int function_t(){
    int Novo_Dia, Novo_Mes, Novo_Ano;
    scanf("%d-%d-%d", &Novo_Dia, &Novo_Mes, &Novo_Ano);
    if (verificar_data(Novo_Dia, Novo_Mes, Novo_Ano) == 0){
        printf("invalid date\n");
        return 0;
    }
    else{
        Data.Dia = Novo_Dia;
        Data.Mes = Novo_Mes;
        Data.Ano = Novo_Ano;
        printf("%02d-%02d-%02d\n", Novo_Dia, Novo_Mes, Novo_Ano);
        return 0;
    }
}
