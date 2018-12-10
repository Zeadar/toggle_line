#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * load(const char * file_name){
    FILE * p_file = fopen(file_name, "r");
    size_t file_size = 0;

    if (p_file == NULL){
        printf("opening error");
        return NULL;
    }

    /* file size counting */
    fseek(p_file, 0, SEEK_END);
    file_size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char * file_content = calloc(file_size + 1, sizeof(char));

    if (file_size != fread(file_content, sizeof(char), file_size, p_file)){
        printf("reading error");
        return NULL;
    }

    fclose(p_file);

    return file_content;
}

int save(const char * file_name, const char * file_content){
    FILE * p_file = fopen(file_name, "w");
    if (p_file == NULL){
        printf("opening error");
        return -1;
    }

    if (fputs(file_content, p_file) < 0){
        printf("writing error");
        return -2;
    }

    fclose(p_file);

    return 0;
}

int main(int argc, char ** argv){
    char * file_name = NULL;
    char * target = NULL;
    size_t target_size = 0;

    if (argc < 2){
        printf("Supply at least two arguments:\n"
                "file_name + string to replace at any quantity divided by spaces\n"
                "(and only spaces)");
    }
    else{
        file_name = *(argv + 1);
        
        for (char ** i = argv + 2; i != argv + argc; ++i){
            target = realloc(target, target_size + strlen(*i) + 1);
            strcat(target, *i);
            if (i + 1 != argv + argc)
                strcat(target, " ");
            target_size = strlen(target); 
        }

        target = realloc(target, target_size + 1);
        strcat(target, "\n");
    }

    char * file = load(file_name);
    size_t file_size = strlen(file);

    /* Search and replace */
    char * phrase_begin = strstr(file, target);
    if (phrase_begin){
        char * new_file = calloc(file_size, sizeof(char));
        strncpy(new_file, file, phrase_begin - file);
        strcat(new_file, phrase_begin + strlen(target));
        file = new_file;
    }
    else{
        file = (char*)realloc(file, (file_size + strlen(target)) * sizeof(char));
        strcat(file, target);
    }
    
     save(file_name, file);
     puts(file);
}
