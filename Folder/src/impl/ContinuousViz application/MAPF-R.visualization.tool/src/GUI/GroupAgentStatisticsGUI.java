package GUI;

import Statistics.ShortestDistanceData;
import Statistics.StatisticsData;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.AreaChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;

import java.net.URL;
import java.util.*;


/**
 * GroupAgentStatisticsGUI class controls window that displays statistics data
 * for group of Agents
 * @author Evgenii Abdalov
 */
public class GroupAgentStatisticsGUI implements Initializable {


    private MainGUI               mainGUI;


    private StatisticsData        statisticsData;
    private ShortestDistanceData  shortestDistanceData;

    private boolean               chartIsSet;
    private boolean               groupDistanceChartMode;
    private boolean               groupRiskCollisionChartMode;



    @FXML
    private AreaChart             agentGroupChart;

    @FXML
    private TreeView              notShortestPathTree;

    @FXML
    private TreeView              groupAgentsTree;


    @FXML
    private CategoryAxis          xAxis;

    @FXML
    private NumberAxis            yAxis;

    @FXML
    private TextField             precisionText;

    private Integer               precision;

    private XYChart.Series<String, Number>          series;

    private Set<Integer>                            groupAgents;



    /**
     * Sets mode to Distance chart mode
     */
    @FXML
    private void      setDistanceChartMode(){

        this.groupDistanceChartMode      = true;
        this.groupRiskCollisionChartMode = false;

    }

    /**
     * Sets mode to RiskCollision chart mode
     */
    @FXML
    private void      setRiskCollisionChartMode(){

        this.groupRiskCollisionChartMode = true;
        this.groupDistanceChartMode      = false;

    }


    /**
     * Sets a group of agents that will be involved
     */
    @FXML
    private void      setGroupAgents(){

        this.groupAgents = this.mainGUI.getGroupAgents();

        for(Integer agentID: this.groupAgents){
            System.out.println("agent: " + agentID);
        }

        //-------------------------------------------------------------

        TreeItem<String> root = new TreeItem<String>("Group of agents: ");
        root.setExpanded(true);


        for(Integer agentID: this.groupAgents){
            TreeItem<String> treeItem = new TreeItem<String>("Agent " + agentID);
            root.getChildren().add(treeItem);
        }

        this.groupAgentsTree.setRoot(root);

    }

    /**
     * Sets StatisticsData object, from that object data will be displayed on the chart
     */
    public void       setStatisticsData(MainGUI mainGUI, StatisticsData statisticsData){

        this.mainGUI = mainGUI;
        this.statisticsData = statisticsData;

        this.setShortestDistanceMap(this.statisticsData.getShortestDistanceData());

    }

    /**
     * Sets StatisticsData object, from that object data will be displayed on the chart
     */
    private void      setShortestDistanceMap(ShortestDistanceData shortestDistanceMap){

        this.shortestDistanceData = shortestDistanceMap;
        this.previewShortestDistance();

    }

    /**
     * Initializes non-optimal list display
     */
    private void      previewShortestDistance(){

        TreeItem<String> root = new TreeItem<String>("Agents with not optimal distances:");
        root.setExpanded(true);

        List<Integer> notShortestDistanceList = this.shortestDistanceData.getNotShortestDistanceList();

        if( notShortestDistanceList.size() == 0 ){

            TreeItem<String> treeItem = new TreeItem<String>("All agents have optimal distances");
            root.getChildren().add(treeItem);

        }
        else{

            for(int i = 0; i < notShortestDistanceList.size(); i++) {

                String treeItemString = "Agent ";

                treeItemString += notShortestDistanceList.get(i);

                TreeItem<String> treeItem = new TreeItem<String>(treeItemString);
                root.getChildren().add(treeItem);

            }



        }

        this.notShortestPathTree.setRoot(root);



    }

    /**
     * Sets precision with which data will be displayed on the chart
     * @return true if precision value is valid, false otherwise
     */
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

