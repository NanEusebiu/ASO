#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (hSCM == NULL) {
        printf("Eroare la deschiderea SCM. Cod eroare: %lu\n", GetLastError());
        return 1;
    }

    DWORD bytesNeeded = 0;
    DWORD servicesReturned = 0;
    DWORD resumeHandle = 0;

    EnumServicesStatusExA(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        NULL,
        0,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    );

    if (bytesNeeded == 0) {
         printf("Eroarela determinarea dimensiunii bufferului. Cod: %lu\n", GetLastError());
         CloseServiceHandle(hSCM);
         return 1;
    }

    LPBYTE buffer = (LPBYTE)malloc(bytesNeeded);
    if (buffer == NULL) {
        printf("Eroare la alocarea memoriei!\n");
        CloseServiceHandle(hSCM);
        return 1;
    }

    LPENUM_SERVICE_STATUS_PROCESSA services = (LPENUM_SERVICE_STATUS_PROCESSA)buffer;

    if (EnumServicesStatusExA(
        hSCM,
        SC_ENUM_PROCESS_INFO,
        SERVICE_WIN32,
        SERVICE_ACTIVE,
        buffer,
        bytesNeeded,
        &bytesNeeded,
        &servicesReturned,
        &resumeHandle,
        NULL
    )) {
        printf("Servicii care ruleaza in prezent:\n");
        for (DWORD i = 0; i < servicesReturned; ++i) {
            printf("- %s (Nume serviciu: %s)\n", 
                   services[i].lpDisplayName, 
                   services[i].lpServiceName);
        }
        printf("Total servicii active: %lu\n", servicesReturned);
    } else {
        printf("Eroare la enumerarea serviciilor. Cod eroare: %lu\n", GetLastError());
    }

    free(buffer);
    CloseServiceHandle(hSCM);
    
    return 0;
}
