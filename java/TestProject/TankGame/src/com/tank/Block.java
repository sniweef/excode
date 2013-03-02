package com.tank;

public interface Block {
	public boolean containsPoint(int x, int y);
	//public int getUpBorderX();
	public int getUpBorderY();
	//public int getDownBorderX();
	public int getDownBorderY();
	public int getLeftBorderX();
	//public int getLeftBorderY();
	public int getRightBorderX();
	//public int getRightBorderY();
}
