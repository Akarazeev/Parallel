#!/usr/local/bin/python
# -*- coding: utf-8 -*-

from random import randint
import os, sys, string

fullpath = os.path.dirname(os.path.realpath(sys.argv[0]))
fullpath += '/tests/'
fullpath = unicode(fullpath,'utf-8')

print(fullpath)

def get_input_name(ind):
	return unicode('i_{}.txt'.format(str(ind).zfill(3)),'utf-8')

def get_out_name(ind):
	return u'o_{}.txt'.format(str(ind).zfill(3))

num_threads = 2
batch_size = 1
assert(batch_size < 1000)
max_val = 10
max_m = 200

for i in range(batch_size):
	open(fullpath + get_input_name(i),'w').close()
	open(fullpath + get_out_name(i),'w').close()

for i in range(batch_size):
	# m = randint(1,max_m)
	# n = randint(1,max_m)
	m = max_m
	n = max_m
	tmp = [randint(0,max_val) for j in range(m * n)]

	with open(fullpath + get_input_name(i),'w') as f:
		# f.write('{}\n'.format(randint(1,20)))
		f.write('{}\n'.format(num_threads))
		f.write('{} {}\n'.format(m,n))
		f.write(' '.join(map(str, tmp)))
	
	with open(fullpath + get_out_name(i),'w') as f:
		f.write(str(max(tmp)))
		f.write('\n')
		f.write(' '.join(map(str,[i for i,j in enumerate(tmp) if j == max(tmp)])))
		f.write('\n')
		f.write(str(min(tmp)))
		f.write('\n')
		f.write(' '.join(map(str,[i for i,j in enumerate(tmp) if j == min(tmp)])))
		f.write('\n')