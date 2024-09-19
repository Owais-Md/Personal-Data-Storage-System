/*
Schema:

database name:  music
tables:
    Musical Instruments:
        instrument_id
        name
        type
        brand
    Music Artist:
        artist_id
        name
        genre
        label_id
        instrument_id
    Concerts:
        concert_id
        name
        start_date
        end_date
        location
        list of artist_id's
    Record Label:
        label_id
        name
        location
        list of artist_id's
*/

/*
Decomposed into BCNF:
    Musical Instruments:
        instrument_id (PK)
        name
        type
        brand
    Music Artist:
        artist_id (PK)
        name
        genre
        instrument_id (FK)
    Concerts:
        concert_id (PK)
        name
        start_date
        end_date
        location
    Record Label:
        label_id (PK)
        name
        location
    Concert_Artists:
        concert_id (PK, FK)
        artist_id (PK, FK)
    Label_Artists:
        label_id (PK, FK)
        artist_id (PK, FK)
*/

-- Create Musical Instruments table
CREATE TABLE MusicalInstruments (
    instrument_id varchar(30) PRIMARY KEY,
    name varchar(50),
    type varchar(30),
    brand varchar(30)
);

-- Create Music Artist table
CREATE TABLE MusicArtist (
    artist_id varchar(30) PRIMARY KEY,
    name varchar(50),
    genre varchar(30),
    instrument_id varchar(30),
    FOREIGN KEY (instrument_id) REFERENCES MusicalInstruments(instrument_id) ON UPDATE CASCADE ON DELETE NO ACTION
);

-- Create Concerts table
CREATE TABLE Concerts (
    concert_id varchar(30) PRIMARY KEY,
    name varchar(50),
    start_date DATE,
    end_date DATE,
    location varchar(30)
);

-- Create Record Label table
CREATE TABLE RecordLabel (
    label_id varchar(30) PRIMARY KEY,
    name varchar(50),
    location varchar(30)
);

-- Create Concert_Artists table (for many-to-many relationship)
CREATE TABLE Concert_Artists (
    concert_id varchar(30),
    artist_id varchar(30),
    PRIMARY KEY (concert_id, artist_id),
    FOREIGN KEY (concert_id) REFERENCES Concerts(concert_id) ON DELETE CASCADE,
    FOREIGN KEY (artist_id) REFERENCES MusicArtist(artist_id) ON DELETE CASCADE
);

-- Create Label_Artists table (for many-to-many relationship)
CREATE TABLE Label_Artists (
    label_id varchar(30),
    artist_id varchar(30),
    PRIMARY KEY (label_id, artist_id),
    FOREIGN KEY (label_id) REFERENCES RecordLabel(label_id) ON DELETE CASCADE,
    FOREIGN KEY (artist_id) REFERENCES MusicArtist(artist_id) ON DELETE CASCADE
);
