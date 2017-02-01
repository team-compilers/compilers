#!/usr/bin/python
import argparse
from os import system, listdir, unlink, mkdir
from os.path import splitext, join, isfile, isdir, exists

DEF_DIRS_IN = ['data/SamplesCorrect/', 'data/SamplesErroneous/']
DEF_DIRS_OUT = ['results/SamplesCorrect/', 'results/SamplesErroneous']

def generate_results_images(dirpaths_input = DEF_DIRS_OUT, dirpaths_output = DEF_DIRS_OUT, verbose = True):
    def sys_command_f(path_input, path_output):
        return ' '.join(['./generateImage.py', path_input, '-o', path_output])
    
    for dirpath_input, dirpath_output in zip(dirpaths_input, dirpaths_output):
        # generate_results(dirpath_input, dirpath_output, sys_command_f)
        for filename in listdir(dirpath_input):
            filepath_input = join(dirpath_input, filename)
            if not isfile(filepath_input) or splitext(filepath_input)[1] != 'gv':
                continue
            if verbose:
                print '\033[1;34m' + filepath_input + '\033[0;39m'

            filepath_output = splitext(join(dirpath_output, filename))[0] + '.svg'
            system(sys_command_f(filepath_input, filepath_output))

def generate_results_run(dirpaths_input = DEF_DIRS_IN, dirpaths_output = DEF_DIRS_OUT, verbose = True):
    def sys_command_f(path_input, path_output):
        return ' '.join(['./minijava', path_input, path_output])
    
    for dirpath_input, dirpath_output in zip(dirpaths_input, dirpaths_output):
        for filename in listdir(dirpath_input):
            filepath_input = join(dirpath_input, filename)
            if not isfile(filepath_input):
                continue
            if verbose:
                print '\033[1;34m' + filepath_input + '\033[0;39m'

            filepath_output_dir = splitext(join(dirpath_output, filename))[0]
            if not exists(filepath_output_dir):
                mkdir(filepath_output_dir)
            system(sys_command_f(filepath_input, filepath_output_dir))

def generate_results_all():
    generate_results_run()
    generate_results_images()

def remove_generated_data(dirpaths = DEF_DIRS_OUT):
    for dirpath in dirpaths:
        for filename in listdir(dirpath):
            filepath = join(dirpath, filename)
            try:
                if isfile(filepath):
                    unlink(filepath)
                else:
                    print 'Not a file:', filepath
                #elif os.path.isdir(filepath): shutil.rmtree(filepath)
            except Exception as e:
                print(e)

mode_to_func = {
    'all': generate_results_all,
    'run': generate_results_run,
    'images': generate_results_images,
    'clean': remove_generated_data
}
modes = mode_to_func.keys()
mode_help = 'Possible values: {}'.format(', '.join(modes))

parser = argparse.ArgumentParser(description='Launch MiniJava.')
parser.add_argument('mode', type=str, help=mode_help)
args = parser.parse_args()

mode = args.mode
if mode in modes:
    mode_to_func[mode]()
else:
    raise ValueError('Wrong mode. ' + mode_help)
