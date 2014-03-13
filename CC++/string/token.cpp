#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** tokenize(const char* input)
{
    printf("%s\n", input);
    char* str = strdup(input);
    int count = 0;
    int capacity = 10;
    char** result = (char **)malloc(capacity*sizeof(*result));

    printf("result sizeof: %lu\n", sizeof(*result));
    char* tok=strtok(str,"/"); 

    while(1)
    {
        if (count >= capacity) {
            result = (char **)realloc(result, (capacity*=2)*sizeof(*result));
        }
        result[count++] = tok? strdup(tok) : tok;
        if (!tok) {
            break;
        }
        tok=strtok(NULL,"/");
    } 

    free(str);
    return result;
}

void plus(int &count)
{
    count += count;
}

int main ()
{
    char** tokens = tokenize("/execlient//master1/node1//test1/");
    char *path = (char *)malloc(128); 
    char** it;
    int count = 5;
    printf("count: %d\n", count);
    plus(count);
    printf("count: %d\n", count);
    memset(path, '\0', 128);
    for(it=tokens; it && *it; ++it)
    {
        if (*it != NULL) {
            if (path == NULL) {
                printf("path is null %s\n", *it);
                sprintf(path, "/%s", *it);
            } else {
                sprintf(path, "%s/%s", path, *it);
            }
            printf("path: %s\n", path);
            free(*it);
        }
    }
    printf("%.9f\n", 3.1415926);
    printf("%.9f\n", atof("3.1415926"));
    free(path);
    free(tokens);
    return 0;
}
