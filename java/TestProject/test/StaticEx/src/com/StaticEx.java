package com;

class ClassWithStatic {
	static int a = inita();
	
	static {
		a = 2;
		System.out.println("static");
	}
	ClassWithStatic() {
		a = 3;
		System.out.println("Constructor");
	}
	static int inita() {
		System.out.println("int a");
		return 1;
	}
}

public class StaticEx {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new ClassWithStatic();
		System.out.println("a=" + ClassWithStatic.a);
		
	}

}
