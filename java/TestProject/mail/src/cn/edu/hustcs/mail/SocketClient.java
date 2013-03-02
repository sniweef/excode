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
				socket = new Socket(server, port); // ����Socket�������ӵ�ָ���������Ͷ˿�
				// ��SOCKET���������ӵ������幦�ܵ�������BufferedReader���Ա�һ�ζ�һ�����Է�������Ӧ����
				inFromServer = new BufferedReader(new InputStreamReader(
						socket.getInputStream()));
				// ��SOCKET��������ӵ������幦�ܵ������PrintWriter���Ա�һ�����һ�б��ĵ�������
				outToServer = new PrintWriter(
						socket.getOutputStream(), true);
				getResponse(); // ��ȡ���Է������ĵ�һ��Ӧ�𣬲���ʾ����Ļ��
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
	 * �ӷ���������һ��Ӧ�𣬲���ӡ��ʾ
	 */
	public String getResponse() {
		try {
			String response = inFromServer.readLine(); // �ӷ���������һ��Ӧ��
			//System.out.println("MAILServer: " + response); // ��ӡ��ʾ
			return response;
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * ���������������,����ӡ��ʾ
	 * 
	 * @param cmd
	 *            ������
	 */
	public void sendCommand(String cmd) {
		outToServer.println(cmd); // ���������������
		System.out.println("Client: " + cmd); // ��ӡ��ʾ
	}
}
