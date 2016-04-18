// This file contains bochs startup and exit procedures bochs_startup() and bochs_exit()
// The former is called when the process starts
// The latter is called when the process is about to exit
// Both functions cause IDA debugger to suspend if they return a non zero value

// This section declares which DLLs will be available:
// * Use the "stub" to mark a dll for stubbing

// * Use the "load" to mark a dll to be loaded as is
// The "load" keyword has an additional attributes called "R0UserEntry"
// This attribute is used to designate an exported function that will be called from ring0
// Such a mechanism is useful to extend bochsys kernel or even replace it after it is loaded
// One simple application is to modify the IDT and add R3 callable interrupts into your kernel

// Only lines containing three forward slashes ("/") are processed:

/// stub ntdll.dll
/// stub kernel32.dll
/// stub user32.dll
/// stub shell32.dll
/// stub shlwapi.dll
/// stub urlmon.dll
/// stub advapi32.dll
/// stub mswsock.dll
/// stub wininet.dll
/// stub msvcrt.dll
/// stub gdi32.dll

//
// For loading drivers, you may uncomment the following stub definition(s):
//
// stub ntoskrnl.exe

// for example, to load a dll as is: load mydll.dll
// for example, to load a dll as is and specify a user R0 entry: load mydll.dll R0UserEntry=MyExportedFunc

#include <idc.idc>

//--------------------------------------------------------------------------
// IDC scripts that will be available during the debugging session

// MS Windows related functions
// ------------------------------
// BochsVirtXXXX functions allocate/free virtual memory in the emulated session.
// The "size" parameter is always rounded to a page.
//

//
// Allocate virtual memory
// This function emulates the VirtualAlloc function from MS Windows
//      addr - the preferred address for the allocation. Zero means no preference
//      size - size of the block to allocate
//      writable - should be allocated memory be wrtiable?
//                 Currently only read/write page protections are supported
// Returns: the address of the allocated block or zero
//
// long BochsVirtAlloc(long addr, long size, bool writable);
//

//
// Change protection of memory page
// This function emulates the VirtualProtect function from MS Windows
//      addr - the desired address to change protection.
//      size - size of the block
//      attr - the new page attribute:
//                 0 = Read only
//                 1 = Read/Write
// Returns: the old protection value or -1 on failure
//
// long BochsVirtProtect(long addr, long size, long attr);
//


//
// Free virtual memory
// This function emulates the VirtualFree function from MS Windows
//      addr - the address of previously allocated memory block
//      size - the size of the block. If zero, then the entire block at addr
//             will be freed.
// Returns: success
//
// bool BochsVirtFree(long addr, long size);
//

//
// Returns the base address of a given module name
//      module_name - The name of the module.
//                    The name can be full path or filename with extension, or simply filename without extension
// Returns: zero if it fails
//
// long BochsGetModuleHandle(string module_name);
//

//
// Returns a procedure's address
// This function calls the internal GetProcAddress to resolve function addresses.
//      hmod      - the module handle
//      procname  - name of the procedure inside the module
// Returns: the zero if procedure not found, otherwise the address
//
// long BochsGetProcAddress(long hmod, string procname);
//

//
// Returns the module name given its base address
//      module_handle: the base address of a given module
// Returns: empty string if module was not found
//
// string BochsGetModuleFileName(long module_handle)
//

//
// Returns the command line value passed to the application
//
// string BochsGetCommandLine()
//

//
// Set last error code
// This function emulates the SetLastError function from MS Windows.
// It writes the specified code to TIB.
//      error_code - new error code to set
// Returns: success
//
// success BochsWin32SetLastError(long error_num);
//

//
// Other functions:
// -------------------
//

//
// Sends arbitrary commands to the internal debugger of BOCHS. The output is returned as a string.
// This is useful for example to send some commands to BOCHS that are not exposed via the GUI of IDA.
//      command: the command you want to send
// Returns: output string or empty string if it failed
//
// string BochsCommand(string command)
//

//
// Retrieves the parameter value passed to an IDC script that is implementing a given API.
// This same function can be implemented with this expression: #define BX_GETPARAM(n) Dword(esp+4*(n+1))
//      arg_num: the argument number (starting by one)
// Returns: the value or zero in case it fails
//
// string BochsGetParam(long arg_num)
//

//
// Calls a function inside Bochs
// This function can call functions inside Bochs. Very useful if you want to call
// functions in the user's code. The arguments are pushed from right to left.
//      func_ptr - The address of the function to be called
//      argN - a set of dwords that contain the arguments.
//             Arguments can be numbers or pointers
// Returns: success
//
// long BochsCall(long func_ptr, arg1, arg2, ...);
//


//
// These functions will return the total physical memory amount and the remaining free
// memory in bytes.
// Returns: memory size in bytes
//
// long BochsGetFreeMem()
// long BochsGetMaxMem()
//

static BochsPatchDbgDword(ea, dv)
{
  auto i;
  for (i=0;i<4;i++)
  {
    PatchDbgByte(ea, dv & 0xFF);
    ea = ea + 1;
    dv = dv >> 8;
  }
}

static bochs_startup()
{
  Message("Bochs debugger has been initialized.\n");
  return 0;
}

static bochs_exit()
{
  Message("Bochs debugger has been terminated.\n");
  return 0;
}

