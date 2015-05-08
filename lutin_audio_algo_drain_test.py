#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug

def get_desc():
	return "audio_algo_drain_test: test for speex ALGO"


def create(target):
	myModule = module.Module(__file__, 'audio_algo_drain_test', 'BINARY')
	myModule.add_src_file([
		'test/main.cpp',
		'test/debug.cpp'
		])
	myModule.add_module_depend(['audio_algo_drain'])
	return myModule









