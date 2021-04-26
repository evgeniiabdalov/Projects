package GUI;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

;import java.net.URL;
import java.util.ResourceBundle;


/**
 * VideoSettingsGUI class is responsible for window that enables User to manipulate photo and video process
 * @author Evgenii Abdalov
 */
public class VideoSettingsGUI implements Initializable {


    private MainGUI mainGUI;


    private Integer  widthScreen;
    private  Integer heightScreen;

    @FXML
    private TextField widthScreenText;

    @FXML
    private TextField  heightScreenText;

    public void setWidthScreen(){

        try{
             widthScreen = Integer.parseInt(widthScreenText.getText());
        }
        catch(NumberFormatException exception){
            AlertBox.display("loading error", "Insert correct SCREEN WIDTH number");
            return;
        }

    }

    public void setHeightScreen(){

        try{
            heightScreen = Integer.parseInt(heightScreenText.getText());
        }
        catch(NumberFormatException exception){
            AlertBox.display("loading error", "Insert correct SCREEN HEIGHT number");
            return;
        }

    }

    @FXML
    public void setVideoSettings(){

       this.setWidthScreen();
       this.setHeightScreen();

       this.mainGUI.setVideoSettings(this.widthScreen, this.heightScreen);


        Stage currentStage = (Stage)this.widthScreenText.getScene().getWindow();
        currentStage.close();

    }


    public void setMainGUI(MainGUI mainGUI){
        this.mainGUI = mainGUI;
    }



    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }
}
