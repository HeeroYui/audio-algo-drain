#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug


def get_type():
	return "LIBRARY"

def get_desc():
	return "drain basic algorithm"

def get_licence():
	return "APACHE-2"


def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1,0]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'audio/algo/drain/debug.cpp',
		'audio/algo/drain/BiQuad.cpp',
		'audio/algo/drain/BiQuadType.cpp',
		'audio/algo/drain/Equalizer.cpp'
		])
	my_module.add_header_file([
		'audio/algo/drain/BiQuad.h',
		'audio/algo/drain/BiQuadType.h',
		'audio/algo/drain/Equalizer.h'
		])
	my_module.add_module_depend(['etk', 'audio'])
	my_module.add_path(tools.get_current_path(__file__))
	return my_module









