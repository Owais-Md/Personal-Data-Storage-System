#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "structures.h"

void show_menu();
void setup_data();
void process_option( int option );
void process_option1();
void process_option2();

int main()
{
    int option;
    setup_data();
    do{
        show_menu();
        scanf("%d",&option);
        process_option(option);
    } while (option != 0);
    return 0;
}

void show_menu()
{
    printf("\nLINKED DATA DEMO\n\n");
    printf("0. Exit\n");
    printf("1. Add linked data\n");
    printf("2. Get linked data\n");
    printf("\nEnter option: ");  
}

void setup_data()
{
    int status;
    status = pds_create("artist","instruments");
    status = pds_open("artist","instruments",75,50);
    for(int i=0; i<10; i++){
        struct Artist artist;
        artist.artist_id = 100+i;
        sprintf(artist.artist_name, "Artist-%d", i+1);
        sprintf(artist.genre, "Genre-%d", i+1);
        status = add_artist(&artist);
    }
    for(int i=0; i<10; i++){
        struct MusicalInstrument instrument;
        instrument.ID = 1000+i;
        sprintf(instrument.name, "Instrument-%d", i);
        status = add_musical_instrument(1000+i, &instrument);
    }
}

void process_option( int option )
{
    switch(option){
        case 0: 
            exit(1);
            break;
        case 1:
            process_option1();
            break;
        case 2:
            process_option2();
            break;
        default:
            printf("Invalid option\n");
            break;
    }
}

void process_option1()
{
    int parent_key, child_key;
    printf("Enter parent key and child key for linking: ");
    scanf("%d%d", &parent_key, &child_key);
    int status;
    status = link_musical_instrument(parent_key, child_key);
}

void process_option2()
{
    int parent_key;
    printf("Enter parent key for finding children: ");
    scanf("%d", &parent_key);
    int status;
    int instrument_ids[MAX_INSTRUMENTS_PER_ARTIST];
    int count;
    status = get_instruments_of_artist(parent_key, instrument_ids, &count);
    if(count>0){
        printf("Instruments of artist with key %d: \n", parent_key);
        struct MusicalInstrument instrument;
        for(int i=0; i<count; i++){
            status = get_musical_instrument(instrument_ids[i], &instrument);
            if(status == PDS_SUCCESS)
                printf("ID: %d, Name: %s\n", instrument.ID, instrument.name);
        }
        printf("\n");
    }
    else{
        printf("No instruments found for artist with key %d\n", parent_key);
    }
}
