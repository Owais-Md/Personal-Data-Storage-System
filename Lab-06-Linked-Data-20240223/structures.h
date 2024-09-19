#ifndef STRUCTURES_H
#define STRUCTURES_H

#define SUCCESS 0
#define FAILURE 1
 
#define MAX_INSTRUMENTS_PER_ARTIST 1000

struct Artist{
	int artist_id;
	char artist_name[30];
    char genre[30];
};

struct MusicalInstrument{
    char name[30];
    int ID;
};

extern struct PDS_RepoInfo *repoHandle;

// Add the given artist into the repository by calling put_rec_by_key
int add_artist( struct Artist *a );

// Display artist info in a single line as a CSV without any spaces
void print_artist( struct Artist *a );

// Use get_rec_by_key function to retrieve artist
int search_artist( int artist_id, struct Artist *a );

// Load all the artists from a CSV file
int store_artists( char *artist_data_file );

int search_artist_by_name( char *name, struct Artist *a, int *io_count );

/* Return 0 if phone of the artist matches with phone parameter */
/* Return 1 if phone of the artist does NOT match */
/* Return > 1 in case of any other error */
int match_artist_name( void *rec, void *key );

int delete_artist( int artist_id );

int add_musical_instrument( int key, struct MusicalInstrument *instrument );

int get_musical_instrument( int key, struct MusicalInstrument *instrument );

int link_musical_instrument( int artist_id, int instrument_id );

int get_instruments_of_artist( int artist_id, int instrument_ids[], int *io_count );

int search_linked_records( int num_records, int linked_musical_instruments_ids[], int *io_count );

#endif
