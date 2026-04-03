#include "../include/fs.hpp"
#include "../include/str.hpp"
#include "../include/vga.hpp"

static const int MAX_FILES = 16;
static const int MAX_SIZE  = 512;

struct File {
    char name[32];
    char data[MAX_SIZE];
    int  used;
};
static File files[MAX_FILES];
static int  count = 0;

void fs_init() {
    count = 0;
}

static File* find(const char* name) {
    for (int i = 0; i < count; i++)
        if (k_strcmp(files[i].name, name) == 0)
            return &files[i];
    return nullptr;
}

/*
simple filesystem implementation
creates new files by appending to the array
deletes files by shifting remaining entries down
stores everything in memory with fixed limits
file operations return negative one on error
file operations return zero on success
writes truncate existing file content
reads never exceed the maximum size
find helper searches for files by name
list shows all files that exist
init clears all stored files
destroy actually removes a file entry
get retrieves the full file contents
upd is now named write for clarity
*/
int fs_create(const char* name) {
    if (find(name) || count >= MAX_FILES) return -1;
    k_strcpy(files[count].name, name);
    files[count].data[0] = 0;
    files[count].used = 0;
    count++;
    return 0;
}

int fs_write(const char* name, const char* data) {
    File* f = find(name);
    if (!f) return -1;
    int i = 0;
    while (data[i] && i < MAX_SIZE - 1) { f->data[i] = data[i]; i++; }
    f->data[i] = 0;
    f->used = i;
    return 0;
}

int fs_read(const char* name, char* out, int maxlen) {
    File* f = find(name);
    if (!f) return -1;
    int i = 0;
    while (f->data[i] && i < maxlen - 1) { out[i] = f->data[i]; i++; }
    out[i] = 0;
    return i;
}

void fs_list() {
    if (count == 0) { vga_println("(empty)"); return; }
    for (int i = 0; i < count; i++)
        vga_println(files[i].name);
}

int fs_delete(const char* name) {
    for (int i = 0; i < count; i++) {
        if (k_strcmp(files[i].name, name) == 0) {
            files[i] = files[--count];
            return 0;
        }
    }
    return -1;
}
