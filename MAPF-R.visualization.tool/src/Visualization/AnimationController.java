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

public class AnimationController {


    private MainGUI         mainGUI;

    private Timeline        timeline;


    public                  AnimationController(MainGUI mainGUI){

        this.mainGUI = mainGUI;

        this.timeline = new Timeline();


        this.timeline.setOnFinished(e->{

            this.mainGUI.visualizationFinished();

        });




    }

    public void             initializeTimeline( GraphController graphController, AgentController currentAgents ) {

        timeline.setCycleCount(1);
        timeline.setAutoReverse(false);

        this.getKeyFrames( graphController, currentAgents );

    }

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

                    double timeDouble = timeline.getCurrentTime().toMillis();

                    int timeInt = (int)timeDouble;

                    String agentState = this.mainGUI.getMainController().getAgentState(Integer.parseInt(agentCircle.getId()), timeInt);



                    if(agentState.equals("INITIALIZING")) {
                        agentCircle.setFill(this.mainGUI.getColorSetter().getInitializedAgentColor());
                    }
                    else if(agentState.equals("MOVING")) {
                        agentCircle.setFill(this.mainGUI.getColorSetter().getMovingAgentColor());
                    }
                    else if(agentState.equals("WAITING")) {
                        agentCircle.setFill(this.mainGUI.getColorSetter().getWaitingAgentColor());
                    }
                    else if(agentState.equals("ARRIVED")) {
                        agentCircle.setFill(this.mainGUI.getColorSetter().getArrivedAgentColor());
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

                        double timeDouble = timeline.getCurrentTime().toMillis();

                        int timeInt = (int)timeDouble;

                        String agentState = this.mainGUI.getMainController().getAgentState(Integer.parseInt(agentCircle.getId()), timeInt);


                        if(agentState.equals("INITIALIZING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getInitializedAgentColor());
                        }
                        else if(agentState.equals("MOVING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getMovingAgentColor());
                        }
                        else if(agentState.equals("WAITING")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getWaitingAgentColor());
                        }
                        else if(agentState.equals("ARRIVED")) {
                            agentCircle.setFill(this.mainGUI.getColorSetter().getArrivedAgentColor());
                        }



                    },
                    kv1,
                    kv2);

            timeline.getKeyFrames().add(kf);


        }

    }

    private void            getKeyFrames( GraphController graphController, AgentController currentAgents ) {

        Set<Integer> agentIDs = currentAgents.getAngentIDs();

        for(Integer agentID : agentIDs){
            this.moveTimeline( this.mainGUI.getCircle(agentID), currentAgents.getAgentPlan(agentID), graphController.getGraph().getVerticesMap());
        }

        this.timerKeyFrames(this.timeline.getTotalDuration(), this.mainGUI.getTimerLabel());

    }


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

    private void            labelTimeline(Label agentLabel, Duration duration, double xCoordinate, double yCoordinate ){

        final KeyValue kv1 = new KeyValue(agentLabel.layoutXProperty(),xCoordinate * 100 - 10);
        final KeyValue kv2 = new KeyValue(agentLabel.layoutYProperty(),yCoordinate * 100 - 15);
        final KeyFrame kf  = new KeyFrame(duration, kv1, kv2);

        timeline.getKeyFrames().add(kf);

    }



    public Animation.Status getAnimationStatus(){
        return this.timeline.getStatus();
    }

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

    public  void            playAnimation(){
        this.timeline.play();
    }

    public void             pauseAnimation(){
        this.timeline.pause();
    }

    public void             stopAnimation(){
        this.timeline.stop();
        this.timeline.setRate(1.0);
    }

    public void             speedUpAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if(speedRate > 0 || speedRate < 0) {
            this.timeline.setRate(speedRate * 2);
        }

    }

    public void             slowDownAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if(speedRate > 0 || speedRate < 0) {
            this.timeline.setRate(speedRate/2);
        }

    }

    public void             forwardDirectionAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if( speedRate < 0.0 )
            this.timeline.setRate(speedRate * -1);

    }

    public void             backwardDirectionAnimation(){

        double speedRate = this.timeline.getCurrentRate();

        if( speedRate > 0.0 )
            this.timeline.setRate(speedRate * -1);

    }

    public Double           getTimelineDurationMilliseconds(){
        return this.timeline.getTotalDuration().toSeconds();
    }

    public Double           getCurrentTimeMilliseconds(){
        return this.timeline.getCurrentTime().toMillis();
    }


}
