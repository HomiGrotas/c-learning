#include "autostart.h"

#include <stdio.h>
#include <windows.h>


int registerEXE()
{
    HKEY hKey;
    LONG lResult;
    DWORD dwSize;
    const char* szValueName = "MyProgram";
    const char* szExePath = "C:\\Program Files\\MyProgram\\MyProgram.exe";
    printf("registering...\n");

    // Open the Run key
    lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        printf("Error opening the Run key: %d\n", lResult);
        return 1;
    }

    // Set the value of the key
    dwSize = (DWORD)strlen(szExePath);
    lResult = RegSetValueExA(hKey, szValueName, 0, REG_SZ, (BYTE*)szExePath, dwSize);

    if (lResult != ERROR_SUCCESS)
    {
        if (lResult == ERROR_ACCESS_DENIED)
        {
            printf("Error: Access denied\n");
        }
        else{
            printf("Error setting the value of the key: %d\n", lResult);
        }
        return 1;
    }

    // Close the key
    RegCloseKey(hKey);

    if (lResult == 0){printf("Successfully registered program to start up automatically.\n");}
    return 0;
}
