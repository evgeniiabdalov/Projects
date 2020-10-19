package DAO;

import Business.Agent;
import Business.Graph;
import Business.Plan;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Map;

/**
 *
 * @author abdalevg
 */
public class DataSet {

    private static Graph graph;
    private static TreeMap<Integer, Agent> agentMap;
    private static TreeMap<Integer, Plan> planMap;

    public void makeDataSet(String agentFile, String graphFile, String planFile) throws FileNotFoundException {

        DataReader dataReader = new DataReader();

        graph = dataReader.readGraph(graphFile);
        agentMap = dataReader.readAgents(agentFile);
        planMap =  dataReader.readPlan(planFile);

        //--------------------------------------------------------------

        Iterator agentMapIt = agentMap.entrySet().iterator();

        while( agentMapIt.hasNext() ){

            Map.Entry agentTreeMapElement = (Map.Entry)agentMapIt.next();

            Plan planDAO = planMap.get((Integer)agentTreeMapElement.getKey());

            agentMap.get((Integer)agentTreeMapElement.getKey()).setPlan(planDAO);

        }

    }

    public Graph getGraph(){
        return this.graph;
    }

    public TreeMap<Integer, Agent> getAgentMap(){
        return this.agentMap;
    }

}