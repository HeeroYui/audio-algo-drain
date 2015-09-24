#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug

def get_desc():
	return "drain basic algorithm"

def get_licence():
	# return a table with : "Licence Name", contamination if link static, contamination if link dynamic, "Licence string description / FileName / auto for classicle licence"]
	return ["APACHE-2", False, False, "auto"]


def create(target):
	my_module = module.Module(__file__, 'audio-algo-drain', 'LIBRARY')
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
	
	my_module.add_export_path(tools.get_current_path(__file__))
	
	#my_module.set_licence(module.APACHE_2)
	
	return my_module









