package GUI;

import Statistics.*;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.*;
import javafx.scene.control.TextField;

import java.net.URL;
import java.util.*;

public class SingleAgentStatisticsGUI implements Initializable {

    private MainGUI mainGUI;

    private StatisticsData statisticsData;

    private XYChart.Series<Number, Number>  speedSeries;
    private XYChart.Series<String, Number>  agentTimeRatioSeries;
    private XYChart.Series<String, Number>  totalTimeRatioSeries;

    private Boolean chartIsSet;
    private Boolean agentTimeRatioChartIsSet;
    private Boolean totalTimeRatioChartIsSet;

    private boolean agentSpeedChartMode;
    private boolean agentLocationChartMode;

    @FXML
    private TextField  precisionText;


    @FXML
    private StackedAreaChart agentChart;

    @FXML
    private BarChart timeRatioChart;

    @FXML
    private TextField agentIDText;


    @FXML
    private NumberAxis xAxis;

    @FXML
    private NumberAxis   yAxis;

    @FXML
    private CategoryAxis stateAxis;

    @FXML
    private NumberAxis timeRatioAxis;

    private Integer agentID;

    private Integer precision;

    public StackedAreaChart getAgentChart(){
        return this.agentChart;
    }

    public BarChart         getTimeRatioChart(){
        return this.timeRatioChart;
    }

    @FXML
    private void            setAgentSpeedChartMode(){

        this.agentSpeedChartMode = true;
        this.agentLocationChartMode = false;

    }

    @FXML
    private void            setAgentLocationChartMode(){

        this.agentSpeedChartMode = false;
        this.agentLocationChartMode = true;

    }

