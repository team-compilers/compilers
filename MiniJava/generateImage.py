#!/usr/bin/python
import argparse
from os import system
from os.path import splitext

parser = argparse.ArgumentParser(description='Compile .png image from GraphViz (DOT language) file format.')
parser.add_argument('input', help='path to the input file')
parser.add_argument('-o', '--output', type=str, help='path to the output file')
args = parser.parse_args()

output_format = '.png'

if args.output is None:
	args.output = splitext(args.input)[0] + output_format
elif splitext(args.output)[1] != output_format:
	args.output += output_format

# print args.output
cmd = 'dot -Tpng ' + args.input + ' -o' + args.output
print cmd
system(cmd)
