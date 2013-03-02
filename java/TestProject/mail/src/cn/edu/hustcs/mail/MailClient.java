package cn.edu.hustcs.mail;


import java.util.List;

public class MailClient {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
//		ReceiveMail rMail = new POP3Rec("pop.163.com", "hustnetwork", "network123", 110);
//		List<Mail> mails;
//		rMail.connect();
//		if (rMail.isConnected()) {
//			mails = rMail.getAllMails("INBOX");
//			for (Mail mail:mails) {
//				mail.display();
//			}
//		}	
//		
//		ReceiveMail rMail = new IMAPRec("imap.163.com", "hustnetwork", "network123", 143);
//		rMail.connect();
//		if (rMail.isConnected()) {
//			rMail.login();
//		}
		
//		Mail mail = new Mail();
//		mail.setMailBox("INBOX");
//		mail.setMailSender("lijian");
//		mail.setMailReceiver("hiswife");
//		mail.setMailContent("***");
//		mail.saveToFile(".\\");
//		
//		POP3Rec rMail = new POP3Rec(null, "hzhigeng", null, 0);
//		List <Mail> listMails = rMail.getLocalMails("INBOX");
//		
//		for (Mail mail:listMails) {
//			mail.display();
//		}
		
//		ReceiveMail rMail = new POP3Rec("pop.163.com", "hustnetwork", "network123", 110);
//		List<Mail> mailList;
//		rMail.connect();
//		if (rMail.isConnected()) {
//			rMail.login();
//			mailList = rMail.getAllMails("INBOX");
//			if (mailList == null)
//				return ;
//			for (Mail mail:mailList) {
//				mail.display();
//			}
//		}
		
		new UI();
	}

}
