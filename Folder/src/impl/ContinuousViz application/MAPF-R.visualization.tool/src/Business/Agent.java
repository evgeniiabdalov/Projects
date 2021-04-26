package Business;

import java.util.TreeMap;

/**
 * Agent class represents a single Agent object that is moving on the Graph
 * @author Evgenii Abdalov
 */
public class Agent {

    private Integer      movingTime;
    private Integer      waitingTime;
    private Integer      initializingTime;
    private Integer      arrivedTime;

    private AgentState   agentState;

    private Integer      agentID;
    private String       agentStringID;

    private Integer      startPosition;
    private Integer      targetPosition;

    private Double       radius;
    private Plan         plan;

    private TreeMap<Integer, Integer> stateMap;


    /**
     * Agent class constructor
     * @param agentID
     **/
    public Agent(Integer agentID) {

        this.agentID = agentID;
        this.agentStringID = this.agentID.toString();

        this.plan = new Plan();

        this.stateMap = new TreeMap<>();
        this.agentState = new AgentState();

        this.movingTime       = 0;
        this.waitingTime      = 0;
        this.initializingTime = 0;
        this.arrivedTime      = 0;


    }


    /**
     * Returns an Integer value which is an identification of Agent
     * @return Agent unique identifier
     **/
    public Integer getAgentID() {
        return this.agentID;
    }

    /**
     * Returns a String value which is an identification of Agent
     * @return Agent unique identifier
     **/
    public String  getAgentStringID(){
        return this.agentStringID;
    }

    /**
     * Returns a Plan assigned to Agent
     * @return agent movement schedule - Plan
     **/
    public Plan    getPlan() {
        return this.plan;
    }

    /**
     * Sets a Plan assigned to Agent
     * @param plan schedule of Agent movement
    **/
    public void    setPlan(Plan plan) {
        this.plan = plan;
    }

    /**
     * Returns a Double value which is radius of Agent
     * @return agent movement schedule - Plan
     **/
    public Double  getRadius() {
        return this.radius;
    }

    /**
     * Sets a Radius value assigned to Agent
     * @param radius Agent object radius
    **/
    public void    setRadius(Double radius) {
        this.radius = radius;
    }

    /**
     * Returns an int value which is Agent start position on the Graph
     * @return agent's start position
     **/
    public int     getStartPosition() {
        return this.startPosition;
    }

    /**
     * Sets an int value which is Agent start position on the Graph
     * @param startPosition
     **/
    public void    setStartPosition(int startPosition) {
        this.startPosition = startPosition;
    }

    /**
     * Returns an int value which is Agent target position on the Graph
     * @return agent's target position
     **/
    public int     getTargetPosition() {
        return this.targetPosition;
    }

    /**
     * Sets an int value which is Agent target position on the Graph
     * @param targetPosition
     **/
    public void    setTargetPosition(int targetPosition) {
        this.targetPosition = targetPosition;
    }

    /**
     * Sets an Integer value which is Agent state in certain time period -
     * delta between Double finishTime and Double startTime
     * @param startTime time when State starts
     * @param finishTime time when State finishes
     * @param stateIndex numerical index of State
     **/
    public void    setState(Double startTime, Double finishTime, Integer stateIndex){

        double startTimeDouble = startTime * 1000;
        double finishTimeDouble = finishTime * 1000;

        int startTimeInt = (int)startTimeDouble;
        int finishTimeInt = (int)finishTimeDouble;


        for(int time = startTimeInt; time <= finishTimeInt; time++)
            this.stateMap.put(time, stateIndex);


        this.setTimeCounter(startTimeInt, finishTimeInt, stateIndex);


    }

    /**
     * Calculates how much time Agent spends in a certain State
     * @param startTime time when State starts
     * @param finishTime time when State finishes
     * @param stateIndex numerical index of State
     **/
    private void   setTimeCounter(Integer startTime, Integer finishTime, Integer stateIndex) {

       String state = this.agentState.getState(stateIndex);

       Integer timePeriod = finishTime - startTime;


       if(state.equals("INITIALIZING")) {
           this.initializingTime += timePeriod;
       }
       else if(state.equals("MOVING")) {
           this.movingTime += timePeriod;
       }
       else if(state.equals("WAITING")) {
           this.waitingTime += timePeriod;
       }
       else if(state.equals("ARRIVED")) {
           this.arrivedTime += timePeriod;
       }


    }


    /**
     * Returns an Integer which is time spent in State - Moving
     * @return time in State - Moving
     **/
    public Integer getMovingTime() {
        return movingTime;
    }


    /**
     * Returns an Integer which is time spent in State - Waiting
     * @return time in State - Waiting
     **/
    public Integer getWaitingTime() {
        return waitingTime;
    }


    /**
     * Returns an Integer which is time spent in State - Initializing
     * @return time in State - Initializing
     **/
    public Integer getInitializingTime() {
        return initializingTime;
    }


    /**
     * Returns an Integer which is time spent in State - Arrived
     * @return time in State - Arrived
     **/
    public Integer getArrivedTime() {
        return arrivedTime;
    }


    /**
     * Returns an Integer which is numerical index  of State in particular moment of time
     * @param  time time in Integer format
     * @return numerica value of State
     **/
    public Integer getState(Integer time){
        return this.stateMap.get(time);
    }


    @Override
    public String  toString(){

        String agentString = "[" + this.agentStringID + "]:\n";
        agentString += this.plan.toString();
        agentString += "Radius: " + this.radius + "\n";
        agentString += "Start: " + this.startPosition + "\n";
        agentString += "Target: " + this.targetPosition + "\n";

        return agentString;

    }


}
