import java.util.*;

public class FormatterEx {
    public static void main(String[] args) {
        Formatter f;
        final int WIDTH = 10;
        String format = "%"+WIDTH+"s %-"+WIDTH+"s\n"; 
        f = new Formatter(System.out);
        f.format(format, "str1", "str2");

        String regex = "Aasd dsf.";
        System.out.println(regex.matches("^[A-Z].*\\.$"));
    }
}
