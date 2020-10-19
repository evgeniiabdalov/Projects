package Statistics;

import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

public class StatisticsData {

    private TreeMap<Integer, AgentMovementData> agentMovementData;

    //distance data
    private TreeMap<Integer, DistanceData>      distanceDataMap;

    //collision risk data
    private TreeMap<Integer, CollisionRiskData> collisionRiskDataMap;

    private AgentTimeRatioData                  agentTimeRatioData;

    private CollisionData                       collisionData;


    public                                       StatisticsData(){
        this.collisionData              = new CollisionData();
        this.distanceDataMap            = new TreeMap<>();
        this.collisionData              = new CollisionData();
    }

    public void                                  setAgentTimeRatioData(AgentTimeRatioData agentTimeRatioData){
        this.agentTimeRatioData = agentTimeRatioData;
    }

    public void                                  setMovementData(TreeMap<Integer, AgentMovementData> agentMovementData){
        this.agentMovementData = agentMovementData;
    }

    public void                                  setCollisionData(CollisionData collisionData) {
        this.collisionData = collisionData;
    }

    public void                                  setDistanceDataMap(TreeMap<Integer, DistanceData> distanceDataMap){
        this.distanceDataMap = distanceDataMap;
    }

    public void                                  setRiskDataMap(TreeMap<Integer, CollisionRiskData> riskDataMap){
        this.collisionRiskDataMap = riskDataMap;
    }

    public AgentTimeRatioData                    getAgentTimeRatioData(){
        return this.agentTimeRatioData;
    }

    public AgentMovementData                     getAgentMovementData(Integer agentIndex){
        return this.agentMovementData.get(agentIndex);
    }

    public TreeMap<Integer, DistanceData>        getDistanceDataMap() {
        return this.distanceDataMap;
    }

    public TreeMap<Integer, CollisionRiskData>   getRiskDataMap() {
        return this.collisionRiskDataMap;
    }

    public CollisionData                         getCollisionData() {
        return this.collisionData;
    }

    public Double                                getTimeAgentSpecificDistanceData(Integer time, Integer firstAgentIndex, Integer secondAgentIndex){
        return this.distanceDataMap.get(time).getAgentsDistanceData(firstAgentIndex, secondAgentIndex);
    }

    public Double                                getTimeAgentSpecificCollisionRiskData(Integer time, Integer firstAgentIndex, Integer secondAgentIndex){
         return this.collisionRiskDataMap.get(time).getAgentsRiskCollisionData(firstAgentIndex, secondAgentIndex);
    }


}
