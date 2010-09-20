# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/CalRecon/SConscript,v 1.12 2010/05/16 18:53:29 usher Exp $
# Authors: Alexandre Chekhtman <chehtman@gamma.nrl.navy.mil>, David Chamont <chamont@poly.in2p3.fr
# Version: CalRecon-06-11-00-gr01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('CalReconLib', depsOnly = 1)

CalRecon=libEnv.SharedLibrary('CalRecon',
                              listFiles(['src/*.cxx','src/Dll/*.cpp',
                                         'src/Display/*.cxx',
                                         'src/Clustering/*.cxx',
                                         'src/MipFinding/*.cxx',
                                         'src/EnergyCorrections/*.cxx',
                                         'src/Utilities/*.cxx']))

progEnv.Tool('CalReconLib')
test_CalRecon = progEnv.GaudiProgram('test_CalRecon',
                                     listFiles(['src/test/*.cxx']), test = 1)

progEnv.Tool('registerTargets', package = 'CalRecon',
             libraryCxts = [[CalRecon, libEnv]],
             testAppCxts = [[test_CalRecon, progEnv]],
             includes = listFiles(['CalRecon/*.h']))





