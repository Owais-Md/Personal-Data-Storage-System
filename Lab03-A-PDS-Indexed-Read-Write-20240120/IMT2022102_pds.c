#include <stdio.h>
#include "pds.h"

struct PDS_RepoInfo repo_handle;

// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name){
    FILE *data_fp, *index_fp;
    char *data_name = malloc(sizeof(char) * 34);
    char *index_name = malloc(sizeof(char) * 34);
    strcpy(data_name, repo_name);
    strcpy(index_name, repo_name);
    strcat(data_name, ".dat");
    strcat(index_name, ".ndx");
    data_fp = fopen(data_name, "wb");
    index_fp = fopen(index_name, "wb");
    if(data_fp == NULL || index_fp == NULL){
        return PDS_FILE_ERROR;
    }
    fclose(data_fp);
    fclose(index_fp);
    return PDS_SUCCESS;
}

// pds_open
// Open the data file and index file in rb+ mode
// Update the fields of PDS_RepoInfo appropriately
// Read the number of records form the index file
// Load the index into the array and store in ndx_array by reading index entries from the index file
// Close only the index file
int pds_open( char *repo_name, int rec_size ){
    if(repo_handle.repo_status == PDS_REPO_OPEN){
        return PDS_REPO_ALREADY_OPEN;
    }
    char* data_name = malloc(sizeof(char) * 34);
    char* index_name = malloc(sizeof(char) * 34);
    strcpy(data_name, repo_name);
    strcpy(index_name, repo_name);
    strcat(data_name, ".dat");
    strcat(index_name, ".ndx");
    FILE* data_fp = fopen(data_name, "rb+");
    FILE* index_fp = fopen(index_name, "rb+");
    if(data_fp == NULL || index_fp == NULL){
        return PDS_FILE_ERROR;
    }
    repo_handle.pds_data_fp = data_fp;
    repo_handle.pds_ndx_fp = index_fp;
    pds_load_ndx();
    int zero = 0;
    fwrite(&zero, sizeof(int), 1, repo_handle.pds_ndx_fp);
    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    return PDS_SUCCESS;
}

// pds_load_ndx
// Internal function used by pds_open to read index entries into ndx_array
int pds_load_ndx(){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
    fread(&(repo_handle.rec_count), sizeof(int), 1, repo_handle.pds_ndx_fp);
    fwrite(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
    return PDS_SUCCESS;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to ndx_array using offset returned by ftell
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
int put_rec_by_key( int key, void *rec ){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_data_fp, 0, SEEK_END);
    int offset = ftell(repo_handle.pds_data_fp);
    repo_handle.ndx_array[repo_handle.rec_count].key = key;
    repo_handle.ndx_array[repo_handle.rec_count].offset = offset;
    repo_handle.rec_count++;
    fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp);
    fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

// get_rec_by_key
// Search for index entry in ndx_array
// Seek to the file location based on offset in index entry
// Read the key at the current location 
// Read the record from the current location
int get_rec_by_key( int key, void *rec ){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
    int key1;
    for(int i = 0; i < repo_handle.rec_count; i++){
        if(repo_handle.ndx_array[i].key == key){
            fseek(repo_handle.pds_data_fp, repo_handle.ndx_array[i].offset, SEEK_SET);
            fread(&key1, sizeof(int), 1, repo_handle.pds_data_fp);
            if(key1 == key){
                fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
                return PDS_SUCCESS;
            }
        }
    }
    return PDS_REC_NOT_FOUND;
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing it (overwrite the entire index file)
// Close the index file and data file
int pds_close(){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    if(repo_handle.rec_count > MAX_NDX_SIZE){
        return PDS_NDX_SAVE_FAILED;
    }
    fclose(repo_handle.pds_ndx_fp);
    char* index_name = malloc(sizeof(char) * 34);
    strcpy(index_name, repo_handle.pds_name);
    strcat(index_name, ".ndx");
    repo_handle.pds_ndx_fp = fopen(index_name, "wb+");
    fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
    fwrite(&(repo_handle.rec_count), sizeof(int), 1, repo_handle.pds_ndx_fp);
    fwrite(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, repo_handle.pds_ndx_fp);
    fclose(repo_handle.pds_ndx_fp);
    fclose(repo_handle.pds_data_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}
