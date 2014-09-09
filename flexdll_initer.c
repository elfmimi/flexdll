/*****************************************************************
   FlexDLL
   Alain Frisch

   Copyright 2007 Institut National de Recherche en Informatique et 
   en Automatique.

******************************************************************/

/* Custom entry point to perform relocations before the real
   entry point is called */

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

typedef int func(void*);

extern int reloctbl;

static int flexdll_init() {
  func *sym = (func *)GetProcAddress(GetModuleHandle(NULL), "__flexdll_relocate");
  if (sym && sym(&reloctbl)) return TRUE;
  return FALSE;
}

#ifdef __GNUC__
#ifdef __CYGWIN__
#define entry  _cygwin_dll_entry
#endif
#ifdef __MINGW32__
#define entry DllMainCRTStartup
#endif
#else
#define entry _DllMainCRTStartup
#endif


BOOL WINAPI entry(HINSTANCE, DWORD, LPVOID);

BOOL WINAPI FlexDLLiniter(HINSTANCE hinstDLL, DWORD fdwReason,
			  LPVOID lpReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH && !flexdll_init())
    return FALSE;

  return entry(hinstDLL, fdwReason, lpReserved);
}
