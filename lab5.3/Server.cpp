#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
	char lpszComLine[80];
	HANDLE hEnableRead;
	char lpszEnableRead[] = "EnableRead";
	HANDLE hEnableRead2;
	char lpszEnableRead2[] = "EnableRead2";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe;
	HANDLE hReadPipe;
	SECURITY_ATTRIBUTES sa;

	hEnableRead = CreateEvent(NULL, FALSE, FALSE, lpszEnableRead);
	hEnableRead2 = CreateEvent(NULL, FALSE, FALSE, lpszEnableRead2);

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		cout << "Create pipe failed.\n";
		return GetLastError();
	}


	int n;
	char* arr;
	cout << "Enter the number of elements: ";
	cin >> n;
	arr = new char[n];
	cout << "Enter elements: ";
	for (int i = 0; i < n; i++)
		cin >> arr[i];



	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, "Consume.exe %d %d", (int)hWritePipe, (int)hReadPipe);

	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "Create process failed.\n";
		return GetLastError();
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n, sizeof(n), &dwBytesWritten, NULL))
	{
		cout << "Write to file failed.\n";
		return GetLastError();
	}
	cout << n << "is written to the pipe.\n";
	for (int j = 0; j < n; j++)
	{
		DWORD dwBytesRead;
		if (!WriteFile(hWritePipe, &arr[j], sizeof(arr[j]), &dwBytesWritten, NULL))
		{
			cout << "Write to file failed.\n";
			return GetLastError();
		}
		cout << arr[j] << "is written to the pipe.\n";
	}
	cout << endl;

	SetEvent(hEnableRead2);


	WaitForSingleObject(hEnableRead, INFINITE);

	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		cout << "Read from the pipe failed.\n";
		return GetLastError();
	}
	cout << n << " is read from the pipe" << endl;

	for (int j = 0; j < n; j++)
	{
		char nData;
		DWORD dwBytesRead;

		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			return GetLastError();
		}

		cout << nData << " is read from the pipe" << endl;
	}

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEnableRead2);
	CloseHandle(hEnableRead);

	return 0;
}