package cn.edu.hustcs.mail;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import java.util.ArrayList;
import java.util.List;


public class POP3Rec extends ReceiveMail {

	public POP3Rec(String mailServer, String username, String passwd, int port) {
		super(mailServer, username, passwd, port);		
	}

	@Override
	boolean login() {
		connect();
		if (isConnected()) {
			sendCommand("USER " + username);
			getResponse();
			sendCommand("PASS " + passwd);
			
			String result = getResponse();		
			return result.startsWith("+OK");
		}
		return false;		
		
	}
	@Override
	boolean isLogin() {
		sendCommand("STAT");
		String response = getResponse();		
		
		return response.startsWith("+OK");
	}

	int getLocalMailAmount(String mailBox) {
		File mailDir = new File(username+"\\"+mailBox+"\\");
		
		if (!mailDir.isDirectory())
			return 0;
		
		File[] mailFiles = mailDir.listFiles();	
		if (mailFiles == null)
			return 0;
		return mailFiles.length;
	}
	@Override
	int getMailAmount(String mailBox) {
		if (!mailBox.equalsIgnoreCase("INBOX"))
			return getLocalMailAmount(mailBox);
		sendCommand("STAT");
		String[] tmp = getResponse().split(" ");
		int amount = Integer.parseInt(tmp[1]);//得到信箱中共有多少封邮件
		
		return amount;
	}
	@Override
	Mail parseMailContent(String content) {
		return null;
	}
	
	private List<Mail> getLocalMails(String mailBox) {		
		File mailDir = new File(username+"\\"+mailBox+"\\");
		
		if (!mailDir.isDirectory())
			return null;
		
		File[] mailFiles = mailDir.listFiles();
		if (mailFiles == null)
			return null;
		
		int mailAmount = mailFiles.length;
		List<Mail> mailList = new ArrayList<Mail>(mailAmount);
		Mail mail;
		FileReader fReader;
		BufferedReader bReader;
		String tmp;
		String text = "";
		
		for (File mailFile:mailFiles) {
			try {
				fReader = new FileReader(mailFile);
				bReader = new BufferedReader(fReader);
				mail = new Mail();
				mail.setMailSender(bReader.readLine());
				mail.setMailReceiver(bReader.readLine());
				mail.setMailSubject(bReader.readLine());
				mail.setMailDate(bReader.readLine());				
				while ((tmp=bReader.readLine()) != null)
					text = text.concat(tmp+"\n");
				mail.setMailContent(text);
				mail.setMailBox(mailBox);
				mailList.add(mail);
				
				mail.display();
				fReader.close();
				bReader.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return mailList;
	}
	private void delLocalRecMails() {
		String path = username +"\\INBOX\\";
		File inboxDir = new File(path);
		if (inboxDir.isDirectory()) {
			File[] files = inboxDir.listFiles();
			if (files != null) {
				for (File file:files) {
					file.delete();
				}
			}
		}
	}
	private String getSRName(String response, int nameIndex) {
		int beginIndex, endIndex;
		
		beginIndex = response.indexOf('<');
		endIndex = response.indexOf('>');
		
		if (beginIndex > 0 && endIndex > 0)
			return response.substring(beginIndex+1, endIndex);
		else
			return response.substring(nameIndex);
	}
	@Override
	List<Mail> getAllMails(String mailBox) {
		connect();
		if (!isConnected()) {
			return getLocalMails(mailBox);
		}
		
		if (!mailBox.equalsIgnoreCase("INBOX"))
			return getLocalMails(mailBox);
		if (!isLogin())
			login();
		
		int mailAmount = getMailAmount(mailBox);
		if (mailAmount == 0)
			return null;
		
		String response = null;
		List<Mail> mailList = new ArrayList<Mail>(mailAmount);
		Mail mail;
		
		String text;
		delLocalRecMails();
		
		for (int i = 0; i < mailAmount; i++) {
			mail = new Mail();
			text = null;
			sendCommand("RETR "+(i+1));
			do {
				response = getResponse();
				if (response.indexOf("From") >= 0) 					
					mail.setMailSender(getSRName(response, 6));
				else if (response.indexOf("To") >= 0)
					mail.setMailReceiver(getSRName(response, 4));
				else if (response.indexOf("Subject") >= 0)
					mail.setMailSubject(response.substring(9));
				else if (response.indexOf("Date") >= 0)
					mail.setMailDate(response.substring(6));
				else if (response.equalsIgnoreCase("")) {
					//开始读取邮件正文
					while(!(response = getResponse()).equalsIgnoreCase(".")) {						
						if (text == null)
							text = response;
						else
							text = text.concat(response);
						text = text.concat("\n");
					}
					mail.setMailContent(text);
				}			
			} while(!response.equalsIgnoreCase("."));	
			mail.setMailBox(mailBox);
			mail.saveToFile(username+"\\"+mailBox+"\\");
			mailList.add(mail);
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		logout();
		return mailList;
	}

	@Override
	void logout() {
		sendCommand("QUIT");
	}
	
}
