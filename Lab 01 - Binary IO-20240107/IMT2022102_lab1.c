//IMT2022102_lab1.c

# include <stdio.h> 
# include <string.h>

struct student {
  int rollnum;
  char name[30];
  int age;
};

//using typedef for student as Stu
typedef struct student Stu;

int save_num_text( char *filename ) {
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    //printing integers from 0 to 19 in the file
    for(int i = 0; i < 20; i++) {
        fprintf(fp, "%d\n", i);
    }
    fclose(fp);
}

int read_num_text( char *filename ) {
    FILE* fp = fopen(filename, "r");
    //reading file written but save_num_text
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for(int i = 0; i < 20; i++) {
        int num;
        fscanf(fp, "%d", &num);
        printf("%d\n", num);
    }
    fclose(fp);
}

int save_struct_text( char *filename ) {
    FILE* fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    //writes ints from 0 to 4 Stu into text file
    char names[][30] = {"ab", "bc", "cd", "de", "ef"};
    for(int i = 0; i < 5; i++) {
        Stu s;
        s.rollnum = i;
        strcpy(s.name, names[i]);
        s.age = i;
        fprintf(fp, "%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(fp);
}

int read_struct_text( char *filename ) {
    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for(int i = 0; i < 5; i++) {
        Stu s;
        //reads the file written by save_struct_text
        fscanf(fp, "%d %s %d", &s.rollnum, s.name, &s.age);
        printf("%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(fp);
}


int save_num_binary( char *filename ) {
    FILE* fp = fopen(filename, "wb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for(int i = 0; i < 20; i++) {
        //writes integers from 0 to 19 into binary file
        fwrite(&i, sizeof(int), 1, fp);
    }
    fclose(fp);
}

int read_num_binary( char *filename ) {
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for(int i = 0; i < 20; i++) {
        int num;
        //reads the file written by save_num_binary
        fread(&num, sizeof(int), 1, fp);
        printf("%d\n", num);
    }
    fclose(fp);
}

int save_struct_binary( char *filename ) {
    FILE* fp = fopen(filename, "wb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    char names[][30] = {"ab", "bc", "cd", "de", "ef"};
    for(int i = 0; i < 5; i++) {
        Stu s;
        //writes Stu from 0 to 4 into binary file
        s.rollnum = i;
        strcpy(s.name, names[i]);
        s.age = i;
        fwrite(&s, sizeof(Stu), 1, fp);
    }
    fclose(fp);
}

int read_struct_binary( char *filename ) {
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    for(int i = 0; i < 5; i++) {
        Stu s;
        //reads the file written by save_struct_binary
        fread(&s, sizeof(Stu), 1, fp);
        printf("%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(fp);
}

