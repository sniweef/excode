package cn.edu.hustcs.mail;

import java.util.HashMap;
import java.util.List;

public class IMAPRec extends ReceiveMail {

	HashMap<String, String> mailBoxMap;

	public IMAPRec(String mailServer, String username, String passwd, int port) {
		super(mailServer, username, passwd, port);
		mailBoxMap = new HashMap<String, String>();
	}

	private void setMailBoxMap(String response) {
		int beginIndex, endIndex;
		String key, value;

		beginIndex = response.indexOf('\\');
		endIndex = response.indexOf(')');

		if (beginIndex > 0 && endIndex > beginIndex) {
			key = response.substring(beginIndex + 1, endIndex);
			endIndex = response.lastIndexOf('\"');
			beginIndex = response.lastIndexOf('\"', endIndex - 1);
			value = response.substring(beginIndex + 1, endIndex);
			mailBoxMap.put(key, value);
		}
	}	

	@Override
	boolean login() {
		String response;
		getResponse();

		sendCommand("A001 LOGIN " + username + " " + passwd);
		response = getResponse();
		if (response.startsWith("A001 OK")) {
			sendCommand("A101 LIST \"\" *");
			while (!(response = getResponse()).startsWith("A101 OK")) {
				setMailBoxMap(response);
			}	

			return true;
		} else {
			return false;
		}

	}
	@Override
	boolean isLogin() {
		return false;
	}
	
	@Override
	Mail parseMailContent(String content) {

		return null;
	}

	private String getMailBoxName(String key) {
		if (key.equalsIgnoreCase("INBOX"))
			return key;
		else if(mailBoxMap != null)
			return mailBoxMap.get(key);	
		else
			return null;
	}
	@Override
	int getMailAmount(String mailBox) {
		sendCommand("A341 CLOSE");
		getResponse();
		sendCommand("A002 SELECT " + getMailBoxName(mailBox));
		String[] tmp = getResponse().split(" ");
		int amount = Integer.valueOf(tmp[1]);

		return amount;
	}

	@Override
	List<Mail> getAllMails(String mailBox) {
		//int mailAmount = getMailAmount(mailBox);
		
		return null;
	}

	@Override
	void logout() {
		sendCommand("A023 LOGOUT");
	}
}
