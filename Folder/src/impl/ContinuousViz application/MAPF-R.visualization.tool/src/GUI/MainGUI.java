package GUI;

import Business.*;
import PhotoVideoOutput.PhotoOutputController;
import PhotoVideoOutput.VideoOutputController;
import Visualization.*;
import Visualization.ColorSetter;
import Visualization.GraphVisual;
import javafx.animation.Animation;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.Pane;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.transform.Scale;
import javafx.stage.Stage;

import java.awt.*;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.*;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


/**
 * MainGUI class controls the whole GUI experience
  * @author Evgenii Abdalov
 */
public class MainGUI implements Initializable {

    MainController                    mainController = new MainController();

    //---------------------------------------------------------------------

    final double                      SCALE_DELTA = 1.1;

    //---------------------------------------------------------------------

    private Set<Integer>              unmarkedCircles;
    private List<String>              notVisibleCircles;
    private List<String>              notVisibleLabels;

    private boolean                   agentVisible;

    //---------------------------------------------------------------------

    private String                    currentSolutionKey;

    //---------------------------------------------------------------------

    private TreeMap<String, Label>    agentLabels;

    private TreeMap<String, Label>    graphLabels;

    //------------------------------------------------------------------------------------

     private  ColorSetter             colorSetter = new ColorSetter();

     private Boolean                  visualizationStarted = false;

    //------------------------------------------------------------------------------------

    private AnimationController       animationController;

    @FXML
    private Button                    playButton;

    @FXML
    private Button                    slowDownButton;

    @FXML
    private Button                    speedUpButton;

    @FXML
    private Button                    stopButton;

    @FXML
    private Button                    backwardButton;

    @FXML
    private Button                    forwardButton;

    @FXML
    private MenuItem                  startVideoButton;

    @FXML
    private Label                     timer;

    //------------------------------------------------------------------------------------

    @FXML
    private Slider                    timelineSlider;

    @FXML
    private Slider                    rotationSlider;

    @FXML
    private Pane                      mainVisualPane;

    private PhotoOutputController     photoOutputController;

    private VideoOutputController     videoOutputController;

    @FXML
    private CheckBox                  showAgentLabelsCheck;

    @FXML
    private CheckBox                  showGraphLabelsCheck;

    //---------------------------------------------------------------------

    @FXML
    private TreeView                  solutionTree;

    private Set<String>               agentKeys;

    //---------------------------------------------------------------------


    private SingleAgentStatisticsGUI singleAgentStatisticsGUI;
    private DoubleAgentStatisticsGUI doubleAgentStatisticsGUI;
    private GroupAgentStatisticsGUI  groupAgentStatisticsGUI;

    private boolean                  isSingleStatisticsSet;
    private boolean                  isDoubleStatisticsSet;
    private boolean                  isGroupStatisticsSet;

    private boolean                  isPlaying;

    private Group                    elementsGroup;

    //---------------------------------------------------------------------

    private Integer                  videoWidth;
    private Integer                  videoHeight;

    //---------------------------------------------------------------------

    /**
     * Returns Label assigned to Agent object
     * @param agentID Agent unique identifier
     * @return Label which displays Agent unique identifier
     */
    public  Label                     getAgentLabel(String agentID){
       return this.agentLabels.get(agentID);
    }

    /**
     * Returns Label assigned to Vertex object
     * @param vertexID Vertex unique identifier
     * @return Label which displays Vertex unique identifier
     */
    public Label                      getGraphLabel(String vertexID){
       return this.graphLabels.get(vertexID);
    }

    public  Circle                    getCircle(Integer id) {
        return (Circle)mainVisualPane.lookup('#' + id.toString());
    }

    /**
     * Returns Circle assigned to Agent object
     * @param id Circle object unique identifier
     * @return Circle that corresponds to id
     */
    public  Circle                    getCircle(String id) {
        return (Circle)mainVisualPane.lookup('#' + id);
    }

    /**
     * Returns MainController singleton, that is responsible for controlling business logic of application
     * @return MainController
     */
    public  MainController            getMainController(){
        return this.mainController;
    }

