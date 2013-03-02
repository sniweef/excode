/**
 * @author sniweef
 * @date 2012-11-22 下午4:42:18
 */

package com;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.*;

import com.panel.GamePanel;

@SuppressWarnings("serial")
public class TankGame extends JFrame {
	private GamePanel gamePanel;
	public static final int WIN_WIDTH = 800;
	public static final int WIN_HEIGHT = 600;
	public static final int PANEL_WIDTH = 600;
	public static final int PANEL_HEIGHT = 400;
	private Thread gameThread;
	private AePlayWave playWave;
	private JPanel currentPanel;
		
	public TankGame() {
		playWave = null;
		currentPanel = null;
		initWindow();
		addMenu();
	}
	private void initWindow() {
		this.setSize(WIN_WIDTH, WIN_HEIGHT);
		this.setLocationByPlatform(true);
		this.setVisible(true);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setResizable(false);
	}
	private void addMenu() {
		JMenuBar jmb = new JMenuBar();
		JMenu gameMenu = new JMenu("Game");
		JMenuItem startMI = new JMenuItem("Start");
		JMenuItem exitMI = new JMenuItem("Exit");
		//String a = "tst";
		
		
		startMI.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {	
				initPanel();
				setKeyListener();
			}			
		});
		exitMI.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
			
		});
		
		this.setJMenuBar(jmb);
		jmb.add(gameMenu);
		gameMenu.add(startMI);
		gameMenu.add(exitMI);
	}
	private void initPanel() {
		if (currentPanel != null)
			this.remove(currentPanel);
		playWave = new AePlayWave("sound/sound.wav");
		playWave.start();
		gamePanel = new GamePanel();
		this.add(gamePanel);
		currentPanel = gamePanel;
		gameThread = new Thread(gamePanel);
		gameThread.start();
		this.setVisible(true);
	}
	
	public void setKeyListener() {
		
		this.addKeyListener(new KeyListener() {
			@Override
			public void keyPressed(KeyEvent e) {
				int keyCode = e.getKeyCode();
				switch (keyCode) {
				case KeyEvent.VK_W:
					gamePanel.heroMoveUp();
					break;
				case KeyEvent.VK_S:
					gamePanel.heroMoveDown();
					break;
				case KeyEvent.VK_A:
					gamePanel.heroMoveLeft();
					break;
				case KeyEvent.VK_D:
					gamePanel.heroMoveRight();
					break;
				case KeyEvent.VK_J:
					gamePanel.heroShoot();
				default:
					break;
				}
				gamePanel.repaint();
			}

			@Override
			public void keyReleased(KeyEvent e) {
				
			}

			@Override
			public void keyTyped(KeyEvent e) {
				//System.out.println("typed:" + e.getKeyChar());
			}
			
		});		
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new TankGame();
	}
}
