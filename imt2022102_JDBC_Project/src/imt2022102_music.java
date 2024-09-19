import java.util.*;
import java.sql.*;

// defining a class with functions for CRUD operations
class CRUD{
    Scanner sc = new Scanner(System.in);
    String temp = null;
    int affected_rows =0;
    ResultSet rs = null;
    public String INSERT(Connection conn, Statement stmt, String tablename) throws SQLException{
        String query = "INSERT INTO "+tablename+" VALUES(";
        if(tablename.equals("MusicalInstruments")){
            System.out.println("Enter instrument_id: ");
            query += "'" + sc.next()+"', ";
            System.out.println("Enter name: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter type: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter brand: ");
            query += "'"+sc.next()+"')";
        }
        else if(tablename.equals("MusicArtist")){
            System.out.println("Enter artist_id: ");
            temp = sc.next();
            query += "'"+temp+"', ";
            System.out.println("Enter name: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter genre: ");
            query += "'"+sc.next()+"', null)";
        }
        else if(tablename.equals("Concerts")){
            System.out.println("Enter concert_id: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter name: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter start_date: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter end_date: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter location: ");
            query += "'"+sc.next()+"')";
        }            
        else if(tablename.equals("RecordLabel")){
            System.out.println("Enter label_id: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter name: ");
            query += "'"+sc.next()+"', ";
            System.out.println("Enter location: ");
            query += "'"+sc.next()+"')";
        }
        else{
            System.out.println("Invalid table name");
        }
        stmt.executeUpdate(query);
        return temp;
    }
    public void UPDATE(Connection conn, Statement stmt, String tablename) throws SQLException{
        //if table exists, show table attributes, else return
        rs = stmt.executeQuery("SELECT * FROM "+tablename);
        ResultSetMetaData rsmd = rs.getMetaData();
        int columnsNumber = rsmd.getColumnCount();
        boolean column_exists = false;
        for(int i=1; i<=columnsNumber; i++){
            System.out.print(rsmd.getColumnName(i)+" ");
        }
        System.out.println();
        String query = "UPDATE "+tablename+" SET ";
        System.out.println("Enter the column name to update: ");
        String column = sc.next();
        for(int i=1; i<=columnsNumber; i++){
            if(rsmd.getColumnName(i).equals(column)){
                column_exists = true;
                break;
            }
        }
        if(column_exists == false){
            System.out.println("Invalid column name" + column);
            return;
        }
        System.out.println("Enter the new value: ");
        String value = sc.next();
        query += column+" = "+value+" WHERE "+column+" = ";
        System.out.println("Enter the value of the column to update: ");
        String value_to_update = sc.next();
        query += value_to_update;
        affected_rows = stmt.executeUpdate(query);
        System.out.println(affected_rows + " rows were affected");
        System.out.println("Do you want to roll back? (y/n)");
        temp = sc.next();
        if(temp.equals("yes") || temp.equals("y")){
            conn.rollback();
            System.out.println("Rollback successful");
        }
        else System.out.println("Updated successfully");
    }
    public void DELETE(Connection conn, Statement stmt, String tablename) throws SQLException{
        //if table exists, show table attributes, else return
        rs = stmt.executeQuery("SELECT * FROM "+tablename);
        ResultSetMetaData rsmd = rs.getMetaData();
        int columnsNumber = rsmd.getColumnCount();
        boolean column_exists = false;
        for(int i=1; i<=columnsNumber; i++){
            System.out.print(rsmd.getColumnName(i)+" ");
        }
        System.out.println();
        String query = "DELETE FROM "+tablename+" WHERE ";
        System.out.println("Enter the column name to delete by using: ");
        String column = sc.next();
        for(int i=1; i<=columnsNumber; i++){
            if(rsmd.getColumnName(i).equals(column)){
                column_exists = true;
                break;
            }
        }
        if(column_exists == false){
            System.out.println("Invalid column name" + column);
            return;
        }
        System.out.println("Enter the value of the column to delete from: ");
        String value = sc.next();
        query += column+" = "+ "'" + value + "'";
        System.err.println(query);
        affected_rows = stmt.executeUpdate(query);
        System.out.println(affected_rows + " rows were affected");
        System.out.println("Do you want to roll back? (y/n)");
        temp = sc.next();
        if(temp.equals("yes") || temp.equals("y")){
            conn.rollback();
            System.out.println("Rollback successful");
        }
        else
        System.out.println("Deleted successfully");
    }
    public void DISPLAY(Connection conn, Statement stmt, String tablename) throws SQLException{
        rs = stmt.executeQuery("SELECT * FROM "+tablename);
        ResultSetMetaData rsmd = rs.getMetaData();
        int columnsNumber = rsmd.getColumnCount();
        for(int i=1; i<=columnsNumber; i++){
            System.out.print(rsmd.getColumnName(i)+"\t");
        }
        System.out.println();
        while(rs.next()){
            for(int i=1; i<=columnsNumber; i++){
                System.out.print(rs.getString(i)+"\t");
            }
            System.out.println();
        }
    }
}