    private boolean         setAgentID(){

        Integer maxAgentID = this.mainGUI.mainController.getCurrentGraphController().getCurrentAgentController().getAngentIDs().size();
        System.out.println("MAX ID" + maxAgentID);
        try {

            this.agentID = Integer.parseInt(this.agentIDText.getText());

            if(this.agentID > maxAgentID){
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

    private void            setTimeRatioChart(XYChart.Series<String, Number> timeRatioSeries, String timeRatioAxisName, String stateAxisName, String timeRatioChartName ){

        timeRatioSeries.setName(timeRatioChartName);

        //defining the axes
        this.timeRatioAxis = new NumberAxis();
        this.stateAxis = new CategoryAxis();

        this.timeRatioAxis.setLabel(timeRatioAxisName);
        this.timeRatioAxis.setAnimated(true);
        this.stateAxis.setLabel(stateAxisName);
        this.stateAxis.setAnimated(true);


        //setting the area chart with two axis created above
        this.timeRatioChart.setTitle(timeRatioChartName);
        this.timeRatioChart.setAnimated(true);
        this.timeRatioChart.getData().addAll(timeRatioSeries);

    }

    @FXML
    private void            setAgentStatsChart(){

        if( !this.setAgentID() )
            return;

        //-----------------------------------------------------

        AgentTimeRatio agentTimeRatio = this.statisticsData.getAgentTimeRatioData().getAgentTimeRatio(this.agentID);

        String timeRatioAxisName = "Time ratio";
        String stateAxisName = "Agent state";

        String timeRatioChartName = "";

        if(this.agentTimeRatioChartIsSet == false) {
            this.setTimeRatioChart(this.agentTimeRatioSeries, timeRatioAxisName, stateAxisName, timeRatioChartName);
            this.agentTimeRatioChartIsSet = true;
        }

        //------------------------------------------------------

        System.out.println("moving " + agentTimeRatio.getMovingTime());
        System.out.println("waiting " + agentTimeRatio.getWaitingTime());
        System.out.println("initializing " + agentTimeRatio.getInitializingTime());
        System.out.println("arrived " + agentTimeRatio.getArrivedTime());
        System.out.println("------------------------------------------------");

        this.agentTimeRatioSeries.getData().add(new XYChart.Data<>("MOVING", agentTimeRatio.getMovingTime()));
        this.agentTimeRatioSeries.getData().add(new XYChart.Data<>("WAITING", agentTimeRatio.getWaitingTime()));
        this.agentTimeRatioSeries.getData().add(new XYChart.Data<>("INITIALIZING", agentTimeRatio.getInitializingTime()));
        this.agentTimeRatioSeries.getData().add(new XYChart.Data<>("ARRIVED", agentTimeRatio.getArrivedTime()));



    }

    private boolean         setPrecision(){

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

    @FXML
    private void            setTotalStatsChart(){

        String timeRatioAxisName = "Time ratio";
        String stateAxisName = "Total state";

        String timeRatioChartName = "";

        if(this.totalTimeRatioChartIsSet == false) {
            this.setTimeRatioChart(this.totalTimeRatioSeries, timeRatioAxisName, stateAxisName, timeRatioChartName);
            this.totalTimeRatioChartIsSet = true;
        }


        //------------------------------------------------------

        this.totalTimeRatioSeries.getData().add(new XYChart.Data<>("MOVING", this.statisticsData.getAgentTimeRatioData().getTotalMovingTime()));
        this.totalTimeRatioSeries.getData().add(new XYChart.Data<>("WAITING", this.statisticsData.getAgentTimeRatioData().getTotalWaitingTime()));
        this.totalTimeRatioSeries.getData().add(new XYChart.Data<>("INITIALIZING", this.statisticsData.getAgentTimeRatioData().getTotalInitializingTime()));
        this.totalTimeRatioSeries.getData().add(new XYChart.Data<>("ARRIVED", this.statisticsData.getAgentTimeRatioData().getTotalArrivedTime()));

    }

    @FXML
    private void            setNotOnlineAgentChart() {

        if( !this.setAgentID() )
            return;

        //---------------------------------------------------

        if( !this.setPrecision()){
            return;
        }

        String timeAxisName = "Time/s";
        String speedAxisName = "Agent speed";

        String speedChartName = "";

        this.speedSeries.setName(speedChartName);

        //defining the axes
        this.xAxis = new NumberAxis();
        this.yAxis = new NumberAxis();

        this.xAxis.setLabel(timeAxisName);
        this.xAxis.setAnimated(true);
        this.yAxis.setLabel(speedAxisName);
        this.yAxis.setAnimated(true);


        //setting the area chart with two axis created above
        this.agentChart.setTitle(speedChartName);
        this.agentChart.setAnimated(true);
        this.agentChart.getData().add(this.speedSeries);

        //--------------------------------------------------------------

        List<Integer> timeKeyList = new ArrayList<>(this.statisticsData.getDistanceDataMap().keySet());

        if (this.agentSpeedChartMode) {

            TreeMap<Integer, Double> speedMovement = this.statisticsData.getAgentMovementData(this.agentID).getSpeedMovement();

            for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                if (speedMovement.containsKey(time)) {
                    this.updateChart(time, speedMovement.get(time));
                }

            }

        }
        else if (this.agentLocationChartMode) {

            for (Integer time = 0; time < timeKeyList.size(); time += this.precision) {

                SpaceTimeData spaceTimeData = this.statisticsData.getAgentMovementData(this.agentID).getSpaceTimeData(time);

                this.updateChart(spaceTimeData.getSpace().getXCoordinate()*1000, spaceTimeData.getSpace().getYCoordinate()*1000);

            }

        }

    }

    @FXML
    private void            setAgentChart(){

        if( !this.setAgentID() )
            return;

        //---------------------------------------------------

        String timeAxisName = "Time/s";
        String speedAxisName = "Agent speed";

        String speedChartName = "";

        this.speedSeries.setName(speedChartName);

        //defining the axes
        this.xAxis = new NumberAxis();
        this.yAxis = new NumberAxis();

        this.xAxis.setLabel(timeAxisName);
        this.xAxis.setAnimated(true);
        this.yAxis.setLabel(speedAxisName);
        this.yAxis.setAnimated(true);


        //setting the area chart with two axis created above
        this.agentChart.setTitle(speedChartName);
        this.agentChart.setAnimated(true);
        this.agentChart.getData().add(this.speedSeries);

        chartIsSet = true;

    }

    public void             setStatisticsData(MainGUI mainGUI, StatisticsData statisticsData) {

        this.mainGUI = mainGUI;
        this.statisticsData = statisticsData;

    }

    public void             updateChart(Integer xValue, Double yValue){
        this.speedSeries.getData().add(new XYChart.Data<>(xValue, yValue));
    }

    public void             updateChart(Double xValue, Double yValue){
        this.speedSeries.getData().add(new XYChart.Data<>(xValue, yValue));
    }

    public void             updateCurrentTime(Integer time) {


        if(this.chartIsSet) {

            if(this.agentSpeedChartMode) {

                TreeMap<Integer, Double> speedMovement = this.statisticsData.getAgentMovementData(this.agentID).getSpeedMovement();

                if (speedMovement.containsKey(time)) {
                    this.updateChart(time, speedMovement.get(time));
                }

            }
            else if(this.agentLocationChartMode){

                SpaceTimeData spaceTimeData = this.statisticsData.getAgentMovementData(this.agentID).getSpaceTimeData(time);

                this.updateChart(spaceTimeData.getSpace().getXCoordinate()*1000, spaceTimeData.getSpace().getYCoordinate()*1000);

            }

        }

    }

    @Override
    public void             initialize(URL url, ResourceBundle resourceBundle) {

        this.chartIsSet = false;

        this.speedSeries = new XYChart.Series<>();
        this.agentTimeRatioSeries = new XYChart.Series<>();
        this.totalTimeRatioSeries = new XYChart.Series<>();

        this.agentTimeRatioChartIsSet = false;
        this.totalTimeRatioChartIsSet = false;

        this.agentSpeedChartMode = true;
        this.agentLocationChartMode = false;

        this.agentID = 0;


    }



}
