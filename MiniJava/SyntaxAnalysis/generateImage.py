#!/usr/bin/python
import argparse
from os import system
from os.path import splitext

parser = argparse.ArgumentParser(description='Compile .png image from GraphViz (DOT language) file format.')
parser.add_argument('input', help='path to the input file')
parser.add_argument('-o', '--output', type=str, help='path to the output file')
args = parser.parse_args()

output_format = 'svg'
output_ext = '.' + output_format

if args.output is None:
	args.output = splitext(args.input)[0] + output_ext
elif splitext(args.output)[1] != output_ext:
	args.output += output_ext

# print args.output
cmd = 'dot -T' + output_format + ' ' + args.input + ' -o' + args.output
print cmd
system(cmd)
