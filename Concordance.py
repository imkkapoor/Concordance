import sys
import re

# importing fileinput to help with file handling
import fileinput

class concord:

	def __init__(self, input=None, output=None):
		self.input = input
		self.output = output
		
		# if output file in given, then calling full concordance
		if self.output!= None:
			self.full_concordance()


	def full_concordance(self):
		
		# setting up the file in use, if no input file is given, then it 
		# automatically redirects to stdin
		
		file_in_use = fileinput.input(files = self.input)
		
		exclusion_words 	= self.__read_ex_words(file_in_use)
		lines			= self.__read_lines(file_in_use)
		sorted_words    	= self.__make_and_sort_words(exclusion_words,lines)
		formatted_lines 	= self.__generate_sentence_sequence(sorted_words, lines)  
	

		# in case we have a output file, I write to the file and then return back
		if self.output!=None:
			self.__write_to_file(formatted_lines)
			return

		return formatted_lines



	# in case the output file is given, this function helps to
	# write in the output file

	def __write_to_file(self,formatted_lines):
		new = open(self.output,"w")

		for line in formatted_lines:
			new.write(line)
			new.write("\n")
		close(new)
		return

	

	# this function reads all the exlusion words and terminates
	# the program if there's a wrong file
	
	def __read_ex_words(self,file_in_use):
	
		exclusion_words=[]
		count=0
		for line in file_in_use:
			if count==0 and line.strip()=="1":
				print("Input is version 1, concord expects version 2")
				sys.exit()
		
			line = line.strip()
		
			if line=="\"\"\"\"":
				break
		
			if count>1:
				line=line.lower()
				exclusion_words.append(line)
			count+=1
		return exclusion_words



	# this function helps to read all the lines in the file
	
	def __read_lines(self,file_in_use):	

		lines=[]
		
		for line in file_in_use:

			line = line.strip()
			if line == "":
				continue
			lines.append(line)
		
		return lines


	#this function helps to make a list of inclusion words and sort
	#them accordingly	

	def __make_and_sort_words(self,exclusion_words,lines):
		sorted_words = []
		for line in lines:
			split_sentence = line.split(" ")
			for each_word in split_sentence:
				if each_word.lower() not in exclusion_words:
					sorted_words.append(each_word.upper())

		sorted_words.sort()
		return sorted_words



	# this function helps to generate the ordered sentence sequence
	# and format them properly according to the specifications in the 
	# assignment description	

	def __generate_sentence_sequence(self, sorted_words, lines):
		sorted_sentences = []
		for i in range (0,len(sorted_words)):
			each_word = sorted_words[i]
			if i>0 and each_word.upper() == sorted_words[i-1].upper():
				continue
			for each_line in lines:
				check_word = re.search(r"\b"+each_word+r"\b", each_line, re.IGNORECASE)
				if check_word:
					
					# as soon as the word is found, the sentence or line is formatted
					
					capitalized_sentence = self.__capitalize(each_line,each_word)
					formatted_sentence = self.__format_sentence(capitalized_sentence, each_word)
					sorted_sentences.append(formatted_sentence)

		return sorted_sentences



	# this is a helper function designed to capitalize the word
	# which is needed to be ordered

	def __capitalize(self, sentence, word):
		upper_case_word = word.upper()
		capitalized = re.sub(r"\b"+ word +r"\b", upper_case_word, sentence, flags = re.IGNORECASE)
		return capitalized


	
	# this function helps to format the sentence with the given 
	# specifications

	def __format_sentence(self, sentence, the_word):
		index = re.search(r"\b" + the_word +r"\b", sentence, re.IGNORECASE).start()
		spaces_to_add = 29 - index
		
		if index>20:
			sentence = sentence[index-21:]
			sentence = " "*8 + sentence
		else:
			sentence = " " * spaces_to_add + sentence
		sentence = sentence[8:61]
		
		sentence = self.__right_cut(sentence)
		sentence = self.__left_cut(sentence)
		
		# here I add the desired number of spaces on the left side, after
		# the excess part has been cut

		index = re.search(r"\b" + the_word + r"\b", sentence, re.IGNORECASE).start()
		spaces_to_add = 29 - index
		sentence = " " * spaces_to_add + sentence
		
		return sentence



	# this function helps to cut the excess right part of the sentence

	def __right_cut(self, sentence):
		if len(sentence) == 53:
			j = 52
			while sentence[j] != " ":
				j = j-1
			sentence = sentence[:j]
		return sentence



	# this function helps to cut the excess left part of the sentence

	def __left_cut(self, sentence): 
		k = 0
		while sentence[k] != " ":
			k = k+1
		sentence = " " * k + sentence[k:].rstrip() 
		return sentence
		
