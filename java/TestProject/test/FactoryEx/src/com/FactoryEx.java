package com;


interface Cycle {
	public void method1();
	public void method2();
}
class Unicycle implements Cycle {
	public void method1() {
		System.out.println("Unicycle:m1");
	}
	public void method2() {
		System.out.println("Unicycle:m2");
	}
}
class Bicycle implements Cycle {
	public void method1() {
		System.out.println("Bicycle:m1");
	}
	public void method2() {
		System.out.println("Bicycle:m2");
	}
}

interface Factory {
	Cycle getCycle();
}
class UnicycleFac implements Factory {
	public Cycle getCycle() {
		return new Unicycle();
	}
}
class BicycleFac implements Factory {
	public Cycle getCycle() {
		return new Bicycle();
	}
}

public class FactoryEx {
	static void callFac(Factory fac) {
		Cycle cycle = fac.getCycle();
		cycle.method1();
		cycle.method2();
	}
	public static void main(String[] args) {
	 
		callFac(new UnicycleFac());
		callFac(new BicycleFac());
	}
}
