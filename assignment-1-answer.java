package homeworkx;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

public class Mywork extends IOException {

	public static String executeCmd(String command) throws IOException {  
	    System.out.println("Execute command : " + command);  
	    Runtime runtime = Runtime.getRuntime();  
	    Process process = runtime.exec("cmd /c " + command);  
	    BufferedReader br = new BufferedReader(new InputStreamReader(process.getInputStream(), "UTF-8"));  
	    String line = null;  
	    StringBuilder build = new StringBuilder();  
	    
	    if ((line = br.readLine()) == null) {  
	        
	        System.out.println("the process ends");
	    } 
	    return build.toString();  
	}  
	  
	
	public static void main(String[] args) {
		Scanner s=new Scanner(System.in);
		String str=null;
		System.out.println("enter the absolute address");
		str=s.next();
		 try {
			executeCmd( str);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	} 

}
