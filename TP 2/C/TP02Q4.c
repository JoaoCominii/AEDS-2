#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void limparEspacos(char** array, int n) {
    for (int i = 0; i < n; i++) {
        char* temp = strdup(array[i]);
        char* ptr = temp;
        // Remove espa�os do in�cio
        while (*ptr == ' ') {
            ptr++;
        }
        // Atualiza o valor no array
        array[i] = strdup(ptr);
        free(temp);
    }
}

// Fun��o auxiliar para trocar elementos em um array de strings
void trocar(char** a, char** b) {
    char* temp = *a;
    *a = *b;
    *b = temp;
}

// Fun��o de parti��o para o Quick Sort
int particao(char** array, int low, int high) {
    char* pivot = array[high]; // Piv�
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (strcmp(array[j], pivot) < 0) { // Compara strings em ordem alfab�tica
            i++;
            trocar(&array[i], &array[j]);
        }
    }
    trocar(&array[i + 1], &array[high]);
    return i + 1;
}

// Fun��o principal do Quick Sort
void quickSort(char** array, int low, int high) {
    if (low < high) {
        int pi = particao(array, low, high); // Obt�m o �ndice de parti��o

        quickSort(array, low, pi - 1);  // Ordena os elementos antes da parti��o
        quickSort(array, pi + 1, high); // Ordena os elementos depois da parti��o
    }
}

// Fun��o auxiliar para remover aspas do in�cio e do final de uma string
char* removerAspas(const char* src) {
    if (src == NULL || strlen(src) == 0) return strdup("NaN");
    char* result = strdup(src);
    size_t len = strlen(result);

    if (len > 0 && result[0] == '"' && result[len - 1] == '"') {
        result[len - 1] = '\0';
        memmove(result, result + 1, len - 1);
    }
    return result;
}

// Fun��o para dividir uma linha CSV, lidando com aspas e v�rgulas manualmente
char** dividirCSV(const char* linha, int* count) {
    if (linha == NULL || strlen(linha) == 0) {
        *count = 0;
        return NULL;
    }

    char** resultado = NULL;
    int tamanho = 0;
    char* copia = strdup(linha);
    char* ptr = copia;
    bool dentroAspas = false;
    char buffer[2048];
    int bufferIndex = 0;

    while (*ptr != '\0') {
        if (*ptr == '"' && (ptr == copia || *(ptr - 1) != '\\')) {
            dentroAspas = !dentroAspas; // Alterna dentro ou fora das aspas
        } else if (*ptr == ',' && !dentroAspas) {
            buffer[bufferIndex] = '\0';
            resultado = realloc(resultado, (tamanho + 1) * sizeof(char*));
            resultado[tamanho++] = removerAspas(buffer);
            bufferIndex = 0;
        } else {
            buffer[bufferIndex++] = *ptr;
        }
        ptr++;
    }

    // Adicionar o �ltimo campo ao resultado
    buffer[bufferIndex] = '\0';
    resultado = realloc(resultado, (tamanho + 1) * sizeof(char*));
    resultado[tamanho++] = removerAspas(buffer);

    free(copia);
    *count = tamanho;
    return resultado;
}

// Estrutura para armazenar informa��es do Show
typedef struct {
    char* Show_ID;
    char* Type;
    char* Title;
    char* Director;
    char** Cast;
    int Cast_Count;
    char* Country;
    char* Date_Added;
    int Release_Year;
    char* Rating;
    char* Duration;
    char** Listed_In;
    int Listed_In_Count;
} Show;

// Fun��o para liberar mem�ria alocada de um Show
void freeShow(Show* show) {
    free(show->Show_ID);
    free(show->Type);
    free(show->Title);
    free(show->Director);

    for (int i = 0; i < show->Cast_Count; i++) {
        free(show->Cast[i]);
    }
    free(show->Cast);

    free(show->Country);
    free(show->Date_Added);
    free(show->Rating);
    free(show->Duration);

    for (int i = 0; i < show->Listed_In_Count; i++) {
        free(show->Listed_In[i]);
    }
    free(show->Listed_In);
}

// Fun��o para imprimir informa��es do Show
void imprimir(Show* show) {
    printf("=> %s ## %s ## %s ## %s ## [", show->Show_ID, show->Title, show->Type, show->Director);

    for (int i = 0; i < show->Cast_Count; i++) {
        if (i > 0) {
            printf(", "); // Adiciona v�rgula antes dos pr�ximos nomes
        }
        printf("%s", show->Cast[i]);
    }
    if (show->Cast_Count == 0) {
        printf("NaN");
    }

    printf("] ## %s ## %s ## %d ## %s ## %s ## [", show->Country, show->Date_Added, show->Release_Year, show->Rating, show->Duration);

    for (int i = 0; i < show->Listed_In_Count; i++) {
        if (i > 0) {
            printf(", "); // Adiciona v�rgula antes dos pr�ximos itens
        }
        printf("%s", show->Listed_In[i]);
    }
    if (show->Listed_In_Count == 0) {
        printf("NaN");
    }

    printf("] ##\n");
}

