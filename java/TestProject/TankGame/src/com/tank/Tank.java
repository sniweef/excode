/**
 * 
 */
package com.tank;

import java.awt.Color;
import java.awt.Graphics;
import java.util.*;

import javax.swing.JPanel;
import com.TankGame;

public abstract class Tank implements Block {
	private final int PANEL_WIDTH = TankGame.PANEL_WIDTH;
	private final int PANEL_HEIGHT = TankGame.PANEL_HEIGHT;
	private final int SHOOT_INTERVAL = 500; // ���������Ժ���Ϊ��λ
	private int x;
	private int y;
	private Vector<Block> blocks;
	//private Thread tankThread;
	private long priorShootTime;
	private Direction direction;
	private Vector<Bullet> bulletList;
	protected Vector<Tank> enemyTanks;
	protected TankDraw tankDraw;	
	protected int speed;
	protected int bulletSpeed;
	protected int life;

	public Tank(int x, int y) {
		this.x = x;
		this.y = y;
		this.blocks = null;
		this.direction = Direction.UP;
		this.speed = 0;
		this.bulletSpeed = 30;
		this.tankDraw = null;
		this.life = 1;
		this.priorShootTime = 0;
		this.enemyTanks = null;
		this.bulletList = new Vector<Bullet>();
		//tankThread = new Thread(this);
		//tankThread.start();
	}
	/**
	 * @return the x
	 */
	public int getX() {
		return x;
	}

	/**
	 * @return the y
	 */
	public int getY() {
		return y;
	}

	/**
	 * @return the speed
	 */
	public int getSpeed() {
		return speed;
	}

	/**
	 * @return the tankDraw
	 */
	public TankDraw getTankDraw() {
		return tankDraw;
	}

	/**
	 * @return the direction
	 */
	public Direction getDirection() {
		return direction;
	}
	public Vector<Tank> getEnemyTanks() {
		return enemyTanks;
	}
	/**
	 * @return is life larger than 0
	 */
	public boolean isAlive() {
		return life > 0;
	}
	
	/**
	 * @return the life
	 */
	public int getLife() {
		return life;
	}
	
	/**
	 * @param isAlive
	 *            the isAlive to set
	 */
	public void setLife(int life) {
		this.life = life;
	}
	/**
	 * @param x
	 *            the x to set
	 */
	public void setX(int x) {
		this.x = x;
	}

	/**
	 * @param y
	 *            the y to set
	 */
	public void setY(int y) {
		this.y = y;
	}

	/**
	 * @param blocks the blocks to set
	 */
	public void setBlocks(Vector<Block> blocks) {
		this.blocks = blocks;
	}
	/**
	 * @param speed
	 *            the speed to set
	 */
	public void setSpeed(int speed) {
		this.speed = speed;
	}

	/**
	 * @param tankDraw
	 *            the tankDraw to set
	 */
	public void setTankDraw(TankDraw tankDraw) {
		this.tankDraw = tankDraw;
	}

