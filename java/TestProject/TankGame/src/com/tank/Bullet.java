package com.tank;
import java.awt.Color;
import java.awt.Graphics;
import java.util.Vector;
import java.util.concurrent.TimeUnit;
import javax.swing.*;
import com.TankGame;

public class Bullet implements Runnable{
	private final int PANEL_WIDTH = TankGame.PANEL_WIDTH;
	private final int PANEL_HEIGHT = TankGame.PANEL_HEIGHT;
	private int x;
	private int y;
	private int speed;
	private Direction direction;
	private boolean isAlive;
	private BulletDraw bulletDraw;
	private Thread bulletThread;
	private Tank belongTank;
	
	public Bullet(Tank belongTank, int x, int y, int speed, Direction direction) {
		this.belongTank = belongTank;
		this.x = x;
		this.y = y;
		this.speed = speed;
		this.direction = direction;
		isAlive = true;
		bulletDraw = new BulletDraw();
		bulletThread = new Thread(this);		
		bulletThread.start();
	}
	public boolean isAlive() {
		return isAlive;
	}
	
	
	@Override
	public void run() {
		Vector<Tank> enemyTanks = belongTank.getEnemyTanks();
		int priorX;
		int priorY;
		Tank tank;
		
		while (isAlive) {
			priorX = x;
			priorY = y;
			try {
				TimeUnit.MILLISECONDS.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			switch (direction) {
			case UP:
				y -= speed;
				break;
			case DOWN:
				y += speed;
				break;
			case LEFT:
				x -= speed;
				break;
			case RIGHT:
				x += speed;
				break;
			default:
				break;
			}
			for (int i = 0; enemyTanks != null && i < enemyTanks.size(); i++) {
				tank = enemyTanks.get(i);
				if (isAlive && (tank.containsPoint(priorX, priorY) || 
						tank.containsPoint(x, y))) {
					tank.beShot();
					this.isAlive = false;
				}
			}			
			if (isAlive)
				isAlive = !outOfBorder();
		}
	}
	@SuppressWarnings("serial")
	private class BulletDraw extends JPanel{
		public void paint(Graphics g) {
			super.paint(g);
			g.setColor(Color.BLACK);
			g.fillOval(x-1, y-1, 3, 3);
		}
	}
	public void paint(Graphics g) {
		if (isAlive)
			bulletDraw.paint(g);
	}
	
	private boolean outOfBorder() {
		boolean isOutOfBorder = false;
		if (x < 0 || x > PANEL_WIDTH || y < 0 || y > PANEL_HEIGHT)
			isOutOfBorder = true;
		return isOutOfBorder;
	}
}