// Fun��o para processar e carregar os dados do CSV
Show** carregarCSV(const char* caminhoArquivo, int* tamanho) {
    FILE* file = fopen(caminhoArquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    Show** shows = NULL;
    char linha[2048];
    int contador = 0;

    fgets(linha, sizeof(linha), file); // Ignorar cabe�alho

    while (fgets(linha, sizeof(linha), file)) {
        int numCampos;
        char** valores = dividirCSV(linha, &numCampos);

        // Tratar valores ausentes como "NaN"
        for (int i = 0; i < numCampos; i++) {
            if (valores[i] == NULL || strlen(valores[i]) == 0) {
                valores[i] = strdup("NaN");
            }
        }

        // Criar objeto Show
        Show* show = malloc(sizeof(Show));
        show->Show_ID = valores[0];
        show->Type = valores[1];
        show->Title = valores[2];
        show->Director = valores[3];
        show->Cast = dividirCSV(valores[4], &show->Cast_Count);
        show->Country = valores[5];
        show->Date_Added = valores[6];
        show->Release_Year = atoi(valores[7]);
        show->Rating = valores[8];
        show->Duration = valores[9];
        show->Listed_In = dividirCSV(valores[10], &show->Listed_In_Count);

        // Limpar espa�os antes de ordenar
        limparEspacos(show->Cast, show->Cast_Count);
        limparEspacos(show->Listed_In, show->Listed_In_Count);

        // Ordenar `Cast` e `Listed_In` usando Quick Sort
        if (show->Cast_Count > 1) {
            quickSort(show->Cast, 0, show->Cast_Count - 1);
        }
        if (show->Listed_In_Count > 1) {
            quickSort(show->Listed_In, 0, show->Listed_In_Count - 1);
        }

        shows = realloc(shows, (contador + 1) * sizeof(Show*));
        shows[contador++] = show;
    }

    fclose(file);
    *tamanho = contador;
    return shows;
}

// Fun��o para ordenar os shows manualmente usando Insertion Sort
void ordenarPorTitulo(Show** shows, int n) {
    for (int i = 1; i < n; i++) {
        Show* chave = shows[i];
        int j = i - 1;

        // Move os elementos que s�o maiores que a chave para frente
        while (j >= 0 && strcmp(shows[j]->Title, chave->Title) > 0) {
            shows[j + 1] = shows[j];
            j--;
        }

        shows[j + 1] = chave;
    }
}

// Fun��o para realizar a pesquisa bin�ria
int buscaBinaria(Show** shows, int tamanho, const char* titulo, int* comparacoes) {
    int esq = 0, dir = tamanho - 1;

    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        (*comparacoes)++; // Incrementa o n�mero de compara��es

        int cmp = strcmp(shows[meio]->Title, titulo);
        if (cmp == 0) {
            return 1; // Elemento encontrado
        } else if (cmp < 0) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }

    return 0; // Elemento n�o encontrado
}


// Fun��o principal
int main() {
    const char* caminhoArquivo = "/tmp/disneyplus.csv";
    int tamanho;

    Show** todosShows = carregarCSV(caminhoArquivo, &tamanho);
    if (!todosShows) return 1;

    Show* showsSelecionados[300];
    int contador = 0;

    char id[256];
    while (scanf("%s", id) != EOF) {
        if (strcmp(id, "FIM") == 0) break;

        for (int i = 0; i < tamanho; i++) {
            if (todosShows[i] && strcmp(todosShows[i]->Show_ID, id) == 0) {
                showsSelecionados[contador++] = todosShows[i];
                break;
            }
        }
    }

    ordenarPorTitulo(showsSelecionados, contador);

      // Realiza as buscas bin�rias
    char titulo[300];
    int comparacoesTotais = 0;
    FILE* logFile = fopen("matricula_binaria.txt", "w");
    clock_t inicio = clock(); // Marca o in�cio do tempo de execu��o

    while (scanf(" %[^\n]s", titulo) != EOF) {
        if (strcmp(titulo, "FIM") == 0) break;

        int comparacoes = 0;
        int encontrado = buscaBinaria(showsSelecionados, contador, titulo, &comparacoes);
        comparacoesTotais += comparacoes;

        if (encontrado) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }

    clock_t fim = clock(); // Marca o fim do tempo de execu��o
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Escreve no arquivo de log
    fprintf(logFile, "00846713\t%.6f\t%d\n", tempoExecucao, comparacoesTotais);

    fclose(logFile);


    for (int i = 0; i < tamanho; i++) {
        freeShow(todosShows[i]);
    }
    free(todosShows);

    return 0;
}
