package com;

public class Filter {
	public String name() {
		return getClass().getSimpleName();
	}
	public String process() {
		return name();
	}
}