    /**
    * Returns ColorSetter, when color changes data is needed
    * @return ColorSetter
    */
    public  ColorSetter               getColorSetter(){
        return this.colorSetter;
    }

    //---------------------------------------------------------------------

    /**
     * Writes out to console zoom parameters
     */
    private void                     zoomInfo(Double scaleX, Double scaleY){
        System.out.println("ZOOM INFO scale x: " + scaleX + " scale y:" + scaleY);
    }

    //---------------------------------------------------------------------

    /**
     * Visualizes Graph and Agents, by invoking methods visualizeGraph(...) and visualizeAgents(...)
     * @param graphVisual visual representation of Graph
     * @param currentAgents AgentController that keeps AgentVisuals, visual representation of Agents
     */
    private void                     visualizeSolution(GraphVisual graphVisual, AgentController currentAgents){


        this.clearVisualizationPane();
        this.setMainVisualPane();



        this.visualizeGraph(graphVisual);
        this.visualizeAgents(currentAgents);


    }

    /**
     * Visualizes Graph
     * @param graphVisual visual representation of Graph
     */
    private void                     visualizeGraph(GraphVisual graphVisual) {

        List<Circle> vertexCircles = graphVisual.getVertexCircles();
        List<Line>   edgeLines     = graphVisual.getEdgeLines();



        for(Circle vertexCircle : vertexCircles ){

            this.createGraphLabel(vertexCircle);

            this.elementsGroup.getChildren().add(vertexCircle);
        }

        for(Line edgeLine : edgeLines){
            this.elementsGroup.getChildren().add(edgeLine);
        }

    }

    /**
     * Visualizes Agents
     * @param currentAgents AgentController that keeps AgentVisuals, visual representation of Agents
     */
    private void                     visualizeAgents(AgentController currentAgents){

        Set<Integer> agentIDs = currentAgents.getAgentIDs();

        for( Integer agentID : agentIDs ) {

            this.agentKeys.add(agentID.toString());

            Circle agentCircle = currentAgents.getAgentVisual(agentID).getAgentCircle();

            agentCircle.setFill(this.colorSetter.getMovingAgentColor());

            this.unmarkedCircles.add(agentID);

            agentCircle.setOnMouseClicked( e -> {


                if(this.unmarkedCircles.contains(Integer.parseInt(agentCircle.getId()))){

                    this.unmarkedCircles.remove(Integer.parseInt(agentCircle.getId()));

                    agentCircle.setFill(this.colorSetter.getMarkedAgentColor());

                    System.out.println("Marked " + Integer.parseInt(agentCircle.getId()));

                }
                else{

                    this.unmarkedCircles.add(Integer.parseInt(agentCircle.getId()));

                    agentCircle.setFill(this.colorSetter.getMovingAgentColor());

                    System.out.println("Unmarked " + Integer.parseInt(agentCircle.getId()));

                }

                //-------------------------------------------------------------------

            });

            this.elementsGroup.getChildren().add(agentCircle);
            this.createAgentLabel(agentCircle);


        }

    }

    //-----------------------------------------------------------------

    /**
     * Creates and assigns label to Agent
     * @param agentCircle current visual representation of Agent
     */
    private void                     createAgentLabel(Circle agentCircle){

        Label agentLabel = new Label();

        Double agentRadius = agentCircle.getRadius();

        agentLabel.setScaleX(0.15 * agentRadius);
        agentLabel.setScaleY(0.15 * agentRadius);

        agentLabel.setLayoutX(agentCircle.getCenterX());
        agentLabel.setLayoutY(agentCircle.getCenterY());

        agentLabel.setText(agentCircle.getId());

        agentLabel.setVisible(false);

        this.agentLabels.put(agentCircle.getId(), agentLabel);

        this.elementsGroup.getChildren().add(agentLabel);

    }

    /**
     * Creates and assigns label to Vertex
     * @param vertexCircle current visual representation of Vertex
     */
    private void                     createGraphLabel(Circle vertexCircle){

        Label graphLabel = new Label();

        graphLabel.setLayoutX(vertexCircle.getCenterX());
        graphLabel.setLayoutY(vertexCircle.getCenterY());

        graphLabel.setText(vertexCircle.getId());

        graphLabel.setVisible(false);

        this.graphLabels.put(vertexCircle.getId(), graphLabel);
        this.elementsGroup.getChildren().add(graphLabel);

    }

