package GUI;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;


/**
 * Starts the whole application run-time
 * @author Evgenii Abdalov
 */
public class StartGUI extends Application {


    @Override
    public void init() throws Exception{}


    @Override
    public void start(Stage primaryStage) throws Exception {

        Parent root = FXMLLoader.load(getClass().getResource("Main.fxml"));
        primaryStage.setScene(new Scene(root));
        primaryStage.show();

    }

    @Override
    public void stop() throws Exception{
        Platform.exit();
        System.exit(0);
    }




}
