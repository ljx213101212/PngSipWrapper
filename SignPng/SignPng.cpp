// SignPng.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "windows.h"
#include "lib/pngsip.h"
#include "lib/PngFileSubjectInterfacePackage.h"


//extern "C" STDAPI __cdecl DllRegisterServer();
extern "C"
{

	//STDAPI DllRegisterServer();
	//__declspec(dllexport) STDAPI DllRegisterServer();
}
typedef int(__cdecl* MYPROC)();
//check the subfix of the png file (.pmg == .png).
typedef int(__cdecl* PISN)(WCHAR*, GUID*);
//Get Signed png info
typedef int(__cdecl* PCDM)(SIP_SUBJECTINFO*, DWORD*, DWORD, DWORD*, BYTE*);
//
typedef int(__cdecl* PCPDM)(SIP_SUBJECTINFO*, DWORD, DWORD*,DWORD, BYTE*);

#define ReadBufferSize 10000

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

void GetTempSubjectInfo(SIP_SUBJECTINFO* pSubjectInfo) {
	if (pSubjectInfo == nullptr) { return; }
	LPCWSTR fileName = L"D:\\SourceCodeTest\\Cryptography\\SignPng\\SignPng\\lib\\hello.png";
	pSubjectInfo->hFile = CreateFile(fileName,               // file to open
		GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template


	char buff [ReadBufferSize];
	DWORD dwBytesRead;
	bool isSuccess = ReadFile(pSubjectInfo->hFile, buff, ReadBufferSize - 1, &dwBytesRead, NULL);
	if (isSuccess) {
		buff[dwBytesRead] = '\0';
		printf("%s\n", buff);
	}
	std::cout << GetLastErrorAsString() << std::endl;
}

int main()
{
	HINSTANCE hinstLib;
	MYPROC ProcAdd;
	PISN PngIsFileSupportedName;
	PCDM PngCryptSIPGetSignedDataMsg;
	PCPDM PngCryptSIPPutSignedDataMsg;
	BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

	// Get a handle to the DLL module.

	hinstLib = LoadLibrary(TEXT("PngFileSubjectInterfacePackage.dll"));
	// If the handle is valid, try to get the function address.

	if (hinstLib != NULL)
	{
		PngIsFileSupportedName = (PISN)GetProcAddress(hinstLib, "PngIsFileSupportedName");
		PngCryptSIPGetSignedDataMsg = (PCDM)GetProcAddress(hinstLib, "PngCryptSIPGetSignedDataMsg");
		PngCryptSIPPutSignedDataMsg = (PCPDM)GetProcAddress(hinstLib, "PngCryptSIPPutSignedDataMsg");
		// If the function address is valid, call the function.

		if (NULL != PngIsFileSupportedName)
		{
			fRunTimeLinkSuccess = TRUE;
			//(ProcAdd)(L"Message sent to the DLL function\n");
			WCHAR* pwszFileName = (WCHAR*)L"D:\\SourceCodeTest\\Cryptography\\SignPng\\SignPng\\lib\\hello.png";
			GUID guid;

			//bool res = PngIsFileSupportedName(pwszFileName, &guid);
			SIP_SUBJECTINFO info;
			GetTempSubjectInfo(&info);
		/*	DWORD pdwEncodingType;
			DWORD pcbSignedDataMsg = 0;
			BYTE pbSignedDataMsg;
			DWORD dwIndex = 0;*/
			//PngCryptSIPGetSignedDataMsg(&info, &pdwEncodingType, dwIndex, &pcbSignedDataMsg, &pbSignedDataMsg);

			DWORD pdwEncodingType = 0;
			DWORD dwIndex = 0;
			DWORD dwSignatureSize = 10000;
			DWORD cbSignedDataMsg = 10000;
			BYTE pbSignedDataMsg[10000];
			for (int i = 0; i < dwSignatureSize; i++) {
				pbSignedDataMsg[i] = 'q';
			}
			PngCryptSIPPutSignedDataMsg(&info, pdwEncodingType, &dwIndex, cbSignedDataMsg ,pbSignedDataMsg);
			std::cout << GetLastErrorAsString() << std::endl;
		}
		// Free the DLL module.

		fFreeResult = FreeLibrary(hinstLib);
	}

	// If unable to call the DLL function, use an alternative.
	if (!fRunTimeLinkSuccess)
		printf("Message printed from executable\n");
    std::cout << "Hello World!\n";
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
