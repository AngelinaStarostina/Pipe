#include <windows.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE hEnableRead;
	char lpszEnableRead[] = "EnableRead";
	HANDLE hEnableRead2;
	char lpszEnableRead2[] = "EnableRead2";

	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);
	hEnableRead2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead2);


	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);

	WaitForSingleObject(hEnableRead2, INFINITE);

	DWORD dwBytesRead;
	int n;
	if (!ReadFile(hReadPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		cout << "Read from the pipe failed.\n";
		return GetLastError();
	}
	cout << "The number " << n << " is read from the pipe" << endl;
	for (int i = 0; i < n; i++)
	{
		int nData;
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			return GetLastError();
		}
		cout << "The number " << nData << " is read from the pipe" << endl;
	}

	for (int j = 0; j < n; j++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile(hWritePipe, &j, sizeof(j), &dwBytesWritten, NULL))
		{
			cout << "Write to file failed.\n";
			return GetLastError();
		}
	}

	SetEvent(hEnableRead);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEnableRead2);
	CloseHandle(hEnableRead);

	system("pause");
	return 0;
}