package com.tank;

import java.awt.Color;
import java.util.concurrent.TimeUnit;


public class EnemyTank extends Tank implements Runnable {
	private Thread tankThread;
	
	public EnemyTank(int x, int y) {
		super(x, y);
		this.speed = 15;
		tankThread = null;
		tankDraw = new EnemyTankDraw();
	}
	public void start() {
		tankThread = new Thread(this);
		tankThread.start();
	}
	public void interrupt() {
		tankThread.interrupt();
	}
	public void run() {
		Direction direction;
		while (isAlive()) {
			try {
				TimeUnit.MILLISECONDS.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			direction = getNextDirection();
			switch (direction) {
			case UP:
				this.moveUp();
				break;
			case DOWN:
				this.moveDown();
				break;
			case LEFT:
				this.moveLeft();
				break;
			case RIGHT:
				this.moveRight();
				break;
			default:
				break;
			}
			if (enemyTanks != null)
				this.shoot();
		}
	}

	protected Direction getNextDirection() {
		Direction direction = Direction.STOP;
		Tank heroTank;
		int tankWidth = tankDraw.getTankWidth();

		if (enemyTanks != null) {
			heroTank = enemyTanks.get(0); // 默认只有一个玩家

			if (heroTank.getX() < this.getX()-speed)
				direction = Direction.LEFT;
			else if (heroTank.getX() > this.getX()+tankWidth+speed || 
					heroTank.containsPoint(heroTank.getX(), this.getY()+tankWidth/2))
				direction = Direction.RIGHT;
			else if (heroTank.getY() < this.getY())
				direction = Direction.UP;
			else
				direction = Direction.DOWN;
		}

		return direction;
	}

	@SuppressWarnings("serial")
	private class EnemyTankDraw extends TankDraw {
		@Override
		public Color getColor() {
			return Color.GRAY;
		}		
	}
	
}
