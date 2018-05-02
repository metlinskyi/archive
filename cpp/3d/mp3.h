#pragma comment ( lib, "mp3.lib" )

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport)bool InitPlayer();
	__declspec(dllexport)bool Play(LPCTSTR fName,bool Loop);
	__declspec(dllexport)void Stop();
	__declspec(dllexport)void Pause();
	__declspec(dllexport)void Clean();

#ifdef __cplusplus
}
#endif 
