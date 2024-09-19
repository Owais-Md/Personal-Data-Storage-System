#include <stdio.h>
#include "pds.h"
#include <string.h>
#include <stdlib.h>

struct PDS_RepoInfo repo_handle;
int preorder_store(struct  BST_Node* node , FILE * fptr);
// pds_create
// Open the data file and index file in "wb" mode
// Initialize index file by storing "0" to indicate there are zero entries in index file
// close the files
int pds_create(char *repo_name){
	char filename[100];
	char index_filename[100];
	strcpy(filename , repo_name);
	strcat(filename , ".dat");
	strcpy(index_filename ,repo_name);
	strcat(index_filename , "_index.ndx");
	FILE *data_fptr = fopen(filename , "wb");
	FILE *index_fptr = fopen(index_filename,"wb");
	if ( data_fptr != NULL && index_fptr != NULL)
	{
		int number_of_records = 0;
		fwrite(&number_of_records , sizeof(int) , 1 , index_fptr);
		fclose(data_fptr);
		fclose(index_fptr);
		return PDS_SUCCESS;
	}
	return PDS_FILE_ERROR;
}

// pds_open
// Open the data file and index file in rb+ mode
// Update the fields of PDS_RepoInfo appropriately
// Read the number of records form the index file
// Load the index into the array and store in ndx_array by reading index entries from the index file
// Close only the index file
int pds_open( char *repo_name, int rec_size ){
	if(repo_handle.repo_status == PDS_REPO_OPEN)
    {
        repo_handle.repo_status = PDS_REPO_ALREADY_OPEN;
        return PDS_FILE_ERROR;
    }
    if(repo_handle.repo_status == PDS_REPO_ALREADY_OPEN)
    {
        repo_handle.repo_status =PDS_REPO_ALREADY_OPEN;
        return PDS_FILE_ERROR;
    }
	strcpy(repo_handle.pds_name , repo_name);
    char  filename[100];
	char index_filename[100];
    strcpy(filename , repo_name);
    strcat(filename , ".dat");
	strcpy(index_filename , repo_name);
	strcat(index_filename , "_index.ndx");
    FILE *data_fptr = fopen( filename, "r+b");
	FILE *ndx_fptr = fopen(index_filename,"r+b");
	if(data_fptr == NULL || ndx_fptr == NULL)	return PDS_FILE_ERROR;
    repo_handle.pds_data_fp = data_fptr;
    strcpy(repo_handle.pds_name , repo_name);
    repo_handle.repo_status = 10;
    repo_handle.rec_size = rec_size;
	int status_1 = pds_load_ndx();
	if(status_1 == PDS_SUCCESS) return PDS_SUCCESS;
	else return PDS_FILE_ERROR;
}

// pds_load_ndx - Internal function
// Load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each 
// index entry. Unlike array, for BST, you need read the index file one by one in a loop
int pds_load_ndx(){
	char index_filename[100];
	strcpy(index_filename , repo_handle.pds_name);
	strcat(index_filename , "_index.ndx");
	repo_handle.pds_ndx_fp = fopen(index_filename,"rb+");
	int status_1 = fread(&repo_handle.rec_count , sizeof(int) , 1 , repo_handle.pds_ndx_fp);
	// int status_2 = fread(&repo_handle.ndx_array , sizeof(struct PDS_NdxInfo) , repo_handle.rec_count , repo_handle.pds_ndx_fp);
	int key , offset;
	for(int i=0;i<repo_handle.rec_count;i++){
		struct PDS_NdxInfo * temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
		fread(temp , sizeof(struct PDS_NdxInfo) , 1 , repo_handle.pds_ndx_fp);
		bst_add_node(&repo_handle.ndx_root , temp->key , &(temp->offset));
		
	}
	int status_3 = fclose(repo_handle.pds_ndx_fp);
	if(status_1 == 1 && status_3 == 0)	return PDS_SUCCESS;
	return PDS_FILE_ERROR;
}

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to BST by calling bst_add_node
// Increment record count
// Write the record at the end of the file
// Return failure in case of duplicate key
int put_rec_by_key( int key, void *rec ){
	if(repo_handle.repo_status != PDS_REPO_OPEN && repo_handle.repo_status != PDS_REPO_ALREADY_OPEN)
    {
        return PDS_ADD_FAILED;
    }
	if(repo_handle.rec_count >= MAX_NDX_SIZE)	return PDS_ADD_FAILED;
	if(bst_search(repo_handle.ndx_root ,key)!=NULL)	return PDS_ADD_FAILED;
	FILE * data_fptr = repo_handle.pds_data_fp;
	// int index = repo_handle.rec_count;
	int status_1 = fseek(data_fptr,0,SEEK_END);
	int *offset = (int *)malloc(sizeof(int));
	*offset= ftell(data_fptr);
	// repo_handle.ndx_array[index].key = key;
	// repo_handle.ndx_array[index].offset = location;
	int status_4 = bst_add_node(&repo_handle.ndx_root ,key , offset); 
    int status_2 = fwrite(&key , sizeof(int) , 1 , data_fptr);
    int status_3 = fwrite(rec , repo_handle.rec_size , 1 , data_fptr);
	repo_handle.rec_count++;
    if(status_1 == 0 && status_2 ==1 && status_3 == 1 && status_4 == 0) return PDS_SUCCESS;
	return PDS_ADD_FAILED;
}

