// IMT2022102 - PDS TESTER
// Mohammad Owais

// To compile:
// gcc -o pds_tester artist_musical_instrument.c IMT2022102_pds.c pds_tester.c bst.c

// To run:
// ./pds_tester testcases.txt 0/1
// testcases.in is a sample file containing the test cases
// Commands like CREATE, OPEN, STORE_ARTIST, STORE_INSTRUMENT, SEARCH_BY_ID, SEARCH_BY_NAME, DELETE_ARTIST, LINK, GET_INSTRUMENTS_OF, CLOSE are used in the test cases

// Usage of commands:
//      PASS/FAIL is expressed as (0/1)
//      CREATE:
//          CREATE repo_name PASS/FAIL
//          CREATE repo_name linked_repo_name PASS/FAIL
//      OPEN:
//          OPEN repo_name PASS/FAIL
//          OPEN repo_name linked_repo_name PASS/FAIL
//      STORE_ARTIST:
//          STORE_ARTIST artist_id artist_name genre PASS/FAIL
//      STORE_INSTRUMENT:
//          STORE_INSTRUMENT artist_id instrument_name PASS/FAIL
//      SEARCH_BY_ID:
//          SEARCH_BY_ID artist_id PASS/FAIL
//      SEARCH_BY_NAME:
//          SEARCH_BY_NAME artist_name index_at_which artist was found PASS/FAIL
//      DELETE_ARTIST:
//          DELETE_ARTIST artist_id PASS/FAIL
//      LINK:
//          LINK artist_id instrument_id PASS/FAIL
//      GET_INSTRUMENTS_OF:
//          GET_INSTRUMENTS_OF artist_id PASS/FAIL
//      CLOSE:
//          CLOSE repo_name PASS/FAIL

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "structures.h"

#define TREPORT(a1,a2) printf("Status: %s -\t%s\n\n\n",a1,a2); fflush(stdout);

// when show_outputs is 0, TREPORT is used to only print pass/fail and when show_outputs is 1, the actual output is printed if testcase passes. Otherwise, TREPORT prints error message
int show_outputs = 0;

void process_line( char *test_case );

