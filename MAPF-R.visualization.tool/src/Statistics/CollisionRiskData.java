package Statistics;

import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

public class CollisionRiskData {

    TreeMap<Integer, TreeMap<Integer, Double>> collisionRiskDataMap;

    public CollisionRiskData(){
          this.collisionRiskDataMap = new TreeMap<>();
    }

    public void addCollisionRiskData(Integer firstAgentIndex,
                                     Integer secondAgentIndex,
                                     Double  collisionRisk
    ){


        if(this.collisionRiskDataMap.containsKey(firstAgentIndex)){
            this.collisionRiskDataMap.get(firstAgentIndex).put(secondAgentIndex, collisionRisk);
        }
        else{

            TreeMap<Integer, Double> distanceSubDataMap = new TreeMap<>();
            distanceSubDataMap.put(secondAgentIndex, collisionRisk);
            this.collisionRiskDataMap.put(firstAgentIndex, distanceSubDataMap);

        }

        if(this.collisionRiskDataMap.containsKey(secondAgentIndex)){
            this.collisionRiskDataMap.get(secondAgentIndex).put(firstAgentIndex, collisionRisk);
        }
        else{

            TreeMap<Integer, Double> distanceSubDataMap = new TreeMap<>();
            distanceSubDataMap.put(firstAgentIndex, collisionRisk);
            this.collisionRiskDataMap.put(secondAgentIndex, distanceSubDataMap);

        }

    }

    public TreeMap<Integer, Double> getAgentRiskCollisionData(Integer agentIndex){
         return  this.collisionRiskDataMap.get(agentIndex);
    }

    public Double getAgentsRiskCollisionData(Integer firstAgentIndex, Integer secondAgentIndex){
          return this.collisionRiskDataMap.get(firstAgentIndex).get(secondAgentIndex);
    }


    public void printCollisionRiskData() {

        Iterator collisionRiskMapIT = this.collisionRiskDataMap.entrySet().iterator();

        while (collisionRiskMapIT.hasNext()) {

            Map.Entry collisionRiskMapElement = (Map.Entry) collisionRiskMapIT.next();

            Integer firstAgentIndex = (Integer) collisionRiskMapElement.getKey();

            TreeMap<Integer, Double> subCollisionRiskMap = (TreeMap<Integer, Double>) collisionRiskMapElement.getValue();

            Iterator collisionRiskMapSubIT = subCollisionRiskMap.entrySet().iterator();

            while (collisionRiskMapSubIT.hasNext()) {

                Map.Entry collisionRiskMapSubElement = (Map.Entry) collisionRiskMapSubIT.next();

                Integer secondAgentIndex = (Integer) collisionRiskMapSubElement.getKey();

                Double collisionRiskValue = (Double) collisionRiskMapSubElement.getValue();

                System.out.println(firstAgentIndex + " -> " + secondAgentIndex + " with collision risk " + collisionRiskValue);

            }

        }

        System.out.println("---------------------------------------------");

    }

}
