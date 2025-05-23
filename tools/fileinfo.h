typedef struct {
    char*   name;
    int     size;
    char*   content;
}Fileinfo;

Fileinfo read_file(const char* filename);