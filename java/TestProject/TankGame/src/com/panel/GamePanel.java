package com.panel;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Arrays;
import java.util.Vector;
import java.util.concurrent.TimeUnit;
import javax.swing.JPanel;
import com.TankGame;
import com.tank.*;

@SuppressWarnings("serial")
public class GamePanel extends JPanel implements Runnable {
	private final int ENEMY_AMOUNT = 5;
	private Tank heroTank;
	private Vector<Tank> enemyTanks;
	private Vector<Bombing> bombings;
	private Vector<Block> blocks;
	private int width;
	private int height;
	
	public GamePanel() {
		int i;
		Vector<Tank> heroTanks;
		EnemyTank nomalEnemyTank;
		EnemyTank strongEnemyTank;
		
		width = TankGame.PANEL_WIDTH;
		height = TankGame.PANEL_HEIGHT;
		
		blocks = new Vector<Block>();
		heroTank = new HeroTank(0, 0);
		heroTank.setBlocks(blocks);
		
		blocks.add(heroTank);
		heroTank.setDirection(Direction.DOWN);
		heroTanks = new Vector<Tank>(Arrays.asList(heroTank));
		enemyTanks = new Vector<Tank>(ENEMY_AMOUNT);		
		
		for (i = 0; i < ENEMY_AMOUNT - 1; i++) {
			nomalEnemyTank = new NomalEnemyTank(i*100, TankGame.PANEL_HEIGHT-40);
			nomalEnemyTank.setBlocks(blocks);
			nomalEnemyTank.setEnemyTanks(heroTanks);
			nomalEnemyTank.start();

			blocks.add(nomalEnemyTank);
			enemyTanks.add(nomalEnemyTank);			
		}
		heroTank.setEnemyTanks(enemyTanks);
		
		strongEnemyTank = new StrongEnemyTank(i*100, TankGame.PANEL_HEIGHT-40);
		strongEnemyTank.setEnemyTanks(heroTanks);
		strongEnemyTank.setBlocks(blocks);
		strongEnemyTank.start();
		enemyTanks.add(strongEnemyTank);
		blocks.add(strongEnemyTank);
		
		bombings = new Vector<Bombing>();
	}
	
	/**
	 * @return the heroTank
	 */
	public Tank getHeroTank() {
		return heroTank;
	}
	
	/**
	 * @return the enemyTanks
	 */
	public Vector<Tank> getEnemyTanks() {
		return enemyTanks;
	}
	
	public void heroMoveUp() {
		if (heroTank != null)
			heroTank.moveUp();
	}
	public void heroMoveDown() {
		if (heroTank != null)
			heroTank.moveDown();
	}
	public void heroMoveLeft() {
		if (heroTank != null)
			heroTank.moveLeft();
	}
	public void heroMoveRight() {
		if (heroTank != null)
			heroTank.moveRight();
	}
	public void heroShoot() {
		heroTank.shoot();
	}
	public void showTankInfo(Graphics g) {
		Tank heroTankInfo = new HeroTank(630, 100);
		EnemyTank enemyTankInfo = new EnemyTank(630, 150);
		
		heroTankInfo.paint(g);
		if (heroTank != null)
			g.drawString(heroTank.getLife()+"", 670, 118);
		else
			g.drawString("0", 670, 118);
		
		enemyTankInfo.paint(g);
		if (enemyTanks != null)
			g.drawString(enemyTanks.size()+"", 670, 168);
		else
			g.drawString("0", 670, 168);
	}
	
	public void paint(Graphics g) {
		super.paint(g);
		Tank tank;
		
		g.setColor(Color.BLUE);
		g.fillRect(0, 0, width, height);
		if (heroTank != null) {
			heroTank.paint(g);
			if (!heroTank.isAlive()) {
				bombings.add(new Bombing(heroTank.getX(), heroTank.getY()));
				heroTank = null;
			}
		}	
			
		for (int i = 0; i < enemyTanks.size(); i++) {		
			tank = enemyTanks.get(i);
			tank.paint(g);
			if (!tank.isAlive()) {
				bombings.add(new Bombing(tank.getX(), tank.getY()));
				enemyTanks.remove(tank);
				blocks.remove(tank);
			}
			if (heroTank == null)
				tank.setEnemyTanks(null);
		}
		
		for (int i = 0; i < bombings.size(); i++)
			bombings.get(i).paint(g);
		showTankInfo(g);
	}

	@Override
	public void run() {
		while (true) {
			try {
				TimeUnit.MILLISECONDS.sleep(80);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			this.repaint();
		}
	}
}