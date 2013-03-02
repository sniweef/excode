package cn.edu.hustcs.mail;

import java.io.IOException;
import java.net.InetAddress;

import sun.misc.BASE64Encoder;
import sun.security.jgss.LoginConfigImpl;

public class SendMail extends SocketClient{
	private String username = null;
	private String passwd = null;

	public SendMail(String mailServer, int port, String username,
			String passwd) {
		super(mailServer, port);
		this.username = username;
		this.passwd = passwd;
		
	}

	

	public boolean sendMail(Mail mail) {
		try {
			connect();
			if (isConnected()) {		
				// 将用户的帐号和密码以BASE64格式进行编码以便进行服务器身份验证
				BASE64Encoder encoder = new BASE64Encoder();
				String encodedUserName = encoder.encode(username.getBytes());
				String encodedPwd = encoder.encode(passwd.getBytes());

				// 和服务器会话，发送EHLO hostname命令
				String hostName = InetAddress.getLocalHost().getHostName(); // 得到本机主机名
				sendCommand("EHLO " + hostName); // 发送EHLO hostname命令到服务器
				// 读入来自服务器的5行应答，并显示在屏幕上
				for (int i = 0; i < 5; i++) {
					getResponse();
				}

				// 和服务器会话，发送AUTH LOGIN命令，请求身份验证
				sendCommand("AUTH LOGIN"); // 向服务器发送AUTH LOGIN命令
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();

				// 向服务器发送自己的经过BASE64编码的帐号
				sendCommand(encodedUserName); // 向服务器发送经过BASE64编码的帐号
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();

				// 向服务器发送自己的密码
				sendCommand(encodedPwd); // 向服务器发送经过BASE64编码的密码
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();

				// 向服务器发送MAIL FROM: 发件人地址
				sendCommand("MAIL FROM: <" + mail.getMailSender() + ">");// 向服务器发送MAIL
																	// FROM: 发件人地址
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();

				// 向服务器发送RCPT TO: 收件人地址
				sendCommand("RCPT TO: <" + mail.getMailReceiver() + ">");// 向服务器发送RCPT
																	// TO: 收件人地址
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();

				// 请求发送邮件正文
				sendCommand("DATA"); // 向服务器发送“DATA”命令，请求发送邮件正文
				// 读入来自服务器的应答，并显示在屏幕上
				getResponse();
				// 开始发送邮件正文
				sendCommand("Subject: "+mail.getMailSubject()+"\n\n");
				sendCommand(mail.getMailContent());
				getResponse();
				
				// 发送邮件结束标志
				sendCommand(".");
				getResponse();
				
				sendCommand("QUIT");
				// 读入来自服务器的应答，并显示在屏幕上
				String result = getResponse();
				
				return result.startsWith("250");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
}
