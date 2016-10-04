#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


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

def configure(target, my_module):
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
	my_module.add_depend([
	    'etk',
	    'audio'
	    ])
	my_module.add_path(".")
	return True









