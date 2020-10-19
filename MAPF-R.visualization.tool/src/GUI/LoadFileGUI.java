package GUI;

import DAO.DataSet;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextField;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;


public class LoadFileGUI implements Initializable {

    private MainGUI mainGUI;

    //-----------------------------------------------------------------------------------

    private FileChooser agentFileChooser    = new FileChooser();
    private FileChooser graphFileChooser      = new FileChooser();
    private FileChooser planFileChooser = new FileChooser();

    //-------------------------------------------------------------------------------------

    private String agentFileString = "";
    private String graphFileString = "";
    private String planFileString = "";

    private boolean agentLoaded  = false;
    private boolean graphLoaded  = false;
    private boolean planLoaded   = false;

    //--------------------------------------------------------------------

    @FXML
    private TextField loadAgentText;

    @FXML
    private  TextField loadGraphText;

    @FXML
    private  TextField loadPlansText;

    @FXML
    private  TextField inputNameText;

    private void  makeDataSet(String solutionKey) throws IOException {

        this.mainGUI.setSolution(solutionKey, agentFileString, graphFileString, planFileString);

        this.mainGUI.setSolutionList();

    }

    @FXML
    private void setAgentFileChooser(ActionEvent event){

        Stage stage = new Stage();

        try {

            File file = agentFileChooser.showOpenDialog(stage);

            agentFileString = file.getAbsolutePath();

            this.loadAgentText.setText(agentFileString);

            if(agentFileString.contains("kruR")) {
               agentLoaded = true;
            }
            else{
                AlertBox.display("loading error","wrong agent file");
            }

        }
        catch(NullPointerException ex){
            System.out.println("agents file is not uploaded");
        }

    }

    @FXML
    private void setGraphFileChooser(ActionEvent event){

        Stage stage = new Stage();

        try {

            File file = graphFileChooser.showOpenDialog(stage);

            graphFileString = file.getAbsolutePath();

            this.loadGraphText.setText(graphFileString);

            if(graphFileString.contains("mapR"))
               graphLoaded = true;
            else{
                AlertBox.display("loading error","wrong graph file");
            }

        }
        catch(NullPointerException ex){
            System.out.println("graph file is not uploaded");
        }

    }

    @FXML
    private void setPlanFileChooser(ActionEvent event){

        Stage stage = new Stage();

        try {

            File file = planFileChooser.showOpenDialog(stage);

            planFileString = file.getAbsolutePath();

            this.loadPlansText.setText(planFileString);

            if(planFileString.contains("txt")) {
                planLoaded = true;
            }
            else{
                AlertBox.display("loading error","wrong plan file");
            }



        }
        catch(NullPointerException ex){
            System.out.println("plan file is not uploaded");
        }


    }

    @FXML
    private void setLoadFileSubmit(ActionEvent event) throws IOException {

        if( !agentLoaded ){
            AlertBox.display("loading error","agents are not loaded");
        }
        else if( !graphLoaded ){
            AlertBox.display("loading error","graph" +
                    " is not loaded");
        }
        else if( !planLoaded ){
            AlertBox.display("loading error","plan is not loaded");
        }
        else if(this.inputNameText.getText().equals("")){
            AlertBox.display("loading error","Please, input solution name");
        }
        else {

            try {

                System.out.println(this.inputNameText.getText());

                this.makeDataSet(this.inputNameText.getText());

                Stage currentStage = (Stage)this.loadAgentText.getScene().getWindow();
                currentStage.close();


            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

        }

    }

    public void setMainGUI(MainGUI mainGUI){
         this.mainGUI = mainGUI;
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle){
    }
}
