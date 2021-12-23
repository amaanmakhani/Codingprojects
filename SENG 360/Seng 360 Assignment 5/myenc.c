#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
int main() {
	// Create needed variables to store data
	char buffer[1024];
	char dictionary[256];
	int len1,len2; 
	char key[16];
	char pad[2]="#";

	// Open dictionary file to test each possible key
	FILE* filepointer;
	filepointer = fopen("words.txt", "r");

	// Load IV, plain text, and cypher text
	unsigned char* iv = (unsigned char*)"\xaa\xbb\xcc\xdd\xee\xff\x00\x99\x88\x77\x66\x55\x44\x33\x22\x11";
	unsigned char* plaintext = (unsigned char*)"This is a top secret.";
	unsigned char* cyphertext = (unsigned char*)"\x76\x4a\xa2\x6b\x55\xa4\xda\x65\x4d\xf6\xb1\x9e\x4b\xce\x00\xf4\xed\x05\xe0\x93\x46\xfb\x0e\x76\x25\x83\xcb\x7d\xa2\xac\x93\xa2";

	while(fgets(dictionary, sizeof(dictionary), filepointer)){
		// Init cypher		
		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);
		
		// Remove "/n"
		dictionary[strlen(dictionary)-1] ='\0'; 
		strncpy(key, dictionary, 50);		
		
		// Pad empty blocks of the key
		for(int i = strlen(key); i < 16; i++){
			strcat(key, pad);
		}
		key[16] = '\0'; 
		
		// Encrypt text using the guessed key
		// See doc at https://www.openssl.org/docs/man1.0.2/man3/EVP_EncryptInit.html
		EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);
        if(!EVP_EncryptUpdate(&ctx, buffer, &len1, plaintext, strlen(plaintext))) break;
        if(!EVP_EncryptFinal_ex(&ctx, buffer + len1, &len2))break;

		// See if the guessed key produces the given cyphertext if so the key has been found
		if(strncmp(buffer, cyphertext, 4) == 0){
			printf("The key used was %s\n", strtok(key, "#"));
			return 1;
		}

		// Clean up variables
		EVP_CIPHER_CTX_cleanup(&ctx);
		dictionary[0] = '\0';		
	}
	printf("Not found\n");
	return 0;
}
