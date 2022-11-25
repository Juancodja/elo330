import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

class visualizador{
    public static void main(String[] args)throws IOException{
        try{      
            Socket s=new Socket("localhost",47200);  
            DataOutputStream dout=new DataOutputStream(s.getOutputStream());  
            dout.writeUTF("a");  
            dout.flush();  
            dout.close();  
            s.close();  
            }catch(Exception e){System.out.println(e);}  
        }  
    }