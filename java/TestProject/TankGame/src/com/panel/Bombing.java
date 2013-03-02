package com.panel;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.JPanel;

@SuppressWarnings("serial")
public class Bombing extends JPanel {
	private final int BOMB_WIDTH = 50;
	private final int BOMB_HEIGHT = 40;
	private int x;
	private int y;
	private int life;
	private Image[] bombingImage = new Image[3];
	
	public Bombing(int x, int y) {
		this.x = x;
		this.y = y;
		this.life = 9;
		
		bombingImage[0] = Toolkit.getDefaultToolkit().getImage("img/bomb_1.gif");
		bombingImage[1] = Toolkit.getDefaultToolkit().getImage("img/bomb_2.gif");
		bombingImage[2] = Toolkit.getDefaultToolkit().getImage("img/bomb_3.gif");
	}
	public void paint(Graphics g) {
		super.paint(g);
		if (life > 6) {
			g.drawImage(bombingImage[0], x, y, BOMB_WIDTH, BOMB_HEIGHT, this);
		} else if (life > 3) {
			g.drawImage(bombingImage[1], x, y, BOMB_WIDTH, BOMB_HEIGHT, this);
		} else if (life > 0) {
			g.drawImage(bombingImage[2], x, y, BOMB_WIDTH, BOMB_HEIGHT, this);
		}
		life--;
	}
}
