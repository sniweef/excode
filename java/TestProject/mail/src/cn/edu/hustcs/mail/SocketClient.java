package cn.edu.hustcs.mail;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketClient {
	protected Socket socket = null;
	protected PrintWriter outToServer = null;
	protected BufferedReader inFromServer = null;
	protected String server = null;
	protected int port = -1;
	
	public SocketClient(String server, int port) {
		this.server = server;
		this.port = port;
	}
	public void setMailServer(String server) {
		this.server = server;
	}
	
	public boolean isConnected() {
		if (socket == null)
			return false;
		else
			return socket.isConnected();
	}

	public boolean connect() {
		if (!isConnected()) {
			try {
				socket = new Socket(server, port); // 创建Socket对象并连接到指定的域名和端口
				// 将SOCKET输入流连接到带缓冲功能的输入流BufferedReader，以便一次读一行来自服务器的应答报文
				inFromServer = new BufferedReader(new InputStreamReader(
						socket.getInputStream()));
				// 将SOCKET输出流连接到带缓冲功能的输出流PrintWriter，以便一次输出一行报文到服务器
				outToServer = new PrintWriter(
						socket.getOutputStream(), true);
				getResponse(); // 读取来自服务器的第一行应答，并显示在屏幕上
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return isConnected();
	}

	public void disconnect() {
		if (isConnected()) {
			try {
				inFromServer.close();
				outToServer.close();
				socket.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 从服务器读入一行应答，并打印显示
	 */
	public String getResponse() {
		try {
			String response = inFromServer.readLine(); // 从服务器读入一行应答
			//System.out.println("MAILServer: " + response); // 打印显示
			return response;
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * 向服务器发送命令,并打印显示
	 * 
	 * @param cmd
	 *            ：命令
	 */
	public void sendCommand(String cmd) {
		outToServer.println(cmd); // 向服务器发送命令
		System.out.println("Client: " + cmd); // 打印显示
	}
}
