package PhotoVideoOutput;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * VideoOutputController is responsible for controlling video recording process
 * @author Evgenii Abdalov
 */
public class VideoOutputController {

    private Thread  videoCaptureThread;

    private Process videoCaptureProcess;

    private Integer videoCounter;

    /**
     * VideoOutputController constructor
     */
    public VideoOutputController(){
        this.videoCounter = 0;
    }

    /**
     * Starts video capture by running FFMPEG script in the background process
     */
    public void startVideoCapture() {


        Runnable captureRunnable = new Runnable() {
            @Override
            public void run() {

                try {

                    String line = "";

                    String command = "ffmpeg -y -f x11grab -s 1365*767 -i :0.0 out" + videoCounter + ".mkv\n";

                    videoCaptureProcess = Runtime.getRuntime().exec(command);

                    BufferedReader br = new BufferedReader(new InputStreamReader(videoCaptureProcess.getInputStream()));

                    while (true) {
                        if (!((line = br.readLine()) != null))
                          System.out.println("line is " + line);
                    }

                }
                catch (IOException e){
                    e.printStackTrace();
                }

            }
        };

        this.videoCaptureThread = new Thread(captureRunnable);
        this.videoCaptureThread.start();


    }

    /**
     * Starts video capture by running FFMPEG script in the background process
     @param videoWidth width of the screen/recorded image
     @param videoHeight height of the screen/recorded image
     */
    public void startVideoCapture(Integer videoWidth, Integer videoHeight) {


        Runnable captureRunnable = new Runnable() {
            @Override
            public void run() {

                try {

                    String line = "";

                    String command = "ffmpeg -y -f x11grab -s " + videoWidth + "*" + videoHeight + " -i :0.0 out" + videoCounter + ".mkv\n";

                    videoCaptureProcess = Runtime.getRuntime().exec(command);

                    BufferedReader br = new BufferedReader(new InputStreamReader(videoCaptureProcess.getInputStream()));

                    while (true) {
                        if (!((line = br.readLine()) != null))
                            System.out.println("line is " + line);
                    }

                }
                catch (IOException e){
                    e.printStackTrace();
                }

            }
        };

        this.videoCaptureThread = new Thread(captureRunnable);
        this.videoCaptureThread.start();


    }

    /**
     * Stops video capture by destroying the backgroud process
     */
    public void stopVideoCapture() {

        videoCaptureProcess.destroy();

        this.videoCaptureThread.stop();

        this.videoCounter++;

    }


}
