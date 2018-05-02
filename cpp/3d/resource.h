// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RESOURCE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RESOURCE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RESOURCE_EXPORTS
#define RESOURCE_API __declspec(dllexport)
#else
#define RESOURCE_API __declspec(dllimport)
#endif

// This class is exported from the resource.dll
class RESOURCE_API Cresource {
public:
	Cresource(void);
	// TODO: add your methods here.
};

extern RESOURCE_API int nresource;

RESOURCE_API int fnresource(void);
