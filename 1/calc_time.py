#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import sys, os, string, time
from subprocess import Popen, PIPE, STDOUT

fullpath = os.path.dirname(os.path.realpath(sys.argv[0]))
fullpath += '/tests/'
fullpath = unicode(fullpath,'utf-8')

list_dir = os.listdir(fullpath)
inputs = [x for x in list_dir if x.startswith('i')]
inputs = sorted(inputs)

batch_size = 10

inp = 'i_000.txt'
times1 = []
times2 = []
with open('tests/' + inp,'r') as f_i:
    no_num = f_i.read()
    for num_threads in xrange(1,20):
        tmp_time1 = []
        tmp_time2 = []
        for i in range(batch_size):
            p = Popen(['./a.out'], shell=True, stdout=PIPE, stdin=PIPE, stderr=STDOUT)
            input_values = '{}\n'.format(num_threads) + no_num
            prog_out = p.communicate(input_values)[0].split()
            if int(prog_out[0]) > 0:
                tmp_time1.append(int(prog_out[0]))
            if int(prog_out[1]) > 0:
                tmp_time2.append(int(prog_out[1]))
        times1.append(sum(tmp_time1) * 1.0 / len(tmp_time1))
        times2.append(sum(tmp_time2) * 1.0 / len(tmp_time2))
print times1
print '-'
print times2