    /**
     * Sets Agent Labels visibility mode
     */
    @FXML
    private void                     setAgentLabelVisibilityCheck(){

        Set<String> agentLabelKeys = this.agentLabels.keySet();


        if(this.showAgentLabelsCheck.isSelected()){

            for(String agentLabelKey : agentLabelKeys){

                Label tmpAgentLabel = this.agentLabels.get(agentLabelKey);

                if(this.getCircle(tmpAgentLabel.getText()).isVisible())
                  tmpAgentLabel.setVisible(true);

            }


        }
        else{

            for(String agentLabelKey : agentLabelKeys){

                Label tmpAgentLabel = this.agentLabels.get(agentLabelKey);

                tmpAgentLabel.setVisible(false);

            }

        }

    }

    /**
     * Sets Graph Labels visibility mode
     */
    @FXML
    private void                     setGraphLabelVisibilityCheck(){

        Set<String> graphLabelKeys = this.graphLabels.keySet();


        if(this.showGraphLabelsCheck.isSelected()){

            for(String graphLabelKey : graphLabelKeys){

                Label tmpGraphLabel = this.graphLabels.get(graphLabelKey);

                tmpGraphLabel.setVisible(true);

            }


        }
        else{

            for(String graphLabelKey : graphLabelKeys){

                Label tmpGraphLabel = this.graphLabels.get(graphLabelKey);

                tmpGraphLabel.setVisible(false);

            }

        }




    }

    //-----------------------------------------------------------------

    /**
     * Removes all elements from Visualization Pane
     */
    @FXML
    public void                      clearVisualizationPane(){

            this.visualizationStarted = false;
            this.unmarkedCircles.clear();

            this.elementsGroup.getChildren().clear();
            //this.mainVisualPane.getChildren().clear();

    }

    //-----------------------------------------------------------------

    /**
     * Makes all Agents invisible, except two Agents, which are involved in Collision event
     * @param firstAgentIndex Agent unique identifier
     * @param secondAgentIndex Agent unique identifier
     */
    public void                      setCollisionAgents(Integer firstAgentIndex, Integer secondAgentIndex){

        Set<Integer> agentIDs = this.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs();

        for(Integer agentID : agentIDs){
             this.unmarkedCircles.add(agentID);
             this.getCircle(agentID).setFill(this.colorSetter.getMovingAgentColor());
             this.getCircle(agentID).setVisible(false);
             this.getAgentLabel(agentID.toString()).setVisible(false);
             this.agentVisible = false;
        }

        this.unmarkedCircles.remove(firstAgentIndex);
        this.unmarkedCircles.remove(secondAgentIndex);

        this.getCircle(firstAgentIndex).setFill(this.colorSetter.getMarkedAgentColor());
        this.getCircle(firstAgentIndex).setVisible(true);

        this.getCircle(secondAgentIndex).setFill(this.colorSetter.getMarkedAgentColor());
        this.getCircle(secondAgentIndex).setVisible(true);

        if(this.showAgentLabelsCheck.isSelected()){
            this.getAgentLabel(firstAgentIndex.toString()).setVisible(true);
            this.getAgentLabel(secondAgentIndex.toString()).setVisible(true);
       }


    }

    //-----------------------------------------------------------------

    /**
     * Sets Agents visibility mode
     */
    @FXML
    public void                      setAgentsVisibilityCheck(ActionEvent event){

        List keyList = new ArrayList<>(this.unmarkedCircles);

        Integer agentAmount = keyList.size();

        if( agentVisible ){

            for(Integer i = 0; i < agentAmount; i++){
                this.getCircle((Integer)keyList.get(i)).setVisible(false);
                this.getAgentLabel(keyList.get(i).toString()).setVisible(false);
            }

            agentVisible = false;
        }
        else
        {

            for(Integer i = 0; i < agentAmount; i++){
                this.getCircle((Integer)keyList.get(i)).setVisible(true);
                this.getAgentLabel(keyList.get(i).toString()).setVisible(false);
                this.unmarkedCircles.add((Integer)keyList.get(i));
            }

            agentVisible = true;

        }


    }

