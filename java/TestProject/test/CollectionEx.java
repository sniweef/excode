import java.util.*;
public class CollectionEx {
    public static void main(String[] args) {
        Set<Integer> set = new HashSet<Integer>(Arrays.asList(1, 2, 3));
        System.out.println(set);

        for (Map.Entry entry: System.getenv().entrySet()) 
            System.out.println(entry.getKey()+": "+entry.getValue());
    }
}