	public void setDirection(Direction direction) {
		this.direction = direction;
	}
	public void setEnemyTanks(Vector<Tank> enemyTanks) {
		this.enemyTanks = enemyTanks;
	}
	@Override
	public boolean containsPoint(int x, int y) {
		boolean result = false; 
		result = (this.x <= x ) && (this.y <= y); 
		if (direction == Direction.UP || direction == Direction.DOWN) {
			result = result && (x <= this.x + tankDraw.getTankWidth())
					&& (y <= this.y + tankDraw.getTankHeight());			
		} else {
			result = result && (x <= this.x + tankDraw.getTankHeight()) 
					&& (y <= this.y + tankDraw.getTankWidth());
		}
		return result;
	}
	@Override
	public int getUpBorderY() {
		return y;
	}
	@Override
	public int getDownBorderY() {
		int borderY;
		switch (direction) {
		case UP:
		case DOWN:
			borderY = y + tankDraw.getTankHeight(); 
			break;
		case LEFT:
		case RIGHT:
			borderY = y + tankDraw.getTankWidth();
			break;
		default:
			borderY = 0;
		}
		return borderY;
	}
	@Override
	public int getLeftBorderX() {
		return x;
	}
	@Override
	public int getRightBorderX() {
		int borderX;
		switch (direction) {
		case UP:
		case DOWN:
			borderX = x + tankDraw.getTankWidth();
			break;
		case LEFT:
		case RIGHT:
			borderX = x + tankDraw.getTankHeight();
			break;
		default:
			borderX = 0;
		}
		return borderX;
	}
	public boolean hasBlock(int x, int y) {
		int rightX;
		int downY;
		boolean isBlock = false;
		
		switch (direction) {
		case UP:
			rightX = x + tankDraw.getTankWidth();
			for (Block block : blocks) {
				if (this == block)
					continue;
				if (block.containsPoint(x, y) || 
						block.containsPoint(rightX, y))  {
					this.y = block.getDownBorderY() + 1;
					isBlock = true;
				}
			}
			break;
		case DOWN:
			downY = y + tankDraw.getTankHeight();
			rightX = x + tankDraw.getTankWidth();
			for (Block block : blocks) {
				if (this == block)
					continue;
				if (block.containsPoint(x, downY) || 
						block.containsPoint(rightX, downY))  {
					this.y = block.getUpBorderY() - tankDraw.getTankHeight() - 1;
					isBlock = true;
				}
			}
			break;
		case LEFT:
			downY = y + tankDraw.getTankWidth();
			for (Block block : blocks) {
				if (this == block)
					continue;
				if (block.containsPoint(x, y) || 
						block.containsPoint(x, downY)) {
					this.x = block.getRightBorderX() + 1;
					isBlock = true;
				}
			}
			break;
		case RIGHT:
			rightX = x + tankDraw.getTankHeight();
			downY = y + tankDraw.getTankWidth();
			for (Block block : blocks) {
				if (this == block)
					continue;
				if (block.containsPoint(rightX, y) || 
						block.containsPoint(rightX, downY)) {
					this.x = block.getLeftBorderX() - tankDraw.getTankHeight() - 1;
					isBlock = true;
				}
			}
			break;
		default:
			break;
		}
		return isBlock;
	}
	
	public void moveUp() {
		int priorY = getY();
		Direction priorDirection = getDirection();
		setDirection(Direction.UP);
		if (priorDirection == Direction.UP) {
			if (!hasBlock(x, priorY-getSpeed())) 
				setY(priorY - getSpeed());
			if (tankDraw.getLineEndY() < 0) {
				setY(0);
			}
		}
	}

	public void moveDown() {
		int priorY = getY();
		Direction priorDirection = getDirection();
		setDirection(Direction.DOWN);
		if (priorDirection == Direction.DOWN) {
			if (!hasBlock(x, priorY+getSpeed())) 
				setY(priorY + getSpeed());
			if (tankDraw.getLineEndY() > PANEL_HEIGHT) {
				setY(PANEL_HEIGHT - tankDraw.getTankHeight());
			}
		}
	}

	public void moveLeft() {
		int priorX = getX();
		Direction priorDirection = getDirection();
		setDirection(Direction.LEFT);

		if (priorDirection == Direction.LEFT) {
			if (!hasBlock(priorX-getSpeed(), y)) 
				setX(priorX - getSpeed());
			if (tankDraw.getLineEndX() < 0) {
				setX(0);
			}
		}

	}

	public void moveRight() {
		int priorX = getX();
		Direction priorDirection = getDirection();
		setDirection(Direction.RIGHT);
		if (priorDirection == Direction.RIGHT) {
			if (!hasBlock(priorX+getSpeed(), y)) 
				setX(priorX + getSpeed());
			if (tankDraw.getLineEndX() > PANEL_WIDTH) {
				setX(PANEL_WIDTH - tankDraw.getTankHeight());
			}
		}
	}

	public void shoot() {
		long currentShootTime = new Date().getTime();

		if (priorShootTime == 0
				|| currentShootTime - priorShootTime > SHOOT_INTERVAL) {
			// System.out.println("Shoot");
			int x = tankDraw.getLineEndX();
			int y = tankDraw.getLineEndY();
			Bullet newBullet = new Bullet(this, x, y, bulletSpeed, direction);
			bulletList.add(newBullet);
			priorShootTime = currentShootTime;
		}
	}
	public void beShot() {
		this.life--;
	}

