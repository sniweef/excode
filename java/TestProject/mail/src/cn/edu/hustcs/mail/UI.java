package cn.edu.hustcs.mail;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.event.*;
import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class UI extends JFrame {
	private JButton RevJButton = new JButton("rev"); // 切换到接收邮件
	private JButton WrJButton = new JButton("wr");
	private JButton sendedJButton = new JButton("sended"); //++

	private JPanel jplbutJPanel = new JPanel();
	private CardLayout cardLayout = new CardLayout();
	private JPanel jplCardJPanel = new JPanel(cardLayout);

	private JPanel revJPanel = new JPanel();
	private JPanel revTJPanel = new JPanel();// 接收界面最顶层面板
	private JPanel wrJPanel = new JPanel();
	private JPanel toJPanel = new JPanel();
	private JPanel revOJPanel = new JPanel();

	private JLabel toJLabel = new JLabel("To :");
	private JLabel subJLabel = new JLabel("Subject :");
	private JTextField toJTextField = new JTextField();
	private JTextField subJTextField = new JTextField();
	private JButton sendJButton = new JButton("send");
	private JButton extraJButton = new JButton("add extra file");
	private JButton delJButton = new JButton("delete");
	private JRadioButton trjrbt = new JRadioButton("to trash");

	private JLabel nullJLabel = new JLabel("");

	private DefaultListModel listModel = new DefaultListModel();
	private JList lst = new JList(listModel);
	private JScrollPane lstJScrollPane = new JScrollPane(lst);

	private JTextArea revJTextArea = new JTextArea("rev");
	private JScrollPane revJScrollPane = new JScrollPane(revJTextArea);

	private JTextArea wrJTextArea = new JTextArea("write");
	private JScrollPane wrJScrollPane = new JScrollPane(wrJTextArea);

	private JMenuBar jmb = new JMenuBar();
	private JMenu logMenu = new JMenu("login");
	private JMenuItem logItem = new JMenuItem("login");

	private JPanel sendedJPanel = new JPanel(); //++
	private JPanel sendedTJPanel = new JPanel(); //++
	private JPanel sendedOJPanel = new JPanel(); //++
	private DefaultListModel sendedListModel = new DefaultListModel(); //++
	private JList sendedJList = new JList(sendedListModel);//++
	private JScrollPane sendedlstJScrollPane = new JScrollPane(sendedJList);//++
	private JTextArea sendedJTextArea = new JTextArea();//++
	private JScrollPane sendedcontentJPanel = new JScrollPane(sendedJTextArea);//++
	private JButton sendeddelJButton = new JButton("delete");//++
	
	private String userStr = new String();
	private String psStr = new String();
	private String smtpStr = new String();
	private String popStr = new String();
	private String toStr = new String();
	private String subStr = new String();
	private String contentStr = null;

	private final String INBOX_NAME = "INBOX";
	private final String SENT_NAME = "SENT";
	private final String TRASH_NAME = "TRASH";

	private SendMail sendMail = null;
	private ReceiveMail receiveMail = null;
	private final int POP3_PORT = 110;
	private final int SMTP_PORT = 25;
	private List<Mail> mailList = null;
	private File[] mailFiles = null;
	private List<Mail> sentMails = null;
	private List<Mail> trashMails = null;

	// private Vector<String> content = new Vector<String>(3);

	public UI() {
		setSize(800, 640);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setLocationRelativeTo(null);
		setLayout(new BorderLayout());
		setJMenuBar(jmb);
		setVisible(true);

		jmb.add(logMenu);
		logMenu.add(logItem);

		add(jplbutJPanel, BorderLayout.WEST);

		jplbutJPanel.setLayout(new GridLayout(15, 1, 2, 5));
		jplbutJPanel.setSize(100, 640);
		jplbutJPanel.add(RevJButton);
		jplbutJPanel.add(WrJButton);
		jplbutJPanel.add(sendedJButton); //++
		
		revTJPanel.setLayout(new BorderLayout());
		revTJPanel.setSize(700, 640);

		revTJPanel.add(revJPanel, BorderLayout.CENTER);
		revTJPanel.add(revOJPanel, BorderLayout.NORTH);
		revOJPanel.setSize(700, 40);
		revOJPanel.setLayout(new GridLayout(1, 4, 5, 5));
		revOJPanel.add(nullJLabel);
		revOJPanel.add(delJButton);
		revOJPanel.add(nullJLabel);
		revOJPanel.add(trjrbt);		
		
		sendedTJPanel.setSize(700, 640);
		sendedTJPanel.setLayout(new BorderLayout());
		sendedTJPanel.add(sendedOJPanel, BorderLayout.NORTH);
		sendedTJPanel.add(sendedJPanel,BorderLayout.CENTER);
		sendedOJPanel.setSize(700, 40);
		sendedOJPanel.setLayout(new GridLayout(1, 4, 5, 5));
		sendedOJPanel.add(sendeddelJButton);
		sendedOJPanel.add(nullJLabel);
		sendedJPanel.setSize(700, 600);
		sendedJPanel.setLayout(new BorderLayout());
		sendedlstJScrollPane.setSize(150, 600);
		sendedcontentJPanel.setSize(550, 600);
		sendedJPanel.add(sendedlstJScrollPane, BorderLayout.WEST);
		sendedJPanel.add(sendedcontentJPanel, BorderLayout.CENTER);
		sendedJTextArea.setLineWrap(true);
		sendedJTextArea.setWrapStyleWord(true);
		sendedJList.setFixedCellHeight(50);
		sendedJList.setFixedCellWidth(150);

		lstJScrollPane.setSize(150, 600);
		revJScrollPane.setSize(550, 600);
		revJTextArea.setLineWrap(true);
		revJTextArea.setWrapStyleWord(true);
		lst.setFixedCellHeight(50);
		lst.setFixedCellWidth(150);
		// listModel.addElement("item1");
		// listModel.addElement("item2");
		// listModel.addElement("item3");

		// content.add("item1");
		// content.add("item2");
		// content.add("item3");
		// content.add("item4");

		revJPanel.setLayout(new BorderLayout());
		revJPanel.add(lstJScrollPane, BorderLayout.WEST);
		revJPanel.add(revJScrollPane, BorderLayout.CENTER);

		wrJPanel.setLayout(new BorderLayout());
		toJPanel.setLayout(new GridLayout(3, 2, 5, 5));
		toJPanel.setSize(700, 80);
		toJPanel.add(toJLabel);
		toJPanel.add(toJTextField);
		toJPanel.add(subJLabel);
		toJPanel.add(subJTextField);

		toJPanel.add(sendJButton);
		toJPanel.add(extraJButton);
		wrJPanel.add(toJPanel, BorderLayout.NORTH);
		// opJPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));

		wrJScrollPane.setSize(700, 500);
		wrJPanel.add(wrJScrollPane, BorderLayout.CENTER);
		wrJTextArea.setLineWrap(true);
		wrJTextArea.setWrapStyleWord(true);

		add(jplCardJPanel, BorderLayout.CENTER);
		jplCardJPanel.setSize(700, 640);
		jplCardJPanel.add(revTJPanel, String.valueOf(1));
		jplCardJPanel.add(wrJPanel, String.valueOf(2));
		jplCardJPanel.add(sendedTJPanel, String.valueOf(3)); 
		
		WrJButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				cardLayout.show(jplCardJPanel, String.valueOf(2));

			}
		});
		sendedJButton.addActionListener(new ActionListener() { //++
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				cardLayout.last(jplCardJPanel);
				
			}
		});

		RevJButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				cardLayout.first(jplCardJPanel);
			}
		});

		trjrbt.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub

			}
		});

		sendJButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				Mail mail = new Mail();
				toStr = toJTextField.getText();
				subStr = subJTextField.getText();
				contentStr = wrJTextArea.getText();

				mail.setMailSender(getSender());
				mail.setMailReceiver(toStr);
				mail.setMailSubject(subStr);
				mail.setMailDate(getDate());
				mail.setMailContent(contentStr);
				mail.setMailBox(SENT_NAME);
				mail.display();
				if (sendMail.sendMail(mail)) {
					mail.saveToFile(userStr + "\\" + SENT_NAME + "\\");
					
				}					
				else { // send failed
					;
				}
			}
		});

		extraJButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub

			}
		});

		logItem.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				new Login();
			}
		});

		lst.addListSelectionListener(new ListSelectionListener() {
			@Override
			public void valueChanged(ListSelectionEvent e) {
				// TODO Auto-generated method stub
				int selection = lst.getSelectedIndex();
				Mail mail = null;
				
				if (selection >= 0) {
					mail = mailList.get(selection);
					if (mail != null) {
						String allContent = "Sender: " + mail.getMailSender() + "\n"
								+ "Subject: " + mail.getMailSubject() + "\n"
								+ "Date:" + mail.getMailDate() + "\n"
								+ "Content: \n" + mail.getMailContent();
						revJTextArea.setText(allContent);
					}	
				}
			}

		});
		sendedJList.addListSelectionListener(new ListSelectionListener() {
			@Override
			public void valueChanged(ListSelectionEvent e) {
				// TODO Auto-generated method stub
				int selection = sendedJList.getSelectedIndex();
				Mail mail = null;
				
				if (selection >= 0) {
					mail = sentMails.get(selection);
					if (mail != null) {
						String allContent = "Sender: " + mail.getMailSender() + "\n"
								+ "Subject: " + mail.getMailSubject() + "\n"
								+ "Date:" + mail.getMailDate() + "\n"
								+ "Content: \n" + mail.getMailContent();
						sendedJTextArea.setText(allContent);
					}	
				}
			}

		});

		delJButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				boolean deleteToTrash = trjrbt.isSelected();
				Mail preMail = null;
				int selection = lst.getSelectedIndex();
				String trashPath = userStr + "\\" + TRASH_NAME +"\\";
				
				if (!listModel.isEmpty()) {
					openMailFiles();
					
					preMail = mailList.remove(selection);
					preMail.delete(mailFiles[selection], deleteToTrash, trashPath);
					listModel.remove(selection);
					revJTextArea.setText(null);					
				}
			}
		});

	}

	private class Login extends JFrame implements Runnable  {
		private JPanel loginJPanel = new JPanel(new GridLayout(5, 2, 5, 5));
		private JLabel usrJLabel = new JLabel("user:");
		private JTextField usrJTextField = new JTextField("hustnetwork");
		private JLabel psJLabel = new JLabel("password:");
		private JTextField psJTextField = new JTextField("network123");
		private JLabel smtpJLabel = new JLabel("smtp server:");
		private JTextField smtpJTextField = new JTextField("smtp.163.com");
		private JLabel pop3JLabel = new JLabel("pop3 server:");
		private JTextField pop3JTextField = new JTextField("pop.163.com");

		private JButton okJButton = new JButton("ok");

		public Login() {
			add(loginJPanel);
			setSize(400, 240);
			setLocationRelativeTo(null);
			setDefaultCloseOperation(HIDE_ON_CLOSE);
			setTitle("Login");
			setVisible(true);

			loginJPanel.add(usrJLabel);
			loginJPanel.add(usrJTextField);
			loginJPanel.add(psJLabel);
			loginJPanel.add(psJTextField);
			loginJPanel.add(smtpJLabel);
			loginJPanel.add(smtpJTextField);
			loginJPanel.add(pop3JLabel);
			loginJPanel.add(pop3JTextField);
			loginJPanel.add(okJButton);

			okJButton.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent arg0) {
					// TODO Auto-generated method stub
					userStr = usrJTextField.getText();
					psStr = psJTextField.getText();
					smtpStr = smtpJTextField.getText();
					popStr = pop3JTextField.getText();

					receiveMail = new POP3Rec(popStr, userStr, psStr, POP3_PORT);
					sendMail = new SendMail(smtpStr, SMTP_PORT, userStr, psStr);

					if (receiveMail.login())
						getMail();
					else { // login failed

					}
					setVisible(false);
				}
			});
		}
		public void run() {
			mailList = receiveMail.getAllMails(INBOX_NAME);
			sentMails = receiveMail.getAllMails(SENT_NAME);
			trashMails = receiveMail.getAllMails(TRASH_NAME);
			for (Mail mail : mailList)
				listModel.addElement(mail.getMailSubject());
			for (Mail mail : sentMails)
				sendedListModel.addElement(mail.getMailSubject());
		}

		private void getMail() {
			new Thread(this).start();
		}

	}

	private String getSender() {
		int index = smtpStr.indexOf(".");
		String sender = userStr + "@" + smtpStr.substring(index + 1);
		System.out.println("sender:" + sender);
		return sender;
	}

	private String getDate() {
		SimpleDateFormat sDateFormat = new SimpleDateFormat(
				"yyyy-MM-dd hh:mm:ss");
		String date = sDateFormat.format(new Date());
		System.out.println("Date:" + date);
		return date;
	}

	private void openMailFiles() {
		String dirPath = userStr + "\\" + INBOX_NAME + "\\";
		File dir = new File(dirPath);
		if (dir.isDirectory())
			mailFiles = dir.listFiles();
	}
	
}