    /**
     * Sets chart that will be displaying preprocessed data not in real time
     */
    @FXML
    public void       setNotOnlineAgentChart(){

        if(!this.chartIsSet)
          this.setStatisticsChart();

        if(!this.setPrecision())
            return;

        this.xAxis.setAnimated(false); // axis animations are removed
        this.yAxis.setAnimated(false); // axis animations are removed
        this.agentGroupChart.setAnimated(false);


        if(this.chartIsSet && this.groupAgents.size() > 0) {

            List<Integer> timeKeyList = new ArrayList<>(this.statisticsData.getDistanceDataMap().keySet());

            Integer agentAmount = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs().size();

            if(this.groupDistanceChartMode) {

                for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                    if (this.statisticsData.getDistanceDataMap().containsKey(time)) {

                        Double groupDistance = 0.0;

                        for( Integer firstAgentID : this.groupAgents ){
                            for( Integer secondAgentID: this.groupAgents ){

                                System.out.println("firstAgentID: " + firstAgentID + " secondAgentID: " + secondAgentID);

                                if( firstAgentID != secondAgentID )
                                    groupDistance += this.statisticsData.getTimeAgentSpecificDistanceData(time, firstAgentID, secondAgentID);

                            }
                        }

                        Double groupDistanceResult = groupDistance / agentAmount;

                        this.updateChart(time.toString(), groupDistanceResult);

                    }
                }

            }
            else if(this.groupRiskCollisionChartMode){

                for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                    if (this.statisticsData.getRiskDataMap().containsKey(time)) {

                        Double groupRiskCollision = 0.0;

                        for( Integer firstAgentID : this.groupAgents ){
                            for( Integer secondAgentID: this.groupAgents ){

                                if( firstAgentID != secondAgentID )
                                    groupRiskCollision += this.statisticsData.getTimeAgentSpecificCollisionRiskData(time, firstAgentID, secondAgentID);

                            }
                        }


                        Double groupRiskCollisionResult = groupRiskCollision / agentAmount;

                        this.updateChart(time.toString(), groupRiskCollisionResult);

                    }


                }

            }


        }



    }

    /**
     * Sets chart that will be displaying data in real time
     */
    @FXML
    private void      setStatisticsChart(){

        if(!chartIsSet) {

            String xAxisName = "Time/s";
            String yAxisName = "";

            String chartName = "";

            if (this.groupDistanceChartMode) {
                chartName = "distance";
                yAxisName = "distance level";
            } else if (this.groupRiskCollisionChartMode) {
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
            this.agentGroupChart.setTitle(chartName);
            this.agentGroupChart.setAnimated(true);
            this.agentGroupChart.getData().add(this.series);


            this.chartIsSet = true;

        }


    }

    /**
     * Updates statistics data that corresponds with time
     * @param time moment of time
     */
    public void       updateCurrentTime(Integer time){

        System.out.println("time update: " + time);

        if(this.chartIsSet && this.groupAgents.size() > 0) {

            Integer agentAmount = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs().size();

            Set<Integer> agentIDs = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs();

            List<Integer> agentIDList = new ArrayList<>(agentIDs);

            if(this.groupDistanceChartMode) {

                if (this.statisticsData.getDistanceDataMap().containsKey(time)) {

                    Double groupDistance = 0.0;

                    for( Integer firstAgentID : this.groupAgents ){
                        for( Integer secondAgentID: this.groupAgents ){

                            System.out.println("firstAgentID: " + firstAgentID + " secondAgentID: " + secondAgentID);

                            if( firstAgentID != secondAgentID )
                                groupDistance += this.statisticsData.getTimeAgentSpecificDistanceData(time, firstAgentID, secondAgentID);

                        }
                    }

                    Double groupDistanceResult = groupDistance / agentAmount;

                    this.updateChart(time.toString(), groupDistanceResult);

                }
            }
            else if(this.groupRiskCollisionChartMode){

                if (this.statisticsData.getRiskDataMap().containsKey(time)) {

                    Double groupRiskCollision = 0.0;

                    for( Integer firstAgentID : this.groupAgents ){
                        for( Integer secondAgentID: this.groupAgents ){

                            if( firstAgentID != secondAgentID )
                                groupRiskCollision += this.statisticsData.getTimeAgentSpecificCollisionRiskData(time, firstAgentID, secondAgentID);

                        }
                    }


                    Double groupRiskCollisionResult = groupRiskCollision / agentAmount;

                    this.updateChart(time.toString(), groupRiskCollisionResult);

                }

            }

        }

    }

    /**
     * Updates data on the chart
     */
    private void      updateChart(String category, Double number){
        this.series.getData().add(new XYChart.Data<>(category, number));
    }


    @Override
    public void       initialize(URL url, ResourceBundle resourceBundle) {

        this.groupDistanceChartMode      = true;
        this.groupRiskCollisionChartMode = false;

        this.groupAgents = new HashSet<>();

        this.series = new XYChart.Series<>();

    }



}
