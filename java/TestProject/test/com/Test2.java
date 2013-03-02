package com;
public class Test2 {
    public Test2(int i) {
        System.out.println("Test2:"+i);
    }
    private interface A {
        int a = 3;
        void f(int i);
    }
    public class AImp implements A {
        public void f(int i) {
            System.out.println("AImp:"+i);
        }
    }
    public A getA() {
        return new AImp();
    }
    public static Test2 test21 = new Test2(1);
    public static Test2 test22;
    static {
        test21 = new Test2(11);
        test22 = new Test2(2);
    }
    public void f() {
    }
}
