#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_


#pragma pack(push, 2)
typedef struct file{
    struct file *parent;
    int inode;
    char *name;
    char type;
    size_t fileSize;
    void *content;
}file;
#pragma pack(pop)


typedef struct record{
    struct record *previous;
    /*
     * previous == NULL - сама директория
     */
    struct record *next;
    /*
     * next == NULL - последний файл в списке.
     */
     file  *current;
}record;

record* lastRecord(file *directory);
void addFile(file *parent, file *child, record* previous);
record* listDirectoryContent(file *directory);
void cutRecord(record *recordToDelete);
char removeFile(char *filename, file *currentDirectory);
file* navigate(char *filename, file *currentDirectory);
void reWriteContent(file *regularFile, char *content, size_t content_len);
char moveFile(char *res, char *dest, file *currentDirectory);
char printFileInfo(FILE* fout, record *recordsList);
void addContent(file *regularFile, char *content, size_t content_len);
file* newFile(file *parent, char *filename, char type, record *prevRecord);
file* find(char *filename, file *currentDirectory);
file* initFileSystem();

#define DEFAULT_INCREASE 8

extern file *workingDirectory;


#endif
