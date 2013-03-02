package another;



interface Cycle {
	void method1();
	void method2();
}
class Unicycle implements Cycle {
	public static Factory unicycleFac = new Factory() {
		public Cycle getCycle() {
			return new Unicycle();
		}
	};
	public void method1() {
		System.out.println("Unicycle:m1");
	}
	public void method2() {
		System.out.println("Unicycle:m2");
	}
}
class Bicycle implements Cycle {
	public static Factory bicycleFac = new Factory() {
		public Cycle getCycle() {
			return new Bicycle();
		}
		
	};
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
public class FactoryInnerEx {

	static void callFac(Factory fac) {
		Cycle cycle = fac.getCycle();
		cycle.method1();
		cycle.method2();
	}
	public static void main(String[] args) {
		callFac(Unicycle.unicycleFac);
		callFac(Bicycle.bicycleFac);
	}
}