#!/usr/bin/python3
import os
import sys
import fileinput
import re
"""
Seng 265 Assignment 4
Amaan Makhani
V00883520

Assignment description:
This File contains an advanced encoder and decoder.The biggest difference
between coding.py and coding2.py is the use of classes.

New innovative assignment functions:
Encoder:
-NEW: Using Classes to encapsulate methods that encode each line, due to this encode function smaller, more 
reusable code(this also eliminated me having to pass in file pointers, and other data every time)

Decoder:
-NEW: Use list comprehension to create string for regex
-NEW: Use regular expressions instead of if statements to check if file contains magic numbers
-Using try and except blocks to catch files that don't exist, or can't be opened
-NEW LOGIC: Create object of class to hold decoding cases and apply function  
-NEW: Logic of decoding was also changed to produce cleaner decoding strategy
"""

MAGIC_NUMBER_1 = chr(0xBA) + chr(0x5E) + chr(0xBA) + chr(0x11)
MAGIC_NUMBER_2 = chr(0xBA) + chr(0x5E) + chr(0xBA) + chr(0x12)
class sentenceEncoding:
	"""this class contains members to deal with encoding of every line"""
	#global class variables needed to store data
	unique_words={}
	stack=[]
	unique=0
	
	def __init__(self, line, file):
		"""initialize class to contain data given when creating instance"""
		self.file=file
		self.words=[]
		self.words=line.split()
		#check if word has been seen
		for word in self.words:
			if word not in self.__class__.unique_words and word not in self.__class__.stack:
				#add all new words in global dictionary and counter
				self.add_unique(word)
				self.__class__.unique+=1
				
	def add_unique(self,word):
		"""add unique words for dictionary"""
		self.__class__.unique_words[word]=self.__class__.unique
		
	def add_in_stack(self, index, word):
		"""add word to the stack"""
		self.print_val(index)
		self.__class__.stack.pop(index)
		self.__class__.stack.insert(0, word)

	def find_in(self, word):
		"""find existing word in stack and add at the top"""
		index=self.__class__.stack.index(word)
		self.add_in_stack(index,word)

	def print_val(self,number):
		"""deals with all code printing from class"""
		code=number+1
		if 1<=code<=120:
			self.file.write(chr(code+128))
		elif 121<=code<=375:
			self.file.write(chr(0xF9))
			self.file.write(chr(code-121))
		elif 376<=code<=65912:
			self.file.write(chr(0xFA))
			self.file.write(chr((code-376)//256))
			self.file.write(chr((code-376)%256))
			
	def apply(self):
		"""apply changes to every line by finding word in map or 'stack'"""
		for word in self.words:
			#if in dictionary and hasn't been used
			if word in self.__class__.unique_words and word not in self.__class__.stack:
				self.print_val(self.unique_words[word])
				#prints word, first time seen
				self.file.write(word)
				self.__class__.stack.insert(0, word)
				#remove in dictionary to show its been used once
				del self.__class__.unique_words[word]
			else:
				#must be in list so find it
				self.find_in(word)
#encode main
def encode(input_name):
	(base_name, _, _) = input_name.rpartition(".")
	output_name = base_name + "." + "mtf"

	#catch errors in file opening
	try:
		input = open(input_name,"r")
	except:
		print("File cannot be found or opened")
		sys.exit(1)

	#create new file name from command line argument
	file = open(output_name,encoding="latin-1",mode="w")

	#write magic numbers
	for code in MAGIC_NUMBER_2:
		file.write(code)

	#read line by line from a file
	for line in input:
		#create a new object to handle all line changes in line
		output=sentenceEncoding(line, file)
		#call method from class
		output.apply()
		file.write("\n")
	file.close()


class sentenceDecoding:
	"""This class contains methods needed for decoding"""
	def __init__(self, words, file):
		"""take in a file pointer and array of words and save in class"""
		self.file=file
		self.words=words
	
	def add_in(self):
		"""if word isn't empty add to 'stack'"""
		if(len(self.word)!=0):
			#combine list then add to stack and final list
			self.reduced_sentence.append(''.join(self.word))
			self.stack.insert(0,''.join(self.word))
			self.unique+=1
			self.word.clear()
			
	def case1(self, char):
		"""This case is when the number previous isn't 249 or 250"""
		#if it is a number for 1-120
		if 129<=char<=248:
			self.add_in()
			#if this is the next unique then pop it from 'stack'
			if char-128<=self.unique:
				temp=self.stack.pop(char-129)
				self.reduced_sentence.append(temp)
				self.stack.insert(0, temp)
				
		#next one in array wil be from 121-320
		elif char==249:
			#set flag so we know
			self.two_char=True
			self.add_in()
			
		#next one in array wil be from 320+
		elif char==250:
			#set flag so we know
			self.three_char=True
			self.add_in()
			
		elif char==10:
			#add to final list right away
			self.add_in()
			self.reduced_sentence.append(char)
		else:
			#if none of theses cases you must be in word so store letters
			self.word.append(chr(char))

	def case2(self, char):
		"""This case represents two_char representable number decoding"""
		#If it is not the next unique retrieve it and add to final list
		if char+121<self.unique:
			temp=self.stack.pop(char+120)
			self.reduced_sentence.append(temp)
			self.stack.insert(0, temp)
		#return flag
		self.two_char=False
			
	def case3(self, char):
		"""This case represents three_char representable number decoding"""
		#If you already have the second of three_char if not get it
		if(len(self.third_case)==1):
			#If not the next unique then retrieve it and add to final list
			if (self.third_case[0]+char)<self.unique:
				temp=self.stack.pop((self.third_case[0]+char-1))
				self.reduced_sentence.append(temp)
				self.stack.insert(0, temp)
			#Then clear list cahnge flag
			self.third_case.clear()
			self.three_char=False
		else:
			self.third_case.append((char*256)+376)
	
	def apply_to(self):
		#create flags and empty lists, all used in cases
		self.two_char=False
		self.three_char=False
		self.reduced_sentence=[]
		self.word=[]
		self.third_case=[]
		self.unique=0
		self.stack=[]

		#determine what case to call
		for char in self.words:
			if not self.two_char and not self.three_char:
				self.case1(char)
			elif self.two_char:
				self.case2(char)	
			elif self.three_char:
				self.case3(char)
	
	def print_to_file(self):	
		"""print reduced sentence to file"""
		for index, char in enumerate(self.reduced_sentence):
			if char==10:
				self.file.write("\n")
			elif index+1<len(self.reduced_sentence) and self.reduced_sentence[index+1]==10:
				self.file.write("%s" %char)
			else:
				self.file.write("%s " %char)
			
#decode main
def decode(input_name):
	(base_name, _, _) = input_name.rpartition(".")
	output_name = base_name + "." + "txt"

	#catch errors in file opening
	try:
		mtf_file= open(sys.argv[1], encoding="latin-1", mode="r", newline="")
	except:
		print("File cannot be found or opened")
		sys.exit(1)

	#read file in to line and create list of words
	line=mtf_file.read()
	words=[]
	
	#ensure character can be converted to ASCII
	try:
		for char in line:
			words.append(ord(char))
	except:
		print("Characters not ASCII representable")
		sys.exit(1)
		
	#use regular expressions to check if file contains magic numbers
	string="".join(str(int) for int in words)
	matchobj = re.match('186941861[78]', string) 
	if not matchobj:
		print("Invalid File")
		sys.exit(1)

	#remove magic numbers and open new file
	words=words[4:]
	file = open(output_name,"w")
	
	#create instance of class, call methods to decode numbers,and print
	decode_line=sentenceDecoding(words, file)
	decode_line.apply_to()
	decode_line.print_to_file()
	
	#close file
	file.close()
