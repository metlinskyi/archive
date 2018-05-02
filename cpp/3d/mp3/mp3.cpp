#pragma comment(linker,"/MERGE:.rdata=.text")
#pragma comment(linker,"/FILEALIGN:512 /SECTION:.text,EWRX /IGNORE:4078")

#include <dshow.h>
#include <atlbase.h>
#include "mp3.h"

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
// DirectShow Graph, Filter & Pins used
IGraphBuilder	*g_pGraphBuilder	;
IMediaControl	*g_pMediaControl	;
IMediaSeeking	*g_pMediaSeeking	;
IMediaEvent		*g_pEvent			;
IMediaPosition	*g_pPosition		;
IBaseFilter		*g_pSourceCurrent	;
IBaseFilter		*g_pSourceNext		;

//------------------------------------------------------------------------------
// Name: GraphInit()
// Desc: Initialization of DirectShow components and initial graph
//------------------------------------------------------------------------------
bool InitPlayer()
{
	g_pGraphBuilder		= NULL;
	g_pMediaControl		= NULL;
	g_pMediaSeeking		= NULL;
	g_pSourceCurrent	= NULL;
	g_pSourceNext		= NULL;
	g_pEvent			= NULL;
	g_pPosition			= NULL;

    // Initialize COM
    if(FAILED(CoInitialize(NULL)))
        return false;

    // Create DirectShow Graph
    if(FAILED(CoCreateInstance(CLSID_FilterGraph,
		NULL,CLSCTX_INPROC, IID_IGraphBuilder,
        reinterpret_cast<void **>(&g_pGraphBuilder))))
        return false;

    // Get the IMediaControl Interface
    if(FAILED(g_pGraphBuilder->QueryInterface(IID_IMediaControl,
        reinterpret_cast<void **>(&g_pMediaControl))))
        return false;

    // Get the IMediaControl Interface
    if(FAILED(g_pGraphBuilder->QueryInterface(IID_IMediaSeeking,
        reinterpret_cast<void **>(&g_pMediaSeeking))))
        return false;


    return true;
}


bool Play(LPCTSTR fName,bool Loop)
{
    HRESULT hr = S_OK;
    IPin *pPin = NULL;

    TCHAR szFilename[MAX_PATH];
    WCHAR wFileName[MAX_PATH];

    _tcsncpy(szFilename, fName, NUMELMS(szFilename));

    szFilename[MAX_PATH-1] = 0;     // Ensure NULL termination


    // Make sure that this file exists
    DWORD dwAttr = GetFileAttributes(szFilename);
    if(dwAttr == (DWORD) -1)
    {
        return false;
    }

    USES_CONVERSION;
    wcsncpy(wFileName, T2W(szFilename), MAX_PATH);

    // OPTIMIZATION OPPORTUNITY
    // This will open the file, which is expensive. To optimize, this
    // should be done earlier, ideally as soon as we knew this was the
    // next file to ensure that the file load doesn't add to the
    // filter swapping time & cause a hiccup.
    //
    // Add the new source filter to the graph. (Graph can still be running)

    // Get the first output pin of the new source filter. Audio sources 
    // typically have only one output pin, so for most audio cases finding 
    // any output pin is sufficient.
    if(SUCCEEDED(g_pGraphBuilder->AddSourceFilter(wFileName, wFileName, &g_pSourceNext)))
    {
		 if(SUCCEEDED(g_pSourceNext->FindPin(L"Output", &pPin)))
		 {
			hr = g_pMediaControl->Stop();
		 }
 
    }

    // Break all connections on the filters. You can do this by adding 
    // and removing each filter in the graph
    if(SUCCEEDED(hr))
    {
        IEnumFilters *pFilterEnum = NULL;

        if(SUCCEEDED(hr = g_pGraphBuilder->EnumFilters(&pFilterEnum)))
        {
            int iFiltCount = 0;
            int iPos = 0;

            // Need to know how many filters. If we add/remove filters during the
            // enumeration we'll invalidate the enumerator
            while(S_OK == pFilterEnum->Skip(1))
            {
                iFiltCount++;
            }

            // Allocate space, then pull out all of the 
            IBaseFilter **ppFilters = reinterpret_cast<IBaseFilter **>
                                      (_alloca(sizeof(IBaseFilter *) * iFiltCount));
            pFilterEnum->Reset();

            while(S_OK == pFilterEnum->Next(1, &(ppFilters[iPos++]), NULL));

            SAFE_RELEASE(pFilterEnum);

            for(iPos = 0; iPos < iFiltCount; iPos++)
            {
                g_pGraphBuilder->RemoveFilter(ppFilters[iPos]);

                // Put the filter back, unless it is the old source
                if(ppFilters[iPos] != g_pSourceCurrent)
                {
                    g_pGraphBuilder->AddFilter(ppFilters[iPos], NULL);
                }
                SAFE_RELEASE(ppFilters[iPos]);
            }
        }
    }

    // We have the new output pin. Render it
    if(SUCCEEDED(hr))
    {
        // Release the old source filter, if it exists
        SAFE_RELEASE(g_pSourceCurrent)

        hr = g_pGraphBuilder->Render(pPin);
        g_pSourceCurrent = g_pSourceNext;
        g_pSourceNext = NULL;
    }

    SAFE_RELEASE(pPin);
    SAFE_RELEASE(g_pSourceNext); // In case of errors

    // Re-seek the graph to the beginning
    if(SUCCEEDED(hr))
    {
        LONGLONG llPos = 0;
        hr = g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
                                           &llPos, AM_SEEKING_NoPositioning);
    } 

    // Start the graph
    if(SUCCEEDED(hr))
    {
        hr = g_pMediaControl->Run();
    }

	return true;
}

//------------------------------------------------------------------------------
// Name: AppCleanUp)
// Desc: Clean up the application
//------------------------------------------------------------------------------
void Clean()
{
    // Stop playback
    if(g_pMediaControl)
        g_pMediaControl->Stop();

    // Release all remaining pointers
    SAFE_RELEASE(g_pSourceNext);
    SAFE_RELEASE(g_pSourceCurrent);
    SAFE_RELEASE(g_pMediaSeeking);
    SAFE_RELEASE(g_pMediaControl);
    SAFE_RELEASE(g_pGraphBuilder);

    // Clean up COM
    CoUninitialize();
}


void Stop()
{

  // Stop playback
  if(g_pMediaControl)
    g_pMediaControl->Stop();

	return;
}

void Pause()
{
  // Stop playback
  if(g_pMediaControl)
    g_pMediaControl->Pause();

	return;
}


