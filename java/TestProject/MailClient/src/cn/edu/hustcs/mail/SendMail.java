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
				// ���û����ʺź�������BASE64��ʽ���б����Ա���з����������֤
				BASE64Encoder encoder = new BASE64Encoder();
				String encodedUserName = encoder.encode(username.getBytes());
				String encodedPwd = encoder.encode(passwd.getBytes());

				// �ͷ������Ự������EHLO hostname����
				String hostName = InetAddress.getLocalHost().getHostName(); // �õ�����������
				sendCommand("EHLO " + hostName); // ����EHLO hostname���������
				// �������Է�������5��Ӧ�𣬲���ʾ����Ļ��
				for (int i = 0; i < 5; i++) {
					getResponse();
				}

				// �ͷ������Ự������AUTH LOGIN������������֤
				sendCommand("AUTH LOGIN"); // �����������AUTH LOGIN����
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();

				// ������������Լ��ľ���BASE64������ʺ�
				sendCommand(encodedUserName); // ����������;���BASE64������ʺ�
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();

				// ������������Լ�������
				sendCommand(encodedPwd); // ����������;���BASE64���������
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();

				// �����������MAIL FROM: �����˵�ַ
				sendCommand("MAIL FROM: <" + mail.getMailSender() + ">");// �����������MAIL
																	// FROM: �����˵�ַ
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();

				// �����������RCPT TO: �ռ��˵�ַ
				sendCommand("RCPT TO: <" + mail.getMailReceiver() + ">");// �����������RCPT
																	// TO: �ռ��˵�ַ
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();

				// �������ʼ�����
				sendCommand("DATA"); // ����������͡�DATA������������ʼ�����
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				getResponse();
				// ��ʼ�����ʼ�����
				sendCommand("Subject: "+mail.getMailSubject()+"\n\n");
				sendCommand(mail.getMailContent());
				getResponse();
				
				// �����ʼ�������־
				sendCommand(".");
				getResponse();
				
				sendCommand("QUIT");
				// �������Է�������Ӧ�𣬲���ʾ����Ļ��
				String result = getResponse();
				
				return result.startsWith("250");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
}
