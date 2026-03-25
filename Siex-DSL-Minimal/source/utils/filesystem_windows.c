#include <windows.h>
#include <siex-dsl/utils/filesystem.h>

int filesystem_file_exists(const char* path) {
  DWORD attr = GetFileAttributesA(path);
  return (attr != INVALID_FILE_ATTRIBUTES &&
         !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

int filesystem_dir_exists(const char* path) {
  DWORD attr = GetFileAttributesA(path);
  return (attr != INVALID_FILE_ATTRIBUTES &&
         (attr & FILE_ATTRIBUTE_DIRECTORY));
}
