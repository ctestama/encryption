import java.security.cert.*;
import java.text.*;
import java.util.*;
import java.io.*;
import java.security.KeyStore;
import javax.crypto.SecretKey;
import javax.security.auth.callback.*;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import org.apache.commons.codec.binary.Base64;
import java.security.InvalidKeyException;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import java.security.KeyPairGenerator;
import java.security.KeyPair;
import javax.crypto.SealedObject;

/**
*Colton Testamarck, and Haoran Lyu
*CSE 539 Homework 4
*/
public class Homework4 {

	//main method makes use of methods defined below
	public static void main(String[] args) {

		String filename = "Raghupub.cer";
        String tcert = "Trustcenter.cer";

        try {
            
        	//read Raghu's certificate
			FileInputStream fis = new FileInputStream(filename);
	 		BufferedInputStream bis = new BufferedInputStream(fis);

		    CertificateFactory cf = CertificateFactory.getInstance("X.509");

 			//while (bis.available() > 0) {
			Certificate cert = cf.generateCertificate(bis);
			System.out.println(cert.toString());

			PublicKey pubk = cert.getPublicKey();
			byte[] pub64 = Base64.encodeBase64(pubk.getEncoded());
			System.out.println("-----BEGIN RAGHU'S PUBLIC KEY-----");
        	System.out.println( new String(pub64));
        	System.out.println("-----END RAGHU'S PUBLIC KEY-----\n");

        	//convert to X509 certificate in order to get signature
        	X509Certificate t = (X509Certificate) cert;
        	byte[] sig = t.getSignature();
        	byte[] sig64 = Base64.encodeBase64(sig);
        	

 			//Read and print the private key
 			String path = "Raghupri.pfx";
		    char[] pass = "raghu".toCharArray();

		    KeyStore ks = KeyStore.getInstance("PKCS12");
		    ks.load(new FileInputStream(path), pass);
		    String alias = (String) ks.aliases().nextElement(); 
		    PrivateKey pk = (PrivateKey) ks.getKey(alias, pass);
		    
        	byte[] b64 = Base64.encodeBase64(pk.getEncoded());
        	System.out.println("-----BEGIN RAGHU'S PRIVATE KEY-----");
        	System.out.println( new String(b64 ));
        	System.out.println("-----END RAGHU'S PRIVATE KEY-----\n");


            //read the Trustcenter Cert
            FileInputStream tfis = new FileInputStream(tcert);
            BufferedInputStream tbis = new BufferedInputStream(tfis);

            CertificateFactory tcf = CertificateFactory.getInstance("X.509");

            //while (bis.available() > 0) {
            Certificate trustcert = tcf.generateCertificate(tbis);
            X509Certificate tcer = (X509Certificate) trustcert;

            //System.out.println(trustcert.toString());

            PublicKey tpubk = tcer.getPublicKey();
            byte[] tpub64 = Base64.encodeBase64(tpubk.getEncoded());
            System.out.println("-----BEGIN TRUSTCENTER CERT PUBLIC KEY-----");
            System.out.println( new String(tpub64));
            System.out.println("-----END TRUSTCENTER CERT PUBLIC KEY-----\n");


        	//Print the signature from Raghu's certificate
        	System.out.println("-----RAGHU'S CERT SIGNATURE-----");
    		System.out.println(new String(sig64));
    		System.out.println("-----END RAGHU CERT SIGNATURE-----\n");
		    

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


    
        //Encrypt and decrypt string using RSA
        try{
            //create the RSA keypair
            KeyPairGenerator keygenerator = KeyPairGenerator.getInstance("RSA");
            KeyPair rsaPair = keygenerator.generateKeyPair();
            
            // Create the cipher 
            Cipher rsaCipher = Cipher.getInstance("RSA");
            
            // Initialize the cipher for encryption
            rsaCipher.init(Cipher.ENCRYPT_MODE, rsaPair.getPublic());

            //sensitive information
            // Create a secret message
            String myMessage = new String("“Our names are Colton Testamarck and Haoran Lyu. We are enrolled in CSE 539.");
                        // Encrypt that message using a new SealedObject and the Cipher we created before
            SealedObject myEncryptedMessage= new SealedObject( myMessage, rsaCipher);

            // Get an instance of the Cipher for RSA encryption/decryption
            Cipher dec = Cipher.getInstance("RSA");
            // Initiate the Cipher, telling it that it is going to Decrypt, giving it the private key
            dec.init(Cipher.DECRYPT_MODE, rsaPair.getPrivate());


            // Tell the SealedObject we created before to decrypt the data and return it
            String message = (String) myEncryptedMessage.getObject(dec);
            System.out.println("Decrypted Message: " +  message);
            
        }catch(NoSuchAlgorithmException e){
            e.printStackTrace();
        }catch(NoSuchPaddingException e){
            e.printStackTrace();
        }catch(InvalidKeyException e){
            e.printStackTrace();
        }catch(IllegalBlockSizeException e){
            e.printStackTrace();
        }catch(BadPaddingException e){
            e.printStackTrace();
        }  catch(IOException ex) {
            ex.printStackTrace();
        }   catch(ClassNotFoundException ex) {
            ex.printStackTrace();
        }

	}
}