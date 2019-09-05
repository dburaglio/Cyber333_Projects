import os

def hello_func(user_input):
	os.system('echo "hello{}!"'.format(user_input))

if __name__ == "__main__":
	user_input = input("This function tells you hello <your name here>! What is your name? ")
	hello_func(user_input)