	public void paint(Graphics g) {
		TankDraw tankDraw = getTankDraw();
		Bullet bullet;

		if (tankDraw != null && this.isAlive()) {
				tankDraw.paint(g);			
		}
		for (int i = 0; bulletList != null && i < bulletList.size(); i++) {
			bullet = bulletList.get(i);
			bullet.paint(g);
			if (!bullet.isAlive()) {
				bulletList.remove(bullet);
			}
		}
	}


	@SuppressWarnings("serial")
	protected abstract class TankDraw extends JPanel {
		// ���±�����Ϊ̹�������ϻ��µ���Ӧ�������̹����������ң���͸ߵ�ֵӦ���� 
		// ̹�˵Ŀ��������ͬ����Ȼ�ڱ任�����ʱ������һЩ����
		protected int leftRectWidth = 10;
		protected int leftRectHeight = 36;
		protected int midRectWidth = 16;
		protected int midRectHeight = 20;
		protected int ovalDiameter = 10;
		protected int lineLength = leftRectHeight / 2;
		protected Graphics g = null;

		public Graphics getGraphics() {
			return g;
		}
		public int getTankWidth() {
			return 2 * leftRectWidth + midRectWidth;
		}
		public int getTankHeight() {
			return leftRectHeight;
		}

		protected int getMidRectX() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return x + leftRectWidth;
			else
				return x + leftRectHeight / 2 - midRectHeight / 2;
		}

		protected int getMidRectY() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return y + (leftRectHeight - midRectHeight) / 2;
			else
				return y + leftRectWidth;
		}

		protected int getOvalX() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return x + leftRectWidth + midRectWidth / 2 - ovalDiameter / 2;
			else
				return x + leftRectHeight / 2 - ovalDiameter / 2;
		}

		protected int getOvalY() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return y + leftRectHeight / 2 - ovalDiameter / 2;
			else
				return y + leftRectWidth + midRectWidth / 2 - ovalDiameter / 2;
		}

		protected int getRightRectX() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return x + leftRectWidth + midRectWidth;
			else
				return x;
		}

		protected int getRightRectY() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return y;
			else
				return y + leftRectWidth + midRectWidth;
		}

		protected int getLineBeginX() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return x + leftRectWidth + midRectWidth / 2;
			else
				return x + leftRectHeight / 2;
		}

		protected int getLineBeginY() {
			if (direction == Direction.UP || direction == Direction.DOWN)
				return y + leftRectHeight / 2;
			else
				return y + leftRectWidth + midRectWidth / 2;
		}

		protected abstract Color getColor();

		protected int getLineEndX() {
			int endX = 0;
			switch (direction) {
			case UP:
			case DOWN:
				endX = getLineBeginX();
				break;
			case LEFT:
				endX = getLineBeginX() - lineLength;
				break;
			case RIGHT:
				endX = getLineBeginX() + lineLength;
				break;
			default:
				break;
			}
			return endX;
		}

		protected int getLineEndY() {
			int endY = 0;
			switch (direction) {
			case UP:
				endY = getLineBeginY() - lineLength;
				break;
			case DOWN:
				endY = getLineBeginY() + lineLength;
				break;
			case LEFT:
			case RIGHT:
				endY = getLineBeginY();
				break;
			default:
				break;
			}
			return endY;
		}

		protected void drawBody() {
			if (direction == Direction.UP || direction == Direction.DOWN) {
				g.fillRect(x, y, leftRectWidth, leftRectHeight);
				g.fillRect(getMidRectX(), getMidRectY(), midRectWidth,
						midRectHeight);
				g.fillRect(getRightRectX(), getRightRectY(), leftRectWidth,
						leftRectHeight);
			} else { // ��͸ߵĲ���Ի�
				g.fillRect(x, y, leftRectHeight, leftRectWidth);
				g.fillRect(getMidRectX(), getMidRectY(), midRectHeight,
						midRectWidth);
				g.fillRect(getRightRectX(), getRightRectY(), leftRectHeight,
						leftRectWidth);
			}
			g.setColor(Color.DARK_GRAY);

			g.drawLine(getLineBeginX(), getLineBeginY(), getLineEndX(),
					getLineEndY());
			g.fillOval(getOvalX(), getOvalY(), ovalDiameter, ovalDiameter);

		}

		public void paint(Graphics g) {
			this.g = g;
			super.paint(g);
			g.setColor(getColor());
			drawBody();
		}
	}

}
