#include "ggml-backend-dl.h"

#ifdef _WIN32

dl_handle * dl_load_library(const char * path) {
    // suppress error dialogs for missing DLLs
    DWORD old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    SetErrorMode(old_mode | SEM_FAILCRITICALERRORS);

    int wide_len = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    if (wide_len <= 0) {
        SetErrorMode(old_mode);
        return nullptr;
    }

    std::wstring wide_path(wide_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, path, -1, &wide_path[0], wide_len);
    HMODULE handle = LoadLibraryW(wide_path.c_str());

    SetErrorMode(old_mode);

    return handle;
}

void * dl_get_sym(dl_handle * handle, const char * name) {
    DWORD old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    SetErrorMode(old_mode | SEM_FAILCRITICALERRORS);

    void * p = (void *) GetProcAddress(handle, name);

    SetErrorMode(old_mode);

    return p;
}

const char * dl_error() {
    return "";
}

#else

dl_handle * dl_load_library(const char * path) {
    dl_handle * handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    return handle;
}

void * dl_get_sym(dl_handle * handle, const char * name) {
    return dlsym(handle, name);
}

const char * dl_error() {
    const char *rslt = dlerror();
    return rslt != nullptr ? rslt : "";
}

#endif
