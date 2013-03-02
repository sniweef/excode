interface U {
    void a();
    void b();
    void c();
}
public class Inner {
    static U createU(){
       return new U(){
           public void a() {
               System.out.println("Inner.a");
           }
           public void b() {}
           public void c() {}
       }; 
    }
    public static void main(String[] args) {
        int amount = 5;
        B b = new B(amount);
        for (int i = 0; i < amount; i++) {
            U u = createU();
            b.add(u);
        }
        b.travel();
    }
}
class B {
    U[] uArray;
    int index;
    B(int i) {
        uArray = new U[i];
        index = 0;
    }

    void add(U u) {
        if (index < uArray.length) 
            uArray[index++] = u;
    }
    void reset() {
        if (uArray != null)
            for (U u: uArray) 
                u = null;
    }
    void travel() {
        if (uArray != null)
            for (U u: uArray) 
                u.a();
    }
}
