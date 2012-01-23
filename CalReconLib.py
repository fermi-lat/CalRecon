#$Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalRecon/CalReconLib.py,v 1.3 2009/11/13 00:46:49 jrb Exp $
def generate(env, **kw):
    #if not kw.get('depsOnly', 0):
    #    env.Tool('addLibrary', library = ['CalRecon'])
    env.Tool('CalXtalResponseLib')
    env.Tool('TkrUtilLib')
    env.Tool('CalUtilLib')
    env.Tool('GuiSvcLib')
    env.Tool('GlastSvcLib')
    env.Tool('RootIoLib')
    #env.Tool('mcRootDataLib')
def exists(env):
    return 1;