    /**
     * Sets Graph visibility mode
     */
    @FXML
    public void                      setGraphVisibilityCheck(ActionEvent event){

        this.mainController.getCurrentGraphController().getGraphVisual().changeVisibility();

    }

    //-----------------------------------------------------------------

    /**
     * Takes photo of current visualization, if Statistics windows are open, it takes a photo of them as well
     */

    @FXML
    private void                     takeSnapshot() throws AWTException, IOException {


        this.photoOutputController.takeSnapshot("solution", this.mainVisualPane);

       if(this.isSingleStatisticsSet){
           this.photoOutputController.takeSnapshot("single statistics chart", this.singleAgentStatisticsGUI.getAgentChart());
           this.photoOutputController.takeSnapshot("single statistics bar chart", this.singleAgentStatisticsGUI.getTimeRatioChart());
       }
       else if(this.isDoubleStatisticsSet){
           this.photoOutputController.takeSnapshot("double statistics chart", this.doubleAgentStatisticsGUI.getAgentChart());
       }

    }

    /**
     * Records video of the screen
     */
    @FXML
    private void                     takeVideo() {

        if(this.startVideoButton.getText().equals("Start video")) {

            System.out.println("starting video capture");

            if( this.videoWidth == 0 && this.videoHeight == 0 )
                this.videoOutputController.startVideoCapture();
            else
                this.videoOutputController.startVideoCapture(this.videoWidth, this.videoHeight);


            this.startVideoButton.setText("Stop video");

        }
        else{

            System.out.println("ending video capture");

            this.videoOutputController.stopVideoCapture();

            this.startVideoButton.setText("Start video");

        }


    }

    /**
     * Opens video settings window
     */
    @FXML
    private void                     openVideoSettings() throws IOException {

        FXMLLoader loader = new FXMLLoader(getClass().getResource("VideoSettings.fxml"));

        Parent root = loader.load();

        Stage videoSettingsStage = new Stage();
        videoSettingsStage.setTitle("video settings");
        videoSettingsStage.setScene(new Scene(root));
        videoSettingsStage.show();

        VideoSettingsGUI videoSettingsGUI = loader.getController();
        videoSettingsGUI.setMainGUI(this);


    }

    /**
     * Sets video settings parameters
     * @param videoWidth width of the screen/recorded image
     * @param videoHeight height of the screen/recorded image
     */
    public void                      setVideoSettings(Integer videoWidth, Integer videoHeight){

        this.videoWidth  = videoWidth;
        this.videoHeight = videoHeight;

        System.out.println("video settings " + this.videoWidth + " " + this.videoHeight);

    }


    //-----------------------------------------------------------------

    /**
     * Uploads solution and starts visualization process
     */
    @FXML
    private void                     startVisualization(){


        if(visualizationStarted){
            this.animationController.stopAnimation();
            this.visualizationFinished();
            visualizationStarted = false;
        }


        TreeItem<String> selectedSolutionItem = (TreeItem<String>)this.solutionTree.getSelectionModel().getSelectedItem();

        String selectedSolutionString;

        if(selectedSolutionItem != null)
           selectedSolutionString = selectedSolutionItem.getValue();
        else
        {
            AlertBox.display("loading error", "Solution has not been found");
            return;
        }

        this.currentSolutionKey = selectedSolutionString;
        this.loadChosenSolution(selectedSolutionString);

        //----------------------------------


        GraphController currentSolution = this.mainController.getCurrentGraphController();
        AgentController currentAgents   = currentSolution.getCurrentAgentController();

        //----------------------------------

        Graph currentGraph = currentSolution.getGraph();

        //----------------------------------

        this.visualizeSolution(currentSolution.getGraphVisual(), currentAgents);

        this.changeAgentColor();
        this.changeGraphColor();

        //----------------------------------

        this.animationController = new AnimationController(this);


        animationController.initializeTimeline( currentSolution, currentAgents);

        this.initializeControls();
        this.startStatistics();


        //----------------------------------

        this.visualizationStarted = true;

    }

