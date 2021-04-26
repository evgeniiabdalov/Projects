package Statistics;


import java.util.TreeMap;

/**
 * StatisticsData class keeps all calculated statistical data
 * @author Evgenii Abdalov
 */
public class StatisticsData {

    /**
     *  TreeMap<Integer, AgentMovementData>  agentMovementData, where key is an Agent unique identifier and value is statistical data object AgentMovementData
     **/
    private TreeMap<Integer, AgentMovementData>    agentMovementData;

    /**
     *  TreeMap<Integer, DistanceData>  distanceDataMap, where key is an Integer value of time(millisecond) and value is statistical data object DistanceData
     **/
    private TreeMap<Integer, DistanceData>         distanceDataMap;

    /**
     *  TreeMap<Integer, AgentMovementData>  collisionRiskDataMap, where key is an Integer value of time(millisecond) and value is statistical data object CollisionRiskData
     **/
    private TreeMap<Integer, CollisionRiskData>    collisionRiskDataMap;

    private AgentTimeRatioData                     agentTimeRatioData;
    private CollisionData                          collisionData;
    private ShortestDistanceData                   shortestDistanceData;


    public                                       StatisticsData(){

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

    public void                                  setShortestDistanceData(ShortestDistanceData shortestDistanceData){
           this.shortestDistanceData = shortestDistanceData;
    }


    public AgentTimeRatioData                    getAgentTimeRatioData(){
        return this.agentTimeRatioData;
    }

    public AgentMovementData                     getAgentMovementData(Integer agentIndex){
        return this.agentMovementData.get(agentIndex);
    }

    public ShortestDistanceData                  getShortestDistanceData(){
        return this.shortestDistanceData;
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
