package executor;

import org.junit.Test;
import phantom.shell.Boot;

public class ExecutorTest {

    @Test
    public void test1() {
        String[] args = new String[]{"src/test/scripts/test2.psh"};
        Boot.main(args);
    }

}
