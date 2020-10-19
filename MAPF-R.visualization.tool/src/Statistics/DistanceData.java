package Statistics;

import javafx.util.Pair;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class DistanceData {

    TreeMap<Integer, TreeMap<Integer, Double>> distanceDataMap;

    public DistanceData(){
        this.distanceDataMap = new TreeMap<>();
    }

    public void addDistanceData(Integer firstAgentIndex,
                           Integer secondAgentIndex,
                           Double  distance){

        if(this.distanceDataMap.containsKey(firstAgentIndex)){
            this.distanceDataMap.get(firstAgentIndex).put(secondAgentIndex, distance);
        }
        else{

            TreeMap<Integer, Double> distanceSubDataMap = new TreeMap<>();
            distanceSubDataMap.put(secondAgentIndex, distance);
            this.distanceDataMap.put(firstAgentIndex, distanceSubDataMap);

        }

        if(this.distanceDataMap.containsKey(secondAgentIndex)){
            this.distanceDataMap.get(secondAgentIndex).put(firstAgentIndex, distance);
        }
        else{

            TreeMap<Integer, Double> distanceSubDataMap = new TreeMap<>();
            distanceSubDataMap.put(firstAgentIndex, distance);
            this.distanceDataMap.put(secondAgentIndex, distanceSubDataMap);

        }


    }

    public TreeMap<Integer, Double> getAgentDistanceData(Integer agentIndex){
        return  this.distanceDataMap.get(agentIndex);
    }

    public Double getAgentsDistanceData(Integer firstAgentIndex, Integer secondAgentIndex){
        return this.distanceDataMap.get(firstAgentIndex).get(secondAgentIndex);
    }

    public void printDistanceData() {

        Iterator distanceMapIT = this.distanceDataMap.entrySet().iterator();

        while (distanceMapIT.hasNext()) {

            Map.Entry distanceMapElement = (Map.Entry)distanceMapIT.next();

            Integer firstAgentIndex = (Integer) distanceMapElement.getKey();

            TreeMap<Integer, Double> subDistanceMap = (TreeMap<Integer, Double>) distanceMapElement.getValue();

            Iterator distanceMapSubIT = subDistanceMap.entrySet().iterator();

            while (distanceMapSubIT.hasNext()) {

                Map.Entry distanceMapSubElement = (Map.Entry) distanceMapSubIT.next();

                Integer secondAgentIndex = (Integer) distanceMapSubElement.getKey();

                Double distanceValue = (Double) distanceMapSubElement.getValue();

                System.out.println(firstAgentIndex + " -> " + secondAgentIndex + " with distance " + distanceValue);

            }

        }

        System.out.println("---------------------------------------------");

    }



}
