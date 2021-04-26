package Business;

import DAO.DataSet;
import javafx.util.Pair;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

/**
 * MainController class controls operations with GraphControllers as well as
 * other business logic operations.
 * @author Evgenii Abdalov
 */
public class MainController {

    private TreeMap<Integer, GraphController>           graphControllers;
    private TreeMap<String, Pair<Integer, Integer>>     solutionKeys;
    private GraphController                             currentGraphController;

    private AgentState                                  agentState;

    private DataSet                                     dataSet;


    /**
     * MainController class constructor
     **/
    public                             MainController(){

        this.graphControllers       = new TreeMap<>();

        this.solutionKeys           = new TreeMap<>();

        this.agentState             = new AgentState();

        this.currentGraphController = null;

        this.dataSet                = new DataSet();

    }

    /**
     * Upload solution according to parameters
     * @param solutionKey String identifier of uploaded solution
     * @param agentFileString name of file with Agents data
     * @param graphFileString name of file with Graph data
     * @param  planFileString name of file with Schedule data
     * @return true if solution is upladed, false if input files are corrupted
     **/
    public  boolean                    uploadSolution(String solutionKey, String agentFileString, String graphFileString, String planFileString) throws FileNotFoundException {

        if( !this.dataSet.makeDataSet(agentFileString, graphFileString, planFileString))
            return false;

        this.addGraphController(solutionKey, this.dataSet.getGraph(), this.dataSet.getAgentMap());

        return true;

    }


    /**
     * Adds Agents to AgentController, which is assigned to GraphController
     * @param solutionKey String identifier of uploaded solution
     * @param graph an instance of Graph object
     * @param agentMap map of Agents that will be assigned to AgentController, AgentController will be added to GraphController
    **/
    private void                       addAgentsTOGraphController(String solutionKey, Graph graph, TreeMap<Integer, Agent>  agentMap){

       Integer tmpID = this.graphControllers.size();

       GraphController graphController = new GraphController( tmpID, graph );
       graphController.addAgentController(agentMap);

       this.graphControllers.put(tmpID, graphController);

        /**
         * solutionKey corresponds to unique pair of two identifiers: GraphController object identifier and AgentController object identifier
         */
       this.solutionKeys.put(solutionKey, new Pair<Integer, Integer>(graphController.getGraphControllerID(), graphController.getLastAgentControllerID()));

       this.currentGraphController = graphController;

    }

    /**
     * Controls if Graph is already present in collection of GraphControllers
     * if yes, then just adds collection of Agents to already existent instance of GraphController
     * if no, then creates a new instance of GraphController and adds it to collction of GraphControllers
     * @param solutionKey String identifier of uploaded solution
     * @param graph an instance of Graph object
     * @param agentMap map of Agents that will be assigned to AgentController, AgentController will be added to GraphController
     **/
    private void                       addGraphController(String solutionKey, Graph graph, TreeMap<Integer, Agent>  agentMap){

        boolean  agentsAdded = false;

        Iterator graphControllerIT = this.graphControllers.entrySet().iterator();

        while( graphControllerIT.hasNext() ) {

            Map.Entry graphControllerElement = (Map.Entry) graphControllerIT.next();

            if( graph.equals( ((GraphController)graphControllerElement.getValue()).getGraph() )){
                ((GraphController)graphControllerElement.getValue()).addAgentController(agentMap);
                agentsAdded = true;
            }

        }

        if( !agentsAdded ){
            this.addAgentsTOGraphController(solutionKey, graph, agentMap);
        }


    }

    /**
     * Returns a set of String of keys for uploaded solutions
     * @return Set of String of keys for uploaded solutions
     **/
    public Set<String>                 getKeys(){
        return this.solutionKeys.keySet();
    }

    /**
     * Returns a GraphController which is set as current GraphController, by default it is last added GraphController objects
     * @return GraphController that is current at the moment
     **/
    public GraphController             getCurrentGraphController(){
       return this.currentGraphController;
    }

    /**
     * Returns a pair of Integer values which corresponds to the solutionKey
     * @param  solutionKey String identifier of uploaded solution
     * @return Pair<Integer, Integer> a pair of Integer values which corresponds to the solutionKey
     **/
    private Pair<Integer, Integer>     getPairID(String solutionKey){
        return this.solutionKeys.get(solutionKey);
    }

    /**
     * Sets current solution by setting current GraphController and current AgentController
     * @param solutionKey String identifier of chosen solution
     * @return true if solution with String identifier exist and false if solution with
     * this identifier does not exist
     **/
    public boolean                     setCurrentSolution(String solutionKey){

        Pair<Integer, Integer> pairID =  this.getPairID(solutionKey);

        this.currentGraphController = null;
        this.currentGraphController = this.graphControllers.get(pairID.getKey());

        if(this.currentGraphController == null)
            return false;


        return this.currentGraphController.setCurrentAgentController(pairID.getValue());

    }

    /**
    * Returns a State of Agent at a moment of time
    * @param agentID Agent unique identifier
    * @param time  current value of time
    * @return String value of State
    **/
    public String                      getAgentState(Integer agentID, Integer time){
       return this.agentState.getState(this.currentGraphController.getAgentState(agentID, time));
    }



}
