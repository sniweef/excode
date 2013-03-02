package com.tank;

import java.awt.Color;

public class HeroTank extends Tank{
	
	public HeroTank(int x, int y) {
		super(x, y);
		this.speed = 20;
//		this.bulletSpeed = 30;
		this.life = 3;
		tankDraw = new HeroTankDraw();
	}
	
	
	@SuppressWarnings("serial")
	private class HeroTankDraw extends TankDraw{
		@Override
		public Color getColor() {
			return Color.PINK;
		}		
	}

	
}
