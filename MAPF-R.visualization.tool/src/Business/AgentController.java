package Business;


import Visualization.AgentVisual;

import java.util.*;

public class AgentController {

    private Integer agentControllerID;

    private TreeMap<Integer, Agent>          agentMap;
    private TreeMap<String,  AgentVisual>    agentVisualMap;

    private Integer totalMovingTime;
    private Integer totalWaitingTime;
    private Integer totalInitializingTime;
    private Integer totalArrivedTime;


    public                              AgentController(Integer agentControllerID, TreeMap<Integer, Agent> agentMap){

        this.agentControllerID = agentControllerID;
        this.agentMap          = agentMap;
        this.agentVisualMap    = new TreeMap<>();

        this.makeAgentVisual();

        this.totalMovingTime = 0;
        this.totalWaitingTime = 0;
        this.totalInitializingTime = 0;
        this.totalArrivedTime = 0;

    }

    public Integer                      getAgentControllerID() {
        return agentControllerID;
    }

    public Set<Integer>                 getAngentIDs(){
        return this.agentMap.keySet();
    }

    private void                        makeAgentVisual() {

        Iterator agentMapIT = this.agentMap.entrySet().iterator();

        while (agentMapIT.hasNext()) {

            Map.Entry agentMapElement = (Map.Entry) agentMapIT.next();

            Agent tmpAgent = (Agent)agentMapElement.getValue();

            AgentVisual agentVisual = new AgentVisual(tmpAgent.getAgentID(), tmpAgent.getRadius());

            this.agentVisualMap.put(tmpAgent.getAgentStringID(), agentVisual);

        }


    }

    public AgentVisual                  getAgentVisual(Integer agentID){
        return this.agentVisualMap.get(agentID.toString());
    }

    public Plan                         getAgentPlan(Integer agentID){
           return this.agentMap.get(agentID).getPlan();
    }

    public Double                       getAgentRadius(Integer agentID){
        return this.agentMap.get(agentID).getRadius();
    }

    public Integer                      getAgentStartPosition(Integer agentID){
        return this.agentMap.get(agentID).getStartPosition();
    }

    public Integer                      getAgentTargetPosition(Integer agentIndex){
        return this.agentMap.get(agentIndex).getTargetPosition();
    }


    public void                         setAgentState(Integer agentIndex, Double startTime, Double finishTime, Integer stateIndex){
        this.agentMap.get(agentIndex).setState(startTime, finishTime, stateIndex);
    }

    public Integer                      getAgentState(Integer agentIndex, Integer time) {
        return this.agentMap.get(agentIndex).getState(time);
    }

    public void                         makeTotalTime(){

        Set<Integer> agentIDs = this.agentMap.keySet();

        for(Integer agentID : agentIDs){

           Agent tmpAgent = this.agentMap.get(agentID);

           this.totalMovingTime       += tmpAgent.getMovingTime();
           this.totalWaitingTime      += tmpAgent.getWaitingTime();
           this.totalInitializingTime += tmpAgent.getInitializingTime();
           this.totalArrivedTime      += tmpAgent.getArrivedTime();

        }

    }

    public Integer getTotalMovingTime() {
        return totalMovingTime;
    }

    public Integer getTotalWaitingTime() {
        return totalWaitingTime;
    }

    public Integer getTotalInitializingTime() {
        return totalInitializingTime;
    }

    public Integer getTotalArrivedTime() {
        return totalArrivedTime;
    }


    public Integer getAgentMovingTime(Integer agentIndex)
    {
        return this.agentMap.get(agentIndex).getMovingTime();
    }

    public Integer getAgentWaitingTime(Integer agentIndex) {
        return this.agentMap.get(agentIndex).getWaitingTime();
    }

    public Integer getAgentInitializingTime(Integer agentIndex) {
       return this.agentMap.get(agentIndex).getInitializingTime();
    }

    public Integer getAgentArrivedTime(Integer agentIndex) {
        return this.agentMap.get(agentIndex).getArrivedTime();
    }



}