    /**
     * Clears buffers after visualization process has been finished
     */
    public void                      visualizationFinished(){

        this.isPlaying = false;
        this.playButton.setText("play");

    }

    /**
     * Sets current solution that will be visualized
     * @param solutionKey uploaded solution unique identifier
     * @return true if solution is found, false if soluttion is not found
     */
    public boolean                   loadChosenSolution(String solutionKey){

        boolean isLoaded = this.mainController.setCurrentSolution(solutionKey);

        return isLoaded;

    }

    //-----------------------------------------------------------------

    /**
     * Initializes slider, that is responsible for rotation of visualization
     */
    private void                     setRotationSlider(){

        rotationSlider.setMin(0);
        rotationSlider.setMax(180);
        rotationSlider.setValue(40);
        rotationSlider.setShowTickLabels(true);
        rotationSlider.setShowTickMarks(true);
        rotationSlider.setMajorTickUnit(50);
        rotationSlider.setMinorTickCount(5);
        rotationSlider.setBlockIncrement(1);
        rotationSlider.valueProperty().addListener(new ChangeListener<Number>() {

            public void changed(ObservableValue<? extends Number> ov, Number oldVal, Number newVal){
                mainVisualPane.setRotate(newVal.doubleValue());
                System.out.println(mainVisualPane.getRotate());
            }

        });

        rotationSlider.setOnMouseReleased( e -> {

            Iterator agentLabelMapIt = this.agentLabels.entrySet().iterator();
            Iterator graphLabelMapIt = this.graphLabels.entrySet().iterator();

            double rotationRate = mainVisualPane.getRotate();

            while ( agentLabelMapIt.hasNext() ) {

                Map.Entry labelElement = (Map.Entry) agentLabelMapIt.next();
                ((Label) labelElement.getValue()).setRotate(-rotationRate);

            }

            while ( graphLabelMapIt.hasNext() ){

                Map.Entry graphElement = (Map.Entry)  graphLabelMapIt.next();
                ((Label) graphElement.getValue()).setRotate(-rotationRate);

            }

        });

    }

    /**
     * Initializes slider, that is responsible for Animation time manipulation
     */
    private void                     setTimelineSlider(){

        timelineSlider.setMin(0);
        timelineSlider.setMax(this.animationController.getTimelineDurationMilliseconds());
        timelineSlider.setValue(40);
        timelineSlider.setShowTickLabels(true);
        timelineSlider.setShowTickMarks(true);
        timelineSlider.setMajorTickUnit(50);
        timelineSlider.setMinorTickCount(5);
        timelineSlider.setBlockIncrement(10);


        timelineSlider.valueProperty().addListener(new ChangeListener<Number>() {

            public void changed(ObservableValue<? extends Number> ov, Number oldVal, Number newVal){
                animationController.playAnimationFrom(newVal);
            }

        });

    }

    //--------------------------------------------------------------------

    /**
     * Initializes ScheduledExecutorService that calls every second a statistics update on data during animation process
     */
    private void                     startStatistics(){


        // schedule service initialization
        ScheduledExecutorService scheduledExecutorService;
        scheduledExecutorService = Executors.newSingleThreadScheduledExecutor();

        scheduledExecutorService.scheduleAtFixedRate(() -> {

            Platform.runLater(() -> {

                if(this.isSingleStatisticsSet && this.isPlaying) {

                    double millisTime = animationController.getCurrentTimeMilliseconds();
                    int millisTimeInt = (int) millisTime;

                    this.singleAgentStatisticsGUI.updateCurrentTime(millisTimeInt);

                }
                else if(this.isDoubleStatisticsSet && this.isPlaying) {

                 double millisTime = animationController.getCurrentTimeMilliseconds();
                 int millisTimeInt = (int) millisTime;

                 this.doubleAgentStatisticsGUI.updateCurrentTime(millisTimeInt);

                }
                else if(this.isGroupStatisticsSet && this.isPlaying){

                    double millisTime = animationController.getCurrentTimeMilliseconds();
                    int millisTimeInt = (int) millisTime;

                    this.groupAgentStatisticsGUI.updateCurrentTime(millisTimeInt);

                }

            });
        }, 0, 1, TimeUnit.SECONDS);


    }

