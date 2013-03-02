package cn.edu.hustcs.mail;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Mail {
	private String mailSender = null;
	private String mailReceiver = null;	
	private String mailSubject = null;
	private String mailDate = null;
	private String mailContent = null;
	private String mailBox = null;

	public Mail() {

	}

	public void setMailSender(String mailSender) {
		this.mailSender = mailSender;
	}

	public void setMailReceiver(String mailReceiver) {
		this.mailReceiver = mailReceiver;
	}
	public void setMailSubject(String mailSubject) {
		this.mailSubject = mailSubject;
	}
	public void setMailDate(String mailDate) {
		this.mailDate = mailDate;
	}
	public void setMailContent(String mailContent) {
		this.mailContent = mailContent;
	}
	public void setMailBox(String mailBox) {
		this.mailBox = mailBox;
	}

	public String getMailSender() {
		return mailSender;
	}
	public String getMailReceiver() {
		return mailReceiver;
	}
	public String getMailSubject() {
		return mailSubject;
	}
	public String getMailDate() {
		return mailDate;
	}
	public String getMailContent() {
		return mailContent;
	}
	public String getMailBox(){
		return mailBox;
	}
	
	public void display() {
		System.out.println(mailReceiver);
		System.out.println(mailSender);
		System.out.println(mailDate);
		System.out.println(mailSubject);
		System.out.println(mailContent);
	}

	public void saveToFile(String dirPath) {
		SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMddHHmmss");
		Date curDate = new Date(System.currentTimeMillis());// 获取当前时间
		String filePath = dirPath + formatter.format(curDate);
		File dir = new File(dirPath);
		File file = new File(filePath);
		
		try {
			if (!(dir.isDirectory()))
				if (!dir.mkdirs())
					throw new Exception("Dir:"+dirPath+" create error!");
			if (!(file.exists()))
				if (!file.createNewFile())
					throw new Exception("File:"+filePath+" create error!");
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		String data = mailSender + "\n" + mailReceiver +"\n" + mailSubject +"\n" +
				mailDate +"\n" + mailContent;
		try {
			FileWriter writeToFile = new FileWriter(file, false);
			writeToFile.write(data);
			writeToFile.close();
		} catch (IOException e) {			
			e.printStackTrace();
		}
	}
	
	public void delete(File preFile, boolean deleteToTrash, String trashPath) {
		mailBox = "TRASH";
		if (preFile.isFile())
			preFile.delete();
		if (deleteToTrash)
			saveToFile(trashPath);
	}	
}
