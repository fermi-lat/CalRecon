# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/SConscript,v 1.5 2009/09/22 16:17:17 usher Exp $
# Authors: Alexandre Chekhtman <chehtman@gamma.nrl.navy.mil>, David Chamont <chamont@poly.in2p3.fr
# Version: CalRecon-06-08-01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('CalReconLib', depsOnly = 1)

if baseEnv['PLATFORM'] == 'win32':
	CalRecon = libEnv.SharedLibrary('CalRecon', listFiles(['src/*.cxx','src/Dll/*.cpp','src/Display/*.cxx',
							       'src/Display/*.h','src/Clustering/*.cxx', 'src/Clustering/*.h',
							       'src/MipFinding/*.cxx', 'src/MipFinding/*.h',
							       'src/EnergyCorrections/*.cxx', 'src/EnergyCorrections/*.h',
							       'src/Utilities/*.cxx', 'src/Utilities/*.h']))

if baseEnv['PLATFORM'] != 'win32':
	CalRecon = libEnv.SharedLibrary('CalRecon', listFiles(['src/*.cxx','src/Dll/*.cpp',
							       'src/Display/*.cxx',
							       'src/Clustering/*.cxx',
							       'src/MipFinding/*.cxx',
							       'src/EnergyCorrections/*.cxx',
							       'src/Utilities/*.cxx']))

progEnv.Tool('CalReconLib')
test_CalRecon = progEnv.GaudiProgram('test_CalRecon', listFiles(['src/test/*.cxx']), test = 1)

if baseEnv['PLATFORM'] != 'win32':
	progEnv.Tool('registerObjects', package = 'CalRecon', libraries = [CalRecon], testApps = [test_CalRecon],
		     includes = listFiles(['CalRecon/*.h']))

if baseEnv['PLATFORM'] == 'win32':
	progEnv.Tool('registerObjects', package = 'CalRecon', libraries = [CalRecon], testApps = [test_CalRecon],
		     includes = listFiles(['CalRecon/*.h']))





