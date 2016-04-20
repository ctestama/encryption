#include <iostream>
#include "RSA.h"
#include "BigInt.h"


using namespace std;
using namespace RSAUtil;
int main() {


    int pprimes[18] = { 30839, 30841, 30851, 30853, 30859, 30869, 30871, 30881, 30893, 30911, 30931, 30937, 30941
            , 30949, 30971, 30977, 30983, 31013 };


    string str = "1";


    //Create 10 instances of the RSA class without giving arguments, generate random message or assign messages,
    // and perform encryption through each of the 10 classes.
    for (int i=0; i < 10; i++) {

        RSA* rsa = new RSA();
        str = str + "0";
        bitset<96> mssg (str);

        BigInt* msg = new BigInt(mssg);

        BigInt encrypted = rsa->encrypt(*msg);

        BigInt decrypted = rsa->decrypt(encrypted);

        string decmsg = decrypted.toString();

        cout << "part a message was:" << decmsg << endl;

        delete rsa;
    }

    //Create 5 instances of the RSA class by passing a large prime number [p](> 30,000), and perform encryption decryption
    for (int j=0; j < 5; j++) {

        RSA* rsa = new RSA(pprimes[j]);
        str = str + "1";

        bitset<96> mssg (str);

        BigInt* msg = new BigInt(mssg);

        BigInt encrypted = rsa->encrypt(*msg);

        BigInt decrypted = rsa->decrypt(encrypted);

        string decmsg = decrypted.toString();

        cout << "part b (1 prime) message was:" << decmsg << endl;

        delete rsa;
    }

    //Create 5 instances of the RSA class by passing 2 large prime numbers [p,q] (> 30,000) and perform encryption decryption
    for (int j=0; j < 5; j++) {

        RSA* rsa = new RSA(pprimes[j], pprimes[j+1]);
        str = str + "0";

        bitset<96> mssg (str);

        BigInt* msg = new BigInt(mssg);

        BigInt encrypted = rsa->encrypt(*msg);

        BigInt decrypted = rsa->decrypt(encrypted);

        string decmsg = decrypted.toString();

        cout << "part c (2 primes) message was:" << decmsg << endl;

        delete rsa;
    }

    //Create 10 instances of the RSA class by passing 2 large non prime numbers (> 30,000) and perform encryption decryption.
    // In most of the cases the message should not get decrypted correctly.
    for (int j=0; j < 10; j++) {
        //add one to the prime number to make it non-prime
        RSA* rsa = new RSA(pprimes[j] + 1, pprimes[j+1] + 1);
        str = str + "1";

        bitset<96> mssg (str);

        BigInt* msg = new BigInt(mssg);

        BigInt encrypted = rsa->encrypt(*msg);

        BigInt decrypted = rsa->decrypt(encrypted);

        string decmsg = decrypted.toString();

        cout << "part d (nonprimes) message was:" << decmsg << endl;

        delete rsa;
    }


    RSA* RSA1 = new RSA();
    RSA* RSA2 = new RSA();
    BigInt pub1 = RSA1->getPublicKey();
    BigInt n = RSA1->getModulus();
    BigInt* randmsg = new BigInt(rand());

    cout << "\n PART 2 " << endl;

    cout << "\nrandom message before encryption: " << randmsg->toString() << endl;

    RSA2->setPublicKey(pub1);
    RSA2->setN(n);

    BigInt encr = RSA2->encrypt(*randmsg);

    BigInt decr = RSA1->decrypt(encr);

    cout << "random message after encryption: " << decr.toString() << endl;

    //PART 3

    cout << "\n PART 3" << endl;

    //a. Alice obtains the public key and Modulus N of the person (Bob) who is to sign the message
    RSA* bob = new RSA();
    RSA* alice = new RSA();

    BigInt bpubkey = bob->getPublicKey();
    BigInt bmod = bob->getModulus();

    alice->setN(bmod);
    alice->setPublicKey(bpubkey);


    //b. Obtain a random number and its inverse with respect to the Modulus [Not phi] of Bob
    BigInt* randnum = new BigInt(rand());
    BigInt inverse = randnum->operator%(bob->getModulus());

    //c. Alice obtains/generates a message to be signed.
    BigInt* rmssg = new BigInt(rand());
    cout << "\nrandom message before encryption: " << rmssg->toString() << endl;

    //d. Alice encrypts the random number with the public key.
    BigInt emessg = alice->encrypt(*rmssg);

    //e. Alice multiplies this value by the message
    BigInt newval = emessg.operator*(*rmssg);

    //f. Alice then takes a modulus over N
    BigInt newmod = newval.operator%(alice->getModulus());

    //g. Alice sends it to Bob
    //h. Bob simply decrypts the received value with the private key

    BigInt bobdecrypt = bob->decrypt(newmod);

    //i. Bob sends it back to Alice
    //j. Alice then multiplied the received value with the inverse and takes a modulus over N.

    BigInt alicemult = bobdecrypt.operator*(inverse);
    BigInt alicemod = alicemult.operator%(alice->getModulus());

    //k. The value obtained above is the signed message. To obtain the original message from it, again encrypt it with Bob’s Public Key.
    BigInt original = alice->encrypt(alicemod);
    cout << "\n Alice decrypted message:  " << original.toString() << endl;

    return 0;
}