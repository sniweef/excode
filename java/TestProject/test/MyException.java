import java.util.logging.*;
import java.io.*;
public class MyException extends Exception{
    private static Logger logger = Logger.getLogger("MyException");

    public MyException(String msg) {
        super(msg);
    }
    static void logException(Exception e) {
        StringWriter trace = new StringWriter();
        e.printStackTrace(new PrintWriter(trace));
        logger.severe(trace.toString());
    }

    static boolean f(int i) throws MyException {
        if (i < 5) {
            System.out.println("Throwing "+i+" exception");
            throw new MyException("MyException");
        }
        return true;
    }

    public static void main(String[] args) {
        int i = 0;
        boolean result = false;
        while (!result) {
            try {
                result = f(++i);
            } catch (MyException e) {
                logException(e);
                e.printStackTrace(System.out);
            } finally {
            }
        }
    }
}
