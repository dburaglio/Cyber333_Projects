int main(void) {
	char* secret = *REDACTED*;
	char* input = calloc(256, sizeof(char));	//size is 256 with null terminator,
	char* guess = calloc(256, sizeof(char));	//size is 256 with null terminator,
	short guessSize = 256;

	printf("Please enter your password, between 5 and 10 characters: \n");
	fflush(stdin);
	fflush(stdout);

	fgets(input, guessSize-1, stdin);

	if (1 != sscanf(input, "%s", guess)) {
		printf("Bad input.\n");
		return 0;
	}

	int secret_len = strlen(secret);
	int match = 0;

	if (strlen(guess) == secret_len) {
		match = 1;
		for (int i = 0; i < secret_len; i++) {
			if (guess[i] != secret[i]) {
				match = 0;
				break;
			}
		}
	}

	if (match == 1)	{
		printf("Correct!\n");
	} else {
		printf("Incorrect, goodbye.\n");
	}

	return 0;
}

