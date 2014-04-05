#!/usr/bin/python3.2
# -*- coding: utf-8 -*-

import sys, argparse, re, tempfile
import gcodecleaner, gcode2svg

from os.path import join
from os.path import basename
from os.path import abspath

MASK = 'G00|G01'

def main() :

	parser = argparse.ArgumentParser(
			formatter_class=argparse.RawDescriptionHelpFormatter,
			description = 'Converts a G-Code file to an svg picture and save it as INPUT_FILE.svg (by default).',
			epilog = '''\

Examples :
  %(prog)s input.ngc
    Convert the G-Code file 'input.ngc' and save the picture as 'input.svg'.
  %(prog)s input.ngc -o output.svg
    Same thing but save the picture as 'output.svg'.
  %(prog)s input.ngc -c
    Clean the G-Code input file 'input.ngc'.
  %(prog)s input.ngc -c -o output.ngc
    Clean the G-Code file and save the output file as 'output.ngc'.
  %(prog)s input.ngc -cf G00 G01
    Clean the G-Code file, allowing only G-Code functions 'G00' and 'G01'.''')

	parser.add_argument('input_file', type = argparse.FileType('r'),
		action = 'store',
		help = 'G-Code input file name.')

	parser.add_argument('-m',
		action = 'store_true',
		dest = 'show_move',
		default = False,
		help = 'Show non-drawing movements in blue on the picture .')

	parser.add_argument('-o', action = 'store', type = argparse.FileType('w'),
		dest = 'output_file', default = False,
		help = 'Set output file name as OUTPUT_FILE.')

	args = parser.parse_args()

	output_file = abspath(args.input_file.name) if args.output_file != False else abspath(re.sub('\.[^.]+$', '.svg', args.input_file.name))
	tmp_file_path = join(tempfile.gettempdir(), basename(args.input_file.name))
	
	gcodecleaner.clean(args.input_file.name, tmp_file_path, MASK)
	gcode2svg.convert(tmp_file_path, output_file, args.show_move);
	
	print 'Gcode file successfully converted, saved as ' + output_file + '.'
		

main()
