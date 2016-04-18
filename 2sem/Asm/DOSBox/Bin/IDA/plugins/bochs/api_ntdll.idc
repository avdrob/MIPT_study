// The format of this file is descriped in api_kernel32.idc

///func=RtlGetLastWin32Error entry=bochsys.BxWin32GetLastError
///func=RtlSetLastWin32Error entry=bochsys.BxWin32SetLastError
///func=NtSetLdtEntries purge=24

///func=RtlAllocateHeap entry=nt_HeapAlloc
static nt_HeapAlloc()
{
  // Redirect HeapAlloc -> VirtualAlloc
  eax = BochsVirtAlloc(0, BochsGetParam(3), 1);
  return 0;
}

///func=RtlEncodePointer entry=nt_EncodePointer purge=4
static nt_EncodePointer()
{
  eax = BochsGetParam(1) ^ 0x11223344; // return the same parameter scrambled with a constant
  return 0;
}

///func=RtlDecodePointer entry=nt_DecodePointer purge=4
static nt_DecodePointer()
{
  eax = BochsGetParam(1) ^ 0x11223344; // return the same parameter scrambled with a constant
  return 0;
}