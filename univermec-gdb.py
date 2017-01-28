import gdb
import re

class CXSCRealPrinter:
    "Print a cxsc::real."

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val['w']

class CXSCIntervalPrinter:
    "Print a cxsc::interval."

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return "[" + str(self.val['inf']['w']) + ", " + str(self.val['sup']['w']) + "]"


class CoreIntervalPrinter:
    "Print a core::arith::interval."

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return printer_lookup(self.val['m_adapt']).to_string()

class CoreMRealPrinter:
    "Print a core::arith::mreal."

    def __init__(self, val):
        self.val = val
    
    def to_string(self):
        return str(self.val['m_adapt'])



def printer_lookup (val):
    "Look-up and return a pretty-printer that can print val."

    # Get the type.
    type = val.type

    # If it points to a reference, get the reference.
    if type.code == gdb.TYPE_CODE_REF:
        type = type.target ()

    # Get the unqualified type, stripped of typedefs.
    type = type.unqualified()
    type = type.strip_typedefs()

    # Get the type name. 
    typename = type.tag
    #typenamed = typed.tag
    #print "Typename: " + str(typename)
    #if typename == NoneType:
    #    typename = typenamed
    #if typenamed == NoneType:
    #    typenamed = typename
    if typename == None:
        return None

    #print "Ddel"
    #print typename
    #print "Ddelend"

    # Iterate over local dictionary of types to determine
    # if a printer is registered for that type.  Return an
    # instantiation of the printer if found.
    for function in unidiwok_printers_dict:
        if function.search (typename): # or function.search(typenamed):
            return unidiwok_printers_dict[function] (val)
        
    # Cannot find a pretty printer.  Return None.
    return None

# Register pretty printers in gdb
def register_printers(objfile):
    if(objfile == None):
        objfile = gdb
    objfile.pretty_printers.append(printer_lookup)

# class MTLDenseVectorIterator:
#     def __init__(self, mtl):
#         self.pos = 0
#         self.count = mtl['my_used_memory']
#         self.data = mtl['data']
        
#     def __iter__(self):
#         return self

#     def next(self):
#         if self.pos >= self.count:
#             raise StopIteration
#         result = self.data.dereference()
#         self.data = self.data + 1
#         pos = self.pos
#         self.pos = self.pos + 1
#         return ('[%d]' % pos, result)

# class MTLDenseVectorPrinter:
#     "Print a mtl::dense_vector"

#     def __init__(self, val):
#         self.val = val

#     def to_string(self):
#         return ('dense_vector of length %d ' % self.val['my_used_memory'])
                
#     def display_hint(self):
#         return 'array'
    
#     def children(self):
#         return MTLDenseVectorIterator(self.val)

def build_dict():
    unidiwok_printers_dict[re.compile('^cxsc::real$')] = lambda val: CXSCRealPrinter(val)
    unidiwok_printers_dict[re.compile('^cxsc::interval$')] = lambda val: CXSCIntervalPrinter(val)
    unidiwok_printers_dict[re.compile('^core::arith::interval$')] = lambda val: CoreIntervalPrinter(val)
    unidiwok_printers_dict[re.compile('^core::arith::mreal$')] = lambda val: CoreMRealPrinter(val)
#    unidiwok_printers_dict[re.compile('^mtl::vector::dense_vector<.*>$')] = lambda val: MTLDenseVectorPrinter(val)
#    unidiwok_printers_dict[re.compile('mtl::vector::dense_vector<.*>$')] = lambda val: MTLDenseVectorPrinter(val)

unidiwok_printers_dict = {}
build_dict()
register_printers(gdb.current_objfile())
