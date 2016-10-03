#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import sys, os, string, time
from subprocess import Popen, PIPE, STDOUT

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

fullpath = os.path.dirname(os.path.realpath(sys.argv[0]))
fullpath += '/tests/'
fullpath = unicode(fullpath,'utf-8')

def get_input_name(ind):
	return u'i_{}.txt'.format(str(ind).zfill(3))

def get_out_name(ind):
	return u'o_{}.txt'.format(str(ind).zfill(3))

list_dir = os.listdir(fullpath)
inputs = [x for x in list_dir if x.startswith('i')]
inputs = sorted(inputs)
outputs = [x for x in list_dir if x.startswith('o')]
outputs = sorted(outputs)
assert(len(inputs) == len(outputs))

for inp, out in zip(inputs,outputs):
	with open('tests/' + inp,'r') as f_i, open('tests/' + out,'r') as f_o:
		p = Popen(['./{}'.format(sys.argv[1])], shell=True, stdout=PIPE, stdin=PIPE, stderr=STDOUT)
		input_values = f_i.read()
		t1 = time.time()
		prog_out = p.communicate(input_values)[0]
		t2 = time.time()
		prog_ans = prog_out
		ans = f_o.read()
		if prog_ans == ans:
			print inp + ' - ' + bcolors.OKGREEN + 'TRUE' + bcolors.ENDC
			# print 'Output: {}, Answer: {}'.format(prog_ans,ans)
		else:
			print inp + ' - ' + bcolors.FAIL + 'FALSE' + bcolors.ENDC
			print 'Output: {}, Answer: {}'.format(prog_ans,ans)
			print input_values
			# print prog_out
			print ''
		print t2 - t1