public class imt2022102_music {
    // defining a menu class for CRUD operations on the tables
    static class Menu{
        Scanner sc = new Scanner(System.in);
        int choice;
        //constructor for menu
        public Menu(){
            choice = 0;
        }
        public int show_tables(){
            System.out.println("which table to you want to access?");
            System.out.println("1. Musical Instruments");
            System.out.println("2. Music Artist");
            System.out.println("3. Concerts");
            System.out.println("4. Record Label");
            System.out.println("5. Commit all changes");
            System.out.println("6. Rollback to previous commit");
            System.out.println("7. Exit");
            System.out.println("Enter your choice: ");
            choice = -1;
            while(choice<0){
                try{
                    choice = sc.nextInt();
                    sc.nextLine();
                }catch(InputMismatchException e){
                    System.out.println("Invalid input type");
                    sc.next();
                }
            }
            return choice;
        }
        // options under each table
        public int MusicalInstruments(){
            System.out.println("1. Insert a new musical instrument");
            System.out.println("2. Update existing musical instruments");
            System.out.println("3. Delete a particular musical instrument");
            System.out.println("4. Display all musical instruments");
            System.out.println("5. Exit");
            System.out.println("Enter your choice: ");
            choice = -1;
            while(choice<0){
                try{
                    choice = sc.nextInt();
                    sc.nextLine();
                }catch(InputMismatchException e){
                    System.out.println("Invalid input type");
                    sc.next();
                }
            }
            return choice;
        }
        public int MusicArtist(){
            System.out.println("1. Insert a new artist");
            System.out.println("2. Update existing artist");
            System.out.println("3. Delete an artist");
            System.out.println("4. Display artist details");
            System.out.println("5. Registration with concert");
            System.out.println("6. Registration with label");
            System.out.println("7. Display upcoming Concerts of the artist");
            System.out.println("8. Display associated Record labels");
            System.out.println("9. Exit");
            System.out.println("Enter your choice: ");
            choice = -1;
            while(choice<0){
                try{
                    choice = sc.nextInt();
                    sc.nextLine();
                }catch(InputMismatchException e){
                    System.out.println("Invalid input type");
                    sc.next();
                }
            }
            return choice;
        }
        public int Concerts(){
            System.out.println("1. Insert a new concert");
            System.out.println("2. Update existing concert");
            System.out.println("3. Delete a particular concert");
            System.out.println("4. Display all registered concerts");
            System.out.println("5. Display all upcoming Concerts");
            System.out.println("6. Exit");
            System.out.println("Enter your choice: ");
            choice = -1;
            while(choice<0){
                try{
                    choice = sc.nextInt();
                    sc.nextLine();
                }catch(InputMismatchException e){
                    System.out.println("Invalid input type");
                    sc.next();
                }
            }
            return choice;
        }
        public int RecordLabel(){
            System.out.println("1. Insert a new Record Label");
            System.out.println("2. Update existing Record labels");
            System.out.println("3. Delete a particular Record label");
            System.out.println("4. Display all Record Labels");
            System.out.println("5. Display all artists currently associated with a particular Record Label");
            System.out.println("6. Exit");
            System.out.println("Enter your choice: ");
            choice = -1;
            while(choice<0){
                try{
                    choice = sc.nextInt();
                    sc.nextLine();
                }catch(InputMismatchException e){
                    System.out.println("Invalid input type");
                    sc.next();
                }
            }
            return choice;
        }

