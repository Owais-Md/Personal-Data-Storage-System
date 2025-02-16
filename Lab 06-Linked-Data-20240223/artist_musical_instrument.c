#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "structures.h"

// Add the given contact into the repository by calling put_rec_by_key
int add_artist( struct Artist *a )
{
    return put_rec_by_key( a->artist_id, a );
}

int add_musical_instrument( int key, struct MusicalInstrument *instrument )
{
    return put_linked_rec_by_key( key, instrument );
}

// Use get_rec_by_key function to retrieve contact
int search_artist( int contact_id, struct Artist *a )
{
    return get_rec_by_ndx_key( contact_id, a );
}

// Use get_rec_by_non_ndx_key function to retrieve contact
int search_artist_by_name( char *name, struct Artist *a, int *io_count )
{
    return get_rec_by_non_ndx_key( name, a, match_artist_name, io_count );
}

int get_musical_instrument( int key, struct MusicalInstrument *instrument )
{
    return get_linked_rec_by_key( key, instrument );
}

int delete_artist( int artist_id )
{
    return delete_rec_by_ndx_key( artist_id );
}

int link_musical_instrument( int artist_id, int instrument_id )
{
    return pds_link_rec( artist_id, instrument_id );
}

int get_instruments_of_artist( int artist_id, int instrument_ids[], int *io_count )
{
    return pds_get_linked_rec( artist_id, instrument_ids, io_count );
}

// Return 0 if phone of the contact matches with phone parameter
// Return 1 if phone of the contact does NOT match
int match_artist_name( void *rec, void *key )
{
    struct Artist *a = (struct Artist *) rec;
    char *name = (char *) key;
    if(strcmp(a->artist_name, name) == 0){
        return 0;
    }
    return 1;
}

void print_artist( struct Artist *a )
{
    printf("%d,%s,%s\n", a->artist_id,a->artist_name,a->genre);
}

// Load all the artists from a text file
int store_artist( char *contact_data_file )
{
	FILE *cfptr;
	char contact_line[500], token;
	struct Artist a, dummy;
	cfptr = (FILE *) fopen(contact_data_file, "r");
	while(fgets(contact_line, sizeof(contact_line)-1, cfptr)){
		sscanf(contact_line, "%d%s%s", &(a.artist_id),a.artist_name,a.genre);
		print_artist( &a );
		add_artist( &a );
	}
}