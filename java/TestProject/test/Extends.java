public class Extends extends Root {
    static void f(Extends e) {
        System.out.println("call f()");
    }
    public static void main(String[] args) {
        Root r = new Root();
        f((Extends)r); //Cannot work without (Extends)

    }
}
class Root {
}
