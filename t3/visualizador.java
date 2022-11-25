import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

class visualizador{
    public static void main(String[] args)throws IOException{

        String hostname = "localhost"; 
        int portnumber = 47201;

        if(args.length == 1){
            hostname = args[0];
        }
        
        else if(args.length == 2){
            hostname = args[0];
            portnumber = Integer.parseInt(args[1]);
        }

        try{      
            Socket s=new Socket(hostname,portnumber);  
            DataOutputStream dout=new DataOutputStream(s.getOutputStream());
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(s.getInputStream()));
            String str;
            
            while(true){
                Thread.sleep(100);
                dout.writeUTF("a");  
                dout.flush(); 

                str = bufferedReader.readLine();
                System.out.println(str);

                if (Thread.interrupted()) {
                    System.out.println("Besistos Besitos shau shau\n");
                    dout.close();  
                    s.close();  
                    break;
                }
            }
            }catch(Exception e){System.out.println(e);}  
        }  
    }