print "imported pmodule"


def get_max(a, b):
    if a > b:
        return a
    return b


def call_PrintString():
    import HostAPI
    res = HostAPI.PrintString("Hello World!")
    print "get result from C func PrintString: " + str(res)
    return res
