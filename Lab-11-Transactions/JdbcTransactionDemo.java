//STEP 1. Import required packages
import java.sql.*;
import java.util.Scanner;
/*************
// Save this in a separate class class Employee.java
public class Employee{
	String first_name;
	String last_name;
	public Employee(String fname, String lname){first_name=fname;last_name=lname;};
	public void setFname(String fname){first_name=fname;}
	public void setLname(String lname){last_name=lname;}
}
*********************/

public class JdbcTransactionDemo {
   //STEP 2a: Set JDBC driver name and database URL
   static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
//   static final String DB_URL = "jdbc:mysql://localhost/companydb";
   static final String DB_URL = "jdbc:mysql://localhost/companydb?useSSL=false";

//Modify the following insert statement with YOUR details... use your roll number for SSN
  static final String INSERT_EMP = "INSERT INTO employee values('Owais',null, 'Mohammad', '102','2000/01/01','address','M',100000,null,dno)";

   //  Database credentials
   static final String USER = "root";
   static final String PASS = "admin";
   
   public static void main(String[] args) {
       Connection conn = null;
       Scanner sc = new Scanner(System.in);
   Statement stmt = null;
   try{
      //STEP 2b: Register JDBC driver
      Class.forName(JDBC_DRIVER);

      //STEP 3: Open a connection
      System.out.println("Connecting to database...");
      conn = DriverManager.getConnection(DB_URL,USER,PASS);
      
      // Set auto commit as false.
      conn.setAutoCommit(false);

      //STEP 4: Insert new employee
      System.out.println("Inserting one row....");
      stmt = conn.createStatement();
      stmt.executeUpdate(INSERT_EMP);


      //STEP 4: Execute a query
      System.out.println("Creating statement...");
      String sql;
      sql = "SELECT fname, lname from employee";
      ResultSet rs = stmt.executeQuery(sql);

      //STEP 5: Extract data from result set
      while(rs.next()){
         //Retrieve by column name
         String fname  = rs.getString("fname");
         String lname = rs.getString("lname");

         //Display values
         System.out.print("fname: " + fname);
         System.out.println(", lname: " + lname);
		 
		 // Create Employee object if you have saved the Employee class in a separate java file
		 // Employee e = new Employee(fname,lname);
		 // Save or pass around e object as needed
      }

      // taking input ssn
        System.out.println("Enter the super_ssn of the employee added initially: ");
        String x = sc.nextLine();
      // making update query
        String updateQuery = "UPDATE employee SET super_ssn = '"+x+"' WHERE ssn = '102'";
      //executing the query, and then committing and showing employee details if the update was successful, and rolling back if the update was not successful
        try{
            stmt.executeUpdate(updateQuery);
            conn.commit();
            System.out.println("Employee details after update: ");
            sql = "SELECT * from employee";
            rs = stmt.executeQuery(sql);
            while(rs.next()){
                String fname  = rs.getString("fname");
                String lname = rs.getString("lname");
                String ssn = rs.getString("ssn");
                String super_ssn = rs.getString("super_ssn");
                System.out.println("First Name: " + fname + ", Last Name: " + lname + ", ssn: " + ssn + ", super_ssn: " + super_ssn);
            }
        }catch(SQLException sl){
            conn.rollback();
            System.out.println("Update failed");
            sl.printStackTrace();
            System.out.println("Rolling back data here....");
        }finally{
        }
        
      //STEP 6: Clean-up environment
      sc.close();
      rs.close();
      stmt.close();
      conn.close();
   }catch(SQLException se){
      //Handle errors for JDBC
      se.printStackTrace();
      // If there is an error then rollback the changes.
      System.out.println("Rolling back data here....");
      try{
         if(conn!=null)
             conn.rollback();
      }catch(SQLException se2){
	      System.out.println("Rollback failed....");
              se2.printStackTrace();
      }
   }catch(Exception e){
      //Handle errors for Class.forName
      e.printStackTrace();
   }finally{
      //finally block used to close resources
      try{
         if(stmt!=null)
            stmt.close();
      }catch(SQLException se2){
      }// nothing we can do
      try{
         if(conn!=null)
            conn.close();
      }catch(SQLException se){
         se.printStackTrace();
      }//end finally try
   }//end try
   System.out.println("Goodbye!");
}//end main
}//end FirstExample
