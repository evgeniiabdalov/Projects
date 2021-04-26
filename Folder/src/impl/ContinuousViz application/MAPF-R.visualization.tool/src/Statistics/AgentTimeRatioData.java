package Statistics;

import java.util.TreeMap;

/**
 * AgentTimeRatioData class keeps TOTAL time for each Agent state: MOVING, WAITING, INITIALIZING, ARRIVED
 * TreeMap<Integer, AgentTimeRatio> agentTimeRatioTreeMap - keeps AgentTimeRatio for each Agent
 * @author Evgenii Abdalov
 */
public class AgentTimeRatioData {

    private Integer  totalMovingTime;
    private Integer  totalWaitingTime;
    private Integer  totalInitializingTime;
    private Integer  totalArrivedTime;


   private TreeMap<Integer, AgentTimeRatio> agentTimeRatioTreeMap;


    public AgentTimeRatioData(){
        this.agentTimeRatioTreeMap = new TreeMap<>();
    }

    public void setAgentTimeRatioTreeMap(Integer agentIndex, AgentTimeRatio agentTimeRatio){
        this.agentTimeRatioTreeMap.put(agentIndex, agentTimeRatio);
    }

    public AgentTimeRatio getAgentTimeRatio(Integer agentID) {
        return agentTimeRatioTreeMap.get(agentID);
    }

    public Integer getTotalMovingTime() {
        return totalMovingTime;
    }

    public void setTotalMovingTime(Integer totalMovingTime) {
        this.totalMovingTime = totalMovingTime;
    }

    public Integer getTotalWaitingTime() {
        return totalWaitingTime;
    }

    public void setTotalWaitingTime(Integer totalWaitingTime) {
        this.totalWaitingTime = totalWaitingTime;
    }

    public Integer getTotalInitializingTime() {
        return totalInitializingTime;
    }

    public void setTotalInitializingTime(Integer totalInitializingTime) {
        this.totalInitializingTime = totalInitializingTime;
    }

    public Integer getTotalArrivedTime() {
        return totalArrivedTime;
    }

    public void setTotalArrivedTime(Integer totalArrivedTime) {
        this.totalArrivedTime = totalArrivedTime;
    }


}
