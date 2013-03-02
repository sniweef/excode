import com.Test2;
public class Test { 
    static int j;
    final int k;
    public Test() {
        k = 3;
    }
    public int f() {
        //k=3; error
        return k;
    }
    public static void main(String[] args) {
        //j = (1, 0); //Error
       int i;
       //i++; Error:i未初始化
       j++; //Fine
       //main(null);
      Test2.test21.f(); 
      int a[];
      Test test = new Test();
      System.out.println("k="+test.f());
      Test2 test2 = new Test2(1);
      Test2.AImp aimp = (Test2.AImp)test2.getA();
      //aimp.a = 4; error
      aimp.f(aimp.a);

      Protected pc = new Protected();
      pc.a = 4;

      if (args.length == 1)
         System.out.println(args[0]); // args[0] is not the name of application
    }
}
