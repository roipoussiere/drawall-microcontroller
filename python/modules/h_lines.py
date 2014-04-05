#!/usr/bin/python3.2
# -*- coding: utf-8 -*-

import argparse
from PIL import Image
import numpy as np

parser = argparse.ArgumentParser(
	formatter_class=argparse.RawDescriptionHelpFormatter,
	description = 'Converts a picture to G-Code.')

parser.add_argument('input_file', action = 'store',
	type = argparse.FileType('r'), help = 'image path')

parser.add_argument('-o', action = 'store', dest = 'output_file', nargs = '?',
	type = argparse.FileType('w'), help = 'output file path')

args = parser.parse_args()

class Lines:
		
	def __init__(self):
		self.pos = (0, 0)
		img_path = args.input_file.name # todo: faire un peu mieux

		img = Image.open(img_path)
		self.bitmap = np.array(img)
		self.img_size = img.size
		self._trace()

	def _trace(self):
		ngc_path = 'output.ngc' if args.output_file == None else args.output_file.name # todo: nommer en input.ngc
		with open(ngc_path, 'w') as gcode_file:
			gcode_file.write('G0 X0 Y0 Z0\nG1\n')
			for line in reversed(self.bitmap):
				for bit in line:
					gcode_file.write(self._draw(True if bit<128 else False))

	def _draw(self, isPixel):
		if self.pos[0] < self.img_size[0] - 1:
			if isPixel:
				self.pos = (self.pos[0] + 0.5, self.pos[1] + 0.5)
				line = _get_line(self.pos)
				self.pos = (self.pos[0] + 0.5, self.pos[1] - 0.5)
				line += _get_line(self.pos)
			else:
				self.pos = (self.pos[0] + 1, self.pos[1])
				line = _get_line(self.pos)
		else:		
			self.pos = (0, self.pos[1] + 1)
			line = 'Z25\n'
			line += _get_line(self.pos)
			line += 'Z0\n'
		
		return line

def _get_line(pos):
	return 'X' + str(pos[0]) + ' Y' + str(pos[1]) + '\n'

Lines()
# converted_img.show()
