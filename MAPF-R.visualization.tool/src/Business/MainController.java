package Business;

import DAO.DataSet;
import javafx.util.Pair;

import java.io.FileNotFoundException;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class MainController {

    private TreeMap<Integer, GraphController>           graphControllers;
    private TreeMap<String, Pair<Integer, Integer>>     solutionKeys;
    private GraphController                             currentGraphController;

    private AgentState                                  agentState;

    private DataSet dataSet;



    public                             MainController(){

        this.graphControllers       = new TreeMap<>();

        this.solutionKeys           = new TreeMap<>();

        this.agentState             = new AgentState();

        this.currentGraphController = null;

        this.dataSet                = new DataSet();

    }

    public  void                       uploadSolution(String solutionKey, String agentFileString, String graphFileString, String planFileString) throws FileNotFoundException {

        this.dataSet.makeDataSet(agentFileString, graphFileString, planFileString);

        this.addGraphController(solutionKey, this.dataSet.getGraph(), this.dataSet.getAgentMap());

    }

    private void                       addAgentsTOGraphController(String solutionKey, Graph graph, TreeMap<Integer, Agent>  agentMap){

       Integer tmpID = this.graphControllers.size();

       GraphController graphController = new GraphController( tmpID, graph );
       graphController.addAgentController(agentMap);

       this.graphControllers.put(tmpID, graphController);

       this.solutionKeys.put(solutionKey, new Pair<Integer, Integer>(graphController.getGraphControllerID(), graphController.getLastAgentControllerID()));

       this.currentGraphController = graphController;

    }

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

    public Set<String>                 getKeys(){
        return this.solutionKeys.keySet();
    }

    public GraphController             getCurrentGraphController(){
       return this.currentGraphController;
    }

    private Pair<Integer, Integer>     getPairID(String solutionKey){
        return this.solutionKeys.get(solutionKey);
    }

    public boolean                     setCurrentSolution(String solutionKey){

        Pair<Integer, Integer> pairID =  this.getPairID(solutionKey);

        this.currentGraphController = null;
        this.currentGraphController = this.graphControllers.get(pairID.getKey());

        if(this.currentGraphController == null)
            return false;


        return this.currentGraphController.setCurrentAgentController(pairID.getValue());

    }

    public String                      getAgentState(Integer agentIndex, Integer time){
       return this.agentState.getState(this.currentGraphController.getAgentState(agentIndex, time));
    }



}
