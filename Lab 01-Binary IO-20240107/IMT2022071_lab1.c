//IMT2022071
//Samyak Jain

# include <stdio.h> 
# include <string.h>

struct student {
  int rollnum;
  char name[30];
  int age;
};

//function for 1a
//writes text file with integers from 0 to 19
int save_num_text( char *filename ) {
    FILE* file = fopen(filename, "w");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 20; i++) {
        fprintf(file, "%d\n", i);
    }
    fclose(file);
}

//function for 1b
//reads the file written by save_num_text
int read_num_text( char *filename ) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 20; i++) {
        int num;
        fscanf(file, "%d", &num);
        printf("%d\n", num);
    }
    fclose(file);
}

//function for 1c
//writes text file with students from nameArray
int save_struct_text( char *filename ) {
    FILE* file = fopen(filename, "w");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    char nameArray[][30] = {"Name1", "Name2", "Name3", "Name4", "Name5"};
    for(int i = 0; i < 5; i++) {
        struct student s;
        s.rollnum = i;
        strcpy(s.name, nameArray[i]);
        s.age = i;
        fprintf(file, "%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(file);
}

//function for 1d
//reads the file written by save_struct_text
int read_struct_text( char *filename ) {
    FILE* file = fopen(filename, "r");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 5; i++) {
        struct student s;
        fscanf(file, "%d %s %d", &s.rollnum, s.name, &s.age);
        printf("%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(file);
}

//function for 2a
//writes binary file with integers from 0 to 19
int save_num_binary( char *filename ) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 20; i++) {
        fwrite(&i, sizeof(int), 1, file);
    }
    fclose(file);
}

//function for 2b
//reads the file written by save_num_binary
int read_num_binary( char *filename ) {
    FILE* file = fopen(filename, "rb");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 20; i++) {
        int num;
        fread(&num, sizeof(int), 1, file);
        printf("%d\n", num);
    }
    fclose(file);
}

//function for 2c
//writes binary file with students from nameArray
int save_struct_binary( char *filename ) {
    FILE* file = fopen(filename, "wb");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    char nameArray[][30] = {"Name1", "Name2", "Name3", "Name4", "Name5"};
    for(int i = 0; i < 5; i++) {
        struct student s;
        s.rollnum = i;
        strcpy(s.name, nameArray[i]);
        s.age = i;
        fwrite(&s, sizeof(struct student), 1, file);
    }
    fclose(file);
}

//function for 2d
//reads the file written by save_struct_binary
int read_struct_binary( char *filename ) {
    FILE* file = fopen(filename, "rb");
    if(!file) {
        printf("Error opening file!\n");
        return -1;
    }
    for(int i = 0; i < 5; i++) {
        struct student s;
        fread(&s, sizeof(struct student), 1, file);
        printf("%d %s %d\n", s.rollnum, s.name, s.age);
    }
    fclose(file);
}
