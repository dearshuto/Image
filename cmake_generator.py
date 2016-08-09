import os
import subprocess
import platform


def cmake_setting():
	rootDirectory = os.getcwd()
	if not os.path.exists("./build"):
       		os.mkdir("./build")
	os.chdir(rootDirectory + "/build")

	# set up cmake commands
	me = platform.system()
	cmd = ['cmake']
	if me == 'Windows':
		print 'run cmake as Windows'	
		cmd = ['cmake', '..', '-G', 'Visual Studio 14 2015']
	elif me == 'Linux':
		cmd = ['cmake', '..', '-G', 'Makefile']
	elif me == 'Darwin':
		cmd = ['cmake', '..', '-G', 'Xcode']

	# run cmake with arguments
	try:
        	res = subprocess.check_call(cmd)
	except:
		print "cmake error"
