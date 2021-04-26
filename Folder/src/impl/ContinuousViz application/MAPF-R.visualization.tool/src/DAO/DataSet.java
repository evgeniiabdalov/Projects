package DAO;

import Business.Agent;
import Business.Graph;
import Business.Plan;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.TreeMap;
import java.util.Map;

/**
 * DataSet class encapsulates data from input files and transports it to Business
 * module.
 * @author Evgenii Abdalov
 */
public class DataSet {

    private static Graph graph;
    private static TreeMap<Integer, Agent> agentMap;
    private static TreeMap<Integer, Plan> planMap;

    /**
     * Invokes data reading process and creates data set
     * @param agentFile
     * @param graphFile
     * @param planFile
     * @return true if files were successfully, false it files are corrupted or non-existent
     **/
    public boolean makeDataSet(String agentFile, String graphFile, String planFile) throws FileNotFoundException {

        DataReader dataReader = new DataReader();

        graph = dataReader.readGraph(graphFile);
        agentMap = dataReader.readAgents(agentFile);
        planMap =  dataReader.readPlan(planFile);


        if(graph.getVerticesMap().isEmpty()) {
            System.out.println("vertices");
            return false;
        }

        if(agentMap.isEmpty()) {
            System.out.println("agent");
            return false;
        }

        if(planMap.isEmpty()) {
            System.out.println("plan");
            return false;
        }

        if( planMap.size() != agentMap.size()){
            System.out.println("not for all agents plans are set");
            return false;
        }

        //--------------------------------------------------------------

        Iterator agentMapIt = agentMap.entrySet().iterator();

        while( agentMapIt.hasNext() ){

            Map.Entry agentTreeMapElement = (Map.Entry)agentMapIt.next();

            Plan planDAO = planMap.get((Integer)agentTreeMapElement.getKey());

            agentMap.get((Integer)agentTreeMapElement.getKey()).setPlan(planDAO);

        }

        return true;

    }

    /**
     * Returns Graph object from input file
     * @return Graph that has been read from file
     **/
    public Graph getGraph(){
        return this.graph;
    }

    /**
     * Returns collection of Agents from input file
     * @return TreeMap<Integer, Agent> where an Integer value is a key identifier and a value is an Agent object  with this identifier
     **/
    public TreeMap<Integer, Agent> getAgentMap(){
        return this.agentMap;
    }

}