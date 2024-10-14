import os

def genconfig() :
    from SCons.Script import SCons

    PreProcessor = SCons.cpp.PreProcessor()

    try:
        f = open('rtconfig.h', 'r')
        contents = f.read()
        f.close()
    except :
        print("Open rtconfig.h file failed.")

    PreProcessor.process_contents(contents)
    options = PreProcessor.cpp_namespace
    conf_buff = []

    try:
        f = open('.config', 'w')
        for (opt, value) in options.items():
            if type(value) == type(1):
                conf_buff.append("CONFIG_{0}={1}\n".format(opt, value))

            if type(value) == type('') and value == '':
                conf_buff.append("CONFIG_{0}=y\n".format(opt))
            elif type(value) == type('str'):
                conf_buff.append("CONFIG_{0}={1}\n".format(opt, value))

        conf_buff.sort()
        f.write("".join(conf_buff))
        print("Generate .config done!")
        f.close()
    except:
        print("Generate .config file failed.")
