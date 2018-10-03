import org.junit.Test;
import phantom.shell.Boot;

public class ArgumentedRun {

    @Test
    public void test1() {
        String[] args = new String[]{"src/test/scripts/parser_simple_test.psh"};
        Boot.main(args);
    }

    @Test
    public void test2(){
        String[] args = new String[]{"src/test/scripts/function_test.psh"};
        Boot.main(args);
    }

    @Test
    public void test3(){
        for (int i = 0;i<10;i++){
            test2();
        }
    }

}
