#!/usr/bin/python
import argparse
from os import system, listdir
from os.path import splitext, join

parser = argparse.ArgumentParser(description='Compile images from GraphViz (DOT language) files.')
parser.add_argument('-i', '--input', type=str, help='path to the input directory')
parser.add_argument('-o', '--output', type=str, help='path to the output directory')
args = parser.parse_args()

dirpath_input = args.input
dirpath_output = args.output
if dirpath_output is None:
	dirpath_output = dirpath_input
if dirpath_input is None and dirpath_output is None:
	dirpath_input, dirpath_output = 'data/Samples/', 'data/SamplesGV/'

for filename in listdir(dirpath_input):
	filepath_input = join(dirpath_input, filename)
	filepath_output = splitext(join(dirpath_output, filename))[0] + '.gv'
	system('./ast ' + filepath_input + ' ' + filepath_output)
