package com.tank;

import java.awt.Color;

public class StrongEnemyTank extends EnemyTank {

	public StrongEnemyTank(int x, int y) {
		super(x, y);
		this.life = 2;
		tankDraw = new SETankDraw();
	}
	
	@SuppressWarnings("serial")
	private class SETankDraw extends TankDraw {

		@Override
		protected Color getColor() {
			return Color.GREEN;
		}		
	}
}
