#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include "pds.h"

// Define the global variable
struct PDS_RepoInfo repo_handle;

// Create an empty repo file using fopen with "wb+" mode
// Close the file
int pds_create(char* repo_name){
    // initializing a file pointer
    FILE *fp;
    // allocating memory to a string
    char *filename = malloc(sizeof(char) * 30);
    sprintf(filename, "%s.dat", repo_name);
    // attempting to open the file
    fp = fopen(filename, "wb+");
    // returning error if file pointer is null
    if(fp == NULL){
        return PDS_FILE_ERROR;
    }
    // closing the file
    fclose(fp);
    return PDS_SUCCESS;
}

// Open data files as per the following convention
// If repo_name is "demo", then data file should be "demo.dat"
// Initialize other members of PDS_RepoInfo
int pds_open( char *repo_name, int rec_size  )
{
    // checking if the repo is already open
    if(repo_handle.repo_status == PDS_REPO_OPEN){
        return PDS_REPO_ALREADY_OPEN;
    }
    // Create the data file name based on the repository name
    char *filename = malloc(sizeof(char) * 30);
    sprintf(filename, "%s.dat", repo_name);
    // Open the data file for writing in binary mode
    FILE *data_file;
    data_file = fopen(filename, "rb+");
    if (data_file == NULL) {
        perror("Error opening data file");
        return PDS_FILE_ERROR;
    }

    // Initialize the repo_handle structure
    strcpy(repo_handle.pds_name, filename);
    repo_handle.pds_data_fp = data_file;
    repo_handle.repo_status = PDS_SUCCESS;
    repo_handle.rec_size = rec_size;

    return repo_handle.repo_status;
}

int put_rec_by_key( int key, void *rec )
{
// Seek to the end of the data file
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
    if(fseek(repo_handle.pds_data_fp, 0, SEEK_END) != 0){
        printf("fseek failed");
    }
    else{
        // if(fwrite(&key,sizeof(int),1,repo_handle.pds_data_fp)==1 && fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp)==1){
        //     return PDS_SUCCESS;
        // }
        // else{
        //     return PDS_ADD_FAILED;
        // }
        fwrite(&key,sizeof(int),1,repo_handle.pds_data_fp);
        fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
// Write the record at the current file location
}

int get_rec_by_key( int key, void *rec )
{
 
    printf("hey");
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
 // Read record-record-by record from data file
    FILE *f = repo_handle.pds_data_fp;
    int key_check;
 // Compare key of the record with the given key
    printf("hey");
    if(fseek(f, 0, SEEK_SET) != 0){
        printf("fseek failed");
    }
    else{
        while(fread(&key_check,repo_handle.rec_size,1,f)==1){
            if(key_check==key){
                fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
                return PDS_SUCCESS;
            }
            fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        }
        return PDS_REC_NOT_FOUND;
    }
 // Return success status if record is found else return failure status 
}

// Close the repo file
// Update file pointer and status in global struct
int pds_close()
{
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
    // closing the file
    fclose(repo_handle.pds_data_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}