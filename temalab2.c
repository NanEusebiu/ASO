#include <stdio.h>
#include <stdlib.h>

int main() {
    char szFilePath[260];
    char szBuffer[1024];
    FILE* pFileHandle = NULL;

    printf("Introduceti calea relativa a fisierului: ");
    if (scanf("%259s", szFilePath) != 1) {
        printf("Eroare la citirea caii.\n");
        return -1;
    }

    pFileHandle = fopen(szFilePath, "r");

    if (pFileHandle == NULL) {
        fprintf(stderr, "Eroare: Fisierul '%s' nu a fost gasit sau nu poate fi deschis!\n", szFilePath);
        return 1;
    }

    printf("\nContinutul fisierului:\n");
    
    while (fgets(szBuffer, sizeof(szBuffer), pFileHandle) != NULL) {
        printf("%s", szBuffer);
    }

    fclose(pFileHandle);

    return 0;
}
