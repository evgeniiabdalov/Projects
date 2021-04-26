package Visualization;

import Business.*;
import GUI.MainGUI;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.scene.control.Label;
import javafx.scene.shape.Circle;
import javafx.util.Duration;

import java.util.Set;
import java.util.TreeMap;

/**
 * AnimationController class controls animation process
 * @author Evgenii Abdalov
 */
public class AnimationController {


    private MainGUI         mainGUI;

    private Timeline        timeline;


    /**
     * AnimationController constructor classs
     * @param mainGUI an instance of MainGUI object with which communication is being handled
     */
    public                  AnimationController(MainGUI mainGUI){

        this.mainGUI = mainGUI;

        this.timeline = new Timeline();


        this.timeline.setOnFinished(e->{

            this.mainGUI.visualizationFinished();

        });




    }

    /**
     * Initializes timeline settings
     * @param graphController an instance of GraphController object, which provides with Graph
     * @param currentAgents an instance of AgentController object, which provides with Agents
     */
    public void             initializeTimeline( GraphController graphController, AgentController currentAgents ) {

        timeline.setCycleCount(1);
        timeline.setAutoReverse(false);

        this.getKeyFrames( graphController, currentAgents );

    }

    /**
     * Creates key frames for movement animation of each Agent according to its Plan and Verices coordinates
     * @param agentCircle visual representation of Agent
     * @param plan Schedule of Agent movement that should be animated
     * @param verticesMap collection of Vertices on which movement is being handled
     */
    private void            moveTimeline( Circle agentCircle, Plan plan, TreeMap<Integer, Vertex> verticesMap ){

        double timeMultiplyer = 1000;

        Step firstStep = plan.getStepList().get(0);

        Vertex vertexFrom = verticesMap.get(firstStep.getFrom());

        agentCircle.setCenterX(vertexFrom.getXCoordinate() * 100);
        agentCircle.setCenterY(vertexFrom.getYCoordinate() * 100);
        agentCircle.setVisible(true);

        double time = 0;

        Label agentLabel = this.mainGUI.getAgentLabel( agentCircle.getId() );
        //agentLabel.setText( agentCircle.getId() );
        agentLabel.setLayoutX(vertexFrom.getXCoordinate() * 100 - 10);
        agentLabel.setLayoutY(vertexFrom.getYCoordinate() * 100 - 15);


        this.labelTimeline( agentLabel, Duration.millis(time), vertexFrom.getXCoordinate(), vertexFrom.getYCoordinate() );

        final KeyValue keyValue1     = new KeyValue(agentCircle.centerXProperty(), vertexFrom.getXCoordinate() * 100);
        final KeyValue keyValue2     = new KeyValue(agentCircle.centerYProperty(), vertexFrom.getYCoordinate() * 100);
        final KeyFrame firstKeyFrame = new KeyFrame(Duration.millis(time),
                event->{


                 if( !agentCircle.getFill().equals(this.mainGUI.getColorSetter().getMarkedAgentColor()) ) {


                     double timeDouble = timeline.getCurrentTime().toMillis();

                     int timeInt = (int) timeDouble;

                     String agentState = this.mainGUI.getMainController().getAgentState(Integer.parseInt(agentCircle.getId()), timeInt);


                     if (agentState.equals("INITIALIZING")) {
                         agentCircle.setFill(this.mainGUI.getColorSetter().getInitializedAgentColor());
                     } else if (agentState.equals("MOVING")) {
                         agentCircle.setFill(this.mainGUI.getColorSetter().getMovingAgentColor());
                     } else if (agentState.equals("WAITING")) {
                         agentCircle.setFill(this.mainGUI.getColorSetter().getWaitingAgentColor());
                     } else if (agentState.equals("ARRIVED")) {
                         agentCircle.setFill(this.mainGUI.getColorSetter().getArrivedAgentColor());
                     }


                  }

                },
                keyValue1,
                keyValue2);

        timeline.getKeyFrames().add(firstKeyFrame);


        for( Step step : plan.getStepList() ){


            time += step.getTimeInterval().getValue() * timeMultiplyer;

            Vertex vertexTo = verticesMap.get(step.getTo());

            this.labelTimeline( agentLabel, Duration.millis(time), vertexTo.getXCoordinate(), vertexTo.getYCoordinate() );

            final KeyValue kv1 = new KeyValue(agentCircle.centerXProperty(),vertexTo.getXCoordinate() * 100);
            final KeyValue kv2 = new KeyValue(agentCircle.centerYProperty(),vertexTo.getYCoordinate() * 100);
            final KeyFrame kf  = new KeyFrame(Duration.millis(time),
                    event->{


                     if( !agentCircle.getFill().equals(this.mainGUI.getColorSetter().getMarkedAgentColor()) ){

                        double timeDouble = timeline.getCurrentTime().toMillis();


                        int timeInt = (int) timeDouble;


                        String agentState = this.mainGUI.getMainController().getAgentState(Integer.parseInt(agentCircle.getId()), timeInt);


                        if (agentState.equals("INITIALIZING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getInitializedAgentColor());
                        } else if (agentState.equals("MOVING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getMovingAgentColor());
                        } else if (agentState.equals("WAITING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getWaitingAgentColor());
                        } else if (agentState.equals("ARRIVED")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getArrivedAgentColor());
                        }


                     }

                    },
                    kv1,
                    kv2);

            timeline.getKeyFrames().add(kf);


        }

    }

    /**
     * Assigns key frames of movement animation to an Agent from an AgentController
     * @param graphController an instance of GraphController object, which provides with Graph
     * @param currentAgents an instance of AgentController object, which provides with Agents
     */
    private void            getKeyFrames( GraphController graphController, AgentController currentAgents ) {

        Set<Integer> agentIDs = currentAgents.getAgentIDs();

        for(Integer agentID : agentIDs){
            this.moveTimeline( this.mainGUI.getCircle(agentID), currentAgents.getAgentPlan(agentID), graphController.getGraph().getVerticesMap());
        }

        this.timerKeyFrames(this.timeline.getTotalDuration(), this.mainGUI.getTimerLabel());

    }

    /**
     * Assigns key frames to timer
     * @param duration duration of time period in which animation runs
     * @param timerLabel Label object that is going to be animated
     */
    private void            timerKeyFrames(  Duration duration, Label timerLabel ) {

        int millisInt = (int)duration.toMillis();

        for( int i = 0; i < millisInt; i += 5 ){

            final KeyFrame timeKeyFrame = new KeyFrame(Duration.millis(i),
                    event->{
                        timerLabel.setText( timeline.getCurrentTime().toString() );
                    });
            timeline.getKeyFrames().add(timeKeyFrame);
        }

        final KeyFrame lastKeyFrame = new KeyFrame(Duration.millis(timeline.getTotalDuration().toMillis()),
                event->{
                    timerLabel.setText( timeline.getCurrentTime().toString() );
                });

        timeline.getKeyFrames().add(lastKeyFrame);

    }

    /**
     * Assigns key frames to Labels that follows movement of assigned Agents
     * @param agentLabel Label object that is going to be animated
     * @param duration duration of time period in which animation runs
     * @param xCoordinate x coordinate of Label position at the end of key frame
     * @param yCoordinate y coordinate of Label position at the end of key frame
     */
    private void            labelTimeline(Label agentLabel, Duration duration, double xCoordinate, double yCoordinate ){

        final KeyValue kv1 = new KeyValue(agentLabel.layoutXProperty(),xCoordinate * 100 - 10);
        final KeyValue kv2 = new KeyValue(agentLabel.layoutYProperty(),yCoordinate * 100 - 15);
        final KeyFrame kf  = new KeyFrame(duration, kv1, kv2);

        timeline.getKeyFrames().add(kf);

    }

    /**
     * Return Animation status to control animation process
     * @return Animation status which could be Running, Paused, Stopped, etc.
     */
    public Animation.Status getAnimationStatus(){
        return this.timeline.getStatus();
    }

    /**
     * Plays animation from exact time position
     * @param timePosition position in time from which animation starts
     */
    public  void            playAnimationFrom(Number timePosition){

        if( this.timeline.getStatus() == Animation.Status.PAUSED ) {
            this.timeline.jumpTo(new Duration(timePosition.doubleValue() * 1000));
            System.out.println("slider");
        }
        else{
            this.timeline.playFrom(new Duration(timePosition.doubleValue() * 1000));
            System.out.println("slider");
        }


    }

    /**
     * Plays animation from the beginning
     */
    public  void            playAnimation(){
        this.timeline.play();
    }

    /**
     * Pauses animation
     */
    public void             pauseAnimation(){
        this.timeline.pause();
    }

    /**
     * Stops animation
     */
    public void             stopAnimation(){
        this.timeline.stop();
        this.timeline.setRate(1.0);
    }

    /**
     * Speeds up animation
     */
    public void             speedUpAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if(speedRate > 0 || speedRate < 0) {
            this.timeline.setRate(speedRate * 2);
        }

    }

    /**
     * Slows down animation
     */
    public void             slowDownAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if(speedRate > 0 || speedRate < 0) {
            this.timeline.setRate(speedRate/2);
        }

    }

    /**
     * Changes animation direction forwards
     */
    public void             forwardDirectionAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if( speedRate < 0.0 )
            this.timeline.setRate(speedRate * -1);

    }

    /**
     * Changes animation direction backwards
     */
    public void             backwardDirectionAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if( speedRate > 0.0 )
            this.timeline.setRate(speedRate * -1);

    }

    /**
     * Returns total duration of animation
     * @return Double value of total animation duration
     */
    public Double           getTimelineDurationMilliseconds(){
        return this.timeline.getTotalDuration().toSeconds();
    }

    /**
     * Returns current time of animation
     * @return Double value of current time
     */
    public Double           getCurrentTimeMilliseconds(){
        return this.timeline.getCurrentTime().toMillis();
    }


}
