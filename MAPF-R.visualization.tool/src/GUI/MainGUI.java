package GUI;

import Business.*;
import PhotoVideoOutput.PhotoOutputController;
import PhotoVideoOutput.VideoOutputController;
import Visualization.*;
import Visualization.AgentVisual;
import Visualization.ColorSetter;
import Visualization.GraphVisual;
import javafx.animation.Animation;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.input.Dragboard;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.Pane;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.stage.FileChooser;
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

public class MainGUI implements Initializable {

    MainController                    mainController = new MainController();

    //---------------------------------------------------------------------

    final double                      SCALE_DELTA = 1.1;

    //---------------------------------------------------------------------

    private List<String>              notVisibleCircles;
    private Set<Integer>              unmarkedCircles;
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

    private boolean                  isSingleStatisticsSet;
    private boolean                  isDoubleStatisticsSet;

    private boolean                  isPlaying;

    //---------------------------------------------------------------------

    public  Label                     getAgentLabel(String agentID){
       return this.agentLabels.get(agentID);
    }

    public Label                      getGraphLabel(String graphID){
       return this.graphLabels.get(graphID);
    }

    public  Circle                    getCircle(Integer id) {
        return (Circle)mainVisualPane.lookup('#' + id.toString());
    }

    public  Circle                    getCircle(String id) {
        return (Circle)mainVisualPane.lookup('#' + id);
    }


    public  MainController            getMainController(){
        return this.mainController;
    }

    public  ColorSetter               getColorSetter(){
        return this.colorSetter;
    }

    //---------------------------------------------------------------------

    private void                     zoomInfo(Double scaleX, Double scaleY){
        System.out.println("scale x: " + scaleX + " scale y:" + scaleY);
    }

    //---------------------------------------------------------------------

    private void                     visualizeSolution(GraphVisual graphVisual, AgentController currentAgents){

        this.clearVisualizationPane();

        this.visualizeGraph(graphVisual);
        this.visualizeAgents(currentAgents);
    }

    private void                     visualizeGraph(GraphVisual graphVisual) {

        List<Circle> vertexCircles = graphVisual.getVertexCircles();
        List<Line>   edgeLines     = graphVisual.getEdgeLines();

        for(Circle vertexCircle : vertexCircles ){
            this.mainVisualPane.getChildren().add(vertexCircle);
            this.createGraphLabel(vertexCircle);
        }

        for(Line edgeLine : edgeLines){
            this.mainVisualPane.getChildren().add(edgeLine);
        }

    }

