import os
import sys
import subprocess
import shutil
import platform


def libjpeg_setting():
    me = platform.system()
    rootDirectory = os.getcwd()
    os.system('git submodule update --init --recursive')
    # libjpeg setting

    #
    myDirectory = os.path.realpath(os.path.dirname(sys.argv[0]))

    if not os.path.exists(myDirectory+'/external/jpeg/jconfig.h'):
        print 'configure libjpeg'
        os.chdir(myDirectory + "/external/jpeg")
        if me == 'Windows':
                shutil.copyfile("jconfig.vc", "jconfig.h") 
        else:
                print 'I use Unix'
                os.system('./configure')
    else:
        print 'libjpeg is already up-to-date'
    os.chdir(rootDirectory)
