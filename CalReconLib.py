#$Header: /nfs/slac/g/glast/ground/cvs/CalRecon/CalReconLib.py,v 1.4 2012/01/23 19:50:50 jrb Exp $
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
