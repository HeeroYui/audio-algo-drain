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
	return "authors.txt"

def get_version():
	return "version.txt"

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'audio/algo/drain/debug.cpp',
		'audio/algo/drain/BiQuad.cpp',
		'audio/algo/drain/BiQuadType.cpp',
		'audio/algo/drain/Equalizer.cpp'
		])
	my_module.add_header_file([
		'audio/algo/drain/BiQuad.hpp',
		'audio/algo/drain/BiQuadType.hpp',
		'audio/algo/drain/Equalizer.hpp'
		])
	my_module.add_depend(['etk', 'audio'])
	my_module.add_path(tools.get_current_path(__file__))
	return my_module









