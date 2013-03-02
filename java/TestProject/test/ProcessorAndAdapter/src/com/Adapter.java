package com;

public class Adapter implements Processor{

	Filter filter;
	public Adapter(Filter filter) {
		this.filter = filter;
	}
	@Override
	public String name() {
		return filter.name();
	}

	@Override
	public Object process(Object input) {
		return filter.process();
	}
	
	public static void main(String[] args) {
		Apply.process(new Adapter(new HighPass()), null);
		Apply.process(new Adapter(new LowPass()), null);
	}
}
