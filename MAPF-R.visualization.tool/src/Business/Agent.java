package Business;

import java.util.TreeMap;

/**
 *
 * @author abdalevg
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


    public Integer getAgentID() {
        return this.agentID;
    }

    public String  getAgentStringID(){
        return this.agentStringID;
    }

    public Plan    getPlan() {
        return this.plan;
    }

    public void    setPlan(Plan plan) {
        this.plan = plan;
    }

    public Double  getRadius() {
        return this.radius;
    }

    public void    setRadius(Double radius) {
        this.radius = radius;
    }

    public int     getStartPosition() {
        return this.startPosition;
    }

    public void    setStartPosition(int startPosition) {
        this.startPosition = startPosition;
    }

    public int     getTargetPosition() {
        return this.targetPosition;
    }

    public void    setTargetPosition(int targetPosition) {
        this.targetPosition = targetPosition;
    }

    public void    setState(Double startTime, Double finishTime, Integer stateIndex){

        double startTimeDouble = startTime * 1000;
        double finishTimeDouble = finishTime * 1000;

        int startTimeInt = (int)startTimeDouble;
        int finishTimeInt = (int)finishTimeDouble;


        for(int time = startTimeInt; time <= finishTimeInt; time++)
            this.stateMap.put(time, stateIndex);


        this.setTimeCounter(startTimeInt, finishTimeInt, stateIndex);


    }

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

    public Integer getMovingTime() {
        return movingTime;
    }

    public Integer getWaitingTime() {
        return waitingTime;
    }

    public Integer getInitializingTime() {
        return initializingTime;
    }

    public Integer getArrivedTime() {
        return arrivedTime;
    }

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