int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if( argc != 3 ){
		fprintf(stderr, "Usage: %s testcasefile show_outputs(takes 0 or 1)\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *) fopen(argv[1], "r");
    if( cfptr == NULL ){
        fprintf(stderr, "Can't open %s\n", argv[1]);
        exit(1);
    }
    show_outputs = atoi(argv[2]);
	while(fgets(test_case, sizeof(test_case)-1, cfptr)){
		// printf("line:%s",test_case);
		if( !strcmp(test_case,"\n") || !strcmp(test_case,"") )
			continue;
		process_line( test_case );
	}
}

void process_line( char *test_case )
{
    static int line = 1;
    char *repo_name = (char *)malloc(30*sizeof(char));
    char *linked_repo_name = (char *)malloc(30*sizeof(char));
	char command[20], param1[30], param2[30], param3[30], param4[30], info[1024];
	int id, status, rec_size, linked_rec_size, expected_status;
	struct Artist testArtist;
    struct MusicalInstrument testInstrument;

	rec_size = sizeof(struct Artist);
    linked_rec_size = sizeof(struct MusicalInstrument);

	sscanf(test_case, "%s%s%s%s%s", command, param1, param2, param3, param4);
	printf("Test case %d:\t%s", line++, test_case); fflush(stdout);
	if( !strcmp(command,"CREATE") ){
        // command  param1  param2      param3 
        // CREATE   artist  instruments 0
        // CREATE   artist  0
		strcpy(repo_name, param1);
        if ( !strcmp(param2,"0") ){
            expected_status = SUCCESS;
            linked_repo_name = NULL;
        }
        else{
            strcpy(linked_repo_name, param2);
            if ( !strcmp(param3,"0") )
                expected_status = SUCCESS;
            else
                expected_status = FAILURE;
        }
        // printf("Repo name: %s(len:%ld), Linked repo name: %s(len:%ld)\n", repo_name, strlen(repo_name), linked_repo_name, strlen(linked_repo_name));
        status = pds_create(repo_name , linked_repo_name);

		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status == expected_status ){
            if(show_outputs == 1)
                printf("Repo created: %s\n\n\n", repo_name);
            else
                TREPORT("PASS", "Repo Created");
		}
		else{
			sprintf(info,"pds_create returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"OPEN") ){
        // command  param1  param2
        // OPEN     artist  0
		strcpy(repo_name, param1);
        if ( !strcmp(param2,"0") ){
            expected_status = SUCCESS;
            linked_repo_name = NULL;
        }
        else{
            strcpy(linked_repo_name, param2);
            if ( !strcmp(param3,"0") )
                expected_status = SUCCESS;
            else
                expected_status = FAILURE;
        }
		status = pds_open( repo_name, linked_repo_name, rec_size, linked_rec_size);
		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status == expected_status ){
            if(show_outputs == 1)
                printf("Repo opened: %s\n\n\n", repo_name);
            else
			    TREPORT("PASS", "Repo Opened");
		}
		else{
			sprintf(info,"pds_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"STORE_ARTIST") ){
        // command           param1  param2  param3  param4
        // STORE_ARTIST      1       K391    EDM     0
        if( !strcmp(param4,"0") )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;

		sscanf(param1, "%d", &id);
		testArtist.artist_id = id;
		sprintf(testArtist.artist_name, "%s", param2);
		sprintf(testArtist.genre, "%s", param3);
		status = add_artist( &testArtist );
		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status == expected_status ){
            if(show_outputs == 1){
                printf("Stored artist: ");
                print_artist(&testArtist);
                printf("\n\n");
            }
            else
                TREPORT("PASS", "Artist Stored");
		}
		else{
			sprintf(info,"add_artist returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"STORE_INSTRUMENT") ){
        // command              param1  param2  param3
        // STORE_INSTRUMENT     1       Guitar  0
        if( !strcmp(param4,"0") )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;

		sscanf(param1, "%d", &id);
        testInstrument.ID = id;
        sprintf(testInstrument.name, "%s", param2);
		status = add_musical_instrument( id, &testInstrument );
		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status == expected_status ){
            if(show_outputs == 1)
                printf("Stored instrument: %d, %s\n\n\n", testInstrument.ID, testInstrument.name);
			else
                TREPORT("PASS", "Instrument Stored");
		}
		else{
			sprintf(info,"add_musical_instrument returned status %d",status);
			TREPORT("FAIL", info);
		}
    }
	else if( !strcmp(command,"SEARCH_BY_ID") ){
        // command           param1  param2
        // SEARCH_BY_ID        1       0
		if( !strcmp(param2,"0") )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;

		sscanf(param1, "%d", &id);
		testArtist.artist_id = -1;
		status = search_artist( id, &testArtist );
		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",id, status);
			TREPORT("FAIL", info);
		}
		else{
            // printing retrieved value:
			// Check if the retrieved values match
			if( show_outputs == 1){
                if( testArtist.artist_id != -1 )
                    printf("Retrieved artist: %d, %s, %s\n\n\n", testArtist.artist_id, testArtist.artist_name, testArtist.genre);
                else
                    printf("No artist found with id: %d\n\n\n", id);
            }
            else{
                if (testArtist.artist_id != -1){
                    TREPORT("PASS", "Artist Retrieved by ID");
                }
                else
                    TREPORT("PASS", "No Artist Found");
            }
		}
	}
	else if( !strcmp(command,"SEARCH_BY_NAME") ){
        // command              param1  param2  param3
        // SEARCH_BY_NAME       K391    -1      0
        int io_count;
        int expected_index;
        char name[30];
        if( !strcmp(param3,"0") )
            expected_status = SUCCESS;
        else
            expected_status = FAILURE;
        sprintf(name, "%s", param1);
        sscanf(param2, "%d", &expected_index);
		status = search_artist_by_name( name, &testArtist, &io_count );
		if(status == PDS_SUCCESS && io_count == expected_index)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status != expected_status ){
			// sprintf(info,"searched by name: %s; Got status %d", name, status);
            sprintf(info,"searched by name: %s; Got status %d and io_count of %d", name, status, io_count);
			TREPORT("FAIL", info);
		}
		else{
            // printing retrieved value:
            if(show_outputs == 1){
                if( io_count > 0 )
                    printf("Retrieved artist: %d, %s, %s\n\n\n", testArtist.artist_id, testArtist.artist_name, testArtist.genre);
                else
                    printf("No artist found with name: %s\n\n\n", name);
            }
            else
                TREPORT("PASS", "Search by Name worked successfully");
		}
	}
    else if( !strcmp(command,"SEARCH_INSTRUMENT") ){
        // command              param1  param2
        // SEARCH_INSTRUMENT    1       0
        if( !strcmp(param2,"0") )
            expected_status = SUCCESS;
        else
            expected_status = FAILURE;
        sscanf(param1, "%d", &id);
        status = get_musical_instrument( id, &testInstrument );
        if(status == PDS_SUCCESS)
            status = SUCCESS;
        else
            status = FAILURE;
        if( status != expected_status ){
            sprintf(info,"searched for instrument: %d; Got status %d",id, status);
            TREPORT("FAIL", info);
        }
        else{
            // printing retrieved value:
            if( expected_status == SUCCESS && show_outputs == 1)
                printf("Retrieved instrument: %d, %s\n\n\n", testInstrument.ID, testInstrument.name);
            else
                TREPORT("PASS", "Instrument Retrieved by ID");
        }
    }
	else if( !strcmp(command,"DELETE_ARTIST") ){
        // command          param1  param2
        // DELETE_ARTIST    1       0
		if( strcmp(param2,"0") == 0 )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;

		sscanf(param1, "%d", &id);
		status = delete_artist( id );
		if( status != expected_status ){
            id = -1;
			sprintf(info,"delete key: %d; Got status %d", id, status);
			TREPORT("FAIL", info);
		}
		else{
            if( expected_status == SUCCESS && show_outputs == 1)
                printf("Deleted artist: %d\n\n\n", id);
			else
                TREPORT("PASS", "Artist Deleted");
		}
	}
	else if( !strcmp(command,"LINK") ){
        // command  param1  param2  param3
        // LINK     1       0       0
		if( strcmp(param3,"0") == 0 )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;
        int parent, child;
        sscanf(param1, "%d", &parent);
        sscanf(param2, "%d", &child);
        status = link_musical_instrument( parent, child );
		if( status != expected_status ){
			sprintf(info,"repo closed, cannot link; Got status %d", status);
			TREPORT("FAIL", info);
		}
		else{
            if( expected_status == SUCCESS && show_outputs == 1)
                printf("Linked artist: %d, instrument: %d\n\n\n", parent, child);
			else
                TREPORT("PASS", "Artist and Instrument Linked");
		}
	}
	else if( !strcmp(command,"GET_INSTRUMENTS_OF") ){
        // command              param1  param2  param3
        // GET_INSTRUMENTS_OF   1       0       0
        int io_count;
        int expected_number_of_instruments;
        int instrument_ids[MAX_INSTRUMENTS_PER_ARTIST];
        if( !strcmp(param3,"0") )
            expected_status = SUCCESS;
        else
            expected_status = FAILURE;
        sscanf(param1, "%d", &id);
        sscanf(param2, "%d", &expected_number_of_instruments);
        status = get_instruments_of_artist( id, instrument_ids, &io_count );
		if(status == PDS_SUCCESS && io_count == expected_number_of_instruments)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status != expected_status){
			sprintf(info,"getting instruments of: %d; Got status %d and io_count = %d", id, status, io_count);
			TREPORT("FAIL", info);
		}
		else{
            // printing retrieved value:
            if( show_outputs == 1){
                printf("Retrieved %d instruments:\n", io_count);
                for(int i=0; i<io_count; i++){
                    //using get_musical_instrument to get the instrument details
                    status = get_musical_instrument( instrument_ids[i], &testInstrument );
                    if(status == PDS_SUCCESS)
                        printf("%d, %s\n", testInstrument.ID, testInstrument.name);
                }
                printf("\n\n");
            }
            else
                TREPORT("PASS", "Instruments Retrieval Successful");
		}
	}
	else if( !strcmp(command,"CLOSE") ){
        // command  param1
        // CLOSE    0
		if( !strcmp(param1,"0") )
			expected_status = SUCCESS;
		else
			expected_status = FAILURE;

		status = pds_close();
		if(status == PDS_SUCCESS)
			status = SUCCESS;
		else
			status = FAILURE;
		if( status == expected_status ){
            if(show_outputs == 1)
                printf("Repo closed\n\n\n");
            else
                TREPORT("PASS", "Repo Closed");
		}
		else{
			sprintf(info,"pds_close returned status %d\n",status);
			TREPORT("FAIL", info);
		}
	}
}