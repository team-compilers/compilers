#!/usr/bin/python
import argparse
from os import system, listdir, unlink
from os.path import splitext, join, isfile

def generate_results(dirpath_input, dirpath_output, sys_command_f, fout_ext = ''):
    for filename in listdir(dirpath_input):
        filepath_input = join(dirpath_input, filename)
        filepath_output = splitext(join(dirpath_output, filename))[0] + fout_ext
        system(sys_command_f(filepath_input, filepath_output))

DEF_DIR_IN_DOT = 'data/Samples/'
DEF_DIR_OUT_DOT = 'data/SamplesGV/'
DEF_DIR_IN_IMGS = DEF_DIR_OUT_DOT
DEF_DIR_OUT_IMGS = 'data/SamplesAST/'
DEF_DIR_IN_CODE = DEF_DIR_IN_DOT
DEF_DIR_OUT_CODE = 'data/SamplesCode/'

def generate_results_dot(dirpath_input = DEF_DIR_IN_DOT, dirpath_output = DEF_DIR_OUT_DOT):
    def sys_command_f(filepath_input, filepath_output):
        return './ast ' + filepath_input + ' ' + filepath_output + ' dot'
    generate_results(dirpath_input, dirpath_output, sys_command_f, fout_ext = '.gv')

def generate_results_images(dirpath_input = DEF_DIR_IN_IMGS, dirpath_output = DEF_DIR_OUT_IMGS):
    def sys_command_f(filepath_input, filepath_output):
        return './generateImage.py ' + filepath_input + ' -o ' + filepath_output
    generate_results(dirpath_input, dirpath_output, sys_command_f)

def generate_results_code(dirpath_input = DEF_DIR_IN_CODE, dirpath_output = DEF_DIR_OUT_CODE):
    def sys_command_f(filepath_input, filepath_output):
        return './ast ' + filepath_input + ' ' + filepath_output + ' code'
    generate_results(dirpath_input, dirpath_output, sys_command_f, fout_ext = '.java')

def generate_results_all():
    generate_results_code()
    generate_results_dot()
    generate_results_images()

def remove_generated_data(dirpaths = [DEF_DIR_OUT_DOT, DEF_DIR_OUT_IMGS, DEF_DIR_OUT_CODE]):
    for dirpath in dirpaths:
        for filename in listdir(dirpath):
            filepath = join(dirpath, filename)
            try:
                if isfile(filepath):
                    unlink(filepath)
                #elif os.path.isdir(filepath): shutil.rmtree(filepath)
            except Exception as e:
                print(e)

mode_to_func = {
    'all': generate_results_all,
    'dot': generate_results_dot,
    'images': generate_results_images,
    'code': generate_results_code,
    'clean': remove_generated_data
}
modes = mode_to_func.keys()
mode_help = 'Possible values: {}'.format(', '.join(modes))

parser = argparse.ArgumentParser(description='Compile images from GraphViz (DOT language) files.')
parser.add_argument('mode', type=str, help=mode_help)
# parser.add_argument('-i', '--input', type=str, help='path to the input directory')
# parser.add_argument('-o', '--output', type=str, help='path to the output directory')
args = parser.parse_args()

# dirpath_input = args.input
# dirpath_output = args.output
# if dirpath_output is None:
    # dirpath_output = dirpath_input

mode = args.mode
if mode in modes:
    mode_to_func[mode]()
    # if dirpath_input is None:
    #     if dirpath_output is None:
    #         mode_to_func[mode]()
    #     else:
    #         mode_to_func[mode](dirpath_output = dirpath_output)
    # else:
    #     mode_to_func[mode](dirpath_input = dirpath_input, dirpath_output = dirpath_output)

else:
    raise ValueError('Wrong mode. ' + mode_help)
