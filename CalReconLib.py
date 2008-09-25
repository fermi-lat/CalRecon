#$Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/CalRecon/CalReconLib.py,v 1.1 2008/08/15 21:42:33 ecephas Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['CalRecon'])
    env.Tool('addLibrary', library = env['minuitLibs'])
    env.Tool('CalUtilLib')
    env.Tool('GuiSvcLib')
    env.Tool('CalXtalResponseLib')
    env.Tool('RootIoLib')
    env.Tool('mcRootDataLib')
def exists(env):
    return 1;
