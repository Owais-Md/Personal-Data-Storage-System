#ifndef PDS_H
#define PDS_H

// Error codes
#define PDS_SUCCESS 0
#define PDS_FILE_ERROR 1
#define PDS_ADD_FAILED 2
#define PDS_REC_NOT_FOUND 3

// Repository status values
#define PDS_REPO_OPEN 10
#define PDS_REPO_CLOSED 11
#define PDS_REPO_ALREADY_OPEN 12

struct PDS_RepoInfo{
	char pds_name[30];
	FILE *pds_data_fp;
	int repo_status; 
	int rec_size; // For fixed length records
};


extern struct PDS_RepoInfo repo_handle;

// Create an empty repo file using fopen with "wb+" mode
// Close the file
int pds_create(char *repo_name);

// Open data files as per the following convention
// If repo_name is "demo", then data file should be "demo.dat"
// Initialize other members of PDS_RepoInfo
int pds_open(char *repo_name, int rec_size);

// Seek to the end of the data file
// Write the record at the current file location
int put_rec_by_key(int key, void *rec);


// Read the record at the current file location
int get_rec_by_key(int key, void *rec);

// Close all files 
int pds_close();

#endif
