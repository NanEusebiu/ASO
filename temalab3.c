#include <windows.h>
#include <stdio.h>

int main() {
    HKEY hKey;
    LPCSTR subKeyPath = "SOFTWARE";
    DWORD dwIndex = 0;
    CHAR szSubKeyName[255];
    DWORD cbName = sizeof(szSubKeyName);
    FILETIME ftLastWriteTime;

    LSTATUS lResult = RegOpenKeyExA(
        HKEY_LOCAL_MACHINE, 
        subKeyPath, 
        0, 
        KEY_READ, 
        &hKey
    );

    if (lResult != ERROR_SUCCESS) {
        printf("Eroare la deschiderea cheii: %ld\n", lResult);
        return 1;
    }

    printf("Subcheile pentru HKEY_LOCAL_MACHINE\\%s:\n", subKeyPath);

    while (RegEnumKeyExA(
               hKey, 
               dwIndex, 
               szSubKeyName, 
               &cbName, 
               NULL, 
               NULL, 
               NULL, 
               &ftLastWriteTime) == ERROR_SUCCESS) 
    {
        printf("[%d] %s\n", dwIndex + 1, szSubKeyName);
        cbName = sizeof(szSubKeyName);
        dwIndex++;
    }

    RegCloseKey(hKey);

    return 0;
}
