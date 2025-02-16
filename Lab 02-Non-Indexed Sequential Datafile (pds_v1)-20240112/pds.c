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
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
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
int pds_open(char *repo_name, int rec_size)
{
    // checking if the repo is already open
    if(repo_handle.repo_status == PDS_REPO_OPEN){
        return PDS_REPO_ALREADY_OPEN;
    }
    // initializing and allocating memory to a string pointer
    char* filename = malloc(sizeof(char) * 30);
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
    // attempting to open the file in rb+ mode
    FILE* Fp = fopen(filename, "rb+");
    if(Fp == NULL){
        return PDS_FILE_ERROR;
    }
    // returning error if file pointer is null
    // initializing memebers of the struct
    repo_handle.pds_data_fp = Fp;
    strcpy(repo_handle.pds_name, filename);
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_SUCCESS;
    return repo_handle.repo_status;
}

// Seek to the end of the data file
// Write the record at the current file location
int put_rec_by_key(int key, void* rec)
{
    // checking if the repo is closed
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
    // seeking to the end of the file
    fseek(repo_handle.pds_data_fp, 0, SEEK_END);
    // writing the key and record to the file
    fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp);
    fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

// Read key and record pairs from the start of file to the end
// Compare key of the record with the given key
// Return success status if record is found else return failure status 
int get_rec_by_key(int key, void* rec)
{
    // checking if the repo is closed
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
    // seeking to the beginning of the file
    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
    int key1;
    // reading the key and record from the file
    while(fread(&key1, sizeof(int), 1, repo_handle.pds_data_fp)){
        if(key1 == key){
            fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
            return PDS_SUCCESS;
        }
        // seeking to the next record
        fseek(repo_handle.pds_data_fp, repo_handle.rec_size, SEEK_CUR);
    }
    return PDS_REC_NOT_FOUND;
}

// Close the repo file
// Update file pointer and status in global struct
int pds_close()
{
    // checking if the repo is closed
    if(repo_handle.repo_status == PDS_REPO_CLOSED){
        return PDS_FILE_ERROR;
    }
    // closing the file
    fclose(repo_handle.pds_data_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}
