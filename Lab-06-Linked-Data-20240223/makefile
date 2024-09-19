CC = gcc
files = artist_musical_instrument.c IMT2022102_pds.c  bst.c
testers = link_tester pds_tester

all: $(testers)

link_tester: $(files) linked_data_demo.c
	$(CC) $^ -o $@

pds_tester: $(files) pds_tester.c 
	$(CC) $^ -o $@

clean:
	rm -f $(testers) *.ndx *.dat