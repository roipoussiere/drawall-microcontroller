#!/usr/bin/python3.2
# -*- coding: utf-8 -*-

import re

def getValue(param, line) :
	return float(re.sub('.*' + param + '([-0-9.]+).*', r'\1', line))

def clean(input_file, output_file, mask = 'G00|G01') :
	x = 0
	y = 0
	z = 0

	output = open(output_file, 'w')
	
	for line in open(input_file).readlines() :
		line = re.sub('[ \t]', '', line) # del whitespaces
		if re.match('^;', line) : continue # del comments
		line = re.sub('\(.*', '', line) # del comments
		line = re.sub('^([XYZ])', r'G01\1', line) # insert G01 if the line begin with an argument. TODO improve this
		line = re.sub('([IJKXYZPQ])', r' \1', line) # insert spaces before each argument
		line = re.sub(r'([GM])(\d\s)', r'\g<1>0\2', line) # set all functions with 3 characters (G1 -> G01)
		line = re.sub('(.)([GM])', r'\1\n\2', line) # insert end-lines
		if re.match('^$', line) : continue # del empty lines
		if not re.match(mask, line) : continue # del forbiden functions

		if 'X' in line : x = getValue('X', line)
		if 'Y' in line : y = getValue('Y', line)
		if 'Z' in line : z = getValue('Z', line)

		if 'X' in line or 'Y' in line or 'Z' in line:
			line = re.sub('([GM][0-9][0-9]).*', r'\1 X' + str(x) + ' Y' + str(y) + ' Z' + str(z), line)
		
		if re.match('^G0[01]$', line) : continue # del moving commands without parameters
			
		output.write(line)

if (__name__ == '__main__'):
	import argparse
	parser = argparse.ArgumentParser(
			formatter_class=argparse.RawDescriptionHelpFormatter)

	parser.add_argument('input_file', type = argparse.FileType('r'),
		action = 'store',
		help = 'G-Code input file name.')
	args = parser.parse_args()

	clean(args.input_file.name, '_' + args.input_file.name)
