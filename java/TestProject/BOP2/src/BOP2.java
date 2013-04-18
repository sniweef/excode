import java.util.HashMap;
import java.util.Scanner;

public class BOP2 {

	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		int dataGroups = scanner.nextInt();
		Object[] hashmapsObjects = new Object[dataGroups];
		StringBuilder[] results = new StringBuilder[dataGroups];
		
		
		for (int i = 0; i < dataGroups; i++) {
			int wordsAmount;
			int peopleAmount = scanner.nextInt(); 
			wordsAmount = scanner.nextInt();
			hashmapsObjects[i]= new HashMap<String, String>(); 
			for (int j = 0; j < wordsAmount; j++) {
				String k = scanner.next();
				String v = scanner.next();
				scanner.nextLine();
				((HashMap<String, String>)hashmapsObjects[i]).put(k, v);
			}
			String[] words = scanner.nextLine().split(" ");
			
			results[i]= new StringBuilder(); 
			for (String word : words) {
				String wordTmp;
				for (int k = 0; k < peopleAmount; k++) {
					if ((wordTmp = ((HashMap<String, String>)hashmapsObjects[i]).get(word))
							== null) 	
						break;
					word = wordTmp;
				}
				results[i].append(word+" ");
			}
			results[i].deleteCharAt(results[i].length()-1);
		}
		
		for (int i = 0; i < dataGroups; i++) {
			System.out.print("Case #" + (i + 1) + ":");
			System.out.println(results[i]);
		}
	}

}