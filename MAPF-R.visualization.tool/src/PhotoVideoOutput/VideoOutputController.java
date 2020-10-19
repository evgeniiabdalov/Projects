package PhotoVideoOutput;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class VideoOutputController {

    private Thread  videoCaptureThread;

    private Process videoCaptureProcess;

    private Integer videoCounter;

    public VideoOutputController(){
        this.videoCounter = 0;
    }


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

    public void stopVideoCapture() {

        videoCaptureProcess.destroy();

        this.videoCaptureThread.stop();

        this.videoCounter++;

    }


}
