import org.junit.Test;
import phantom.shell.Boot;

public class ArgumentedRun {

    @Test
    public void test1() {
        String[] args = new String[]{"src/test/scripts/parser_simple_test.psh"};
        Boot.main(args);
    }

}
