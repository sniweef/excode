package com;

interface Processor {
	public String name();
	public Object process(Object input);
}
abstract class StringProcessor implements Processor {
	public String name() {
		return getClass().getSimpleName();
	}
	
}
class Upcase extends StringProcessor {
	public String process(Object input) {
		return input.toString().toLowerCase(); 
	}
}
class Downcase extends StringProcessor {
	public String process(Object input) {
		return input.toString().toUpperCase();
	}
}

public class Apply {
	public static void process(Processor p, Object s) {
		System.out.println("Using Processor" + p.name());
		System.out.println(p.process(s));
	}
	 
	public static void main(String[] args) {
		String s = "Test";
		process(new Upcase(), s);
		process(new Downcase(), s);
	}

}
