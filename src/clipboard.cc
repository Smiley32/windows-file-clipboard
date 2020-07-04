#include <iostream>
#include <cstdlib>
#include <nan.h>

#include <windows.h>
#include <shlobj.h>

#include <locale>
#include <codecvt>
#include <string>

using namespace v8;

/**
 * Write the given paths in the clipboard (Windows)
 * 
 * @param sFiles - Array of char containing every paths, separated by '\0'
 * @param nLen - Length of the char array
 */
void writePathsInClipboard (TCHAR * sFiles, int nLen) {
	DROPFILES dobj = { 20, { 0, 0 }, 0, 1 };
#if UNICODE
	int nGblLen = sizeof(dobj) + nLen + 5;
#else // UNICODE
    int nGblLen = sizeof(dobj) + nLen * 2 + 5;
#endif // UNICODE
	HGLOBAL hGbl = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, nGblLen);
	TCHAR * sData = (char*)::GlobalLock(hGbl);
	memcpy(sData, &dobj, 20);
#if UNICODE
    TCHAR * sWStr = sData + 20;
	for (int i = 0; i < nLen; i += 1) {
		sWStr[i] = sFiles[i];
	}
#else // UNICODE
	TCHAR * sWStr = sData + 20;
	for (int i = 0; i < nLen * 2; i += 2) {
		sWStr[i] = sFiles[i / 2];
	}
#endif // UNICODE
	::GlobalUnlock(hGbl);
	if (OpenClipboard(0)) {
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
    Isolate * isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Array> array = Local<Array>::Cast(info[0]);

    TCHAR * sFiles = NULL;
    int totalSize = 0;
    for (size_t i = 0; i < array->Length(); i++) {
        MaybeLocal<Value> maybeIndex = array->Get(context, i);
        Local<Value> index = maybeIndex.ToLocalChecked();

        String::Utf8Value path(isolate, index);

        int prevSize = totalSize;
        totalSize += path.length() + 1;
        // path length + \0
        sFiles = (TCHAR*)realloc(sFiles, totalSize * sizeof(TCHAR));
        
        // copy with \0

#if UNICODE
        std::string narrow(*path);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(narrow);

        std::wcscpy(sFiles + prevSize, wide.c_str);
#else // UNICODE

        std::strcpy(sFiles + prevSize, *path);
#endif // UNICODE
        // std::string narrow = converter.to_bytes(wide_utf16_source_string);
    }
    
    writePathsInClipboard(sFiles, totalSize);

    free(sFiles);
}

NAN_METHOD(readFiles) {
    Isolate * isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Array> response = Array::New(isolate, 0);

    TCHAR lpszFileName[MAX_PATH];

	OpenClipboard(0);
	HANDLE handleData = GetClipboardData(CF_HDROP);

	if (nullptr != handleData) {
		HDROP hDrop = (HDROP)GlobalLock(handleData);
		if (nullptr != hDrop) {
			UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, 0, 0);
			UINT filenameLength;
			for (UINT i = 0; i < fileCount; ++i) {
				filenameLength = DragQueryFile(hDrop, i, 0, 0);
				DragQueryFile(hDrop, i, lpszFileName, filenameLength + 1);
				
                
                // I have a warning here, but I dunno why
                // https://v8docs.nodesource.com/node-13.2/d2/db3/classv8_1_1_string.html#aa752a2765da43127eba96192580671f3
                // It's not written as deprecated though
#if UNICODE
                std::wstring txt(lpszFileName);
                MaybeLocal<String> maybeS = String::NewFromTwoByte(isolate, txt.c_str());
                Local<String> s = maybeS.FromMaybe(String::Empty(isolate));
                response->Set(context, i, s);
#else // UNICODE
                std::string txt(lpszFileName);
                MaybeLocal<String> maybeS = String::NewFromUtf8(isolate, txt.c_str());
                Local<String> s = maybeS.FromMaybe(String::Empty(isolate));
                response->Set(context, i, s);
#endif // UNICODE
			}
			GlobalUnlock(handleData);
		}
	}
	else
	{
        if (0 != GetLastError()) {
		    std::cerr << "Error clipboard: " << GetLastError() << std::endl;
        }
	}
	CloseClipboard();

    info.GetReturnValue().Set(response);
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target,
        Nan::New<String>("writeFiles").ToLocalChecked(),
        Nan::GetFunction(Nan::New<FunctionTemplate>(writeFiles)).ToLocalChecked()
    );
    Nan::Set(target,
        Nan::New<String>("readFiles").ToLocalChecked(),
        Nan::GetFunction(Nan::New<FunctionTemplate>(readFiles)).ToLocalChecked()
    );
}

NODE_MODULE(clipboard, Init)
