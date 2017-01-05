#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "TOOL"

def get_desc():
	return "audio algo drain tools"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def configure(target, my_module):
	my_module.add_src_file([
	    'freqResp.cpp'
	    ])
	my_module.compile_version("c++", 2011)
	my_module.add_depend([
	    'm',
	    'etk',
	    'test-debug'
	    ])
	return True
