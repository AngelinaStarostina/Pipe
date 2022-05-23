#include <windows.h>
#include <iostream>
#include <time.h>
using namespace std;

int main(int argc, char* argv[])
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
	char* arr = new char[n];
	for (int i = 0; i < n; i++)
	{
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			return GetLastError();
		}
		cout << "The number " << arr[i] << " is read from the pipe" << endl;
	}
	cout << endl;


	int count;
	do {
		cout << "Enter the number of elements:  ";
		cin >> count;
	} while (n < count);


	int count2 = count;

	srand(time(NULL));
	while (count != NULL)
	{
		int i = rand() % (n - 1);
		if (arr[i] == NULL)
		{
			count2--;
		}
		arr[i] = NULL;
		cout << "i = " << i << endl;
		count--;

	}

	int n2 = n - count2;

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n2, sizeof(n2), &dwBytesWritten, NULL))
	{
		cout << "Write n to file failed.\n";
		return GetLastError();
	}
	cout << n2 << "is written to the pipe.\n";

	for (int j = 0; j < n; j++)
	{
		DWORD dwBytesWritten;
		if (arr[j] != NULL)
		{
			if (!WriteFile(hWritePipe, &arr[j], sizeof(arr[j]), &dwBytesWritten, NULL))
			{
				cout << "Write to file failed.\n";
				return GetLastError();
			}
			cout << arr[j] << "is written to the pipe.\n";
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