    /**
     * Sets SingleAgentStatistics parameters and opens its window
     */
    @FXML
    private void                     setSingleAgentStatistics() throws IOException {

        if(!this.visualizationStarted){
            AlertBox.display("loading error", "Solution has not been visualized");
            return;
        }


        FXMLLoader loader = new FXMLLoader(getClass().getResource("SingleAgentStatistics.fxml"));

        Parent root = loader.load();

        Stage singleStatisticsStage = new Stage();
        singleStatisticsStage.setTitle("single agent statistics data view");
        singleStatisticsStage.setScene(new Scene(root));
        singleStatisticsStage.show();
        singleStatisticsStage.setOnHidden(e -> {
            this.isSingleStatisticsSet = false;
            System.out.println("Single statistics closing");
        });


        this.singleAgentStatisticsGUI = loader.getController();
        this.singleAgentStatisticsGUI.setStatisticsData(this, this.mainController.getCurrentGraphController().getGraphStatisticsData());

        this.isSingleStatisticsSet = true;
        this.isDoubleStatisticsSet = false;
        this.isGroupStatisticsSet  = false;

    }

    /**
     * Sets DoubleAgentStatistics parameters and opens its window
     */
    @FXML
    private void                     setDoubleAgentStatistics() throws IOException {


        if(!this.visualizationStarted){
            AlertBox.display("loading error", "Solution has not been visualized");
            return;
        }


        FXMLLoader loader = new FXMLLoader(getClass().getResource("DoubleAgentStatistics.fxml"));

        Parent root = loader.load();

        Stage doubleStatisticsStage = new Stage();
        doubleStatisticsStage.setTitle("double agent statistics data view");
        doubleStatisticsStage.setScene(new Scene(root));
        doubleStatisticsStage.show();double millisTime = animationController.getCurrentTimeMilliseconds();
                 int millisTimeInt = (int) millisTime;


        doubleStatisticsStage.setOnHiding(e -> {
            this.isDoubleStatisticsSet = false;
            System.out.println("Double statistics closing");
        });

        this.doubleAgentStatisticsGUI = loader.getController();
        this.doubleAgentStatisticsGUI.setStatisticsData(this, this.mainController.getCurrentGraphController().getGraphStatisticsData());

        this.isSingleStatisticsSet = false;
        this.isDoubleStatisticsSet = true;
        this.isGroupStatisticsSet  = false;


    }

    /**
     * Sets GroupAgentStatistics parameters and opens its window
     */
    @FXML
    private void                     setGroupAgentStatistics() throws IOException {


        if(!this.visualizationStarted){
            AlertBox.display("loading error", "Solution has not been visualized");
            return;
        }


        FXMLLoader loader = new FXMLLoader(getClass().getResource("GroupAgentStatistics.fxml"));

        Parent root = loader.load();

        Stage groupStatisticsStage = new Stage();
        groupStatisticsStage.setTitle("group agent statistics data view");
        groupStatisticsStage.setScene(new Scene(root));
        groupStatisticsStage.show();
        groupStatisticsStage.setOnHiding(e -> {
            this.isGroupStatisticsSet = false;
            System.out.println("Group statistics closing");
        });

        this.groupAgentStatisticsGUI = loader.getController();
        this.groupAgentStatisticsGUI.setStatisticsData(this, this.mainController.getCurrentGraphController().getGraphStatisticsData());

        this.isDoubleStatisticsSet = false;
        this.isSingleStatisticsSet = false;
        this.isGroupStatisticsSet  = true;


    }

    /**
     * Sets and returns a group of agents that will be involved in GroupAgentStatistics
     * @return Set<Integer> collection of Agent unique identifiers
     */
    public  Set<Integer>             getGroupAgents(){

        Set<Integer> groupAgents = new HashSet<>();

        Set<Integer> allAgents = this.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs();

        for(Integer agentID : allAgents){
            if(!this.unmarkedCircles.contains(agentID)){
                groupAgents.add(agentID);
            }
        }

        return groupAgents;


    }

