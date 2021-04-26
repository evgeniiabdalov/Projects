package Business;


import Statistics.ShortestDistanceData;
import Visualization.AgentVisual;

import java.util.*;

/**
 * AgentController class represents a group of Agents, that are corresponding to
 * a current solution. AgentController class main objectives are to store Agents,
 * to convey operations with Agents and to modificate parameters of Agents.
 * @author Evgenii Abdalov
 */
public class AgentController {

    private Integer agentControllerID;

    private TreeMap<Integer, Agent>          agentMap;
    private TreeMap<String,  AgentVisual>    agentVisualMap;

    private Integer                          totalMovingTime;
    private Integer                          totalWaitingTime;
    private Integer                          totalInitializingTime;
    private Integer                          totalArrivedTime;


    /**
     * AgentController class constructor
     * @param agentControllerID
     * @param agentMap map of Agents assigned to AgentController
     **/
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

    /**
     * Returns an Integer value which is an identification of AgentController
     * @return AgentController unique identifier
     **/
    public Integer                      getAgentControllerID() {
        return agentControllerID;
    }

    /**
     * Returns a Set of Agent unique identifiers that are assigned to AgentController
     * @return Set of Agent unique identifiers
     **/
    public Set<Integer>                 getAgentIDs(){
        return this.agentMap.keySet();
    }

    /**
     * Generates visual images for each Agent assigned to AgentController
     **/
    private void                        makeAgentVisual() {

        Iterator agentMapIT = this.agentMap.entrySet().iterator();

        while (agentMapIT.hasNext()) {

            Map.Entry agentMapElement = (Map.Entry) agentMapIT.next();

            Agent tmpAgent = (Agent)agentMapElement.getValue();

            AgentVisual agentVisual = new AgentVisual(tmpAgent.getAgentID(), tmpAgent.getRadius());

            this.agentVisualMap.put(tmpAgent.getAgentStringID(), agentVisual);

        }


    }

    /**
     * Returns AgentVisual object that is assigned to Agent object with unique identifier
     * @param agentID Agent unique identifier
     * @return AgentVisual object that is assigned to Agent object
     **/
    public AgentVisual                  getAgentVisual(Integer agentID){
        return this.agentVisualMap.get(agentID.toString());
    }

    /**
     * Returns Plan object that is assigned to Agent object with unique identifier
     * @param agentID Agent unique identifier
     * @return Plan object that is assigned to Agent object
     **/
    public Plan                         getAgentPlan(Integer agentID){
           return this.agentMap.get(agentID).getPlan();
    }

    /**
     * Returns Double value radius that is assigned to Agent object with unique identifier
     * @param agentID Agent unique identifier
     * @return Radius value that is assigned to Agent object
     **/
    public Double                       getAgentRadius(Integer agentID){
        return this.agentMap.get(agentID).getRadius();
    }

    /**
     * Returns an Integer value which is an index of Vertex that is assigned as a start position to Agent object with unique identifier
     * @param agentID Agent unique identifier
     * @return an Integer value which is an index of Vertex
     **/
    public Integer                      getAgentStartPosition(Integer agentID){
        return this.agentMap.get(agentID).getStartPosition();
    }

    /**
    * Returns an Integer value which is an index of Vertex that is assigned as a target position to Agent object with unique identifier
    * @param agentID Agent unique identifier
    * @return an Integer value which is an index of Vertex
    **/
    public Integer                      getAgentTargetPosition(Integer agentID){
        return this.agentMap.get(agentID).getTargetPosition();
    }

    /**
     * Sets State to Agent for a certain time period
     * @param agentID Agent unique identifier
     * @param startTime time moment when State starts
     * @param finishTime time moment when State ends
     **/
    public void                         setAgentState(Integer agentID, Double startTime, Double finishTime, Integer stateIndex){
        this.agentMap.get(agentID).setState(startTime, finishTime, stateIndex);
    }

    /**
     * Returns State of Agent at a certain time moment
     * @param agentID Agent unique identifier
     * @param time moment when Agent is in a certain State
     * @return State of Agent at particular moment of time
     **/
    public Integer                      getAgentState(Integer agentID, Integer time) {
        return this.agentMap.get(agentID).getState(time);
    }

    /**
     * Summarizes all Agents total time for each type of State
     **/
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

    /**
     * Returns an Integer value of total time in State - Moving
     * @return an Integer value of total time in State - Moving
     **/
    public Integer                      getTotalMovingTime() {
        return totalMovingTime;
    }

    /**
     * Returns an Integer value of total time in State - Waiting
     * @return an Integer value of total time in State - Waiting
     **/
    public Integer                      getTotalWaitingTime() {
        return totalWaitingTime;
    }

    /**
     * Returns an Integer value of total time in State - Initializing
     * @return an Integer value of total time in State - Initializing
     **/
    public Integer                      getTotalInitializingTime() {
        return totalInitializingTime;
    }

    /**
     * Returns an Integer value of total time in State - Arrived
     * @return an Integer value of total time in State - Arrived
     **/
    public Integer                      getTotalArrivedTime() {
        return totalArrivedTime;
    }

    /**
     * Returns an Integer value of time in State - Moving for an Agent with unique identifier
     * @param agentID Agent unique identifier
     * @return an Integer value of time in State - Moving
     **/
    public Integer                      getAgentMovingTime(Integer agentID)
    {
        return this.agentMap.get(agentID).getMovingTime();
    }

    /**
     * Returns an Integer value of time in State - Waiting for an Agent with unique identifier
     * @param agentID Agent unique identifier
     * @return an Integer value of time in State - Waiting
     **/
    public Integer                      getAgentWaitingTime(Integer agentID) {
        return this.agentMap.get(agentID).getWaitingTime();
    }

    /**
     * Returns an Integer value of time in State - Initializing for an Agent with unique identifier
     * @param agentID Agent unique identifier
     * @return an Integer value of time in State - Initializing
     **/
    public Integer                      getAgentInitializingTime(Integer agentID) {
       return this.agentMap.get(agentID).getInitializingTime();
    }

    /**
     * Returns an Integer value of time in State - Arrived for an Agent with unique identifier
     * @param agentID Agent unique identifier
     * @return an Integer value of time in State - Arrived
     **/
    public Integer                      getAgentArrivedTime(Integer agentID) {
        return this.agentMap.get(agentID).getArrivedTime();
    }

    /**
     * Generates an instance of ShortestDistanceData object which contains List of Agents with non-optimal path distances
     * @return ShortestDistaceData object
     **/
    public ShortestDistanceData         getShortestDistanceData(Graph graph) {

        ShortestDistanceData shortestDistanceData = new ShortestDistanceData(graph, this.agentMap);

        return shortestDistanceData;
    }
}
