#mess
class TestClass:
    print 'TestClass aaa..'
    def __init__(self, name):
        self._name = name
    def alterName(newName):
        self._name = newName
    def disName(self):
        #print name : error
        print self._name

another = TestClass('ano')
thrid = TestClass('thrid')
print another._name
TestClass.name = 'Class'
TestClass.new = 'new'
another.disName()
print another.name
print another.new
thrid.name = 'Thrid'
print another.name
class SubClass(TestClass):
    def __init__(self, name):
        TestClass.__init__(self,name)
        #self.name = name
    def dis(self, name):
        TestClass.disName(self,name)
subclass = SubClass("subclass")
subclass.disName()
subclass.newVar = 3