    //--------------------------------------------------------------------

    /**
     * Opens FileLoader window
     */
    @FXML
    private void                     setFileLoader() throws IOException {

        FXMLLoader loader = new FXMLLoader(getClass().getResource("LoadFile.fxml"));

        Parent root = loader.load();

        Stage fileLoadStage = new Stage();
        fileLoadStage.setTitle("Upload new solution");
        fileLoadStage.setScene(new Scene(root));
        fileLoadStage.show();

        LoadFileGUI loadFileGUI = loader.getController();
        loadFileGUI.setMainGUI(this);

    }

    /**
     * Sets solution paramters that are needed for uploading new solution
     * @param solutionKey name of the solution
     * @param agentFileString name and address of Agent input data file
     * @param graphFileString name and address of Graph input data file
     * @param planFileString name and address of Plans input data file
     */
    public void                      setSolution(String solutionKey, String agentFileString, String graphFileString, String planFileString) throws FileNotFoundException {

        if(this.mainController.uploadSolution( solutionKey, agentFileString, graphFileString, planFileString )) {
            this.currentSolutionKey = solutionKey;
            this.setSolutionList();
        }
        else{
            AlertBox.display("loading error","incorrect solution files");
        }


    }

    /**
     * Displays solutions that have been uploaded
     */

    public void                      setSolutionList(){

        TreeItem<String> root = new TreeItem<String>("Solution list");
        root.setExpanded(true);


        Set<String>  keySet = this.mainController.getKeys();

        for(String key : keySet){

            TreeItem<String> treeItem = new TreeItem<String>(key);
            root.getChildren().add(treeItem);

        }

        this.solutionTree.setRoot(root);
        this.solutionTree.setShowRoot(false);

    }

    //--------------------------------------------------------------------

    /**
     * Initializes window user interface control elements(Buttons, Sliders, etc.)
     */
    private void                     initializeControls(){

        this.setRotationSlider();
        this.setTimelineSlider();


        playButton.setOnAction( e -> {

            if( this.animationController.getAnimationStatus() != Animation.Status.RUNNING ) {

                this.animationController.playAnimation();

                playButton.setText("pause");

                this.isPlaying = true;

            }
            else if( this.animationController.getAnimationStatus() == Animation.Status.RUNNING){

                this.animationController.pauseAnimation();

                playButton.setText("play");

                this.isPlaying = false;

            }

        });

        speedUpButton.setOnAction( e -> {
            this.animationController.speedUpAnimation();
        });

        slowDownButton.setOnAction( e -> {
            this.animationController.slowDownAnimation();
         });

        stopButton.setOnAction( e -> {

            this.animationController.stopAnimation();

            playButton.setText("play");

            this.isPlaying = false;

        });

        forwardButton.setOnAction( e -> {
            this.animationController.forwardDirectionAnimation();
         });

        backwardButton.setOnAction( e -> {
            this.animationController.backwardDirectionAnimation();
        });

    }


    //--------------------------------------------------------------------

    /**
     * Opens ColorChanger window
     */
    @FXML
    public void                      setColorChanger() throws IOException {

        FXMLLoader loader = new FXMLLoader(getClass().getResource("ColorChanger.fxml"));

        Parent root = loader.load();

        Stage colorChangeStage = new Stage();
        colorChangeStage.setTitle("change color");
        colorChangeStage.setScene(new Scene(root));
        colorChangeStage.show();

        ColorChangerGUI colorChangerGUI = loader.getController();
        colorChangerGUI.setMainGui(this);

    }

    /**
     * Sets a current ColorSetter object
     * @param colorSetter object that stores color settings data
     */
    public void                      setColorSetter(ColorSetter colorSetter){

        if(!(this.colorSetter.equals(colorSetter))) {

            this.colorSetter = colorSetter;

            if( this.visualizationStarted ) {
                this.changeAgentColor();
                this.changeGraphColor();
            }

        }

    }

