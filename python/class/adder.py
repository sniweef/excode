class Adder:
    def __init__(self, data):
        self.data = data
    def add(self, y):
        return self.data + y
    def __add__(self, y):
        return add(self, y)

class ListAdder:
    def add(self, x, y):
        return x+y

class DictAdder:
    def add(self, x, y):
        d = x.copy()
        d.update(y)
        return d

print Adder(3)+3
print ListAdder().add([1,3],[4,2])
print DictAdder().add({1:2,3:4}, {2:3,4:5})