        public void show_menu(Connection conn, Statement stmt) throws SQLException{
            int choice1 = 0, choice2;
            String temp, temp2, returnedString, query;
            CRUD crud = new CRUD();
            while(choice1!=7){
                choice1 = show_tables();
                if(choice1 == 1){
                    do{
                        choice2 = MusicalInstruments();
                        if(choice2 == 1){
                            crud.INSERT(conn, stmt, "MusicalInstruments");
                        }
                        else if(choice2 == 2){
                            crud.UPDATE(conn, stmt, "MusicalInstruments");
                        }
                        else if(choice2 == 3){
                            crud.DELETE(conn, stmt, "MusicalInstruments");
                        }
                        else if(choice2 == 4){
                            crud.DISPLAY(conn, stmt, "MusicalInstruments");
                        }
                        else if(choice2 == 5){}
                        else{
                            System.out.println("Invalid choice");
                        }
                        break;
                    }while(choice2!=5);
                }
                else if(choice1 == 2){
                    do{
                        choice2 = MusicArtist();
                        if(choice2 == 1){
                            returnedString = crud.INSERT(conn, stmt, "MusicArtist");
                            System.out.println("Do you want to enter instrument id of "+ returnedString +"? (y/n)");
                            temp = sc.next();
                            if(temp.equals("yes") || temp.equals("y")){
                                System.out.println("Enter instrument_id: ");
                                temp = sc.next();
                                // checking if given instrument id exists or not
                                query = "Select Count(*) from MusicalInstruments Where instrument_id = " + "'" + temp + "'";
                                ResultSet rs = stmt.executeQuery(query);
                                rs.next();
                                if(rs.getInt(1) == 0){
                                    System.out.println("Instrument id does not exist");
                                    throw new SQLException("Instrument doesn't exist.");
                                }
                                query = "UPDATE MusicArtist SET instrument_id = '"+temp+"' WHERE artist_id = " + "'"+ returnedString+"'";
                            }
                        }
                        else if(choice2 == 2){
                            crud.UPDATE(conn, stmt, "MusicArtist");
                        }
                        else if(choice2 == 3){
                            crud.DELETE(conn, stmt, "MusicArtist");
                        }
                        else if(choice2 == 4){
                            crud.DISPLAY(conn, stmt, "MusicArtist");
                        }

                        else if(choice2 == 5 || choice2 == 6){
                            // registration with concert
                            // should be given choice to add, update or remove registration
                            System.out.println("Enter artist_id: ");
                            temp = sc.next();
                            // checking if given artist id exists or not
                            query = "Select Count(*) from MusicArtist Where artist_id = " + "'" + temp + "'";
                            ResultSet rs = stmt.executeQuery(query);
                            rs.next();
                            if(rs.getInt(1) == 0){
                                System.out.println("Artist id does not exist");
                                throw new SQLException("Artist doesn't exist.");
                            }
                            if(choice2 == 5){
                                System.out.println("Do you want to add, update or remove registration with "+temp+"? (add/update/remove)");
                                String choice = sc.next();
                                if(choice.equals("add")){
                                    // add registration
                                    System.out.println("Enter Concert id: ");
                                    temp2 = sc.next();
                                    // checking if given concert id exists or not
                                    query = "Select Count(*) from Concerts Where concert_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Concert id does not exist");
                                        throw new SQLException("Concert doesn't exist.");
                                    }
                                    query = "INSERT INTO Concert_Artists VALUES(" + "'" + temp2 + "', '" + temp + "')";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration added successfully");
                                }
                                else if(choice.equals("update")){
                                    // update registration
                                    System.out.println("Enter Concert id: ");
                                    temp2 = sc.next();
                                    // checking if given concert id exists or not
                                    query = "Select Count(*) from Concerts Where concert_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Concert id does not exist");
                                        throw new SQLException("Concert doesn't exist.");
                                    }
                                    query = "UPDATE Concert_Artists SET concert_id = " + "'" + temp2 + "' WHERE artist_id = " + "'" + temp + "'";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration updated successfully");
                                }
                                else if(choice.equals("remove")){
                                    // remove registration
                                    System.out.println("Enter Concert id: ");
                                    temp2 = sc.next();
                                    // checking if given concert id exists or not
                                    query = "Select Count(*) from Concerts Where concert_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Concert id does not exist");
                                        throw new SQLException("Concert doesn't exist.");
                                    }
                                    query = "DELETE FROM Concert_Artists WHERE concert_id = " + "'" + temp2 + "' AND artist_id = " + "'" + temp + "'";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration removed successfully");
                                }
                            }

