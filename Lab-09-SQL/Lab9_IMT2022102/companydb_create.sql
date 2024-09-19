create table department(
    dname varchar(30) NOT NULL,
    dnumber smallint,
    mgr_ssn char(9),
    mgr_start_date date,
    constraint pk_department PRIMARY KEY (dnumber)
);

create table employee(
    Fname varchar(30) NOT NULL,
    Minit char(1),
    Lname varchar(30) NOT NULL,
    Ssn char(9),
    Bdate date,
    Address varchar(30),
    Sex char(1),
    Salary INT,
    Super_ssn char(9),
    Dno smallint,
    constraint pk_employee PRIMARY KEY (Ssn)
);

create table  dept_locations(
    Dnumber smallint,
    Dlocation varchar(30),
    constraint pk_dept_locations PRIMARY KEY (Dnumber, Dlocation)
);

create table project(
    Pname varchar(30) NOT NULL,
    Pnumber smallint,
    Plocation varchar(30),
    Dnum smallint,
    constraint pk_project PRIMARY KEY (Pnumber)
);

create table works_on(
    Essn char(9),
    Pno smallint,
    Hours decimal(4,1),
    constraint pk_works_on PRIMARY KEY (Essn, Pno)
);

create table dependent(
    Essn char(9),
    Dependent_name varchar(30),
    Sex char(1),
    Bdate date,
    Relationship varchar(30),
    constraint pk_dependent PRIMARY KEY (Essn, Dependent_name)
);