    private void                     visualizeAgents(AgentController currentAgents){

        Set<Integer> agentIDs = currentAgents.getAngentIDs();

        for( Integer agentID : agentIDs ) {

            this.agentKeys.add(agentID.toString());

            Circle agentCircle = currentAgents.getAgentVisual(agentID).getAgentCircle();

            agentCircle.setFill(this.colorSetter.getMovingAgentColor());

            this.unmarkedCircles.add(Integer.parseInt(agentCircle.getId()));

            agentCircle.setOnMouseClicked( e -> {

                if(agentCircle.getFill() == this.colorSetter.getMovingAgentColor()) {

                    agentCircle.setFill(this.colorSetter.getMarkedAgentColor());

                    this.unmarkedCircles.remove(Integer.parseInt(agentCircle.getId()));

                    System.out.println("Marked " + Integer.parseInt(agentCircle.getId()));

                }
                else if(agentCircle.getFill() == this.colorSetter.getMarkedAgentColor()) {

                     agentCircle.setFill(this.colorSetter.getMovingAgentColor());

                     this.unmarkedCircles.add(Integer.parseInt(agentCircle.getId()));

                     System.out.println("Unmarked " + Integer.parseInt(agentCircle.getId()));

                }

            });


            mainVisualPane.getChildren().add( agentCircle );
            this.createAgentLabel(agentCircle);


        }

    }


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
        this.mainVisualPane.getChildren().add(agentLabel);


    }

    private void                     createGraphLabel(Circle vertexCircle){

        Label graphLabel = new Label();

        graphLabel.setLayoutX(vertexCircle.getCenterX());
        graphLabel.setLayoutY(vertexCircle.getCenterY());

        graphLabel.setText(vertexCircle.getId());

        graphLabel.setVisible(false);

        this.graphLabels.put(vertexCircle.getId(), graphLabel);
        this.mainVisualPane.getChildren().add(graphLabel);

    }

    @FXML
    private void                     changeAgentLabelVisibility(){

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

    @FXML
    private void                     changeGraphLabelVisibility(){

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

    @FXML
    public void                      clearVisualizationPane(){

            this.visualizationStarted = false;
            this.unmarkedCircles.clear();

            this.mainVisualPane.getChildren().clear();
    }

    //-----------------------------------------------------------------

    public void                      setCollisionAgents(Integer firstAgentIndex, Integer secondAgentIndex){

        Set<Integer> agentIDs = this.mainController.getCurrentGraphController().getCurrentAgentController().getAngentIDs();

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

    @FXML
    public void                      setGraphVisibilityCheck(ActionEvent event){

        this.mainController.getCurrentGraphController().getGraphVisual().changeVisibility();

    }

    //-----------------------------------------------------------------

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

    @FXML
    private void                     takeVideo() {

        if(this.startVideoButton.getText().equals("Start video")) {

            System.out.println("starting video capture");

            this.videoOutputController.startVideoCapture();

            this.startVideoButton.setText("Stop video");

        }
        else{

            System.out.println("ending video capture");

            this.videoOutputController.stopVideoCapture();

            this.startVideoButton.setText("Start video");

        }


    }


    //-----------------------------------------------------------------

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

    public void                      visualizationFinished(){

        this.isPlaying = false;
        this.playButton.setText("play");

    }

    public boolean                   loadChosenSolution(String solutionKey){

        boolean isLoaded = this.mainController.setCurrentSolution(solutionKey);

        return isLoaded;

    }

    //-----------------------------------------------------------------

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

            Iterator labelMapIt = this.agentLabels.entrySet().iterator();

            double rotationRate = mainVisualPane.getRotate();

            while (labelMapIt.hasNext()) {

                Map.Entry labelElement = (Map.Entry) labelMapIt.next();
                ((Label) labelElement.getValue()).setRotate(-rotationRate);

            }

        });

    }

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

            });
        }, 0, 1, TimeUnit.SECONDS);


    }

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

    }

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
        doubleStatisticsStage.show();
        doubleStatisticsStage.setOnHiding(e -> {
            this.isDoubleStatisticsSet = false;
            System.out.println("Double statistics closing");
        });

        this.doubleAgentStatisticsGUI = loader.getController();
        this.doubleAgentStatisticsGUI.setStatisticsData(this, this.mainController.getCurrentGraphController().getGraphStatisticsData());

        this.isDoubleStatisticsSet = true;
        this.isSingleStatisticsSet = false;


    }

    //--------------------------------------------------------------------

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

    public void                      setSolution(String solutionKey, String fileString, String graphFileString, String planFileString) throws FileNotFoundException {

        this.mainController.uploadSolution( solutionKey, fileString, graphFileString, planFileString );

        this.currentSolutionKey = solutionKey;

    }

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

    public void                      setColorSetter(ColorSetter colorSetter){

        if(!(this.colorSetter.equals(colorSetter))) {

            this.colorSetter = colorSetter;

            if( this.visualizationStarted ) {
                this.changeAgentColor();
                this.changeGraphColor();
            }

        }

    }

    private void                     changeAgentColor(){

        Set<Integer> agentIDs = this.mainController.getCurrentGraphController().getCurrentAgentController().getAngentIDs();

        for(Integer agentID : agentIDs){
            this.getCircle(agentID).setFill(this.colorSetter.getMovingAgentColor());
        }

    }

    private void                     changeGraphColor(){
           this.mainController.getCurrentGraphController().getGraphVisual().changeVertexColor(this.colorSetter.getVertexColor());
    }

    //--------------------------------------------------------------------

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

    public Label                     getTimerLabel() {
        return this.timer;
    }

    //--------------------------------------------------------------------



    @Override
    public void                      initialize(URL url, ResourceBundle resourceBundle)
    {
        this.photoOutputController = new PhotoOutputController();
        this.videoOutputController = new VideoOutputController();

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

        mainVisualPane.setOnScroll(
                e->{

                    e.consume();

                    if (e.getDeltaY() == 0) {
                        return;
                    }

                    double scaleFactor = (e.getDeltaY() > 0)
                            ? SCALE_DELTA
                            : 1/SCALE_DELTA;

                    mainVisualPane.setScaleX(mainVisualPane.getScaleX() * scaleFactor);
                    mainVisualPane.setScaleY(mainVisualPane.getScaleY() * scaleFactor);


                    this.zoomInfo(mainVisualPane.getScaleX(), mainVisualPane.getScaleY());

                }
        );

        mainVisualPane.setOnDragDetected(mouseEvent -> {

            System.out.println("Drag");

            Dragboard db = mainVisualPane.startDragAndDrop(TransferMode.ANY);

            mouseEvent.consume();

        });



    }



}
