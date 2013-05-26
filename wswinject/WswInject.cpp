/*
	warsow_x86.exe
*/

#include "stdafx.h"

DWORD GetWarsowPid( ) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	bool found = false;
	if ( Process32First( snap, &pe32 ) ) {
		do {
			// maybe not the best solution, but support for x64 and x86
			if( strstr( pe32.szExeFile, "warsow" ) ) {
				found = true;
				break;
			}
		} while ( Process32Next( snap, &pe32 ) );
	}

	if( snap != INVALID_HANDLE_VALUE )
		CloseHandle( snap );

	// once we got our pid, ready to host some code in it! :3
	return ( found ) ? pe32.th32ProcessID : 0;
}

int WarsowAreYouGoneYet( DWORD pid ) {
	// maybe cache the handle, since procid's can be recycled
    HANDLE process = OpenProcess( SYNCHRONIZE, 0, pid );
    int ret = ( int ) ( WaitForSingleObject( process, 0 ) == WAIT_TIMEOUT );
    CloseHandle( process );

	// or use threadpool API's (RegisterWaitForSingleObject if you're oldsk00l)
    return ret;
}

bool Host( DWORD pid, char* path ) {
	if ( !pid ) // sanity check? shouldn't happen
		return 1;

	// this is bad practice, using PROCESS_ALL_ACCESS should be avoided at any time!
	// keep this for testing (for now)
	HANDLE proc = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );

	if ( !proc ) {
		// cout << GetLastError( ) << endl;
		return 2;
	}

	void* loadlib = ( void* ) GetProcAddress( GetModuleHandle( "kernel32.dll" ), "LoadLibraryA" );

	// this is the real deal, allocate space in proc for our code
	// request read/write access to the page so we can host it there
	void* rem = ( void* ) VirtualAllocEx( proc, 0, strlen( path ), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );

	// b00m, write it away!
	WriteProcessMemory( proc, ( void* ) rem, path, strlen( path ), 0 );

	// once we got our code in place, nicely use LoadLibrary WinAPI to execute the start
	// the end call will be our DllMain func
	CreateRemoteThread( proc, 0, 0, ( LPTHREAD_START_ROUTINE ) loadlib, rem, 0, 0 );

	CloseHandle( proc );

	return 0;

}

int _tmain( int argc, _TCHAR* argv[] ) {
	cout << "zZz zZz zZz" << endl;

	DWORD pid;
	while ( !GetWarsowPid( ) )
		Sleep( 1000 );

	cout << "Warsow is up and running." << endl;

	char buffer[MAX_PATH];
	GetFullPathName( "wswh.dll", MAX_PATH, buffer, 0 );

	// spawn our code in our memalloc and make sure the entry is called
	int error = 0;
	if ( ( error = Host( pid, buffer ) ) != 0 )
		cout << "Ready, set, 'n go!" << endl;
	else
		cout << "Error: " << error << endl; // wups, this is not good...

	// hold on...
	while ( !WarsowAreYouGoneYet( pid ) )
		Sleep( 2000 );

	cout << "kkthxbb" << endl;
	cin.get( );

	return 0;
}

