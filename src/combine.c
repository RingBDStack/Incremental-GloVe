//by bmj
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TSIZE 1048576
#define SEED 1159241
#define HASHFN bitwisehash

static const int MAX_STRING_LENGTH = 1000;
typedef double real;

typedef struct cooccur_rec {
    int word1;
    int word2;
    real val;
} CREC;

typedef struct cooccur_rec_id {
    int word1;
    int word2;
    real val;
    int id;
} CRECID;

typedef struct cooccur_delta {
    int word1;
    int word2;
    real Xij;
    real Xij_delta;
} CRECDELTA;

typedef struct hashrec {
    char	*word;
    long long id;
    struct hashrec *next;
} HASHREC;

char *cooccurrence,*cooccurrencedelta,*result;

int scmp( char *s1, char *s2 ) {
    while (*s1 != '\0' && *s1 == *s2) {s1++; s2++;}
    return(*s1 - *s2);
}

int find_arg(char *str, int argc, char **argv) {
    int i;
    for (i = 1; i < argc; i++) {
        if (!scmp(str, argv[i])) {
            if (i == argc - 1) {
                printf("No argument given for %s\n", str);
                exit(1);
            }
            return i;
        }
    }
    return -1;
}

int compare_crec(const CREC *a, const CREC *b) {
    int c = a->word1 - b->word1;
    int d = a->word2 - b->word2;
    if (c!= 0) return c;
    else return d;
    
}

int main(int argc, char **argv)
{

    FILE *fin,*fin_delta,*fout;

    cooccurrence = malloc(sizeof(char) * MAX_STRING_LENGTH);
    cooccurrencedelta = malloc(sizeof(char) * MAX_STRING_LENGTH);
    result = malloc(sizeof(char) * MAX_STRING_LENGTH);

    char c;
    real zero = 0.0;
    int i = 0;
    if (argc == 1) {}
    else{
        if ((i = find_arg((char *)"-cooccurrence-file", argc, argv)) > 0) strcpy(cooccurrence, argv[i + 1]);
        else strcpy(cooccurrence, (char *)"cooccurrence.bin");
        if ((i = find_arg((char *)"-cooccurrencedelta-file", argc, argv)) > 0) strcpy(cooccurrencedelta, argv[i + 1]);
        else strcpy(cooccurrencedelta, (char *)"cooccurrencedelta.bin");
        if ((i = find_arg((char *)"-result-file", argc, argv)) > 0) strcpy(result, argv[i + 1]);
        else strcpy(result, (char *)"result.bin");
    }

    fin = fopen(cooccurrence,"rb");
    fin_delta = fopen(cooccurrencedelta,"rb");
    fout = fopen(result,"w");
    if(fin==NULL||fin_delta==NULL||fout==NULL){
        printf("Error,cann not open FILE\n");
    }
    CREC temp, tempdelta;
    fread(&temp,sizeof(CREC),1,fin);
    fread(&tempdelta,sizeof(CREC),1,fin_delta);
    while(!feof(fin)&&!feof(fin_delta))
    {
        int compare_flag = compare_crec(&temp,&tempdelta);
        if(compare_flag==0){
            fwrite(&temp.word1, sizeof(int), 1, fout);
            fwrite(&temp.word2, sizeof(int), 1, fout);
            fwrite(&temp.val, sizeof(real), 1, fout);
            fwrite(&tempdelta.val, sizeof(real), 1, fout);
            fread(&temp,sizeof(CREC),1,fin);
            fread(&tempdelta,sizeof(CREC),1,fin_delta);
        }else if(compare_flag>0){
            fwrite(&tempdelta.word1, sizeof(int), 1, fout);
            fwrite(&tempdelta.word2, sizeof(int), 1, fout);
            fwrite(&zero, sizeof(real), 1, fout);
            fwrite(&tempdelta.val, sizeof(real), 1, fout);
            fread(&tempdelta,sizeof(CREC),1,fin_delta);
        }else if(compare_flag<0){
            fwrite(&temp.word1, sizeof(int), 1, fout);
            fwrite(&temp.word2, sizeof(int), 1, fout);
            fwrite(&temp.val, sizeof(real), 1, fout);
            fwrite(&zero, sizeof(real), 1, fout);
            fread(&temp,sizeof(CREC),1,fin);
        }
    }
    if(feof(fin)){
            while(!feof(fin_delta))
            {
                fwrite(&tempdelta.word1, sizeof(int), 1, fout);
                fwrite(&tempdelta.word2, sizeof(int), 1, fout);
                fwrite(&zero, sizeof(real), 1, fout);
                fwrite(&tempdelta.val, sizeof(real), 1, fout);
                fread(&tempdelta,sizeof(CREC),1,fin_delta);
            }        }
    if(feof(fin_delta)){
            while(!feof(fin))
            {
                fwrite(&temp.word1, sizeof(int), 1, fout);
                fwrite(&temp.word2, sizeof(int), 1, fout);
                fwrite(&temp.val, sizeof(real), 1, fout);
                fwrite(&zero, sizeof(real), 1, fout);
                fread(&temp,sizeof(CREC),1,fin);
            }
        }
    fclose(fin);
    fclose(fin_delta);
    fclose(fout);
    free(cooccurrence);
    free(cooccurrencedelta);
    free(result);
    return 0;
}
