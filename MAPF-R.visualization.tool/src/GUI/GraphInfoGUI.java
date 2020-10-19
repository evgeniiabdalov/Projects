package GUI;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeView;

import java.net.URL;
import java.util.ResourceBundle;

public class GraphInfoGUI implements Initializable {

    @FXML
    private TreeView agentInfoTree;

    @FXML
    private TextField graphInfoText;

    public void setGraphInfo(Integer verticesAmount, Integer edgesAmount, Double graphDensity, Double timeDuration){

        String graphInfo = "amount of edges: " + edgesAmount + "\n amount of vertices: " + verticesAmount + "\n graphDensity: " + graphDensity + "\n time duration: " + timeDuration;

        graphInfoText.setText(graphInfo);

    }

    public void addAgentInfo(Integer agentID, Double radius){





    }


    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }
}
