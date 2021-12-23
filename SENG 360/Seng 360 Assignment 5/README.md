# Cipher Text Key Finder
myenc.c is used to guess the key used in the cypher. The program is given the plain text and the cipher text. It then finds the word that is used as the key to decode future text. In this example the key used was Syracuse.<br/><br/>
To run just place the given words.txt in same directory as the myenc.c file and run the following command to compile the program:<br/>
gcc -o myenc myenc.c -lcrypto<br/><br/>
Then execute the program by:<br/>
myenc