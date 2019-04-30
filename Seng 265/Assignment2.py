#!/usr/bin/env python3 # For use on ELW B238 machines
# -*- coding: utf-8 -*-
#credit for name handling and outline from Assignment description from Micheal Zastre(UVIC)
import os
import sys
import fileinput

"""
Seng 265
Assignment 2
Amaan Makhani
V00883520

Assignment description:
This File contains a encoder is built to compress data for smaller transition by replacing
words with fewer codes and character. This File also contains a decoder used to reverse the change.

Assignment functions:
Encoder:
-Reads lines in from file
-Prints magic numbers
-Separates unique words and places them in a dictionary
-If unique word hasn't used print the word and the index
-Then delete it from dictionary, and add it to the stack
-If it's in the stack find the index and put it on top
-output all of this to a file with .mtf file

Decoder:
-Open MTF file
-Turn bytes into ASCII representation
-Check for Magic Numbers, if there is no magic numbers in file exit
-Find the numbers 129 and greater so I know which ones are numbers
-If new line put that in file
-If number then number put into stack and get word from that position and pop it
-If number than word create string for word
-Output all to a txt file

Scripts:
-Run all necessary tests
-Return pass or fail result

Innovative features:
In program:
	-Using default value in function definition
	-Using try and catch blocks to catch files that don't exist, or can't be opened
In the script:
	-sophisticated testing
	-color change for better indication
	-checks invalid handling to for decoding version
"""
#functions needed for encoding
#store all unique words and their index using a map
def add_unique(word, unique_words, unique):
	"""add unique words for dictionary"""
	unique_words[word]=unique

#If it has been used create a list with that word at the beginning
def add_in_stack(index, word, file, stack):
	"""add word to the stack"""
	file.write(chr(index+129))
	stack.pop(index)
	stack.insert(0, word)

#If number is 1, find word in list or data type that keeps order
def find_in(word, file, stack):
	"""find existing word in stack and add at the top"""
	index=stack.index(word)
	add_in_stack(index,word,file,stack)

#the main for the encoding
def encode_main():
	#dictionary to store unique words and unique index, create stack
	unique_words={}
	unique = 0
	stack=[]
	#catch errors in file opening
	try:
		#attempt to open file
		input = open(sys.argv[1],"r")
	except:
		print("File cannot be found or opened")
		sys.exit(1)

	#create new file name from command line argument
	argc=len(sys.argv[1])
	output=sys.argv[1]
	output=output.split('.')
	output=output[0]+".mtf"
	file = open(output,encoding="latin-1",mode="w")

	#write magic numbers
	MAGIC_NUMBER = chr(0xba) + chr(0x5e) + chr(0xba) + chr(0x11)
	for code in MAGIC_NUMBER:
		file.write(code)

	#read line by line from a file
	for line in input:

		#split the words up and add new unique ones
		sentence=line.split()
		for word in sentence:
			if word not in unique_words and word not in stack:
				add_unique(word, unique_words, unique)
				unique+=1

		#apply changes to every line by finding word in map
		for word in sentence:

			#apply changes to every word in line
			if word in unique_words and word not in stack:
				temp_print_val=unique_words[word]+129
				file.write(chr(temp_print_val))
				for char in word:
					file.write(char)
				stack.insert(0, word)
				del unique_words[word]
			else:
				find_in(word, file, stack)
		file.write("\n")
	file.close()

#functions used for decoding
#add to "stack" actually a list
def add_stack(word, file, stack, last=False):
	"""Collect the word, place in list, print word to file"""
	if last:
		file.write("%s" %word)
	else:
		file.write("%s " %word)
	stack.insert(0, word)

#get from "stack" and place at front
def get_from_stack(index, file, stack, last=False):
	"""get a word from the stack, pop, them place at the beginning, and print it"""
	if last:
		file.write("%s" %stack[index])
		temp = stack.pop(index)
		stack.insert(0, temp)
	else:
		file.write("%s " %stack[index])
		temp = stack.pop(index)
		stack.insert(0, temp)

#the main for the decoding
def decode_main():
	#catch errors in file opening
	try:
		#Open mtf for reading
		mtf_file= open(sys.argv[1], encoding="latin-1", mode="r")
	except:
		print("File cannot be found or opened")
		sys.exit(1)

	#create stack and a list of words
	stack=[]
	words=[]

	#read file in to line
	line=mtf_file.read()

	#ensure character can be converted
	try:
		#replace bytes with ASCII representation
		for char in line:
			words.append(ord(char))
	except:
		print("Characters not ASCII representable")
		sys.exit(1)

	#check if file could possibly contain magic numbers
	if len(words)<3:
		print("Invalid File")
		sys.exit(1)

	#If magic numbers at the beginning of the file continue if not exit and report a incorrect file
	magic_numbers=[186,94,186,17]
	if magic_numbers[0]!=words[0] or magic_numbers[1]!=words[1] or magic_numbers[2]!=words[2] or magic_numbers[3]!=words[3]:
		print("Invalid File")
		sys.exit(1)

	#remove magic numbers
	words=words[4:]

	#create new file name from command line argument
	argc=len(sys.argv[1])
	output=sys.argv[1]
	output=output.split('.')
	output=output[0]+".txt"
	file = open(output,"w")

	#find the numbers in line
	index=0
	numbers_at=[]
	for char in words:
		if 129<=char<=248:
			numbers_at.append(index)
		index+=1

	#make index zero, create a list to hold a word, and set last to true for future use
	index=0
	word=[]
	last=True

	#determine what to do with every char in the line
	for char in words:
		#if it can add one to index safely, i.e not the last character
		if index+1<len(words)-1:

			#if 10 print newline and continue
			if words[index]==10:
				#if word exist add it to the stack and clear
				if(len(word)!=0):
					add_stack(''.join(word), file, stack, last)
					word.clear()
					#write new line character
				file.write("\n")
				index+=1
				#continue to avoid jumping into other cases
				continue

			#your in a word and will be in the next char to
			if index not in numbers_at and index+1 not in numbers_at:
				#form word
				word.append(chr(char))

			#next one leaves the word, so add it to the stack and clear
			if index not in numbers_at and index+1 in numbers_at:
				word.append(chr(char))
				add_stack(''.join(word), file, stack)
				word.clear()

			#number followed by a number, get number from stack because word already exists
			if index in numbers_at and index+1 in numbers_at:
				get_from_stack(words[index]-129, file, stack)

			#if number then newline retrieve from stack but don't print a space
			if index in numbers_at and words[index+1]==10:
				get_from_stack(words[index]-129, file, stack, last)

		#if it's the last word in the list
		else:

			#if 10 print newline and continue
			if words[index]==10:
				#if word exist add it to the stack and clear
				if(len(word)!=0):
					add_stack(''.join(word), file, stack, last)
					word.clear()
				#write new line character
				file.write("\n")
				index+=1
				#continue to avoid jumping into other cases
				continue

			#if it is a number get it from stack and don't print a space
			if index in numbers_at:
				get_from_stack(words[index]-129, file, stack, last)

			#if it is a word add it to the stack and don't print a space
			else:
				word.append(chr(char))
				last=True
				add_stack(''.join(word), file, stack, last)
				word.clear()

		#increment index
		index+=1

	#close the files
	mtf_file.close()
	file.close()

command = os.path.basename(__file__)
if __name__ == "__main__" and command == "text2mtf.py":
	encode_main()
elif __name__ == "__main__" and command == "mtf2text.py":
	decode_main()
