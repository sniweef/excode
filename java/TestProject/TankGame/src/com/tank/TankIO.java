package com.tank;

import java.io.*;

public class TankIO {
	private final String fileName;
	
	public TankIO() {
		fileName = "tank.sav";
	}
	public TankIO(String fileName) {
		this.fileName = fileName;
	}
	public void save(Tank tank) {
		ObjectOutputStream oos = null;
		try {
			oos = new ObjectOutputStream(
					new FileOutputStream(fileName));
			oos.writeObject(tank);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				oos.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	public Tank read() {
		ObjectInputStream ois = null;
		Tank tank = null;
		try {
			ois = new ObjectInputStream(new FileInputStream(fileName));
			tank = (Tank) ois.readObject();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} finally {
			try {
				ois.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return tank;
	}
}