                            else if(choice2 == 6){
                                // registration with label
                                // should be given choice to add, update or remove registration
                                System.out.println("Do you want to add, update or remove registration with "+temp+"? (add/update/remove)");
                                String choice = sc.next();
                                if(choice.equals("add")){
                                    // add registration
                                    System.out.println("Enter Label id: ");
                                    temp2 = sc.next();
                                    // checking if given label id exists or not
                                    query = "Select Count(*) from RecordLabel Where label_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Label id does not exist");
                                        throw new SQLException("Label doesn't exist.");
                                    }
                                    query = "INSERT INTO Label_Artists VALUES(" + "'" + temp2 + "', '" + temp + "')";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration added successfully");
                                }
                                else if(choice.equals("update")){
                                    // update registration
                                    System.out.println("Enter Label id: ");
                                    temp2 = sc.next();
                                    // checking if given label id exists or not
                                    query = "Select Count(*) from RecordLabel Where label_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Label id does not exist");
                                        throw new SQLException("Label doesn't exist.");
                                    }
                                    query = "UPDATE Label_Artists SET label_id = " + "'" + temp2 + "' WHERE artist_id = " + "'" + temp + "'";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration updated successfully");
                                }
                                else if(choice.equals("remove")){
                                    // remove registration
                                    System.out.println("Enter Label id: ");
                                    temp2 = sc.next();
                                    // checking if given label id exists or not
                                    query = "Select Count(*) from RecordLabel Where label_id = " + "'" + temp2 + "'";
                                    rs = stmt.executeQuery(query);
                                    rs.next();
                                    if(rs.getInt(1) == 0){
                                        System.out.println("Label id does not exist");
                                        throw new SQLException("Label doesn't exist.");
                                    }
                                    query = "DELETE FROM Label_Artists WHERE label_id = " + "'" + temp2 + "' AND artist_id = " + "'" + temp + "'";
                                    stmt.executeUpdate(query);
                                    System.out.println("Registration removed successfully");
                                }
                            }
                        }

                        else if(choice2 == 7){
                            // display upcoming concerts of artist
                            System.out.println("Enter artist_id: ");
                            temp = sc.next();
                            // checking if given artist id exists or not
                            query = "Select Count(*) from MusicArtist Where artist_id = " + "'" + temp + "'";
                            ResultSet rs = stmt.executeQuery(query);
                            rs.next();
                            if(rs.getInt(1) == 0){
                                System.out.println("Artist id does not exist");
                                throw new SQLException("Artist doesn't exist.");
                            }
                            // the concerts can be older concerts too
                            // need to compare with date also
                            
                            query = "SELECT ca.*, c.* FROM Concert_Artists ca join Concerts c on ca.concert_id = c.concert_id WHERE ca.artist_id = " + "'" + temp + "' AND c.start_date >= CURDATE()" ;
                            rs = stmt.executeQuery(query);
                            ResultSetMetaData rsmd = rs.getMetaData();
                            int columnsNumber = rsmd.getColumnCount();
                            for(int i=1; i<=columnsNumber; i++){
                                System.out.print(rsmd.getColumnName(i)+"\t");
                            }
                            System.out.println();
                            while(rs.next()){
                                for(int i=1; i<=columnsNumber; i++){
                                    System.out.print(rs.getString(i)+"\t");
                                }
                                System.out.println();
                            }
                        }
                        else if(choice2 == 8){
                            // Display associated Record Labels
                            System.out.println("Enter artist_id: ");
                            temp = sc.next();
                            // checking if given artist id exists or not
                            query = "Select Count(*) from MusicArtist Where artist_id = " + "'" + temp + "'";
                            ResultSet rs = stmt.executeQuery(query);
                            rs.next();
                            if(rs.getInt(1) == 0){
                                System.out.println("Artist id does not exist");
                                throw new SQLException("Artist doesn't exist.");
                            }
                            //inner join instead of this
                            // query = "SELECT * FROM Label_Artists WHERE artist_id = " + "'" + temp + "'";
                            query = "SELECT la.*, l.* from Label_Artists la join RecordLabel l on la.label_id = l.label_id Where la.artist_id = '" + temp + "'";
                            rs = stmt.executeQuery(query);
                            ResultSetMetaData rsmd = rs.getMetaData();
                            int columnsNumber = rsmd.getColumnCount();
                            for(int i=1; i<=columnsNumber; i++){
                                System.out.print(rsmd.getColumnName(i)+"\t");
                            }
                            System.out.println();
                            while(rs.next()){
                                for(int i=1; i<=columnsNumber; i++){
                                    System.out.print(rs.getString(i)+"\t");
                                }
                                System.out.println();
                            }
                        }
                        else if(choice2 == 9){}
                        else{
                            System.out.println("Invalid choice");
                        }
                        break;
                    }while(choice2!=9);
                }
                else if(choice1 == 3){
                    do{
                        choice2 = Concerts();
                        if(choice2 == 1){
                            crud.INSERT(conn, stmt, "Concerts");
                        }
                        else if(choice2 == 2){
                            crud.UPDATE(conn, stmt, "Concerts");
                        }
                        else if(choice2 == 3){
                            crud.DELETE(conn, stmt, "Concerts");
                        }
                        else if(choice2 == 4){
                            crud.DISPLAY(conn, stmt, "Concerts");
                        }
                        else if(choice2 == 5){
                            //display all upcoming concerts
                            query = "SELECT * FROM Concerts WHERE start_date >= CURDATE()";
                            ResultSet rs = stmt.executeQuery(query);
                            ResultSetMetaData rsmd = rs.getMetaData();
                            int columnsNumber = rsmd.getColumnCount();
                            for(int i=1; i<=columnsNumber; i++){
                                System.out.print(rsmd.getColumnName(i)+"\t");
                            }
                            System.out.println();
                            while(rs.next()){
                                for(int i=1; i<=columnsNumber; i++){
                                    System.out.print(rs.getString(i)+"\t");
                                }
                                System.out.println();
                            }
                        }
                        else if(choice2 == 6){}
                        else{
                            System.out.println("Invalid choice");
                        }
                        break;
                    }while(choice2!=6);
                }
                else if(choice1 == 4){
                    do{
                        choice2 = RecordLabel();
                        if(choice2 == 1){
                            crud.INSERT(conn, stmt, "RecordLabel");
                        }
                        else if(choice2 == 2){
                            crud.UPDATE(conn, stmt, "RecordLabel");
                        }
                        else if(choice2 == 3){
                            crud.DELETE(conn, stmt, "RecordLabel");
                        }
                        else if(choice2 == 4){
                            crud.DISPLAY(conn, stmt, "RecordLabel");
                        }
                        else if(choice2 == 5){
                            //display all artists associated with a particular label
                            System.out.println("Enter label_id: ");
                            temp = sc.next();
                            // checking if given label id exists or not
                            query = "Select Count(*) from RecordLabel Where label_id = " + "'" + temp + "'";
                            ResultSet rs = stmt.executeQuery(query);
                            rs.next();
                            if(rs.getInt(1) == 0){
                                System.out.println("Label id does not exist");
                                throw new SQLException("Label doesn't exist.");
                            }
                            //join on Label_artists and artists
                            // query = "SELECT * FROM Label_Artists WHERE label_id = " + "'" + temp + "'";
                            query = "SELECT la.label_id, a.* from Label_Artists la inner join MusicArtist a on la.artist_id = a.artist_id Where la.label_id = '" + temp + "'";
                            rs = stmt.executeQuery(query);
                            ResultSetMetaData rsmd = rs.getMetaData();
                            int columnsNumber = rsmd.getColumnCount();
                            for(int i=1; i<=columnsNumber; i++){
                                System.out.print(rsmd.getColumnName(i)+"\t");
                            }
                            System.out.println();
                            while(rs.next()){
                                for(int i=1; i<=columnsNumber; i++){
                                    System.out.print(rs.getString(i)+"\t");
                                }
                                System.out.println();
                            }
                        }
                        else if(choice2 == 6){}
                        else{
                            System.out.println("Invalid choice");
                        }
                        break;
                    }while(choice2!=6);
                }
                else if(choice1 == 5){
                    conn.commit();
                    System.out.println("Changes committed successfully");
                }
                else if(choice1 == 6){
                    conn.rollback();
                    System.out.println("Rollback successful");
                }
                else if(choice1 == 7){
                    System.out.println("Exiting...");
                }
                else{
                    System.out.println("Invalid choice");
                }
            };
        }
    }
    Scanner sc = new Scanner(System.in);
    
    static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
    static final String DB_URL = "jdbc:mysql://localhost/music?useSSL=false";
    
    static final String USER = "root";
    static final String PASS = "admin";
    static Menu menu = new Menu();
    public static void main(String[] args) {
    Connection conn = null;
    Statement stmt = null;
        try{
            Class.forName(JDBC_DRIVER);

            System.out.println("Connecting to database...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);
            stmt = conn.createStatement();
            conn.setAutoCommit(false);
            menu.show_menu(conn, stmt);
        }catch(SQLException se){
            se.printStackTrace();
            System.out.println("Rolling back data here....");
            try{
                if(conn!=null) conn.rollback();
            }catch(SQLException se2){
                System.out.println("Rollback failed....");
                se2.printStackTrace();
            }
        }catch(Exception e){
            System.out.println("Exception occured....");
            e.printStackTrace();
        }finally{
            try{
            if(stmt!=null)
                stmt.close();
            }catch(SQLException se2){
            }
            try{
            if(conn!=null)
                conn.close();
            }catch(SQLException se){
                se.printStackTrace();
            }
        }            
    }
}
