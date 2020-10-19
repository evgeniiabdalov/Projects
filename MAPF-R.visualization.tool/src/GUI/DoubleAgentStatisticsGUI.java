package GUI;

import Statistics.CollisionData;
import Statistics.StatisticsData;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.*;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.util.Pair;

import javax.swing.*;
import java.net.URL;
import java.util.*;

public class DoubleAgentStatisticsGUI implements Initializable {

    private TreeMap<Integer, List<Integer>>         collisionMap;
    private TreeMap<String, Pair<Integer,Integer>>  selectedAgentsMap;

    private CollisionData                           collisionData;
    private StatisticsData                          statisticsData;

    private boolean                                 distanceChartMode;
    private boolean                                 riskCollisionChartMode;

    private boolean                                 chartIsSet;

    private boolean                                 firstAgentIsSet;
    private boolean                                 secondAgentIsSet;

    private MainGUI                                 mainGUI;

    @FXML
    private TreeView                                collisionTree;

    @FXML
    private TextField                               firstAgentIDText;

    @FXML
    private TextField                               secondAgentIDText;

    @FXML
    private TextField                               precisionText;

    @FXML
    private AreaChart                               agentChart;

    private Integer                                 currentTime;
    private Integer                                 previousTime;

    private Integer                                 firstAgentID;
    private Integer                                 secondAgentID;

    private Integer                                 precision;

    private Set<Integer>                            agentIDs;

    private XYChart.Series<String, Number>          series;

    @FXML
    private CategoryAxis xAxis;

    @FXML
    private NumberAxis yAxis;


    //-----------------------------------------------------------------------

    public AreaChart  getAgentChart(){
        return this.agentChart;
    }


    @FXML
    private void      setDistanceChartMode(){

        this.distanceChartMode      = true;
        this.riskCollisionChartMode = false;

    }

    @FXML
    private void      setRiskCollisionChartMode(){

        this.riskCollisionChartMode = true;
        this.distanceChartMode      = false;

    }

    private boolean   setFirstAgentID(){

        Integer maxAgentID = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAngentIDs().size();
        System.out.println("MAX ID" + maxAgentID);
        try {

            this.firstAgentID = Integer.parseInt(this.firstAgentIDText.getText());

            if(this.firstAgentID  > maxAgentID){
                AlertBox.display("loading error", "Insert correct agent index");
                return false;
            }


            if(this.firstAgentID> maxAgentID){
                AlertBox.display("loading error", "Insert correct agent index");
                return false;
            }

        }
        catch(NumberFormatException exception){
            AlertBox.display("loading error", "Insert correct agent index");
            return false;
        }

        return true;

    }

    private boolean   setSecondAgentID(){

         Integer maxAgentID = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAngentIDs().size();
         System.out.println("MAX ID" + maxAgentID);
         try {

             this.secondAgentID = Integer.parseInt(this.secondAgentIDText.getText());

             if(this.secondAgentID > maxAgentID){
                 AlertBox.display("loading error", "Insert correct agent index");
                 return false;
             }

         }
         catch(NumberFormatException exception){
             AlertBox.display("loading error", "Insert correct agent index");
             return false;
         }

         return true;

    }


    private boolean   setPrecision(){

        try {

            this.precision = Integer.parseInt(this.precisionText.getText());

            if(this.precision < 60){
                AlertBox.display("loading error", "Precision can be less than 60");
                return false;
            }

        }
        catch(NumberFormatException exception){
            AlertBox.display("loading error", "Insert correct precision number");
            return false;
        }

        return true;

    }

    private void      previewCollisions(){

        TreeItem<String> root = new TreeItem<String>("Collision list");
        root.setExpanded(true);

        List<Pair<Double, Double>> collisionTimeList = this.collisionData.getCollisionTimeList();
        TreeMap<Integer, Pair<Integer, Integer>> collisionTimeMap = this.collisionData.getCollisionTimeMap();


        for(int i = 0; i < collisionTimeList.size(); i++){

            String treeItemString = "agents ";

            Pair<Integer, Integer> pairAgents = collisionTimeMap.get(i);

            treeItemString += pairAgents.getKey() + " <-> " + pairAgents.getValue();
            treeItemString += " time: start " + collisionTimeList.get(i).getKey() + " finish " + collisionTimeList.get(i).getValue();

            this.selectedAgentsMap.put(treeItemString, pairAgents);

            TreeItem<String> treeItem = new TreeItem<String>(treeItemString);
            root.getChildren().add(treeItem);

        }


        collisionTree.setRoot(root);


    }

    public void       setStatisticsData(MainGUI mainGUI, StatisticsData statisticsData){

        this.mainGUI = mainGUI;
        this.statisticsData = statisticsData;

        this.setCollisionMap(this.statisticsData.getCollisionData());

    }

