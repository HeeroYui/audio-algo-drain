#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_type():
	return "BINARY"

def get_sub_type():
	return "TOOL"

def get_desc():
	return "audio algo drain tools"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'freqResp.cpp'
		])
	my_module.compile_version("c++", 2011)
	my_module.add_depend(['m', 'etk', 'test-debug'])
	return my_module
