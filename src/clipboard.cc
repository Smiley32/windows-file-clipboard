#include <iostream>
#include <cstdlib>
#include <nan.h>

#include <windows.h>
#include <shlobj.h>

using namespace v8;

void writePathsInClipboard (char * sFiles, int nLen) {
    // char sFiles[] = "D:\\Alita Battle Angel AC3 Dubbed 1080p\0";
	//	"C:\\Users\\Smiley32\\Documents\\Godot\\2048\\2048.exe";
	DROPFILES dobj = { 20, { 0, 0 }, 0, 1 };

    for(int i = 0; i < nLen; i++) {
        if (sFiles[i] == '\0') {
            continue;
        }
        std::cout << sFiles[i];
    }
    std::cout << std::endl;

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
}

NAN_METHOD(writeFiles) {
    Isolate        *isolate     = info.GetIsolate();
    Local<Context>  context     = isolate->GetCurrentContext();

    // MaybeLocal<String> maybe_str = info[0]->ToString(context);
    MaybeLocal<Object> maybeArray = info[0]->ToObject(context);

    if (maybeArray.IsEmpty()) {
        std::cout << "YEEES" << std::endl;
        return;
    }
    std::cout << "WUT?" << std::endl;

    Local<Object> array = maybeArray.ToLocalChecked();
    Local<Array> test = Local<Array>::Cast(info[0]);

    std::cout << test->Length() << " ?!?" << std::endl;

    char * sFiles = NULL;
    int totalSize = 0;
    for (int i = 0; i < test->Length(); i++) {
        MaybeLocal<Value> maybeIndex = test->Get(context, i);
        Local<Value> index = maybeIndex.ToLocalChecked();

        String::Utf8Value path(isolate, index);

        int prevSize = totalSize;
        totalSize += path.length() + 1;
        std::cout << "ICI?" << std::endl;
        sFiles = (char*)realloc(sFiles, totalSize);
        

        std::strcpy(sFiles + prevSize, *path);

        // std::cout << "[" << i << "]: " << std::string(*path) << std::endl;
        /*
        pathArray[i] = new char[path.length() + 1];
        std::strcpy(pathArray[i], *path);

        std::cout << "ici " << *path << " -:- " << path.length() << std::endl;
        */
    }
    
    std::cout << "chaines: ";
    for (int iChar = 0; iChar < totalSize; iChar++) {
        std::cout << sFiles[iChar];
    }
    std::cout << std::endl;

    writePathsInClipboard(sFiles, totalSize);

    free(sFiles);

    /*
    for (int i = 0; i < test->Length(); i++) {
        delete[] pathArray[i];
    }
    delete[] pathArray;
    */
}

/*
NAN_MODULE_INIT(InitModule) {
  NODE_SET_METHOD(target, "writeFiles", writeFiles);
}
*/


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