    private void      setCollisionMap(CollisionData collisionData){
        this.collisionData = collisionData;
        this.previewCollisions();
    }

    @FXML
    public void       setCollision(){

        TreeItem<String> selectedCollisionItem = (TreeItem<String>)collisionTree.getSelectionModel().getSelectedItem();

        String selectedItemString = selectedCollisionItem.getValue();

        String firstAgentString = selectedItemString.substring(selectedItemString.indexOf("s")+1, selectedItemString.indexOf("<-") );
        String secondAgentString = selectedItemString.substring(selectedItemString.indexOf(">")+1, selectedItemString.indexOf("time") );

        firstAgentString = firstAgentString.trim();
        secondAgentString = secondAgentString.trim();

        System.out.println("agent string " + firstAgentString);
        System.out.println("agent string " + secondAgentString);


        Integer firstAgentIndex = Integer.parseInt(firstAgentString);
        Integer secondAgentIndex = Integer.parseInt(secondAgentString);

        this.mainGUI.setCollisionAgents(firstAgentIndex, secondAgentIndex );

    }

    public void       updateCurrentTime(Integer time){

        System.out.println("time update: " + time);

        if(this.chartIsSet) {


            if(this.distanceChartMode) {

                if (this.statisticsData.getDistanceDataMap().containsKey(time)) {

                    Double distance = this.statisticsData.getTimeAgentSpecificDistanceData(time, this.firstAgentID, this.secondAgentID);
                    this.updateChart(time.toString(), distance);

                }
            }
            else if(this.riskCollisionChartMode){

                if (this.statisticsData.getRiskDataMap().containsKey(time)) {

                    Double riskCollision = this.statisticsData.getTimeAgentSpecificCollisionRiskData(time, this.firstAgentID, this.secondAgentID);
                    this.updateChart(time.toString(), riskCollision);

                }

            }

        }

    }

    @FXML
    public void       setNotOnlineChart() throws InterruptedException {

        this.setStatisticsChart();

        if(!this.setPrecision())
            return;

        this.xAxis.setAnimated(false); // axis animations are removed
        this.yAxis.setAnimated(false); // axis animations are removed
        this.agentChart.setAnimated(false);


        if(this.chartIsSet) {


            List<Integer> timeKeyList = new ArrayList<>(this.statisticsData.getDistanceDataMap().keySet());

            if(this.distanceChartMode) {

                for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                    Double distance = this.statisticsData.getTimeAgentSpecificDistanceData(time, this.firstAgentID, this.secondAgentID);
                    this.updateChart(time.toString(), distance);

                }

            }
            else if(this.riskCollisionChartMode){

                for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                    Double riskCollision = this.statisticsData.getTimeAgentSpecificCollisionRiskData(time, this.firstAgentID, this.secondAgentID);
                    this.updateChart(time.toString(), riskCollision);

                }

            }


        }

    }

    @FXML
    private void      setStatisticsChart(){

        if(!this.setFirstAgentID())
            return;
        if(!this.setSecondAgentID())
            return;

        if( this.firstAgentID == this.secondAgentID ){
             AlertBox.display("loading error", "Agent indexes are same, insert correct agent index");
             return;
        }

        String xAxisName = "Time/s";
        String yAxisName = "";

        String chartName = "";

        if(this.distanceChartMode) {
            chartName = "distance";
            yAxisName = "distance level";
        }
        else if(this.riskCollisionChartMode) {
            chartName = "risk collision";
            yAxisName = "distance level";
        }


        //--------------------------------------------------
        this.series.setName(chartName);

        //defining the axes
        this.xAxis = new CategoryAxis();
        this.yAxis = new NumberAxis();

        this.xAxis.setLabel(xAxisName);
        this.xAxis.setAnimated(true);
        this.yAxis.setLabel(yAxisName);
        this.yAxis.setAnimated(true);


        //setting the area chart with two axis created above
        this.agentChart.setTitle(chartName);
        this.agentChart.setAnimated(true);
        this.agentChart.getData().add(this.series);


        this.chartIsSet = true;


    }

    private void      updateChart(String category, Double number){
        this.series.getData().add(new XYChart.Data<>(category, number));
    }



    @Override
    public void       initialize(URL url, ResourceBundle resourceBundle) {

        this.selectedAgentsMap = new TreeMap<>();

        this.distanceChartMode = true;
        this.riskCollisionChartMode = false;
        this.chartIsSet = false;
        this.firstAgentIsSet = true;
        this.secondAgentIsSet = true;

        this.previousTime = 0;
        this.currentTime = 0;

        this.series = new XYChart.Series<>();

    }

}
