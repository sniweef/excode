package cn.edu.hustcs.mail;

import java.util.List;

public abstract class ReceiveMail extends SocketClient {
	protected String username = null;
	protected String passwd = null;	
	
	public ReceiveMail(String mailServer, String username, String passwd,
			int port) {
		super(mailServer, port);
		this.username = username;
		this.passwd = passwd;
	}
	
	abstract boolean login();
	abstract boolean isLogin();
	abstract int getMailAmount(String mailBox);
	abstract Mail parseMailContent(String content);
	abstract List<Mail> getAllMails(String mailBox);
	abstract void logout();	
}
