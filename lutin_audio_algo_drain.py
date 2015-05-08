#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug

def get_desc():
	return "audio_algo_drain : drain basic algorithm"

def get_licence():
	# return a table with : "Licence Name", contamination if link static, contamination if link dynamic, "Licence string description / FileName / auto for classicle licence"]
	return ["APACHE-2", False, False, "auto"]


def create(target):
	myModule = module.Module(__file__, 'audio_algo_drain', 'LIBRARY')
	myModule.add_src_file([
		'audio/algo/drain/debug.cpp',
		'audio/algo/drain/BiQuad.cpp',
		'audio/algo/drain/BiQuadType.cpp',
		'audio/algo/drain/Equalizer.cpp'
		])
	myModule.add_module_depend(['etk', 'audio'])
	
	myModule.add_export_path(tools.get_current_path(__file__))
	
	#myModule.set_licence(module.APACHE_2)
	
	return myModule









