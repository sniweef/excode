import java.util.Arrays;
public class Vampire {
    public static void main(String[] args) {
        String[] arg1 ,arg2;
        int sum = 0;
        int from;
        int to;
        int i_val;
        int count = 0;

        for (int i = 10; i < 100; i++){
            from = Math.max(1000/i, i+1);
            to = Math.min(10000/i, 100);
            //System.out.println(from+" "+to);
            for (int j = from; j < to; j++) {
                i_val = i *j;
                //j=i+1避免重复？
                if (i_val%100==0 || (i_val-i-j)%9!=0)
                    continue;
                count++;
                arg1 = String.valueOf(i_val).split("");
                arg2 = (String.valueOf(i)+String.valueOf(j)).split("");
                Arrays.sort(arg1);
                Arrays.sort(arg2);
                if (Arrays.equals(arg1, arg2)) {
                    sum++;
                    System.out.println("第"+sum+"组："+i+"*"+j+"="+i_val);
                }
            }
        }
        System.out.println("共找到"+sum+"组吸血鬼数");
        System.out.println("count="+count);
    }
}
