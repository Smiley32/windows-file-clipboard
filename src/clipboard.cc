#include <iostream>
#include <cstdlib>
#include <nan.h>

#include <windows.h>
#include <shlobj.h>

using namespace v8;

/**
 * Write the given paths in the clipboard (Windows)
 * 
 * @param sFiles - Array of char containing every paths, separated by '\0'
 * @param nLen - Length of the char array
 */
void writePathsInClipboard (char * sFiles, int nLen) {
	DROPFILES dobj = { 20, { 0, 0 }, 0, 1 };

	int nGblLen = sizeof(dobj) + nLen * 2 + 5;
	HGLOBAL hGbl = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, nGblLen);
	char* sData = (char*)::GlobalLock(hGbl);
	memcpy(sData, &dobj, 20);
	char* sWStr = sData + 20;
	for (int i = 0; i < nLen * 2; i += 2)
	{
		sWStr[i] = sFiles[i / 2];
	}
	::GlobalUnlock(hGbl);
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		SetClipboardData(CF_HDROP, hGbl);
		CloseClipboard();
	}
}

/**
 * Method called by javascript
 * Will write each path given in the clipboard
 * An array of strings must be sent
 */
NAN_METHOD(writeFiles) {
    Isolate        *isolate     = info.GetIsolate();
    Local<Context>  context     = isolate->GetCurrentContext();

    Local<Array> test = Local<Array>::Cast(info[0]);

    char * sFiles = NULL;
    int totalSize = 0;
    for (int i = 0; i < test->Length(); i++) {
        MaybeLocal<Value> maybeIndex = test->Get(context, i);
        Local<Value> index = maybeIndex.ToLocalChecked();

        String::Utf8Value path(isolate, index);

        int prevSize = totalSize;
        totalSize += path.length() + 1;
        // path length + \0
        sFiles = (char*)realloc(sFiles, totalSize);
        
        // copy with \0
        std::strcpy(sFiles + prevSize, *path);
    }
    
    writePathsInClipboard(sFiles, totalSize);

    free(sFiles);
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target,
        Nan::New<String>("writeFiles").ToLocalChecked(),
        Nan::GetFunction(Nan::New<FunctionTemplate>(writeFiles)).ToLocalChecked()
    );
}

NODE_MODULE(clipboard, Init)


/*
int main()
{
	char sFiles[] = "D:\\Alita Battle Angel AC3 Dubbed 1080p\0";
	//	"C:\\Users\\Smiley32\\Documents\\Godot\\2048\\2048.exe";
	DROPFILES dobj = { 20, { 0, 0 }, 0, 1 };
	int nLen = sizeof(sFiles);
	int nGblLen = sizeof(dobj) + nLen * 2 + 5;//lots of nulls and multibyte_char
	HGLOBAL hGbl = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, nGblLen);
	char* sData = (char*)::GlobalLock(hGbl);
	memcpy(sData, &dobj, 20);
	char* sWStr = sData + 20;
	for (int i = 0; i < nLen * 2; i += 2)
	{
		sWStr[i] = sFiles[i / 2];
	}
	::GlobalUnlock(hGbl);
	if (OpenClipboard(0))
	{
		EmptyClipboard();
		SetClipboardData(CF_HDROP, hGbl);
		CloseClipboard();
	}

	TCHAR lpszFileName[MAX_PATH];

	OpenClipboard(0);
	HANDLE handleData = GetClipboardData(CF_HDROP);

	if (nullptr != handleData)
	{
		HDROP hDrop = (HDROP)GlobalLock(handleData);
		if (nullptr != hDrop)
		{
			UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);
			UINT filenameLength;
			for (UINT i = 0; i < fileCount; ++i)
			{
				filenameLength = DragQueryFile(hDrop, i, 0, 0);
				DragQueryFile(hDrop, i, lpszFileName, filenameLength + 1);
				
				std::wstring test(lpszFileName);
				std::string txt(test.begin(), test.end());
				std::cout << "Str " << txt << std::endl;
			}

			GlobalUnlock(handleData);
		}
		
		
	}
	else
	{
		printf("Error:%d", GetLastError());
	}
	CloseClipboard();
}
*/