    /**
     * Runs through all Agents and changes their color according to current color settings
     */
    private void                     changeAgentColor(){

        Set<Integer> agentIDs = this.mainController.getCurrentGraphController().getCurrentAgentController().getAgentIDs();

        for(Integer agentID : agentIDs){
            this.getCircle(agentID).setFill(this.colorSetter.getMovingAgentColor());
        }

    }

    /**
     * Runs through all Vertices of the Graph and changes their color according to current color settings
     */
    private void                     changeGraphColor(){
           this.mainController.getCurrentGraphController().getGraphVisual().changeVertexColor(this.colorSetter.getVertexColor());
    }

    //--------------------------------------------------------------------

    /**
     * Opens SolutionParameters window
     */
    @FXML
    public void                      setSolutionParameters() throws IOException {

        if(this.currentSolutionKey.equals("")){
            AlertBox.display("loading error", "Solution has not been found");
            return;
        }


        GraphController currentGraphController = this.mainController.getCurrentGraphController();

        FXMLLoader loader = new FXMLLoader(getClass().getResource("SolutionParameters.fxml"));

        Parent root = loader.load();

        Stage solutionParametersStage = new Stage();
        solutionParametersStage.setTitle("solution parameters");
        solutionParametersStage.setScene(new Scene(root));
        solutionParametersStage.show();

        SolutionParametersGUI solutionParametersGUI = loader.getController();
        solutionParametersGUI.setSolutionParameters(this.currentSolutionKey,
                                                    currentGraphController.getGraph().getVertexAmount(),
                                                    currentGraphController.getGraph().getEdgeAmount(),
                                                    currentGraphController.getGraph().getGraphDensity(),
                                                    currentGraphController.getCurrentAgentController() );

    }

    /**
     * Returns current TimerLabel, which is basically is a timer watch to display current time of animation
     * @return Label timer that diplays animation time
     */
    public Label                     getTimerLabel() {
        return this.timer;
    }

    //--------------------------------------------------------------------

    @FXML
    private void openHelp() throws IOException {

        FXMLLoader loader = new FXMLLoader(getClass().getResource("Help.fxml"));

        Parent root = loader.load();

        Stage helpStage = new Stage();
        helpStage.setTitle("manual");
        helpStage.setScene(new Scene(root));
        helpStage.show();
    }

    /**
     * Initializes the main VisualPane, where visualization process is being run.
     */
    public void                     setMainVisualPane(){


         this.elementsGroup = new Group();
         this.mainVisualPane.getChildren().add(this.elementsGroup);

         this.mainVisualPane.setOnScroll(
                 e->{

                     e.consume();

                     if (e.getDeltaY() == 0) {
                         return;
                     }

                     double scaleFactor = (e.getDeltaY() > 0)
                             ? SCALE_DELTA
                             : 1/SCALE_DELTA;


                     Scale newScale = new Scale();
                     newScale.setPivotX(e.getX());
                     newScale.setPivotY(e.getY());
                     newScale.setX( this.elementsGroup.getScaleX() * scaleFactor );
                     newScale.setY( this.elementsGroup.getScaleY() * scaleFactor );


                     this.elementsGroup.getTransforms().add(newScale);

                 }
         );




     }

    //--------------------------------------------------------------------

    @Override
    public void                      initialize(URL url, ResourceBundle resourceBundle)
    {
        this.photoOutputController = new PhotoOutputController();
        this.videoOutputController = new VideoOutputController();

        this.videoWidth = 0;
        this.videoHeight = 0;

        this.currentSolutionKey = "";

        this.agentLabels       = new TreeMap<>();
        this.graphLabels       = new TreeMap<>();
        this.agentKeys         = new HashSet<>();
        this.notVisibleCircles = new ArrayList<>();
        this.notVisibleLabels  = new ArrayList<>();
        this.unmarkedCircles   = new HashSet<>();
        this.agentVisible      = true;

        this.isSingleStatisticsSet = false;
        this.isDoubleStatisticsSet = false;
        this.isGroupStatisticsSet  = false;

        this.elementsGroup = new Group();


    }



}
