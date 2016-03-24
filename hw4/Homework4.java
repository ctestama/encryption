import java.security.cert.*;
import java.text.*;
import java.util.*;
import java.io.*;
import java.security.KeyStore;
import javax.crypto.SecretKey;
import javax.security.auth.callback.*;
import java.security.PrivateKey;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import org.apache.commons.codec.binary.Base64;




public class Homework4 {

	//main method makes use of methods defined below
	public static void main(String[] args) {

		String filename = "Raghupub.cer";

        try {
            
        	//read the certificate
			FileInputStream fis = new FileInputStream(filename);
	 		BufferedInputStream bis = new BufferedInputStream(fis);

		    CertificateFactory cf = CertificateFactory.getInstance("X.509");

 			while (bis.available() > 0) {
    			Certificate cert = cf.generateCertificate(bis);
    			System.out.println(cert.toString());
 			}

 			//Read the private key

 			String path = "Raghupri.pfx";
		    char[] pass = "raghu".toCharArray();

		    KeyStore ks = KeyStore.getInstance("PKCS12");
		    ks.load(new FileInputStream(path), pass);
		    String alias = (String) ks.aliases().nextElement(); 
		    PrivateKey pk = (PrivateKey) ks.getKey(alias, pass);/* returns null */
		    //Certificate[] chain = ks.getCertificateChain(alias);/* returns null */
		    //X509Certificate last = (X509Certificate) chain[chain.length - 1];


		    //Key key = ks.getKey(alias, "raghu".toCharArray())
        	byte[] b64 = Base64.encodeBase64(pk.getEncoded());
        	System.out.println("-----BEGIN PRIVATE KEY-----");
        	System.out.println( new String(b64 ));
        	System.out.println("-----END PRIVATE KEY-----");
		    //System.out.println(last.getNotBefore());
		    //System.out.println(last.toString());

            // Always close files.
            bis.close();         
        }
        catch(FileNotFoundException ex) {
            System.out.println(
                "Unable to open file '" + 
                filename + "'");                
        }
        catch(IOException ex) {
            System.out.println(
                "Error reading file '" 
                + filename + "'");                  
            // Or we could just do this: 
            // ex.printStackTrace();
        }
        catch(CertificateException ex) {
        	System.out.println("Certificate Exception");
        }
        catch(KeyStoreException ex) {
        	System.out.println("Keystore Exception");
        }
        catch(NoSuchAlgorithmException ex) {
        	System.out.println("NoSuchAlgorithmException");
        }
        catch(UnrecoverableKeyException ex) {
        	System.out.println("UnrecoverableKeyException");
        }
	}
}