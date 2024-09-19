#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pds.h"
#include "bst.h"

struct PDS_RepoInfo repo_handle;

// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name){
    FILE *data_fp, *index_fp;
    // char *data_name = malloc(sizeof(char) * 50);
    // char *index_name = malloc(sizeof(char) * 50);
    char data_name[50];
    char index_name[50];
    strcpy(data_name, repo_name);
    strcpy(index_name, repo_name);
    strcat(data_name, ".dat");
    strcat(index_name, ".ndx");
    data_fp = fopen(data_name, "wb");
    index_fp = fopen(index_name, "wb");
    if(data_fp == NULL || index_fp == NULL){
        return PDS_FILE_ERROR;
    }
    int zero = 0;
    fwrite(&zero, sizeof(int), 1, index_fp);
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
    char data_name[50];
    char index_name[50];
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
    int ret = pds_load_ndx();
    if(ret != PDS_SUCCESS){
        return PDS_FILE_ERROR;
    }
    fclose(index_fp);
    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    return PDS_SUCCESS;
}

// pds_load_ndx - Internal function
// Load the index entries into the BST pds_bst by calling bst_add_node repeatedly for each 
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx(){
    if(!fread(&(repo_handle.rec_count), sizeof(int), 1, repo_handle.pds_ndx_fp)){
        return PDS_FILE_ERROR;
    }
    if(repo_handle.rec_count == 0){
        return PDS_SUCCESS;
    }
    for(int i = 0; i<repo_handle.rec_count; i++){
        struct PDS_NdxInfo * ndx = (struct PDS_NdxInfo *) malloc(sizeof(struct PDS_NdxInfo));
        fread(ndx, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);
        bst_add_node(&repo_handle.pds_bst, ndx->key, ndx);
    }
    return PDS_SUCCESS;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to BST by calling bst_add_node
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
int put_rec_by_key( int key, void *rec ){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_ADD_FAILED;
    }
    struct BST_Node* node = bst_search(repo_handle.pds_bst, key);
    struct PDS_NdxInfo *ndx;
    if(node != NULL){
        ndx = (struct PDS_NdxInfo *)(node->data);
        if(ndx->is_deleted == 1){
            fseek(repo_handle.pds_data_fp, ndx->offset, SEEK_SET);
            ndx->is_deleted = 0;
            return PDS_SUCCESS;
        }
        //node not deleted, trying to add already added node (update not implemented yet)
        return PDS_ADD_FAILED;
    }
    else{
        ndx = (struct PDS_NdxInfo *) malloc(sizeof(struct PDS_NdxInfo));
        fseek(repo_handle.pds_data_fp, 0, SEEK_END);
        ndx->key = key;
        ndx->offset = ftell(repo_handle.pds_data_fp);
        ndx->is_deleted = 0;
        bst_add_node(&repo_handle.pds_bst, key, ndx);
        repo_handle.rec_count++;
    }
    fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp);
    fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);;
    return PDS_SUCCESS;
}

// get_rec_by_key
// Search for index entry in BST by calling bst_search
// Seek to the file location based on offset in index entry
// Read the record from the current location
int get_rec_by_ndx_key( int key, void *rec ){
    // bst_print(repo_handle.pds_bst);
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    struct BST_Node* node = bst_search(repo_handle.pds_bst, key);
    if(node == NULL || ((struct PDS_NdxInfo *)(node->data))->is_deleted == 1){
        return PDS_REC_NOT_FOUND;
    }
    int offset = ((struct PDS_NdxInfo*)node->data)->offset;
    fseek(repo_handle.pds_data_fp, offset, SEEK_SET);
    int temp;
    fread(&temp, sizeof(int), 1, repo_handle.pds_data_fp);
    if(temp != node->key){
        return PDS_REC_NOT_FOUND;
    }
    fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

int get_rec_by_non_ndx_key( void *key, void *rec, int (*matcher)(void *rec, void *key), int *io_count ){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
    for(*io_count = 1; *io_count <= repo_handle.rec_count; (*io_count)++){
        int temp;
        fread(&temp, sizeof(int), 1, repo_handle.pds_data_fp);
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        if(matcher(rec, key) == 0){
            //check against temp?
            struct BST_Node* node = bst_search(repo_handle.pds_bst, temp);
            if(node == NULL || ((struct PDS_NdxInfo *)(node->data))->is_deleted == 1){
                return PDS_REC_NOT_FOUND;
            }
            return PDS_SUCCESS;
        }
    }
    return PDS_REC_NOT_FOUND;
}

int delete_rec_by_ndx_key( int key ){
    int status = PDS_FILE_ERROR;
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return status;
    }
    struct BST_Node* node = bst_search(repo_handle.pds_bst, key);
    if (node == NULL) return status;
    if(((struct PDS_NdxInfo *)(node->data))->is_deleted == 1) return status;
    status = PDS_SUCCESS;
    ((struct PDS_NdxInfo *)(node->data))->is_deleted = 1;
    return PDS_SUCCESS;

}

void preorder(struct BST_Node* node, FILE* index_fp) {
    if (node == NULL)
        return;
    struct PDS_NdxInfo * ndx = malloc(sizeof(struct PDS_NdxInfo));
    ndx->key = node->key;
    ndx->offset = ((struct PDS_NdxInfo *)(node->data))->offset;
    ndx->is_deleted = ((struct PDS_NdxInfo *)(node->data))->is_deleted;
    fwrite(ndx, sizeof(struct PDS_NdxInfo), 1, index_fp);
    preorder(node->left_child, index_fp);
    preorder(node->right_child, index_fp);
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// Think why should it NOT be in-order?
// Close the index file and data file
int pds_close(){
    if(repo_handle.repo_status != PDS_REPO_OPEN){
        return PDS_FILE_ERROR;
    }
    char index_name[50];
    strcpy(index_name, repo_handle.pds_name);
    strcat(index_name, ".ndx");
    repo_handle.pds_ndx_fp = fopen(index_name, "wb+");

    fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
    fwrite(&(repo_handle.rec_count), sizeof(int), 1, repo_handle.pds_ndx_fp);

    preorder(repo_handle.pds_bst, repo_handle.pds_ndx_fp);

    fclose(repo_handle.pds_ndx_fp);
    fclose(repo_handle.pds_data_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    // bst_free(repo_handle.pds_bst);
    bst_destroy(repo_handle.pds_bst);
    repo_handle.pds_bst = NULL;
    return PDS_SUCCESS;
}