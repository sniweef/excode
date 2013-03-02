
public class XiXieGui { 
    /** 
     * ??????? 
     * ?:12*60=1260 
     * YangL. 
     */ 
    public static void main(String[] args) { 
        String[] ar_str1, ar_str2; 
        int sum = 0; 
        int count = 0;
        for (int i = 10; i < 100; i++) { 
            for (int j = i + 1; j < 100; j++) { 
                int i_val = i * j; 
                if (i_val < 1000 || i_val > 9999) 
                    continue; // ???1000???9999??,?????? 
                ar_str1 = String.valueOf(i_val).split(""); 
                ar_str2 = (String.valueOf(i) + String.valueOf(j)).split(""); 
                java.util.Arrays.sort(ar_str1); 
                java.util.Arrays.sort(ar_str2); 
                if (java.util.Arrays.equals(ar_str1, ar_str2)) { 
                    // ?????,??????? 
                    sum++; 
                    System.out.println("?" + sum + "?: " + i + "*" + j + "=" 
                            + i_val); 
                } 
                count++;
            } 
        } 
        System.out.println("???" + sum + "?????" + count); 
    } 
}
