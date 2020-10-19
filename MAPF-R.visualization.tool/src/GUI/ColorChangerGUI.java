package GUI;

import Visualization.ColorSetter;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ColorPicker;
import javafx.stage.Stage;

import java.net.URL;
import java.util.ResourceBundle;

public class ColorChangerGUI implements Initializable {


    @FXML
    private ColorPicker markedAgentColor;

    @FXML
    private ColorPicker vertexColor;

    @FXML
    private ColorPicker arrivedAgentColor;

    @FXML
    private ColorPicker initializedAgentColor;

    @FXML
    private ColorPicker waitingAgentColor;

    @FXML
    private ColorPicker movingAgentColor;


    private ColorSetter colorSetter;

    private MainGUI     mainGui;


    @FXML
    public void submitColor(){

        this.colorSetter = new ColorSetter();

        this.colorSetter.setMovingAgentColor(movingAgentColor.getValue());
        this.colorSetter.setArrivedAgentColor(arrivedAgentColor.getValue());
        this.colorSetter.setInitializedAgentColor(initializedAgentColor.getValue());
        this.colorSetter.setWaitingAgentColor(waitingAgentColor.getValue());
        this.colorSetter.setVertexColor(vertexColor.getValue());
        this.colorSetter.setMarkedAgentColor(markedAgentColor.getValue());

        this.mainGui.setColorSetter(this.colorSetter);

        Stage stage = (Stage)this.movingAgentColor.getScene().getWindow();
        stage.close();

    }

    public void setMainGui(MainGUI mainGui){
          this.mainGui = mainGui;
    }


    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }
}
