class Iter:
    def __init__(self):
        self.str = 'abcdefghijk'
        self.value = 0
    def __getitem__(self, i):
        return self.str[i]
    #def __iter__(self):
    #    return self
    def next(self):
        if self.value == len(self.str)-1:
            raise StopIteration
        self.value += 1
        return self.str[self.value]
it = Iter()
print map(None, it)
print map(None, it)

for x in it:
    for y in it:
        print x + y,
print '\n---Cut---'
for x in it[::2]:
    for y in it[::2]:
        print x + y,
