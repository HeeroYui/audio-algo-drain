#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug

def get_desc():
	return "test drain audio algo"


def create(target):
	my_module = module.Module(__file__, 'audio-algo-drain-test', 'BINARY')
	my_module.add_src_file([
		'test/main.cpp',
		'test/debug.cpp'
		])
	my_module.add_module_depend(['audio-algo-drain'])
	return my_module









