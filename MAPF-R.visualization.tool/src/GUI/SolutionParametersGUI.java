package GUI;


import Business.AgentController;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.TextArea;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;

import java.net.URL;
import java.util.ResourceBundle;
import java.util.Set;

public class SolutionParametersGUI implements Initializable {

    private AgentController agentController;

    @FXML
    private TextArea graphParametersText;

    @FXML
    private TreeView agentsTree;

    @FXML
    private TextArea agentParametersText;

    public void setSolutionParameters(String solutionName, Integer vertexAmount, Integer edgeAmount, Double graphDensity, AgentController agentController){

         this.agentController = agentController;

         this.setGraphParameters(solutionName, vertexAmount, edgeAmount, graphDensity);

         this.setAgentParametersTree();

    }

    private void setGraphParameters(String solutionName, Integer vertexAmount, Integer edgeAmount, Double graphDensity){

        String graphParametersString = "";

        graphParametersString = "Name: " + solutionName + "\n";

        graphParametersString += "-------------------" + "\n\n";

        graphParametersString += "vertex amount: " + vertexAmount + "\n\n";

        graphParametersString += "edge amount: " + edgeAmount + "\n\n";

        graphParametersString += "graph density: " + graphDensity + "\n\n";

        graphParametersString += "agent amount: " + this.agentController.getAngentIDs().size() + "\n\n";

        graphParametersText.setText(graphParametersString);

    }

    private void setAgentParametersTree(){

        TreeItem<Integer> root = new TreeItem<Integer>(0);
        root.setExpanded(true);

        Set<Integer> agentIDs = this.agentController.getAngentIDs();

        for(Integer agentID : agentIDs){

            TreeItem<Integer> treeItem = new TreeItem<Integer>(agentID);
            root.getChildren().add(treeItem);

        }

        this.agentsTree.setRoot(root);
        this.agentsTree.setShowRoot(false);

        this.agentsTree.getSelectionModel().selectedItemProperty().addListener(
                new ChangeListener() {
                    @Override
                    public void changed(ObservableValue observableValue, Object o, Object t1) {

                        TreeItem<Integer> selectedAgentItem = (TreeItem<Integer>)agentsTree.getSelectionModel().getSelectedItem();

                        String agentParametersString = "";

                        agentParametersString = "agent start position vertex: " + agentController.getAgentStartPosition(selectedAgentItem.getValue()) + "\n\n";

                        agentParametersString += "agent target position vertex: " + agentController.getAgentTargetPosition(selectedAgentItem.getValue()) + "\n\n";

                        agentParametersString += "agent radius: " + agentController.getAgentRadius(selectedAgentItem.getValue()) + "\n\n";

                        agentParametersString += "agent moving time: " + agentController.getAgentMovingTime(selectedAgentItem.getValue()) + " ms\n\n";

                        agentParametersString += "agent waiting time: " + agentController.getAgentWaitingTime(selectedAgentItem.getValue()) + " ms\n\n";

                        agentParametersString += "agent initializing time: " + agentController.getAgentInitializingTime(selectedAgentItem.getValue()) + " ms\n\n";

                        agentParametersString += "agent arrived time: " + agentController.getAgentArrivedTime(selectedAgentItem.getValue()) + " ms\n\n";

                        agentParametersText.setText(agentParametersString);

                    }
                });

    }



    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }
}