// get_rec_by_key
// Search for index entry in BST by calling bst_search
// Seek to the file location based on offset in index entry
// Read the record from the current location
int get_rec_by_key( int key, void *rec ){
	int found = 0;
	int key_from_file = 0;
	struct BST_Node *node = bst_search(repo_handle.ndx_root , key);
	if(node != NULL)
	{
		found = 1;
		int status_1 = fseek(repo_handle.pds_data_fp , *((int *)node->data) , SEEK_SET);
		int status_2 = fread(&key_from_file , sizeof(int), 1,repo_handle.pds_data_fp );
		int status_3 = fread(rec , repo_handle.rec_size , 1 , repo_handle.pds_data_fp);
		if(status_1 == 0 && status_2 == 1 && status_3 == 1) return PDS_SUCCESS;
	}
	return PDS_REC_NOT_FOUND;
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Store the number of records
// Unload the ndx_array into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// Think why should it NOT be in-order?
// Close the index file and data file
int pds_close(){
	if(repo_handle.repo_status == PDS_REPO_CLOSED)
    {
        return PDS_FILE_ERROR;
    }
	char index_filename[100];
	strcpy(index_filename , repo_handle.pds_name);
	strcat(index_filename , "_index.ndx");
	FILE * ndx_fptr = fopen(index_filename , "wb");
	if ( ndx_fptr == NULL )	return PDS_NDX_SAVE_FAILED;
	int number_of_records = repo_handle.rec_count;
	int status_1 = fwrite(&number_of_records , sizeof(int) , 1 , ndx_fptr);
	struct  BST_Node *temp = repo_handle.ndx_root;
	int status_2 = preorder_store(temp , ndx_fptr);
	// int status_2 = fwrite(repo_handle.ndx_array , sizeof(struct PDS_NdxInfo)  , repo_handle.rec_count , ndx_fptr);
	bst_free(repo_handle.ndx_root);
	repo_handle.ndx_root= NULL;
	int status_3 = fclose(ndx_fptr);
	int status_4 = fclose(repo_handle.pds_data_fp);
	
	repo_handle.repo_status = PDS_REPO_CLOSED;
	if( status_1 == 1 && status_2 == 0 && status_3 == 0 && status_4 == 0) return PDS_SUCCESS;
	if( status_2 != repo_handle.rec_count) return PDS_NDX_SAVE_FAILED;
	return PDS_FILE_ERROR;
}

int preorder_store(struct  BST_Node* node , FILE * fptr){
	if(node == NULL)	return 0;

	// int status_1 = fwrite(&node->key,sizeof(int),1,fptr);
	// int status_2 = fwrite(node->data, sizeof(int) , 1 , fptr);
	struct PDS_NdxInfo *temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
	temp->key = node->key;
	temp->offset = *((int *)node->data);
	int status_1 = fwrite(temp,sizeof(struct PDS_NdxInfo),1,fptr);
	int status_3 = preorder_store(node->left_child , fptr);
	int status_4 = preorder_store(node->right_child , fptr);
	if(status_1 !=1 || status_3 != 0 || status_4 != 0) return 1;
	else return 0;
}